/***************************************************************************//**
*  \addtogroup ADRV9001
*   @{
*******************************************************************************/
/***************************************************************************//**
*  \file       adrv9001.c
*
*  \details    This file contains the ADRV9001 implementation
*
*  \copyright
*
*  Copyright 2021(c) NextGen RF Design, Inc.
*
*  All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions are met:
*   - Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*   - Redistributions in binary form must reproduce the above copyright notice,
*     this list of conditions and the following disclaimer in the documentation
*     and/or other materials provided with the distribution.
*   - The use of this software may or may not infringe the patent rights of one
*     or more patent holders.  This license does not release you from the
*     requirement that you obtain separate licenses from these patent holders
*     to use this software.
*   - Use of the software either in source or binary form, must be run on or
*     directly connected to a NextGen RF Design, Inc. product.
*
*  THIS SOFTWARE IS PROVIDED BY NEXTGEN RF DESIGN "AS IS" AND ANY EXPRESS OR
*  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-INFRINGEMENT,
*  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
*  EVENT SHALL NEXTGEN RF DESIGN BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
*  INTELLECTUAL PROPERTY RIGHTS, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
*  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
*  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
*  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*******************************************************************************/
#include <stdio.h>
#include <string.h>
#include "adrv9001.h"
#include "initialize.h"
#include "calibrate.h"
#include "prime.h"
#include "configure.h"
#include "sleep.h"

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

  if((status = adi_adrv9001_Tx_Attenuation_Set(&Instance->Device, ADI_CHANNEL_1, Instance->Params->Tx1Attn)) != 0)
    return Adrv9001Status_TxAttnErr;

  if((status = adi_adrv9001_Tx_Attenuation_Set(&Instance->Device, ADI_CHANNEL_2, Instance->Params->Tx2Attn)) != 0)
    return Adrv9001Status_TxAttnErr;

  if((status = adi_adrv9001_Tx_OutputPowerBoost_Set(&Instance->Device, ADI_CHANNEL_1, Instance->Params->Tx1Boost)) != 0)
    return Adrv9001Status_TxBoostErr;

  if((status = adi_adrv9001_Tx_OutputPowerBoost_Set(&Instance->Device, ADI_CHANNEL_2, Instance->Params->Tx2Boost)) != 0)
    return Adrv9001Status_TxBoostErr;

  /* Configure Analog Outputs */
  if( adi_adrv9001_gpio_ManualAnalogOutput_Configure(&Instance->Device, ADI_ADRV9001_GPIO_ANALOG_PIN_NIBBLE_03_00 ) != 0 )
    return Adrv9001Status_GpioErr;

  /* Configure Analog Outputs */
  if( adi_adrv9001_gpio_ManualAnalogOutput_Configure(&Instance->Device, ADI_ADRV9001_GPIO_ANALOG_PIN_NIBBLE_07_04 ) != 0 )
    return Adrv9001Status_GpioErr;

  /* Configure Analog Outputs */
  if( adi_adrv9001_gpio_ManualAnalogOutput_Configure(&Instance->Device, ADI_ADRV9001_GPIO_ANALOG_PIN_NIBBLE_11_08 ) != 0 )
    return Adrv9001Status_GpioErr;

  /* Enable TCXO */
  if( Instance->Params->TcxoEnablePin != ADI_ADRV9001_GPIO_UNASSIGNED )
	  if( adi_adrv9001_gpio_OutputPinLevel_Set(&Instance->Device, Instance->Params->TcxoEnablePin, ADI_ADRV9001_GPIO_PIN_LEVEL_HIGH) != 0)
		  return Adrv9001Status_GpioErr;

  /* Disable Rx1A */
  if( Instance->Params->Rx1FrontendEnablePin != ADI_ADRV9001_GPIO_UNASSIGNED )
	  if( adi_adrv9001_gpio_OutputPinLevel_Set(&Instance->Device, Instance->Params->Rx1FrontendEnablePin, ADI_ADRV9001_GPIO_PIN_LEVEL_LOW) != 0)
		  return Adrv9001Status_GpioErr;

  /* Disable Rx2A */
  if( Instance->Params->Rx2FrontendEnablePin != ADI_ADRV9001_GPIO_UNASSIGNED )
	  if( adi_adrv9001_gpio_OutputPinLevel_Set(&Instance->Device, Instance->Params->Rx2FrontendEnablePin, ADI_ADRV9001_GPIO_PIN_LEVEL_LOW) != 0)
		  return Adrv9001Status_GpioErr;

  /* Disable Tx1 */
  if( Instance->Params->Tx1FrontendEnablePin != ADI_ADRV9001_GPIO_UNASSIGNED )
	  if( adi_adrv9001_gpio_OutputPinLevel_Set(&Instance->Device, Instance->Params->Tx1FrontendEnablePin, ADI_ADRV9001_GPIO_PIN_LEVEL_LOW) != 0)
		  return Adrv9001Status_GpioErr;

  /* Disable Tx2 */
  if( Instance->Params->Tx2FrontendEnablePin != ADI_ADRV9001_GPIO_UNASSIGNED )
	  if( adi_adrv9001_gpio_OutputPinLevel_Set(&Instance->Device, Instance->Params->Tx2FrontendEnablePin, ADI_ADRV9001_GPIO_PIN_LEVEL_LOW) != 0)
		  return Adrv9001Status_GpioErr;

  /* Enable DAC */
  if(( status = Adrv9001_EnableDac(Instance, Instance->Params->TcxoDacChannel, true )) != 0 )
    return status;

  /* Set TCXO to mid scale */
  if(Adrv9001_SetVcTcxo( Instance, 0.9 ) != 0)
    printf("Adrv9001 SetVcTcxo Failure\r\n");

  if( Adrv9001_SetEnableMode(Instance, ADI_TX, ADI_CHANNEL_1, Instance->Params->Tx1EnableMode) != 0)
    return Adrv9001Status_EnableModeErr;

  if( Adrv9001_SetEnableMode(Instance, ADI_TX, ADI_CHANNEL_2, Instance->Params->Tx2EnableMode) != 0)
    return Adrv9001Status_EnableModeErr;

  if( Adrv9001_SetEnableMode(Instance, ADI_RX, ADI_CHANNEL_1, Instance->Params->Rx1EnableMode) != 0)
    return Adrv9001Status_EnableModeErr;

  if( Adrv9001_SetEnableMode(Instance, ADI_RX, ADI_CHANNEL_2, Instance->Params->Rx2EnableMode) != 0)
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

  /* Calibrate SSI */
  if(Adrv9001_CalibrateSsiDelay( Instance, ADI_TX, ADI_CHANNEL_1 ) != 0)
    printf("ADRV9001 Tx1 SSI Calibration Failure\r\n");

  if(Adrv9001_CalibrateSsiDelay( Instance, ADI_TX, ADI_CHANNEL_2 ) != 0)
    printf("ADRV9001 Tx2 SSI Calibration Failure\r\n");

  if( Adrv9001_CalibrateSsiDelay( Instance, ADI_RX, ADI_CHANNEL_1 ) != 0)
    printf("ADRV9001 Rx1 SSI Calibration Failure\r\n");

  if(Adrv9001_CalibrateSsiDelay( Instance, ADI_RX, ADI_CHANNEL_2 ) != 0)
    printf("ADRV9001 Rx2 SSI Calibration Failure\r\n");

  return Adrv9001Status_Success;
}

int32_t Adrv9001_SetEnableMode( adrv9001_t *Instance, adi_common_Port_e port, adi_common_ChannelNumber_e channel, adi_adrv9001_ChannelEnableMode_e mode )
{
  if( adi_adrv9001_Radio_ChannelEnableMode_Set(&Instance->Device, port, channel, mode) != 0)
    return Adrv9001Status_EnableModeErr;

  AxiAdrv9001_SetEnableMode( &Instance->Axi, port, channel, mode );

  return Adrv9001Status_Success;
}

int32_t Adrv9001_Initialize( adrv9001_t *Instance, adrv9001_init_t *Init )
{
  int32_t status;

  memset(Instance, 0, sizeof(adrv9001_t));

  Instance->Params = &Adrv9001Params;
  Instance->Params->Rx1FrontendEnablePin = Init->Rx1FrontendEnablePin;
  Instance->Params->Rx2FrontendEnablePin = Init->Rx2FrontendEnablePin;
  Instance->Params->Tx1FrontendEnablePin = Init->Tx1FrontendEnablePin;
  Instance->Params->Tx2FrontendEnablePin = Init->Tx2FrontendEnablePin;
  Instance->Params->TcxoEnablePin = Init->TcxoEnablePin;
  Instance->Params->TcxoDacChannel = Init->TcxoDacChannel;
  Instance->IrqInstance = Init->IrqInstance;

  /* Assign Hal Reference to adrv9001 */
  Instance->Device.common.devHalInfo = (void*)Instance;

  /* Enable Logging */
  Instance->Device.common.error.logEnable = 1;

#ifdef ADRV9001_USE_FS
  /* Initialize Log Path */
  if( Init->LogFilename != NULL )
    strcpy( Instance->Params->LogPath, Init->LogFilename );
  else
    Instance->Params->LogPath[0] = 0;
#endif

  axi_adrv9001_init_t AxiInit = {
      .Base = Init->AxiBase,
      .IrqId = Init->AxiIrqId,
      .IrqInstance = Init->IrqInstance,
  };

  /* Initialize AXI ADRV9001 */
  if((status = AxiAdrv9001_Initialize( &Instance->Axi, &AxiInit )) != 0)
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

  if( mode == ADI_ADRV9001_SPI_MODE )
  {
    /* Set Radio State */
    if( adi_adrv9001_Radio_Channel_ToCalibrated(&Instance->Device, port, channel) != 0 )
      return Adrv9001Status_ToCalErr;
  }
  else
  {
    return Adrv9001Status_InvalidEnableMode;
  }

  return Adrv9001Status_Success;
}

int32_t Adrv9001_ToRfEnabled( adrv9001_t *Instance, adi_common_Port_e port, adi_common_ChannelNumber_e channel )
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

  /* Set PA/LNA Enable */
  if((status = Adrv9001_SetPaEnable(Instance, port, channel, true)) != 0 )
    return status;

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

  adi_adrv9001_ChannelEnableMode_e mode;

  if(adi_adrv9001_Radio_ChannelEnableMode_Get( &Instance->Device, port, channel, &mode) != 0)
    return Adrv9001Status_ReadErr;

  if( Adrv9001_SetEnableMode(Instance, port, channel, ADI_ADRV9001_SPI_MODE) != 0)
    return Adrv9001Status_EnableModeErr;

  if((status = Adrv9001_ToPrimed( Instance, port, channel )) != 0)
    return status;

  if( port == ADI_TX )
  {
    AxiAdrv9001_SetTxDataPath( &Instance->Axi, channel, 1);

    AxiAdrv9001_SetTxData( &Instance->Axi, channel, ADRV9001_TEST_MODE_PATTERN);
  }

  usleep(1000);

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

      if((status = Adrv9001_ToRfEnabled( Instance, port, channel )) != 0)
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
        AxiAdrv9001_GetRxData( &Instance->Axi, channel, &Data );
        AxiAdrv9001_GetRxData( &Instance->Axi, channel, &Data );

        if( Data == ADRV9001_TEST_MODE_PATTERN)
          results[clkDly][dataDly] = 1;
        else
          results[clkDly][dataDly] = 0;
      }

      if((status = Adrv9001_ToPrimed( Instance, port, channel )) != 0)
        return status;
    }
  }

  if( port == ADI_TX )
  {
	  AxiAdrv9001_SetTxDataPath( &Instance->Axi, channel, 0);
  }

  adi_adrv9001_RxSsiTestModeCfg_t RxCfg = { .testData = ADI_ADRV9001_SSI_TESTMODE_DATA_NORMAL };
  if( adi_adrv9001_Ssi_Rx_TestMode_Configure(&Instance->Device, channel, ADI_ADRV9001_SSI_TYPE_LVDS, ADI_ADRV9001_SSI_FORMAT_16_BIT_I_Q_DATA, &RxCfg ) != 0)
    return Adrv9001Status_SsiTestModeErr;

  adi_adrv9001_TxSsiTestModeCfg_t TxCfg = { .testData = ADI_ADRV9001_SSI_TESTMODE_DATA_NORMAL };
  if( adi_adrv9001_Ssi_Tx_TestMode_Configure(&Instance->Device, channel, ADI_ADRV9001_SSI_TYPE_LVDS, ADI_ADRV9001_SSI_FORMAT_16_BIT_I_Q_DATA, &TxCfg ) != 0)
    return Adrv9001Status_SsiTestModeErr;

  if( Adrv9001_SetEnableMode(Instance, port, channel, mode) != 0)
    return Adrv9001Status_EnableModeErr;

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

bool Adrv9001_IsPortEnabled( adrv9001_t *Instance, adi_common_Port_e port, adi_common_ChannelNumber_e channel )
{
  if( (port == ADI_TX) && (channel == ADI_CHANNEL_1))
  {
    if( (Instance->Params->tx.txInitChannelMask & ADI_ADRV9001_TX1 ) == ADI_ADRV9001_TX1 )
      return true;
  }
  else if( (port == ADI_TX) && (channel == ADI_CHANNEL_2))
  {
    if( (Instance->Params->tx.txInitChannelMask & ADI_ADRV9001_TX2 ) == ADI_ADRV9001_TX2 )
      return true;
  }
  else if( (port == ADI_RX) && (channel == ADI_CHANNEL_1))
  {
    if( (Instance->Params->rx.rxInitChannelMask & ADI_ADRV9001_RX1 ) == ADI_ADRV9001_RX1 )
      return true;
  }
  else if( (port == ADI_RX) && (channel == ADI_CHANNEL_2))
  {
    if( (Instance->Params->rx.rxInitChannelMask & ADI_ADRV9001_RX2 ) == ADI_ADRV9001_RX2 )
      return true;
  }

  return false;
}

int32_t Adrv9001_CalibrateSsiDelay( adrv9001_t *Instance, adi_common_Port_e port, adi_common_ChannelNumber_e channel )
{
  int32_t status;
  uint16_t results[8][8];
  uint8_t clkMax = 0;
  uint8_t dataMax = 0;

  if( Adrv9001_IsPortEnabled( Instance, port, channel ) == false )
    return Adrv9001Status_Success;

  if((status = Adrv9001_PerformSsiSweep( Instance, port, channel, results )) != 0)
    return status;

  if((status = Adrv9001_FindBestSsiDelay( results, &dataMax, &clkMax )) != 0)
    return status;

  if((status = Adrv9001_SetSsiDataDelay( Instance, port, channel, dataMax )) != 0)
    return status;

  if((status = Adrv9001_SetSsiClkDelay( Instance, port, channel, clkMax )) != 0)
    return status;

  return Adrv9001Status_Success;
}

int32_t Adrv9001_SetPaEnable( adrv9001_t *Instance, adi_common_Port_e port, adi_common_ChannelNumber_e channel, bool Enable )
{
  adi_adrv9001_GpioPinLevel_e level = Enable? ADI_ADRV9001_GPIO_PIN_LEVEL_HIGH : ADI_ADRV9001_GPIO_PIN_LEVEL_LOW;

  /* Check if Rx */
  if( (port == ADI_RX) && (channel == ADI_CHANNEL_1) )
  {
    if( Instance->Params->Rx1FrontendEnablePin != ADI_ADRV9001_GPIO_UNASSIGNED )
      if( adi_adrv9001_gpio_OutputPinLevel_Set(&Instance->Device, Instance->Params->Rx1FrontendEnablePin, level) != 0)
        return Adrv9001Status_GpioErr;
  }
  else if( (port == ADI_RX) && (channel == ADI_CHANNEL_2) )
  {
    if( Instance->Params->Rx2FrontendEnablePin != ADI_ADRV9001_GPIO_UNASSIGNED )
      if( adi_adrv9001_gpio_OutputPinLevel_Set(&Instance->Device, Instance->Params->Rx2FrontendEnablePin, level) != 0)
        return Adrv9001Status_GpioErr;
  }
  else if( (port == ADI_TX) && (channel == ADI_CHANNEL_1) )
  {
    if( Instance->Params->Tx1FrontendEnablePin != ADI_ADRV9001_GPIO_UNASSIGNED )
      if( adi_adrv9001_gpio_OutputPinLevel_Set(&Instance->Device, Instance->Params->Tx1FrontendEnablePin, level) != 0)
        return Adrv9001Status_GpioErr;
  }
  else
  {
    if( Instance->Params->Tx2FrontendEnablePin != ADI_ADRV9001_GPIO_UNASSIGNED )
      if( adi_adrv9001_gpio_OutputPinLevel_Set(&Instance->Device, Instance->Params->Tx2FrontendEnablePin, level) != 0)
        return Adrv9001Status_GpioErr;
  }

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

int32_t Adrv9001_EnableDac( adrv9001_t *Instance, adi_adrv9001_AuxDac_e Id, bool Enable )
{
  adi_adrv9001_GpioSignal_e signal;

  adi_adrv9001_GpioCfg_t Cfg = {
      .polarity = ADI_ADRV9001_GPIO_POLARITY_NORMAL,
      .master = ADI_ADRV9001_GPIO_MASTER_ADRV9001
  };

  switch( Id )
  {
    case ADI_ADRV9001_AUXDAC0 : signal = ADI_ADRV9001_GPIO_SIGNAL_AUX_DAC_0; Cfg.pin = ADI_ADRV9001_GPIO_ANALOG_00; break;
    case ADI_ADRV9001_AUXDAC1 : signal = ADI_ADRV9001_GPIO_SIGNAL_AUX_DAC_1; Cfg.pin = ADI_ADRV9001_GPIO_ANALOG_01; break;
    case ADI_ADRV9001_AUXDAC2 : signal = ADI_ADRV9001_GPIO_SIGNAL_AUX_DAC_2; Cfg.pin = ADI_ADRV9001_GPIO_ANALOG_02; break;
    case ADI_ADRV9001_AUXDAC3 : signal = ADI_ADRV9001_GPIO_SIGNAL_AUX_DAC_3; Cfg.pin = ADI_ADRV9001_GPIO_ANALOG_03; break;
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

  if( adi_adrv9001_AuxDac_Code_Set(&Instance->Device, Instance->Params->TcxoDacChannel, Value) != 0)
    return Adrv9001Status_DacErr;

  return Adrv9001Status_Success;
}

int32_t Adrv9001_GetVcTcxo( adrv9001_t *Instance, float *Voltage )
{
  uint16_t Value;

  if( adi_adrv9001_AuxDac_Code_Get(&Instance->Device, Instance->Params->TcxoDacChannel, &Value) != 0 )
    return Adrv9001Status_DacErr;

  *Voltage = 0.9000 + (((float)Value - 2048.000) / 4096.000) * 1.7;

  return Adrv9001Status_Success;
}

int32_t Adrv9001_LogWrite(void *devHalCfg, uint32_t logLevel, const char *comment, va_list argp)
{
#ifdef ADRV9001_USE_FS

  adrv9001_t *Adrv9001 = (adrv9001_t*)devHalCfg;

  if( strlen(Adrv9001->Params->LogPath) > 0 )
  {
    UINT len = 0;

    char *str = calloc(1,1024);

    vsnprintf( str, 1023, comment, argp );

    if(f_write(&Adrv9001->LogFil, str, strlen(str), (UINT*)&len) != FR_OK)
      return Adrv9001Status_LogErr;

    f_sync(&Adrv9001->LogFil);

    free(str);
  }
#endif
  return Adrv9001Status_Success;
}

int32_t Adrv9001_DelayUs(void *devHalCfg, uint32_t time_us)
{
  usleep(time_us);

  return Adrv9001Status_Success;
}

int32_t Adrv9001_Open( void *devHalCfg )
{
#ifdef ADRV9001_USE_FS
  adrv9001_t *Adrv9001 = (adrv9001_t*)devHalCfg;

  if( strlen(Adrv9001->Params->LogPath) > 0 )
  {
    /* Delete PHY Log file */
    f_unlink(Adrv9001->Params->LogPath);

    /* Open File */
    if( f_open(&Adrv9001->LogFil, Adrv9001->Params->LogPath, FA_CREATE_NEW | FA_WRITE | FA_READ) != FR_OK)
      return ADI_COMMON_ACT_WARN_RESET_LOG;

    /* Point to beginning of file */
    f_lseek(&Adrv9001->LogFil, 0);

    f_sync(&Adrv9001->LogFil);
  }

#endif

  return Adrv9001Status_Success;
}

int32_t Adrv9001_Close( void *devHalCfg )
{
#ifdef ADRV9001_USE_FS
  adrv9001_t *Adrv9001 = (adrv9001_t*)devHalCfg;

  if( strlen(Adrv9001->Params->LogPath) > 0 )
  {
    f_sync(&Adrv9001->LogFil);

    f_close(&Adrv9001->LogFil);
  }
#endif
  return Adrv9001Status_Success;
}

int32_t Adrv9001_SpiWrite( void *devHalCfg, const uint8_t txData[], uint32_t numTxBytes )
{
  adrv9001_t *Adrv9001 = (adrv9001_t*)devHalCfg;

  return AxiAdrv9001_MspiTransfer( &Adrv9001->Axi, (uint8_t*)txData, NULL, numTxBytes);
}

int32_t Adrv9001_SpiRead( void *devHalCfg, const uint8_t txData[], uint8_t rxData[], uint32_t numRxBytes )
{
  adrv9001_t *Adrv9001 = (adrv9001_t*)devHalCfg;

  return AxiAdrv9001_MspiTransfer( &Adrv9001->Axi, (uint8_t*)txData, rxData, numRxBytes);
}

int32_t Adrv9001_ResetbPinSet( void *devHalCfg, uint8_t pinLevel )
{
  adrv9001_t *Adrv9001 = (adrv9001_t*)devHalCfg;

  AxiAdrv9001_ResetbPinSet( &Adrv9001->Axi, pinLevel );

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



