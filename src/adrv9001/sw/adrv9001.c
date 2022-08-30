
#include <stdio.h>
#include <string.h>
#include "adrv9001.h"
#include "initialize.h"
#include "calibrate.h"
#include "prime.h"
#include "configure.h"
#include "sleep.h"
#include "adrv9001_pib.h"

#define ADI_CHANNEL_RX1_INIT_MASK               (0x40)
#define ADI_CHANNEL_RX2_INIT_MASK               (0x80)
#define ADI_CHANNEL_TX1_INIT_MASK               (0x04)
#define ADI_CHANNEL_TX2_INIT_MASK               (0x08)
#define ADRV9001_TEST_MODE_PATTERN        (0xABCDABCD)

int32_t Adrv9001_ClearError( adrv9001_t *Instance )
{
  adi_common_ErrorClear(&Instance->Device.common);

  return Adrv9001Status_Success;
}

int32_t Adrv9001_LoadProfile( adrv9001_t *Instance )
{
  int32_t status;

  Instance->PendingReboot = 0;

  if((status = initialize( &Instance->Device )) != 0)
    return Adrv9001Status_ProfileInitErr;

  if((status = calibrate( &Instance->Device )) != 0)
    return Adrv9001Status_ProfileCalErr;

  if((status = configure( &Instance->Device )) != 0)
    return Adrv9001Status_ProfileCfgErr;

  if((status = prime( &Instance->Device )) != 0)
    return Adrv9001Status_ToPrimedErr;

  if( Instance->Params->HwVer == 2 )
  {
    /* TCXO DAC    = ADI_ADRV9001_GPIO_ANALOG_00 */
    /* TCXO Enable = ADI_ADRV9001_GPIO_ANALOG_07 */
    /* Rx1A Enable = ADI_ADRV9001_GPIO_ANALOG_06 */
    /* Rx2A Enable = ADI_ADRV9001_GPIO_ANALOG_05 */
    if( adi_adrv9001_gpio_ManualAnalogOutput_Configure(&Instance->Device, ADI_ADRV9001_GPIO_ANALOG_PIN_NIBBLE_07_04 ) != 0 )
      return Adrv9001Status_GpioErr;

    /* Enable TCXO */
    if( adi_adrv9001_gpio_OutputPinLevel_Set(&Instance->Device, ADI_ADRV9001_GPIO_ANALOG_07, ADI_ADRV9001_GPIO_PIN_LEVEL_HIGH) != 0)
      return Adrv9001Status_GpioErr;

    /* Disable Rx1A */
    if( adi_adrv9001_gpio_OutputPinLevel_Set(&Instance->Device, ADI_ADRV9001_GPIO_ANALOG_06, ADI_ADRV9001_GPIO_PIN_LEVEL_LOW) != 0)
      return Adrv9001Status_GpioErr;

    /* Disable Rx2A */
    if( adi_adrv9001_gpio_OutputPinLevel_Set(&Instance->Device, ADI_ADRV9001_GPIO_ANALOG_05, ADI_ADRV9001_GPIO_PIN_LEVEL_LOW) != 0)
      return Adrv9001Status_GpioErr;

    /* Enable DAC */
    if((status = Adrv9001_EnableDac(Instance, 0, true )) != 0 )
      return status;

  }
  else if( Instance->Params->HwVer == 3 )
  {
    /* TCXO DAC    = ADI_ADRV9001_GPIO_ANALOG_03 */
    /* TCXO Enable = ADI_ADRV9001_GPIO_ANALOG_07 */
    /* Rx1A Enable = ADI_ADRV9001_GPIO_ANALOG_01 */
    /* Rx2A Enable = ADI_ADRV9001_GPIO_ANALOG_09 */
    /* Tx1 Enable  = ADI_ADRV9001_GPIO_ANALOG_00 */
    /* Tx2 Enable  = ADI_ADRV9001_GPIO_ANALOG_08 */

    if( adi_adrv9001_gpio_ManualAnalogOutput_Configure(&Instance->Device, ADI_ADRV9001_GPIO_ANALOG_PIN_NIBBLE_03_00 ) != 0 )
      return Adrv9001Status_GpioErr;

    if( adi_adrv9001_gpio_ManualAnalogOutput_Configure(&Instance->Device, ADI_ADRV9001_GPIO_ANALOG_PIN_NIBBLE_07_04 ) != 0 )
      return Adrv9001Status_GpioErr;

    if( adi_adrv9001_gpio_ManualAnalogOutput_Configure(&Instance->Device, ADI_ADRV9001_GPIO_ANALOG_PIN_NIBBLE_11_08 ) != 0 )
      return Adrv9001Status_GpioErr;

    /* Enable TCXO */
    if( adi_adrv9001_gpio_OutputPinLevel_Set(&Instance->Device, ADI_ADRV9001_GPIO_ANALOG_07, ADI_ADRV9001_GPIO_PIN_LEVEL_HIGH) != 0)
      return Adrv9001Status_GpioErr;

    /* Disable Rx1A */
    if( adi_adrv9001_gpio_OutputPinLevel_Set(&Instance->Device, ADI_ADRV9001_GPIO_ANALOG_01, ADI_ADRV9001_GPIO_PIN_LEVEL_LOW) != 0)
      return Adrv9001Status_GpioErr;

    /* Disable Rx2A */
    if( adi_adrv9001_gpio_OutputPinLevel_Set(&Instance->Device, ADI_ADRV9001_GPIO_ANALOG_09, ADI_ADRV9001_GPIO_PIN_LEVEL_LOW) != 0)
      return Adrv9001Status_GpioErr;

    /* Disable Tx1 */
    if( adi_adrv9001_gpio_OutputPinLevel_Set(&Instance->Device, ADI_ADRV9001_GPIO_ANALOG_00, ADI_ADRV9001_GPIO_PIN_LEVEL_LOW) != 0)
      return Adrv9001Status_GpioErr;

    /* Disable Tx2 */
    if( adi_adrv9001_gpio_OutputPinLevel_Set(&Instance->Device, ADI_ADRV9001_GPIO_ANALOG_08, ADI_ADRV9001_GPIO_PIN_LEVEL_LOW) != 0)
      return Adrv9001Status_GpioErr;

    /* Enable DAC */
    if(( status = Adrv9001_EnableDac(Instance, 3, true )) != 0 )
      return status;
  }

  /* Set TCXO to mid scale */
  if(Adrv9001_SetVcTcxo( Instance, 0.9 ) != 0)
    printf("Adrv9001 SetVcTcxo Failure\r\n");

  /* Calibrate SSI */
  if((status = Adrv9001_CalibrateSsiDelay( Instance, ADI_RX, ADI_CHANNEL_1 )) != 0)
    return status;

  if((status = Adrv9001_CalibrateSsiDelay( Instance, ADI_RX, ADI_CHANNEL_2 )) != 0)
    return status;

  if((status = Adrv9001_CalibrateSsiDelay( Instance, ADI_TX, ADI_CHANNEL_1 )) != 0)
    return status;

  if((status = Adrv9001_CalibrateSsiDelay( Instance, ADI_TX, ADI_CHANNEL_2 )) != 0)
    return status;

  if( adi_adrv9001_Radio_ChannelEnableMode_Set(&Instance->Device, ADI_TX, ADI_CHANNEL_1, ADI_ADRV9001_PIN_MODE) != 0)
    return Adrv9001Status_EnableModeErr;

  if( adi_adrv9001_Radio_ChannelEnableMode_Set(&Instance->Device, ADI_TX, ADI_CHANNEL_2, ADI_ADRV9001_PIN_MODE) != 0)
    return Adrv9001Status_EnableModeErr;

  if( adi_adrv9001_Radio_ChannelEnableMode_Set(&Instance->Device, ADI_RX, ADI_CHANNEL_1, ADI_ADRV9001_PIN_MODE) != 0)
    return Adrv9001Status_EnableModeErr;

  if( adi_adrv9001_Radio_ChannelEnableMode_Set(&Instance->Device, ADI_RX, ADI_CHANNEL_2, ADI_ADRV9001_PIN_MODE) != 0)
    return Adrv9001Status_EnableModeErr;

  printf("%s Version Information:\r\n","ADRV9002" );

  adi_adrv9001_SiliconVersion_t SiliconVer;
  if(adi_adrv9001_SiliconVersion_Get( &Instance->Device, &SiliconVer) == 0)
    printf("  -Silicon Version: %X%X\r\n", SiliconVer.major, SiliconVer.minor );
  else
    printf("  -Silicon Version: Failed\r\n" );

  adi_adrv9001_ArmVersion_t ArmVer;
  if(adi_adrv9001_arm_Version( &Instance->Device, &ArmVer) == 0)
    printf("  -Firmware Version: %u.%u.%u.%u\r\n",ArmVer.majorVer, ArmVer.minorVer, ArmVer.maintVer, ArmVer.rcVer );
  else
    printf("  -Firmware Version: Failed\r\n");

  adi_common_ApiVersion_t ApiVer;
  if(adi_adrv9001_ApiVersion_Get( &Instance->Device, &ApiVer) == 0)
    printf("  -API Version: %lu.%lu.%lu\r\n\r\n", ApiVer.major, ApiVer.minor, ApiVer.patch );
  else
    printf("  -API Version: Failed\r\n\r\n");

  return Adrv9001Status_Success;
}

int32_t Adrv9001_Initialize( adrv9001_t *Instance, adrv9001_init_t *Init )
{
  int32_t status;

  memset(Instance, 0, sizeof(adrv9001_t));

  Instance->Params = &Adrv9001Params;
  Instance->Params->HwVer = Init->HwVer;

  /* Assign Hal Reference to adrv9001 */
  Instance->Device.common.devHalInfo = (void*)Instance;

  /* Enable Logging */
  Instance->Device.common.error.logEnable = 1;

  /* Initialize Log Path */
  strcpy( Instance->Params->LogPath, Init->LogFilename );

  Instance->CtrlBase = Init->CtrlBase;

  /* Initialize PIB */
  if((status = Adrv9001Pib_Initialize( Instance )) != 0)
    return status;

  /* Initialize AXI ADRV9001 */
  if((status = AxiAdrv9001_Initialize( Instance->CtrlBase)) != 0)
    return status;

  /* Load Profile */
  if((status = Adrv9001_LoadProfile( Instance )) != 0)
    return status;

  return Adrv9001Status_Success;
}

int32_t Adrv9001_ToPrimed( adrv9001_t *Instance, adi_common_Port_e port, adi_common_ChannelNumber_e channel )
{
  int32_t status;
  adi_adrv9001_ChannelEnableMode_e mode;

  if(adi_adrv9001_Radio_ChannelEnableMode_Get( &Instance->Device, port, channel, &mode) != 0)
    return Adrv9001Status_ReadErr;

  /* Set PA/LNA Enable */
  if((status = Adrv9001_SetPaEnable(Instance, port, channel, false)) != 0 )
    return status;

  /* Set Enable Pin */
  AxiAdrv9001_SetEnable( Instance->CtrlBase, port, channel, false );

  if( mode == ADI_ADRV9001_SPI_MODE )
  {
    /* Set Radio State */
    if( adi_adrv9001_Radio_Channel_ToPrimed(&Instance->Device, port, channel) != 0 )
      return Adrv9001Status_ToEnabledErr;
  }

  return Adrv9001Status_Success;
}

int32_t Adrv9001_ToCalibrated( adrv9001_t *Instance, adi_common_Port_e port, adi_common_ChannelNumber_e channel )
{
  int32_t status;
  adi_adrv9001_ChannelEnableMode_e mode;

  if(adi_adrv9001_Radio_ChannelEnableMode_Get( &Instance->Device, port, channel, &mode) != 0)
    return Adrv9001Status_ReadErr;

  /* Set PA/LNA Enable */
  if((status = Adrv9001_SetPaEnable(Instance, port, channel, false)) != 0 )
    return status;

  /* Set Enable Pin */
  AxiAdrv9001_SetEnable( Instance->CtrlBase, port, channel, false );

  if( mode == ADI_ADRV9001_SPI_MODE )
  {
    /* Set Radio State */
    if( adi_adrv9001_Radio_Channel_ToCalibrated(&Instance->Device, port, channel) != 0 )
      return Adrv9001Status_ToCalErr;
  }

  return Adrv9001Status_Success;
}

int32_t Adrv9001_SetTddTiming( adrv9001_t *Instance, adi_common_Port_e port, adi_common_ChannelNumber_e channel, uint32_t SampleCnt )
{
  uint32_t SsiEnableCnt;
  uint32_t DisableCnt;
  uint32_t SsiDisableCnt;


  if( (port == ADI_TX) && (channel == ADI_CHANNEL_1) )
  {
    DisableCnt = Instance->Params->Tx1SsiEnableDly + SampleCnt + Instance->Params->Tx1DisableDly;
    SsiEnableCnt = Instance->Params->Tx1SsiEnableDly;
    SsiDisableCnt = DisableCnt;
  }
  else if( (port == ADI_TX) && (channel == ADI_CHANNEL_2) )
  {
    DisableCnt = Instance->Params->Tx2SsiEnableDly + SampleCnt + Instance->Params->Tx2DisableDly;
    SsiEnableCnt = Instance->Params->Tx2SsiEnableDly;
    SsiDisableCnt = DisableCnt;
  }
  else if( (port == ADI_RX) && (channel == ADI_CHANNEL_1) )
  {
    DisableCnt = Instance->Params->Rx1SsiEnableDly + SampleCnt;
    SsiEnableCnt = Instance->Params->Rx1SsiEnableDly;
    SsiDisableCnt = DisableCnt + Instance->Params->Rx1SsiDisableDly;
  }
  else if( (port == ADI_RX) && (channel == ADI_CHANNEL_2) )
  {
    DisableCnt = Instance->Params->Rx2SsiEnableDly + SampleCnt;
    SsiEnableCnt = Instance->Params->Rx2SsiEnableDly;
    SsiDisableCnt = DisableCnt + Instance->Params->Rx2SsiDisableDly;
  }


  if( SampleCnt == ADRV9001_TDD_ENABLE_DUR_FOREVER )
    AxiAdrv9001_SetDisableCnt( Instance->CtrlBase, port, channel, ADRV9001_TDD_ENABLE_DUR_FOREVER );
  else
    AxiAdrv9001_SetDisableCnt( Instance->CtrlBase, port, channel, DisableCnt );

  AxiAdrv9001_SetSsiEnableCnt( Instance->CtrlBase, port, channel, SsiEnableCnt );

  if( SampleCnt == ADRV9001_TDD_ENABLE_DUR_FOREVER )
    AxiAdrv9001_SetSsiDisableCnt( Instance->CtrlBase, port, channel, ADRV9001_TDD_ENABLE_DUR_FOREVER );
  else
    AxiAdrv9001_SetSsiDisableCnt( Instance->CtrlBase, port, channel, SsiDisableCnt );

  return Adrv9001Status_Success;

}

int32_t Adrv9001_ToRfEnabled( adrv9001_t *Instance, adi_common_Port_e port, adi_common_ChannelNumber_e channel, uint32_t SampleCnt )
{
  int32_t status;

  if( Instance->PendingReboot > 0 )
  {
    printf("ADRV9001 Reboot Pending...\r\n");
    Adrv9001_LoadProfile( Instance );
  }

  adi_adrv9001_ChannelEnableMode_e mode;
  if(adi_adrv9001_Radio_ChannelEnableMode_Get( &Instance->Device, port, channel, &mode) != 0)
    return Adrv9001Status_ReadErr;

  if( port == ADI_TX )
  {
    uint16_t attn;
    uint8_t boost;

    if( channel == ADI_CHANNEL_1 )
    {
      attn = Instance->Params->Tx1Attn;
      boost = Instance->Params->Tx1Boost;
    }
    else
    {
      attn = Instance->Params->Tx2Attn;
      boost = Instance->Params->Tx2Boost;
    }

    if( adi_adrv9001_Tx_Attenuation_Set(&Instance->Device, channel, attn) != 0 )
      return Adrv9001Status_TxAttnErr;

    if( adi_adrv9001_Tx_OutputPowerBoost_Set(&Instance->Device, channel, boost ) != 0 )
      return Adrv9001Status_TxBoostErr;
  }


  /* Set PA/LNA Enable */
  if((status = Adrv9001_SetPaEnable(Instance, port, channel, true)) != 0 )
    return status;

  /* Set TDD Timing */
  Adrv9001_SetTddTiming(Instance, port, channel, SampleCnt);

  /* Set Enable Pin */
  AxiAdrv9001_SetEnable( Instance->CtrlBase, port, channel, true );

  if( mode == ADI_ADRV9001_SPI_MODE )
  {
    /* Set Radio State */
    if( adi_adrv9001_Radio_Channel_ToRfEnabled(&Instance->Device, port, channel) != 0 )
      return Adrv9001Status_ToEnabledErr;
  }

  return Adrv9001Status_Success;
}

int32_t Adrv9001_PerformSsiSweep( adrv9001_t *Instance, adi_common_Port_e port, adi_common_ChannelNumber_e channel, uint16_t results[8][8] )
{
  int32_t status;

  if((status = Adrv9001_ToPrimed( Instance, port, channel )) != 0)
    return status;

  for(uint8_t clkDly = 0; clkDly < 8; clkDly++)
  {
    if((status = Adrv9001_SetSsiClkDelay( Instance, port, channel, clkDly )) != 0 )
      return status;

    for(uint8_t dataDly = 0; dataDly < 8; dataDly++)
    {
      if((status = Adrv9001_SetSsiDataDelay( Instance, port, channel, dataDly )) != 0 )
        return status;

      if(port == ADI_TX)
      {
        adi_adrv9001_TxSsiTestModeCfg_t Cfg =
        {
            .fixedDataPatternToCheck = ADRV9001_TEST_MODE_PATTERN,
            .testData = ADI_ADRV9001_SSI_TESTMODE_DATA_FIXED_PATTERN
        };

        if( adi_adrv9001_Ssi_Tx_TestMode_Configure(&Instance->Device, channel, ADI_ADRV9001_SSI_TYPE_LVDS, ADI_ADRV9001_SSI_FORMAT_16_BIT_I_Q_DATA, &Cfg ) != 0)
          return Adrv9001Status_SsiTestModeErr;

        AxiAdrv9001_SetTxDataPath( Instance->CtrlBase, channel, 1);

        AxiAdrv9001_SetTxData( Instance->CtrlBase, channel, ADRV9001_TEST_MODE_PATTERN);
      }
      else
      {
        adi_adrv9001_RxSsiTestModeCfg_t Cfg =
        {
            .fixedDataPatternToTransmit = ADRV9001_TEST_MODE_PATTERN,
            .testData = ADI_ADRV9001_SSI_TESTMODE_DATA_FIXED_PATTERN
        };

        if( adi_adrv9001_Ssi_Rx_TestMode_Configure(&Instance->Device, channel, ADI_ADRV9001_SSI_TYPE_LVDS, ADI_ADRV9001_SSI_FORMAT_16_BIT_I_Q_DATA, &Cfg ) != 0)
          return Adrv9001Status_SsiTestModeErr;
      }

      if((status = Adrv9001_ToRfEnabled( Instance, port, channel, ADRV9001_TDD_ENABLE_DUR_FOREVER)) != 0)
        return status;

      usleep(1000);

      if( port == ADI_TX )
      {
        /* Read Test Mode Status */
        adi_adrv9001_TxSsiTestModeCfg_t Cfg =
        {
            .fixedDataPatternToCheck = ADRV9001_TEST_MODE_PATTERN,
            .testData = ADI_ADRV9001_SSI_TESTMODE_DATA_FIXED_PATTERN
        };

        adi_adrv9001_TxSsiTestModeStatus_t s;
        if( adi_adrv9001_Ssi_Tx_TestMode_Status_Inspect( &Instance->Device, channel, ADI_ADRV9001_SSI_TYPE_LVDS, ADI_ADRV9001_SSI_FORMAT_16_BIT_I_Q_DATA, &Cfg, &s ) != 0)
          return Adrv9001Status_SsiTestModeErr;

        if( s.dataError != 0 )
          results[clkDly][dataDly] = 0;
        else
          results[clkDly][dataDly] = 1;
      }
      else
      {
        uint32_t Data;
        AxiAdrv9001_GetRxData( Instance->CtrlBase, channel, &Data );

        if( Data == ADRV9001_TEST_MODE_PATTERN)
          results[clkDly][dataDly] = 1;
        else
          results[clkDly][dataDly] = 0;
      }

      if((status = Adrv9001_ToPrimed( Instance, port, channel )) != 0)
        return status;

      AxiAdrv9001_SetTxDataPath(  Instance->CtrlBase, channel, 0);
    }
  }

  adi_adrv9001_RxSsiTestModeCfg_t RxCfg = { .testData = ADI_ADRV9001_SSI_TESTMODE_DATA_NORMAL };
  if( adi_adrv9001_Ssi_Rx_TestMode_Configure(&Instance->Device, channel, ADI_ADRV9001_SSI_TYPE_LVDS, ADI_ADRV9001_SSI_FORMAT_16_BIT_I_Q_DATA, &RxCfg ) != 0)
    return Adrv9001Status_SsiTestModeErr;

  adi_adrv9001_TxSsiTestModeCfg_t TxCfg = { .testData = ADI_ADRV9001_SSI_TESTMODE_DATA_NORMAL };
  if( adi_adrv9001_Ssi_Tx_TestMode_Configure(&Instance->Device, channel, ADI_ADRV9001_SSI_TYPE_LVDS, ADI_ADRV9001_SSI_FORMAT_16_BIT_I_Q_DATA, &TxCfg ) != 0)
    return Adrv9001Status_SsiTestModeErr;

  return status;
}

static int32_t Adrv9001_FindBestSsiDelay( uint16_t results[8][8], uint8_t *DataIdx, uint8_t *ClkIdx )
{
  for (int y = 0; y < 8; y++)
  {
    /* Find longest string of passes */
    for( int x = 0; x < 8; x++)
    {
      if( results[y][x] > 0 )
      {
        for( int xr = (x + 1); xr < 8; xr++)
        {
          if( results[y][xr] > 0 )
            results[y][x] += 128 >> (xr - x - 1);
          else
            break;
        }

        for( int xl = (x - 1); xl > -1; xl--)
        {
          if( results[y][xl] > 0 )
            results[y][x] += 128 >> (x - xl - 1);
          else
            break;
        }
      }
    }
  }

  uint16_t max = 0;
  for(uint8_t clkDelay = 0; clkDelay < 8; clkDelay++)
  {
    for(uint8_t dataDelay = 0; dataDelay < 8; dataDelay++)
    {
      if( results[clkDelay][dataDelay] > max )
      {
        max = results[clkDelay][dataDelay];
        *ClkIdx = clkDelay;
        *DataIdx = dataDelay;
      }
    }
  }

  if( max == 0 )
    return Adrv9001Status_SsiSweepErr;

  return Adrv9001Status_Success;
}

int32_t Adrv9001_CalibrateSsiDelay( adrv9001_t *Instance, adi_common_Port_e port, adi_common_ChannelNumber_e channel )
{
  int32_t status;
  uint16_t results[8][8];
  uint8_t clkMax = 0;
  uint8_t dataMax = 0;

  if((status = Adrv9001_PerformSsiSweep( Instance, port, channel, results )) != 0)
    return status;

  if((status = Adrv9001_FindBestSsiDelay( results, &dataMax, &clkMax )) != 0)
    return status;

  if((status = Adrv9001_SetSsiDataDelay( Instance, port, channel, dataMax )) != 0)
    return status;

  if((status = Adrv9001_SetSsiClkDelay( Instance, port, channel, clkMax )) != 0)
    return status;

  return 0;
}

int32_t Adrv9001_SetPaEnable( adrv9001_t *Instance, adi_common_Port_e port, adi_common_ChannelNumber_e channel, bool Enable )
{
  adi_adrv9001_GpioPin_e pin;
  adi_adrv9001_GpioPinLevel_e level = Enable? ADI_ADRV9001_GPIO_PIN_LEVEL_HIGH : ADI_ADRV9001_GPIO_PIN_LEVEL_LOW;

  /* Check if Rx */
  if( (port == ADI_RX) && (channel == ADI_CHANNEL_1) )
  {
    if( Instance->Params->HwVer == 2 )
      pin = ADI_ADRV9001_GPIO_ANALOG_06;
    else if( Instance->Params->HwVer == 3 )
      pin = ADI_ADRV9001_GPIO_ANALOG_01;
    else
      return Adrv9001Status_InvalidPaEnable;
  }
  else if( (port == ADI_RX) && (channel == ADI_CHANNEL_2) )
  {
    if( Instance->Params->HwVer == 2 )
      pin = ADI_ADRV9001_GPIO_ANALOG_05;
    else if( Instance->Params->HwVer == 3 )
      pin = ADI_ADRV9001_GPIO_ANALOG_09;
    else
      return Adrv9001Status_InvalidPaEnable;
  }
  else if( (port == ADI_TX) && (channel == ADI_CHANNEL_1) )
  {
    if( Instance->Params->HwVer == 2 )
      return Adrv9001Status_Success;
    else if( Instance->Params->HwVer == 3 )
      pin = ADI_ADRV9001_GPIO_ANALOG_00;
    else
      return Adrv9001Status_InvalidPaEnable;
  }
  else
  {
    if( Instance->Params->HwVer == 2 )
      return Adrv9001Status_Success;
    else if( Instance->Params->HwVer == 3 )
      pin = ADI_ADRV9001_GPIO_ANALOG_08;
    else
      return Adrv9001Status_InvalidPaEnable;
  }

  if( adi_adrv9001_gpio_OutputPinLevel_Set(&Instance->Device, pin, level) != 0)
    return Adrv9001Status_GpioErr;

  return Adrv9001Status_Success;
}

int32_t Adrv9001_SetSsiDataDelay( adrv9001_t *Instance, adi_common_Port_e port, adi_common_ChannelNumber_e channel, uint8_t Delay )
{
  adi_adrv9001_ChannelState_e State;

  if( adi_adrv9001_Radio_Channel_State_Get( &Instance->Device, port, channel, &State ) != 0)
    return Adrv9001Status_SsiSetErr;

  if( State == ADI_ADRV9001_CHANNEL_RF_ENABLED )
    return Adrv9001Status_SsiSetErr;

  adi_adrv9001_SsiCalibrationCfg_t cal;

  if( adi_adrv9001_Ssi_Delay_Inspect(&Instance->Device, ADI_ADRV9001_SSI_TYPE_LVDS, &cal ) != 0)
    return Adrv9001Status_SsiSetErr;

  if((port == ADI_RX) && (channel == ADI_CHANNEL_1))
  {
    cal.rxIDataDelay[0] = Delay;
    cal.rxQDataDelay[0] = Delay;
    cal.rxStrobeDelay[0] = Delay;
  }
  else if((port == ADI_RX) && (channel == ADI_CHANNEL_2))
  {
    cal.rxIDataDelay[1] = Delay;
    cal.rxQDataDelay[1] = Delay;
    cal.rxStrobeDelay[1] = Delay;
  }
  else if((port == ADI_TX) && (channel == ADI_CHANNEL_1))
  {
    cal.txIDataDelay[0] = Delay;
    cal.txQDataDelay[0] = Delay;
    cal.txStrobeDelay[0] = Delay;
  }
  else if((port == ADI_TX) && (channel == ADI_CHANNEL_2))
  {
    cal.txIDataDelay[1] = Delay;
    cal.txQDataDelay[1] = Delay;
    cal.txStrobeDelay[1] = Delay;
  }

  if( adi_adrv9001_Ssi_Delay_Configure(&Instance->Device, ADI_ADRV9001_SSI_TYPE_LVDS, &cal) != 0)
    return Adrv9001Status_SsiSetErr;

  return Adrv9001Status_Success;
}

int32_t Adrv9001_SetSsiClkDelay( adrv9001_t *Instance, adi_common_Port_e port, adi_common_ChannelNumber_e channel, uint8_t Delay )
{
  adi_adrv9001_ChannelState_e State;

  if( adi_adrv9001_Radio_Channel_State_Get( &Instance->Device, port, channel, &State ) != 0)
    return Adrv9001Status_SsiSetErr;

  if( State == ADI_ADRV9001_CHANNEL_RF_ENABLED )
    return Adrv9001Status_SsiSetErr;

  adi_adrv9001_SsiCalibrationCfg_t cal;

  if( adi_adrv9001_Ssi_Delay_Inspect(&Instance->Device, ADI_ADRV9001_SSI_TYPE_LVDS, &cal) != 0)
    return Adrv9001Status_SsiSetErr;

  if((port == ADI_RX) && (channel == ADI_CHANNEL_1))
  {
    cal.rxClkDelay[0] = Delay;
  }
  else if((port == ADI_RX) && (channel == ADI_CHANNEL_2))
  {
    cal.rxClkDelay[1] = Delay;
  }
  else if((port == ADI_TX) && (channel == ADI_CHANNEL_1))
  {
    cal.txClkDelay[0] = Delay;
  }
  else if((port == ADI_TX) && (channel == ADI_CHANNEL_2))
  {
    cal.txClkDelay[1] = Delay;
  }

  if( adi_adrv9001_Ssi_Delay_Configure(&Instance->Device, ADI_ADRV9001_SSI_TYPE_LVDS, &cal) != 0)
    return Adrv9001Status_SsiSetErr;

  return Adrv9001Status_Success;
}

int32_t Adrv9001_EnableDac( adrv9001_t *Instance, uint8_t Id, bool Enable )
{
  adi_adrv9001_GpioSignal_e signal;

  adi_adrv9001_GpioCfg_t Cfg = {
      .polarity = ADI_ADRV9001_GPIO_POLARITY_NORMAL,
      .master = ADI_ADRV9001_GPIO_MASTER_ADRV9001
  };

  switch( Id )
  {
    case 0 : signal = ADI_ADRV9001_GPIO_SIGNAL_AUX_DAC_0; Cfg.pin = ADI_ADRV9001_GPIO_ANALOG_00; break;
    case 1 : signal = ADI_ADRV9001_GPIO_SIGNAL_AUX_DAC_1; Cfg.pin = ADI_ADRV9001_GPIO_ANALOG_01; break;
    case 2 : signal = ADI_ADRV9001_GPIO_SIGNAL_AUX_DAC_2; Cfg.pin = ADI_ADRV9001_GPIO_ANALOG_02; break;
    case 3 : signal = ADI_ADRV9001_GPIO_SIGNAL_AUX_DAC_3; Cfg.pin = ADI_ADRV9001_GPIO_ANALOG_03; break;
    default: return Adrv9001Status_DacErr;
  }

  if( adi_adrv9001_gpio_Configure(&Instance->Device, signal, &Cfg) != 0)
    return Adrv9001Status_GpioErr;

  if( adi_adrv9001_AuxDac_Configure(&Instance->Device, Id, Enable) != 0 )
    return Adrv9001Status_GpioErr;

  return Adrv9001Status_Success;
}

int32_t Adrv9001_SetVcTcxo( adrv9001_t *Instance, float Voltage )
{
  uint16_t Value = (uint16_t)((((Voltage - 0.900)/ 1.700) * 4096.000) + 2048);

  if( Instance->Params->HwVer == 2 )
  {
    if( adi_adrv9001_AuxDac_Code_Set(&Instance->Device, ADI_ADRV9001_AUXDAC0, Value) != 0)
      return Adrv9001Status_DacErr;
  }
  else if( Instance->Params->HwVer == 3 )
  {
    if( adi_adrv9001_AuxDac_Code_Set(&Instance->Device, ADI_ADRV9001_AUXDAC3, Value) != 0 )
      return Adrv9001Status_DacErr;
  }
  else
  {
    return Adrv9001Status_DacErr;
  }

  return Adrv9001Status_Success;
}

int32_t Adrv9001_GetVcTcxo( adrv9001_t *Instance, float *Voltage )
{
  uint16_t Value;

  if( Instance->Params->HwVer == 2 )
  {
    if( adi_adrv9001_AuxDac_Code_Get(&Instance->Device, ADI_ADRV9001_AUXDAC0, &Value) != 0 )
      return Adrv9001Status_DacErr;
  }
  else if( Instance->Params->HwVer == 3 )
  {
    if( adi_adrv9001_AuxDac_Code_Get(&Instance->Device, ADI_ADRV9001_AUXDAC3, &Value) != 0)
      return Adrv9001Status_DacErr;
  }
  else
  {
    return Adrv9001Status_DacErr;
  }

  *Voltage = 0.9000 + (((float)Value - 2048.000) / 4096.000) * 1.7;

  return Adrv9001Status_Success;
}

int32_t Adrv9001_LogWrite(void *devHalCfg, uint32_t logLevel, const char *comment, va_list argp)
{
  adrv9001_t *Adrv9001 = (adrv9001_t*)devHalCfg;
  UINT len = 0;

  char *str = calloc(1,1024);

  vsnprintf( str, 1023, comment, argp );

  if(f_write(&Adrv9001->LogFil, str, strlen(str), (UINT*)&len) != FR_OK)
    return Adrv9001Status_LogErr;

  f_sync(&Adrv9001->LogFil);

  free(str);

  return Adrv9001Status_Success;
}

int32_t Adrv9001_DelayUs(void *devHalCfg, uint32_t time_us)
{
  usleep(time_us);

  return Adrv9001Status_Success;
}

int32_t Adrv9001_Open( void *devHalCfg )
{
  adrv9001_t *Adrv9001 = (adrv9001_t*)devHalCfg;

  XSpiPs_Config *SpiConfig;

  if((SpiConfig = XSpiPs_LookupConfig(ADRV9001_SPI_DEVICE_ID)) == NULL)
    return Adrv9001Status_SpiErr;

  if( XSpiPs_CfgInitialize(&Adrv9001->Spi, SpiConfig, SpiConfig->BaseAddress) != 0)
    return Adrv9001Status_SpiErr;

  if( XSpiPs_SelfTest(&Adrv9001->Spi) != 0)
    return Adrv9001Status_SpiErr;

  XSpiPs_SetOptions(&Adrv9001->Spi, XSPIPS_MANUAL_START_OPTION | XSPIPS_MASTER_OPTION | XSPIPS_FORCE_SSELECT_OPTION);

  XSpiPs_SetClkPrescaler(&Adrv9001->Spi, XSPIPS_CLK_PRESCALE_8);

  XSpiPs_SetOptions(&Adrv9001->Spi, XSPIPS_MASTER_OPTION | XSPIPS_FORCE_SSELECT_OPTION );

  if( XSpiPs_SetSlaveSelect(&Adrv9001->Spi, ADRV9001_SPI_CS) != 0 )
    return Adrv9001Status_SpiErr;

  /* Delete PHY Log file */
  f_unlink(Adrv9001->Params->LogPath);

  /* Open File */
  if( f_open(&Adrv9001->LogFil, Adrv9001->Params->LogPath, FA_CREATE_NEW | FA_WRITE | FA_READ) != FR_OK)
    return ADI_COMMON_ACT_WARN_RESET_LOG;

  /* Point to beginning of file */
  f_lseek(&Adrv9001->LogFil, 0);

  f_sync(&Adrv9001->LogFil);

  return Adrv9001Status_Success;
}

int32_t Adrv9001_Close( void *devHalCfg )
{
  adrv9001_t *Adrv9001 = (adrv9001_t*)devHalCfg;

  f_sync(&Adrv9001->LogFil);

  f_close(&Adrv9001->LogFil);

  return Adrv9001Status_Success;
}

int32_t Adrv9001_SpiWrite( void *devHalCfg, const uint8_t txData[], uint32_t numTxBytes )
{
  static const int32_t MAX_SIZE = 4096;
  uint32_t toWrite = 0;
  int32_t remaining = numTxBytes;

  adrv9001_t *Adrv9001 = (adrv9001_t*)devHalCfg;

  do
  {
    toWrite = (remaining > MAX_SIZE) ? MAX_SIZE : remaining;
    uint8_t *Buf = (uint8_t*)&txData[numTxBytes - remaining];

    if( XSpiPs_PolledTransfer(&Adrv9001->Spi, Buf, Buf, toWrite) != 0)
      return Adrv9001Status_SpiErr;

    remaining -= toWrite;

  } while (remaining > 0);

  return Adrv9001Status_Success;
}

int32_t Adrv9001_SpiRead( void *devHalCfg, const uint8_t txData[], uint8_t rxData[], uint32_t numRxBytes )
{

  static const int32_t MAX_SIZE = 4096;
  uint32_t toWrite = 0;
  int32_t remaining = numRxBytes;

  memcpy(rxData, txData, numRxBytes);

  adrv9001_t *Adrv9001 = (adrv9001_t*)devHalCfg;

  do
  {
    toWrite = (remaining > MAX_SIZE) ? MAX_SIZE : remaining;

    uint8_t *Buf = (uint8_t*)&rxData[numRxBytes - remaining];

    if( XSpiPs_PolledTransfer(&Adrv9001->Spi, Buf, Buf, toWrite) != 0)
      return Adrv9001Status_SpiErr;

    remaining -= toWrite;

  } while (remaining > 0);

  return Adrv9001Status_Success;
}

int32_t Adrv9001_ResetbPinSet( void *devHalCfg, uint8_t pinLevel )
{
  adrv9001_t *Adrv9001 = (adrv9001_t*)devHalCfg;

  AxiAdrv9001_ResetbPinSet( Adrv9001->CtrlBase, pinLevel );

  return Adrv9001Status_Success;
}


int32_t(*adi_common_hal_LogWrite)(void *devHalCfg, uint32_t logLevel, const char *formatStr, va_list argp)    = Adrv9001_LogWrite;
int32_t(*adi_common_hal_Wait_us)(void *devHalCfg, uint32_t time_us)                                           = Adrv9001_DelayUs;
int32_t(*adi_adrv9001_hal_open)(void *devHalCfg)                                                              = Adrv9001_Open;
int32_t(*adi_adrv9001_hal_close)(void *devHalCfg)                                                             = Adrv9001_Close;
int32_t(*adi_hal_SpiWrite)(void *devHalCfg, const uint8_t txData[], uint32_t numTxBytes)                      = Adrv9001_SpiWrite;
int32_t(*adi_hal_SpiRead)(void *devHalCfg, const uint8_t txData[], uint8_t rxData[], uint32_t numRxBytes)     = Adrv9001_SpiRead;
int32_t(*adi_adrv9001_hal_resetbPin_set)(void *devHalCfg, uint8_t pinLevel)                                   = Adrv9001_ResetbPinSet;

int32_t(*adi_hal_ArmImagePageGet)(void *devHalCfg, const char *armImagePath, uint32_t pageIndex, uint32_t pageSize, uint8_t *rdBuff) = NULL;
int32_t(*adi_hal_StreamImagePageGet)(void *devHalCfg, const char *streamImagePath, uint32_t pageIndex, uint32_t pageSize, uint8_t *rdBuff) = NULL;
int32_t(*adi_hal_RxGainTableEntryGet)(void *devHalCfg, const char *rxGainTablePath, uint16_t lineCount, uint8_t *gainIndex, uint8_t *rxFeGain, uint8_t *tiaControl, uint8_t *adcControl, uint8_t *extControl, uint16_t *phaseOffset, int16_t *digGain) = NULL;
int32_t(*adi_hal_TxAttenTableEntryGet)(void *devHalCfg, const char *txAttenTablePath, uint16_t lineCount, uint16_t *attenIndex, uint8_t *txAttenHp, uint16_t *txAttenMult) = NULL;



