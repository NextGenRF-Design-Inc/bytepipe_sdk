
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "adi_common_error_types.h"
#include "config.h"
#include "axi_io.h"
#include "spi.h"
#include "spi_extra.h"
#include "sleep.h"
#include "adrv9001.h"
#include "adrv9001_hal.h"
#include "ff.h"
#include "mem.h"


int32_t Adrv9001Hal_LogWrite(void *devHalCfg, uint32_t logLevel, const char *comment, va_list argp)
{
  adrv9001_hal_t *Hal = (adrv9001_hal_t*)devHalCfg;
  UINT len = 0;

  char *str = calloc(1,1024);

  vsnprintf( str, 1023, comment, argp );

  if(f_write(&Hal->LogFil, str, strlen(str), (UINT*)&len) != FR_OK)
    return ADI_COMMON_ACT_WARN_RESET_LOG;

  f_sync(&Hal->LogFil);

  free(str);

  return ADI_COMMON_ERR_OK;
}

int32_t Adrv9001Hal_DelayUs(void *devHalCfg, uint32_t time_us)
{
  usleep(time_us);

  return ADI_COMMON_ERR_OK;
}

int32_t Adrv9001Hal_Open( void *devHalCfg )
{
  adrv9001_hal_t *Hal = (adrv9001_hal_t*)devHalCfg;

  xil_spi_init_param SpiExtra = {
    .type = SPI_PS,
    .flags = 0
  };

  spi_init_param SpiInit = {
    .device_id = ADRV9001_SPI_DEVICE_ID,
    .max_speed_hz = 20000000,
    .mode = SPI_MODE_0,
    .chip_select = ADRV9001_SPI_CS,
    .platform_ops = &xil_platform_ops,
    .extra = &SpiExtra
  };

  if( spi_init(&Hal->Spi, &SpiInit) != 0)
    return ADI_COMMON_ERR_SPI_FAIL;

  /* Delete PHY Log file */
  f_unlink(Hal->LogPath);

  /* Open File */
  if( f_open(&Hal->LogFil, Hal->LogPath, FA_CREATE_NEW | FA_WRITE | FA_READ) != FR_OK)
    return ADI_COMMON_ACT_WARN_RESET_LOG;

  /* Point to beginning of file */
  f_lseek(&Hal->LogFil, 0);

  f_sync(&Hal->LogFil);

  return ADI_COMMON_ERR_OK;
}

int32_t Adrv9001Hal_Close( void *devHalCfg )
{
  adrv9001_hal_t *Hal = (adrv9001_hal_t*)devHalCfg;

  f_sync(&Hal->LogFil);

  f_close(&Hal->LogFil);

  return ADI_COMMON_ERR_OK;
}

int32_t Adrv9001Hal_SpiWrite( void *devHalCfg, const uint8_t txData[], uint32_t numTxBytes )
{
  static const int32_t MAX_SIZE = 4096;
  uint32_t toWrite = 0;
  int32_t remaining = numTxBytes;

  adrv9001_hal_t *Hal = (adrv9001_hal_t*)devHalCfg;

  do
  {
    toWrite = (remaining > MAX_SIZE) ? MAX_SIZE : remaining;

    if( spi_write_and_read(Hal->Spi, (uint8_t*)&txData[numTxBytes - remaining], toWrite) != 0)
      return ADI_COMMON_ERR_SPI_FAIL;

    remaining -= toWrite;

  } while (remaining > 0);

  return ADI_COMMON_ERR_OK;
}

int32_t Adrv9001Hal_SpiRead( void *devHalCfg, const uint8_t txData[], uint8_t rxData[], uint32_t numRxBytes )
{

  static const int32_t MAX_SIZE = 4096;
  uint32_t toWrite = 0;
  int32_t remaining = numRxBytes;

  memcpy(rxData, txData, numRxBytes);

  adrv9001_hal_t *Hal = (adrv9001_hal_t*)devHalCfg;

  do
  {
    toWrite = (remaining > MAX_SIZE) ? MAX_SIZE : remaining;

    if( spi_write_and_read(Hal->Spi, &rxData[numRxBytes - remaining], toWrite) != 0)
      return ADI_COMMON_ERR_SPI_FAIL;

    remaining -= toWrite;

  } while (remaining > 0);

  return ADI_COMMON_ERR_OK;
}

int32_t Adrv9001Hal_ResetbPinSet( void *devHalCfg, uint8_t pinLevel )
{
  uint32_t regVal;

  axi_io_read(ADRV9001_CTRL_BASE_ADDR, ADRV9001_CTRL_REG_OFFSET, &regVal);

  if(pinLevel)
    regVal |= ADRV9001_CTRL_RSTN_MASK;
  else
    regVal &= ~ADRV9001_CTRL_RSTN_MASK;

  axi_io_write(ADRV9001_CTRL_BASE_ADDR, ADRV9001_CTRL_REG_OFFSET, regVal);

  return ADI_COMMON_ERR_OK;
}


int32_t(*adi_common_hal_LogWrite)(void *devHalCfg, uint32_t logLevel, const char *formatStr, va_list argp)    = Adrv9001Hal_LogWrite;
int32_t(*adi_common_hal_Wait_us)(void *devHalCfg, uint32_t time_us)                                           = Adrv9001Hal_DelayUs;
int32_t(*adi_adrv9001_hal_open)(void *devHalCfg)                                                              = Adrv9001Hal_Open;
int32_t(*adi_adrv9001_hal_close)(void *devHalCfg)                                                             = Adrv9001Hal_Close;
int32_t(*adi_hal_SpiWrite)(void *devHalCfg, const uint8_t txData[], uint32_t numTxBytes)                      = Adrv9001Hal_SpiWrite;
int32_t(*adi_hal_SpiRead)(void *devHalCfg, const uint8_t txData[], uint8_t rxData[], uint32_t numRxBytes)     = Adrv9001Hal_SpiRead;
int32_t(*adi_adrv9001_hal_resetbPin_set)(void *devHalCfg, uint8_t pinLevel)                                   = Adrv9001Hal_ResetbPinSet;

int32_t(*adi_hal_ArmImagePageGet)(void *devHalCfg, const char *armImagePath, uint32_t pageIndex, uint32_t pageSize, uint8_t *rdBuff) = NULL;
int32_t(*adi_hal_StreamImagePageGet)(void *devHalCfg, const char *streamImagePath, uint32_t pageIndex, uint32_t pageSize, uint8_t *rdBuff) = NULL;
int32_t(*adi_hal_RxGainTableEntryGet)(void *devHalCfg, const char *rxGainTablePath, uint16_t lineCount, uint8_t *gainIndex, uint8_t *rxFeGain, uint8_t *tiaControl, uint8_t *adcControl, uint8_t *extControl, uint16_t *phaseOffset, int16_t *digGain) = NULL;
int32_t(*adi_hal_TxAttenTableEntryGet)(void *devHalCfg, const char *txAttenTablePath, uint16_t lineCount, uint16_t *attenIndex, uint8_t *txAttenHp, uint16_t *txAttenMult) = NULL;
