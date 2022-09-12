
#include <stdio.h>
#include <string.h>
#include <xil_io.h>
#include "axi_adrv9001.h"
#include "adrv9001.h"
#include "xscugic.h"
#include "xparameters.h"

extern XScuGic          xInterruptController;



/* Register Address */
#define ADRV9001_TX1_EN_ADDR              ((0) << 2)
#define ADRV9001_TX2_EN_ADDR              ((1) << 2)
#define ADRV9001_RX1_EN_ADDR              ((2) << 2)
#define ADRV9001_RX2_EN_ADDR              ((3) << 2)
#define ADRV9001_RSTN_ADDR                ((4) << 2)

#define ADRV9001_TX1_DATA_PATH_ADDR       ((5) << 2)
#define ADRV9001_TX2_DATA_PATH_ADDR       ((6) << 2)

#define ADRV9001_CH1_DATA_ADDR            ((7) << 2)
#define ADRV9001_CH2_DATA_ADDR            ((8) << 2)

#define ADRV9001_DGPIO_DIR_ADDR           ((9) << 2)
#define ADRV9001_DGPIO_IO_ADDR            ((10) << 2)

#define ADRV9001_TX1_DISABLE_CNT          ((11) << 2)
#define ADRV9001_TX1_SSI_ENABLE_CNT       ((12) << 2)

#define ADRV9001_TX2_DISABLE_CNT          ((13) << 2)
#define ADRV9001_TX2_SSI_ENABLE_CNT       ((14) << 2)

#define ADRV9001_RX1_DISABLE_CNT          ((15) << 2)
#define ADRV9001_RX1_SSI_ENABLE_CNT       ((16) << 2)
#define ADRV9001_RX1_SSI_DISABLE_CNT      ((17) << 2)

#define ADRV9001_RX2_DISABLE_CNT          ((18) << 2)
#define ADRV9001_RX2_SSI_ENABLE_CNT       ((19) << 2)
#define ADRV9001_RX2_SSI_DISABLE_CNT      ((20) << 2)

#define ADRV9001_MSPI_ADDR   		          ((21) << 2)

#define ADRV9001_ID_ADDR                  ((31) << 2)

static uint8_t volatile AxiAdrv9001SpiDone = 0;


static void AxiAdrv9001_SpiIrq( void *ref )
{
  AxiAdrv9001SpiDone = 1;

  XScuGic_Disable(&xInterruptController, XPAR_FABRIC_ADRV9002_0_SPI_IRQ_INTR);
}

int32_t AxiAdrv9001_Initialize( uint32_t Base )
{
  uint32_t regVal = Xil_In32(Base + ADRV9001_ID_ADDR);

  if(XScuGic_Connect(&xInterruptController, XPAR_FABRIC_ADRV9002_0_SPI_IRQ_INTR, (XInterruptHandler)AxiAdrv9001_SpiIrq, NULL ) != 0)
    return Adrv9001Status_IrqErr;

  if(regVal == 0x12345678)
    return 0;
  else
    return -1;
}

void AxiAdrv9001_ResetbPinSet( uint32_t Base, uint8_t Level )
{
  Xil_Out32(Base + ADRV9001_RSTN_ADDR, Level);
}

static void AxiAdrv9001_MspiWrite( uint32_t Base, uint8_t Value, uint8_t Start )
{
  uint32_t regValue = (uint32_t)Value + ((uint32_t)Start << 8);

  Xil_Out32(Base + ADRV9001_MSPI_ADDR, regValue);
}

static bool AxiAdrv9001_MspiRead( uint32_t Base, uint8_t *Value )
{
  uint32_t regValue = Xil_In32(Base + ADRV9001_MSPI_ADDR);

  if( (regValue & 0x100) == 0x100)
  {
    *Value = (uint8_t)regValue;
    return true;
  }
  else
  {
    return false;
  }
}

int32_t AxiAdrv9001_MspiTransfer( uint32_t Base, uint8_t *TxBuf, uint8_t *RxBuf, uint32_t Length)
{
  AxiAdrv9001SpiDone = 0;

  XScuGic_Enable(&xInterruptController, XPAR_FABRIC_ADRV9002_0_SPI_IRQ_INTR);

  for( int i = 0; i < Length; i++ )
  {
    if( i == (Length - 1))
      AxiAdrv9001_MspiWrite(Base, TxBuf[i], 1);
    else
      AxiAdrv9001_MspiWrite(Base, TxBuf[i], 0);
  }

  while( AxiAdrv9001SpiDone == 0 );

  uint8_t regValue;
  for( int i = 0; i < Length; i++ )
  {
    if( AxiAdrv9001_MspiRead( Base, &regValue) == false)
      return Adrv9001Status_SpiErr;

    if( RxBuf != NULL )
      RxBuf[i] = regValue;
  }

  return Adrv9001Status_Success;
}

void AxiAdrv9001_SetDisableCnt( uint32_t Base, adi_common_Port_e Port, adi_common_ChannelNumber_e Channel, uint32_t SampleCnt )
{
  if( (Port == ADI_TX) && (Channel == ADI_CHANNEL_1) )
  {
    Xil_Out32(Base + ADRV9001_TX1_DISABLE_CNT, SampleCnt << 1);
  }
  else if( (Port == ADI_TX) && (Channel == ADI_CHANNEL_2) )
  {
    Xil_Out32(Base + ADRV9001_TX2_DISABLE_CNT, SampleCnt << 1);
  }
  else if( (Port == ADI_RX) && (Channel == ADI_CHANNEL_1) )
  {
    Xil_Out32(Base + ADRV9001_RX1_DISABLE_CNT, SampleCnt << 1);
  }
  else if( (Port == ADI_RX) && (Channel == ADI_CHANNEL_2) )
  {
    Xil_Out32(Base + ADRV9001_RX2_DISABLE_CNT, SampleCnt << 1);
  }
}

void AxiAdrv9001_SetSsiEnableCnt( uint32_t Base, adi_common_Port_e Port, adi_common_ChannelNumber_e Channel, uint32_t SampleCnt )
{
  if( (Port == ADI_TX) && (Channel == ADI_CHANNEL_1) )
  {
    Xil_Out32(Base + ADRV9001_TX1_SSI_ENABLE_CNT, SampleCnt << 1);
  }
  else if( (Port == ADI_TX) && (Channel == ADI_CHANNEL_2) )
  {
    Xil_Out32(Base + ADRV9001_TX2_SSI_ENABLE_CNT, SampleCnt << 1);
  }
  else if( (Port == ADI_RX) && (Channel == ADI_CHANNEL_1) )
  {
    Xil_Out32(Base + ADRV9001_RX1_SSI_ENABLE_CNT, SampleCnt << 1);
  }
  else if( (Port == ADI_RX) && (Channel == ADI_CHANNEL_2) )
  {
    Xil_Out32(Base + ADRV9001_RX2_SSI_ENABLE_CNT, SampleCnt << 1);
  }
}

void AxiAdrv9001_SetSsiDisableCnt( uint32_t Base, adi_common_Port_e Port, adi_common_ChannelNumber_e Channel, uint32_t SampleCnt )
{
  if( (Port == ADI_RX) && (Channel == ADI_CHANNEL_1) )
  {
    Xil_Out32(Base + ADRV9001_RX1_SSI_DISABLE_CNT, SampleCnt << 1);
  }
  else if( (Port == ADI_RX) && (Channel == ADI_CHANNEL_2) )
  {
    Xil_Out32(Base + ADRV9001_RX2_SSI_DISABLE_CNT, SampleCnt << 1);
  }
}

void AxiAdrv9001_SetEnable( uint32_t Base, adi_common_Port_e Port, adi_common_ChannelNumber_e Channel, bool Enabled )
{
  if( (Port == ADI_TX) && (Channel == ADI_CHANNEL_1) )
    Xil_Out32(Base + ADRV9001_TX1_EN_ADDR, (uint32_t)Enabled);
  else if( (Port == ADI_TX) && (Channel == ADI_CHANNEL_2) )
    Xil_Out32(Base + ADRV9001_TX2_EN_ADDR, (uint32_t)Enabled);
  else if( (Port == ADI_RX) && (Channel == ADI_CHANNEL_1) )
    Xil_Out32(Base + ADRV9001_RX1_EN_ADDR, (uint32_t)Enabled);
  else if( (Port == ADI_RX) && (Channel == ADI_CHANNEL_2) )
    Xil_Out32(Base + ADRV9001_RX2_EN_ADDR, (uint32_t)Enabled);
}

void AxiAdrv9001_SetDgpio( uint32_t Base, uint32_t Value )
{
  Xil_Out32(Base + ADRV9001_DGPIO_IO_ADDR, Value);
}

void AxiAdrv9001_GetDgpio( uint32_t Base, uint32_t *Value )
{
  *Value = Xil_In32(Base + ADRV9001_DGPIO_IO_ADDR);
}

void AxiAdrv9001_SetDgpioDir( uint32_t Base, uint32_t Value )
{
  Xil_Out32(Base + ADRV9001_DGPIO_DIR_ADDR, Value);
}

void AxiAdrv9001_GetDgpioDir( uint32_t Base, uint32_t *Value )
{
  *Value = Xil_In32(Base + ADRV9001_DGPIO_DIR_ADDR);
}

void AxiAdrv9001_SetTxDataPath( uint32_t Base, adi_common_ChannelNumber_e Channel, axi_adrv9001_data_path_t Value )
{
  if( Channel == ADI_CHANNEL_1 )
    Xil_Out32(Base + ADRV9001_TX1_DATA_PATH_ADDR, (uint32_t)Value);
  else
    Xil_Out32(Base + ADRV9001_TX2_DATA_PATH_ADDR, (uint32_t)Value);
}

void AxiAdrv9001_GetTxDataPath( uint32_t Base, adi_common_ChannelNumber_e Channel, axi_adrv9001_data_path_t *Value )
{
  if( Channel == ADI_CHANNEL_1 )
    *Value = Xil_In32(Base + ADRV9001_TX1_DATA_PATH_ADDR);
  else
    *Value = Xil_In32(Base + ADRV9001_TX2_DATA_PATH_ADDR);
}

void AxiAdrv9001_SetTxData( uint32_t Base, adi_common_ChannelNumber_e Channel, uint32_t Value )
{
  if( Channel == ADI_CHANNEL_1 )
    Xil_Out32(Base + ADRV9001_CH1_DATA_ADDR, Value);
  else
    Xil_Out32(Base + ADRV9001_CH2_DATA_ADDR, Value);
}

void AxiAdrv9001_GetRxData( uint32_t Base, adi_common_ChannelNumber_e Channel, uint32_t *Value )
{
  if( Channel == ADI_CHANNEL_1 )
    *Value = Xil_In32(Base + ADRV9001_CH1_DATA_ADDR);
  else
    *Value = Xil_In32(Base + ADRV9001_CH2_DATA_ADDR);
}

