/***************************************************************************//**
 *  \addtogroup ADRV9001_CLI
 *   @{
 *******************************************************************************/
/***************************************************************************//**
 *  \file       adrv9001_cli.c
 *
 *  \details    This file contains the ADRV9001 layer CLI commands.
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
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "xstatus.h"
#include "adrv9001_cli.h"
#include "math.h"
#include "status.h"


#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

static void Adrv9001Cli_GetPortChannelParameter(cli_t *CliInstance, const char *cmd, uint8_t pNum, adi_common_ChannelNumber_e *Channel, adi_common_Port_e *Port)
{
  char pName[64] = {0};

  Cli_GetParameter(cmd, pNum, CliParamTypeStr, pName);

  if(strncmp(pName, "Tx1", 3) == 0 )
  {
    *Port = ADI_TX;
    *Channel = ADI_CHANNEL_1;
  }
  else if(strncmp(pName, "Tx2", 3) == 0 )
  {
    *Port = ADI_TX;
    *Channel = ADI_CHANNEL_2;
  }
  else if(strncmp(pName, "Rx1", 3) == 0 )
  {
    *Port = ADI_RX;
    *Channel = ADI_CHANNEL_1;
  }
  else if(strncmp(pName, "Rx2", 3) == 0 )
  {
    *Port = ADI_RX;
    *Channel = ADI_CHANNEL_2;
  }
}

static void Adrv9001Cli_GetParam(cli_t *CliInstance, const char *cmd, adrv9001_t *Adrv9001)
{
  char *name;
  char *value;

  if((name = calloc(1, 256 )) == NULL)
  {
    Cli_Printf(CliInstance,"Memory Error\r\n");
    return;
  }

  if((value = calloc(1, 1024 )) == NULL)
  {
    free(name);
    Cli_Printf(CliInstance,"Memory Error\r\n");
    return;
  }

  /* Get Name */
  Cli_GetParameter(cmd, 1, CliParamTypeStr, name);

  adi_common_ChannelNumber_e Channel;
  adi_common_Port_e Port;

  if(strncmp(name, "Tx1", 3) == 0 )
  {
    Port = ADI_TX;
    Channel = ADI_CHANNEL_1;
  }
  else if(strncmp(name, "Tx2", 3) == 0 )
  {
    Port = ADI_TX;
    Channel = ADI_CHANNEL_2;
  }
  else if(strncmp(name, "Rx1", 3) == 0 )
  {
    Port = ADI_RX;
    Channel = ADI_CHANNEL_1;
  }
  else if(strncmp(name, "Rx2", 3) == 0 )
  {
    Port = ADI_RX;
    Channel = ADI_CHANNEL_2;
  }

  if( strcmp(name, "SiliconVersion") == 0 )
  {
    adi_adrv9001_SiliconVersion_t tmp;
    Adrv9001_GetSiliconVersion( Adrv9001, &tmp );
    Cli_Printf(CliInstance, "%s = %hhx%hhx\r\n", name, tmp.major, tmp.minor );
  }
  else if( strcmp(name, "FirmwareVersion") == 0 )
  {
    adi_adrv9001_ArmVersion_t tmp;
    Adrv9001_GetFirmwareVersion( Adrv9001, &tmp );
    Cli_Printf(CliInstance, "%s = %hhu.%hhu.%hhu.%hhu\r\n", name, tmp.majorVer, tmp.minorVer, tmp.maintVer, tmp.armBuildType );
  }
  else if( strcmp(name, "APIVersion") == 0 )
  {
    adi_common_ApiVersion_t tmp;
    Adrv9001_GetApiVersion( Adrv9001, &tmp );
    Cli_Printf(CliInstance, "%s = %lu.%lu.%lu\r\n", name, tmp.major, tmp.minor, tmp.patch );
  }
  else if( strcmp(name, "Temp") == 0 )
  {
    int16_t tmp;
    Adrv9001_GetTemperature( Adrv9001, &tmp );
    Cli_Printf(CliInstance, "%s = %d C\r\n",name, tmp);
  }
  else if( strcmp(&name[3], "SampleRate") == 0 )
  {
    uint32_t tmp;
    Adrv9001_GetSampleRate( Adrv9001, Port, Channel, &tmp);
    float tmpf = (float)tmp / 1e6;
    Cli_Printf(CliInstance, "%s = %2.3f MHz\r\n",name, tmpf );
  }
  else if( strcmp(&name[3], "CarrierFreq") == 0 )
  {
    uint64_t tmp;
    Adrv9001_GetCarrierFrequency( Adrv9001, Port, Channel, &tmp );
    float tmpf = (float)tmp / 1e6;
    Cli_Printf(CliInstance, "%s = %2.4f MHz\r\n", name, tmpf );
  }
  else if( strcmp(&name[3], "SsiClockDelay") == 0 )
  {
    uint16_t tmp;
    Adrv9001_GetSsiClockDelay( Adrv9001, Port, Channel, &tmp );
    Cli_Printf(CliInstance, "%s = %hu ps\r\n", name, tmp );
  }
  else if( strcmp(&name[3], "SsiDataDelay") == 0 )
  {
    uint16_t tmp;
    Adrv9001_GetSsiDataDelay( Adrv9001, Port, Channel, &tmp );
    Cli_Printf(CliInstance, "%s = %hu ps\r\n",name, tmp );
  }
  else if( strcmp(&name[3], "SsiStatus") == 0 )
  {
    bool tmp;
    Adrv9001_GetSsiStatus( Adrv9001, Port, Channel, &tmp );
    Cli_Printf(CliInstance, "%s = %hhu\r\n", name, (uint8_t)tmp );
  }
  else if( strcmp(&name[3], "RadioState") == 0 )
  {
    adi_adrv9001_ChannelState_e tmp;
    Adrv9001_GetRadioState( Adrv9001, Port, Channel, &tmp );
    if( tmp == ADI_ADRV9001_CHANNEL_STANDBY )
    {
      Cli_Printf(CliInstance, "%s = %s\r\n", name, "Standby" );
    }
    else if( tmp == ADI_ADRV9001_CHANNEL_CALIBRATED )
    {
      Cli_Printf(CliInstance, "%s = %s\r\n", name, "Calibrated" );
    }
    else if( tmp == ADI_ADRV9001_CHANNEL_PRIMED )
    {
      Cli_Printf(CliInstance, "%s = %s\r\n", name, "Primed" );
    }
    else if( tmp == ADI_ADRV9001_CHANNEL_RF_ENABLED )
    {
      Cli_Printf(CliInstance, "%s = %s\r\n", name, "Enabled" );
    }
  }
  else if( strcmp(&name[3], "GainIndex") == 0 )
  {
    uint8_t tmp;
    Adrv9001_GetRxCurGainIndex( Adrv9001, Channel, &tmp );
    Cli_Printf(CliInstance, "%s = %hhu\r\n", name, tmp );
  }
  else if( strcmp(&name[3], "GainMode") == 0 )
  {
    adi_adrv9001_RxGainControlMode_e tmp;
    Adrv9001_GetRxGainMode( Adrv9001, Channel, &tmp );

    if( tmp == ADI_ADRV9001_RX_GAIN_CONTROL_MODE_SPI )
    {
      Cli_Printf(CliInstance, "%s = %s\r\n", name, "Spi" );
    }
    else if( tmp == ADI_ADRV9001_RX_GAIN_CONTROL_MODE_PIN )
    {
      Cli_Printf(CliInstance, "%s = %s\r\n", name, "Pin" );
    }
    else
    {
      Cli_Printf(CliInstance, "%s = %s\r\n", name, "Auto" );
    }
  }
  else if( strcmp(&name[3], "Rssi") == 0 )
  {
    float tmp;
    Adrv9001_GetRxRssi( Adrv9001, Channel, &tmp );
    Cli_Printf(CliInstance, "%s = %2.2f dB\r\n", name, tmp );
  }
  else if( strcmp(&name[3], "SwapIq") == 0 )
  {
    bool tmp = AxiAdrv9001_GetSwapIq( &Adrv9001->Axi, Port, Channel );
    Cli_Printf(CliInstance, "%s = %hhu\r\n", name, (uint8_t)tmp );
  }
  else if( strcmp(&name[3], "Attn") == 0 )
  {
    float tmp;
    Adrv9001_GetTxAttn( Adrv9001, Channel, &tmp );
    Cli_Printf(CliInstance, "%s = %2.2f dB\r\n", name, tmp );
  }
  else if( strcmp(&name[3], "Boost") == 0 )
  {
    bool tmp;
    Adrv9001_GetTxBoost( Adrv9001, Channel, &tmp );
    Cli_Printf(CliInstance, "%s = %hhu\r\n", name, (uint8_t)tmp );
  }
  else if( strcmp(&name[3], "FixedPattern") == 0 )
  {
    uint32_t tmp = AxiAdrv9001_GetFixedPattern( &Adrv9001->Axi, Port, Channel );
    Cli_Printf(CliInstance, "%s = 0x%lx\r\n", name, tmp );
  }
  else if( strcmp(&name[3], "DataSrc") == 0 )
  {
    adrv9001_tx_data_src_t tmp;
    Adrv9001_GetTxDataSrc( Adrv9001, Channel, &tmp );

    if( tmp == Adrv9001TxDataSrc_Ones )
    {
      Cli_Printf(CliInstance, "%s = %s\r\n", name, "Ones" );
    }
    else if( tmp == Adrv9001TxDataSrc_Zeros )
    {
      Cli_Printf(CliInstance, "%s = %s\r\n", name, "Zeros" );
    }
    else if( tmp == Adrv9001TxDataSrc_Ramp )
    {
      Cli_Printf(CliInstance, "%s = %s\r\n", name, "Ramp" );
    }
    else if( tmp == Adrv9001TxDataSrc_Pn15 )
    {
      Cli_Printf(CliInstance, "%s = %s\r\n", name, "Pn15" );
    }
    else if( tmp == Adrv9001TxDataSrc_FixedPattern )
    {
      Cli_Printf(CliInstance, "%s = %s\r\n", name, "Fixed" );
    }
    else if( tmp == Adrv9001TxDataSrc_Axis )
    {
      Cli_Printf(CliInstance, "%s = %s\r\n", name, "Axis" );
    }
    else if( tmp == Adrv9001TxDataSrc_RxLoopback )
    {
      Cli_Printf(CliInstance, "%s = %s\r\n", name, "Rx Loopback" );
    }
  }
  else if( strcmp(name, "Tx1ToRx1LoopBack") == 0 )
  {
    bool tmp;
    Adrv9001_GetTxToRxLoopBack( Adrv9001, ADI_CHANNEL_1, &tmp );
    Cli_Printf(CliInstance, "%s = %hhu\r\n", name, (uint8_t)tmp );
  }
  else if( strcmp(name, "Tx2ToRx2LoopBack") == 0 )
  {
    bool tmp;
    Adrv9001_GetTxToRxLoopBack( Adrv9001, ADI_CHANNEL_2, &tmp );
    Cli_Printf(CliInstance, "%s = %hhu\r\n", name, (uint8_t)tmp );
  }
  else if( strcmp(name, "Dac0Voltage") == 0)
  {
    float tmp;
    Adrv9001_GetDacVoltage(Adrv9001, ADI_ADRV9001_AUXDAC0, &tmp );
    Cli_Printf(CliInstance, "%s = %2.2f\r\n", name, tmp );
  }
  else if( strcmp(name, "Dac1Voltage") == 0)
  {
    float tmp;
    Adrv9001_GetDacVoltage(Adrv9001, ADI_ADRV9001_AUXDAC1, &tmp );
    Cli_Printf(CliInstance, "%s = %2.2f\r\n", name, tmp );
  }
  else if( strcmp(name, "Dac2Voltage") == 0)
  {
    float tmp;
    Adrv9001_GetDacVoltage(Adrv9001, ADI_ADRV9001_AUXDAC2, &tmp );
    Cli_Printf(CliInstance, "%s = %2.2f\r\n", name, tmp );
  }
  else if( strcmp(name, "Dac3Voltage") == 0)
  {
    float tmp;
    Adrv9001_GetDacVoltage(Adrv9001, ADI_ADRV9001_AUXDAC3, &tmp );
    Cli_Printf(CliInstance, "%s = %2.2f\r\n", name, tmp );
  }
  else if( strcmp(name, "Dac0Enable") == 0 )
  {
    bool tmp;
    Adrv9001_GetEnableDac( Adrv9001, ADI_ADRV9001_AUXDAC0, &tmp );
    Cli_Printf(CliInstance, "%s = %hhu\r\n", name, (uint8_t)tmp );
  }
  else if( strcmp(name, "Dac1Enable") == 0 )
  {
    bool tmp;
    Adrv9001_GetEnableDac( Adrv9001, ADI_ADRV9001_AUXDAC1, &tmp );
    Cli_Printf(CliInstance, "%s = %hhu\r\n", name, (uint8_t)tmp );
  }
  else if( strcmp(name, "Dac2Enable") == 0 )
  {
    bool tmp;
    Adrv9001_GetEnableDac( Adrv9001, ADI_ADRV9001_AUXDAC2, &tmp );
    Cli_Printf(CliInstance, "%s = %hhu\r\n", name, (uint8_t)tmp );
  }
  else if( strcmp(name, "Dac3Enable") == 0 )
  {
    bool tmp;
    Adrv9001_GetEnableDac( Adrv9001, ADI_ADRV9001_AUXDAC3, &tmp );
    Cli_Printf(CliInstance, "%s = %hhu\r\n", name, (uint8_t)tmp );
  }
  else if( strcmp(&name[3], "EnableMode") == 0 )
  {
    adi_adrv9001_ChannelEnableMode_e tmp;
    Adrv9001_GetEnableMode( Adrv9001, Port, Channel, &tmp );

    if( tmp == ADI_ADRV9001_PIN_MODE)
    {
      Cli_Printf(CliInstance, "%s = %s\r\n", name, "Pin" );
    }
    else
    {
      Cli_Printf(CliInstance, "%s = %s\r\n", name, "Spi" );
    }
  }
  else if(strncmp(name, "Dgpio",5) == 0 )
  {
    adi_adrv9001_GpioPinLevel_e tmp;

    uint8_t pin;
    sscanf(&name[5], "%hhu", &pin);

    Adrv9001_GetGpioPinLevel( Adrv9001, (adi_adrv9001_GpioPin_e)pin, &tmp );

    if( tmp == ADI_ADRV9001_GPIO_PIN_LEVEL_LOW )
      Cli_Printf(CliInstance, "%s = 0\r\n", name );
    else
      Cli_Printf(CliInstance, "%s = 1\r\n", name );
  }
  else if(strncmp(name, "Agpio",5) == 0 )
  {
    adi_adrv9001_GpioPinLevel_e tmp;

    uint8_t pin;
    sscanf(&name[5], "%hhu", &pin);

    pin = pin + (uint8_t)ADI_ADRV9001_GPIO_ANALOG_00;

    Adrv9001_GetGpioPinLevel( Adrv9001, (adi_adrv9001_GpioPin_e)pin, &tmp );

    if( tmp == ADI_ADRV9001_GPIO_PIN_LEVEL_LOW )
      Cli_Printf(CliInstance, "%s = 0\r\n", name );
    else
      Cli_Printf(CliInstance, "%s = 1\r\n", name );
  }
  else
  {
    Cli_Printf(CliInstance, "Unknown Parameter %s\r\n", name );
  }


  free(name);
  free(value);
}

static void Adrv9001Cli_SetParam(cli_t *CliInstance, const char *cmd, adrv9001_t *Adrv9001)
{
  char *name;
  char *Value;
  int32_t status = Adrv9001Status_Success;

  if((name = calloc(1, 256 )) == NULL)
  {
    Cli_Printf(CliInstance,"Memory Error\r\n");
    return;
  }

  if((Value = calloc(1, 1024 )) == NULL)
  {
    free(name);
    Cli_Printf(CliInstance,"Memory Error\r\n");
    return;
  }

  /* Get Name */
  Cli_GetParameter(cmd, 1, CliParamTypeStr, name);

  /* Get Value */
  Cli_GetParameter(cmd, 2, CliParamTypeStr, Value);

  adi_common_ChannelNumber_e Channel;
  adi_common_Port_e Port;

  if(strncmp(name, "Tx1", 3) == 0 )
  {
    Port = ADI_TX;
    Channel = ADI_CHANNEL_1;
  }
  else if(strncmp(name, "Tx2", 3) == 0 )
  {
    Port = ADI_TX;
    Channel = ADI_CHANNEL_2;
  }
  else if(strncmp(name, "Rx1", 3) == 0 )
  {
    Port = ADI_RX;
    Channel = ADI_CHANNEL_1;
  }
  else if(strncmp(name, "Rx2", 3) == 0 )
  {
    Port = ADI_RX;
    Channel = ADI_CHANNEL_2;
  }

  if( strcmp(&name[3], "CarrierFreq") == 0 )
  {
    float tmpf;
    sscanf(Value, "%f", &tmpf);
    uint64_t tmp = (uint64_t)(tmpf * 1e6);
    status = Adrv9001_SetCarrierFrequency( Adrv9001, Port, Channel, tmp );
  }
  else if( strcmp(&name[3], "SsiClockDelay") == 0 )
  {
    uint16_t tmp;
    sscanf(Value, "%hu", &tmp);
    status = Adrv9001_SetSsiClkDelay( Adrv9001, Port, Channel, tmp );
  }
  else if( strcmp(&name[3], "SsiDataDelay") == 0 )
  {
    uint16_t tmp;
    sscanf(Value, "%hu", &tmp);
    status = Adrv9001_SetSsiDataDelay( Adrv9001, Port, Channel, tmp );
  }
  else if( strcmp(&name[3], "RadioState") == 0 )
  {
    if( strcmp(Value, "Calibrated") == 0 )
    {
      status = Adrv9001_ToCalibrated( Adrv9001, Port, Channel );
    }
    else if( strcmp(Value, "Primed") == 0 )
    {
      status = Adrv9001_ToPrimed( Adrv9001, Port, Channel );
    }
    else if( strcmp(Value, "Enabled") == 0 )
    {
      status = Adrv9001_ToRfEnabled( Adrv9001, Port, Channel );
    }
  }
  else if( strcmp(&name[3], "GainIndex") == 0 )
  {
    uint8_t tmp;
    sscanf(Value, "%hhu", &tmp);
    status = Adrv9001_SetRxCurGainIndex( Adrv9001, Channel, tmp );
  }
  else if( strcmp(&name[3], "GainMode") == 0 )
  {
    adi_adrv9001_RxGainControlMode_e tmp;
    if( strcmp(Value, "Spi") == 0 )
    {
      tmp = ADI_ADRV9001_RX_GAIN_CONTROL_MODE_SPI;
    }
    else if( strcmp(Value, "Pin") == 0 )
    {
      tmp = ADI_ADRV9001_RX_GAIN_CONTROL_MODE_PIN;
    }
    else
    {
      tmp = ADI_ADRV9001_RX_GAIN_CONTROL_MODE_AUTO;
    }

    status = Adrv9001_SetRxGainMode( Adrv9001, Channel, tmp );
  }
  else if( strcmp(&name[3], "SwapIq") == 0 )
  {
    uint8_t tmp;
    sscanf(Value, "%hhu", &tmp);
    AxiAdrv9001_SetSwapIq( &Adrv9001->Axi, Port, Channel, (bool)tmp );
    sprintf(Value, "%hhu,bool", (uint8_t)tmp );
  }
  else if( strcmp(&name[3], "Attn") == 0 )
  {
    float tmp;
    sscanf(Value, "%f", &tmp);
    status = Adrv9001_SetTxAttn( Adrv9001, Channel, tmp );
  }
  else if( strcmp(&name[3], "Boost") == 0 )
  {
    uint8_t tmp;
    sscanf(Value, "%hhu", &tmp);
    status = Adrv9001_SetTxBoost( Adrv9001, Channel, (bool)tmp );
  }
  else if( strcmp(&name[3], "FixedPattern") == 0 )
  {
    uint32_t tmp;
    //sscanf(Value, "%lu", &tmp);
    sscanf(Value, "%lx", &tmp);
    AxiAdrv9001_SetFixedPattern( &Adrv9001->Axi, Port, Channel, tmp );
  }
  else if( strcmp(&name[3], "DataSrc") == 0 )
  {
    adrv9001_tx_data_src_t tmp;
    if( strcmp(Value, "Ones") == 0 )
    {
      tmp = Adrv9001TxDataSrc_Ones;
    }
    else if( strcmp(Value, "Zeros") == 0 )
    {
      tmp = Adrv9001TxDataSrc_Zeros;
    }
    else if( strcmp(Value, "Ramp") == 0 )
    {
      tmp = Adrv9001TxDataSrc_Ramp;
    }
    else if( strcmp(Value, "Pn15") == 0 )
    {
      tmp = Adrv9001TxDataSrc_Pn15;
    }
    else if( strcmp(Value, "Fixed") == 0 )
    {
      tmp = Adrv9001TxDataSrc_FixedPattern;
    }
    else if( strcmp(Value, "Axis") == 0 )
    {
      tmp = Adrv9001TxDataSrc_Axis;
    }
    else if( strcmp(Value, "RxLoopback") == 0 )
    {
      tmp = Adrv9001TxDataSrc_RxLoopback;
    }

    status = Adrv9001_SetTxDataSrc( Adrv9001, Channel, tmp );
  }
  else if( strcmp(name, "Tx1ToRx1LoopBack") == 0 )
  {
    uint8_t tmp;
    sscanf(Value, "%hhu", &tmp);
    status = Adrv9001_SetTxToRxLoopBack( Adrv9001, ADI_CHANNEL_1, (bool)tmp );
  }
  else if( strcmp(name, "Tx2ToRx2LoopBack") == 0 )
  {
    uint8_t tmp;
    sscanf(Value, "%hhu", &tmp);
    status = Adrv9001_SetTxToRxLoopBack( Adrv9001, ADI_CHANNEL_2, (bool)tmp );
  }
  else if( strcmp(name, "Dac0Voltage") == 0)
  {
    float tmp;
    sscanf(Value, "%f", &tmp);
    status = Adrv9001_SetDacVoltage(Adrv9001, ADI_ADRV9001_AUXDAC0, tmp );
  }
  else if( strcmp(name, "Dac1Voltage") == 0)
  {
    float tmp;
    sscanf(Value, "%f", &tmp);
    status = Adrv9001_SetDacVoltage(Adrv9001, ADI_ADRV9001_AUXDAC1, tmp );
  }
  else if( strcmp(name, "Dac2Voltage") == 0)
  {
    float tmp;
    sscanf(Value, "%f", &tmp);
    status = Adrv9001_SetDacVoltage(Adrv9001, ADI_ADRV9001_AUXDAC2, tmp );
  }
  else if( strcmp(name, "Dac3Voltage") == 0)
  {
    float tmp;
    sscanf(Value, "%f", &tmp);
    status = Adrv9001_SetDacVoltage(Adrv9001, ADI_ADRV9001_AUXDAC3, tmp );
  }
  else if( strcmp(name, "Dac0Enable") == 0 )
  {
    uint8_t tmp;
    sscanf(Value, "%hhu", &tmp);
    status = Adrv9001_SetEnableDac( Adrv9001, ADI_ADRV9001_AUXDAC0, (bool)tmp );
  }
  else if( strcmp(name, "Dac1Enable") == 0 )
  {
    uint8_t tmp;
    sscanf(Value, "%hhu", &tmp);
    status = Adrv9001_SetEnableDac( Adrv9001, ADI_ADRV9001_AUXDAC1, (bool)tmp );
  }
  else if( strcmp(name, "Dac2Enable") == 0 )
  {
    uint8_t tmp;
    sscanf(Value, "%hhu", &tmp);
    status = Adrv9001_SetEnableDac( Adrv9001, ADI_ADRV9001_AUXDAC2, (bool)tmp );
  }
  else if( strcmp(name, "Dac3Enable") == 0 )
  {
    uint8_t tmp;
    sscanf(Value, "%hhu", &tmp);
    status = Adrv9001_SetEnableDac( Adrv9001, ADI_ADRV9001_AUXDAC3, (bool)tmp );
  }
  else if( strcmp(&name[3], "EnableMode") == 0 )
  {
    if( strcmp(Value, "Pin") == 0 )
    {
      status = Adrv9001_SetEnableMode( Adrv9001, Port, Channel, ADI_ADRV9001_PIN_MODE );
    }
    else
    {
      status = Adrv9001_SetEnableMode( Adrv9001, Port, Channel, ADI_ADRV9001_SPI_MODE );
    }
  }
  else if(strncmp(name, "Dgpio",5) == 0 )
  {
    uint8_t pin = 0;
    sscanf(&name[5], "%hhu", &pin);

    uint8_t tmp;
    sscanf(Value, "%hhu", &tmp);

    Adrv9001_SetGpioPinLevel( Adrv9001, (adi_adrv9001_GpioPin_e)pin, (adi_adrv9001_GpioPinLevel_e)tmp );
  }
  else if(strncmp(name, "Agpio",5) == 0 )
  {
    uint8_t pin = 0;
    sscanf(&name[5], "%hhu", &pin);

    uint8_t tmp;
    sscanf(Value, "%hhu", &tmp);

    pin = pin + (uint8_t)ADI_ADRV9001_GPIO_ANALOG_00;

    Adrv9001_SetGpioPinLevel( Adrv9001, (adi_adrv9001_GpioPin_e)pin, (adi_adrv9001_GpioPinLevel_e)tmp );
  }
  else
  {
    status = Adrv9001Status_InvalidParameter;
  }

  Cli_Printf(CliInstance,"SetParam %s\r\n",StatusString(status));

  free(name);
  free(Value);
}

static void Adrv9001Cli_ListParams(cli_t *CliInstance, const char *cmd, adrv9001_t *Adrv9001)
{
  Cli_Printf(CliInstance,"ADRV9001 Parameter Names:\r\n");

  Cli_Printf(CliInstance,"  SiliconVersion\r\n" );
  Cli_Printf(CliInstance,"  FirmwareVersion\r\n" );
  Cli_Printf(CliInstance,"  APIVersion\r\n" );
  Cli_Printf(CliInstance,"  Temp\r\n" );

  Cli_Printf(CliInstance,"  Rx1SampleRate\r\n" );
  Cli_Printf(CliInstance,"  Rx1CarrierFreq\r\n" );
  Cli_Printf(CliInstance,"  Rx1SsiClockDelay\r\n" );
  Cli_Printf(CliInstance,"  Rx1SsiDataDelay\r\n" );
  Cli_Printf(CliInstance,"  Rx1SsiStatus\r\n" );
  Cli_Printf(CliInstance,"  Rx1RadioState\r\n" );
  Cli_Printf(CliInstance,"  Rx1GainIndex\r\n" );
  Cli_Printf(CliInstance,"  Rx1GainMode\r\n" );
  Cli_Printf(CliInstance,"  Rx1Rssi\r\n" );
  Cli_Printf(CliInstance,"  Rx1SwapIq\r\n" );
  Cli_Printf(CliInstance,"  Rx1EnableMode\r\n" );

  Cli_Printf(CliInstance,"  Rx2SampleRate\r\n" );
  Cli_Printf(CliInstance,"  Rx2CarrierFreq\r\n" );
  Cli_Printf(CliInstance,"  Rx2SsiClockDelay\r\n" );
  Cli_Printf(CliInstance,"  Rx2SsiDataDelay\r\n" );
  Cli_Printf(CliInstance,"  Rx2SsiStatus\r\n" );
  Cli_Printf(CliInstance,"  Rx2RadioState\r\n" );
  Cli_Printf(CliInstance,"  Rx2GainIndex\r\n" );
  Cli_Printf(CliInstance,"  Rx2GainMode\r\n" );
  Cli_Printf(CliInstance,"  Rx2Rssi\r\n" );
  Cli_Printf(CliInstance,"  Rx2SwapIq\r\n" );
  Cli_Printf(CliInstance,"  Rx2EnableMode\r\n" );

  Cli_Printf(CliInstance,"  Tx1SampleRate\r\n" );
  Cli_Printf(CliInstance,"  Tx1CarrierFreq\r\n" );
  Cli_Printf(CliInstance,"  Tx1SsiClockDelay\r\n" );
  Cli_Printf(CliInstance,"  Tx1SsiDataDelay\r\n" );
  Cli_Printf(CliInstance,"  Tx1SsiStatus\r\n" );
  Cli_Printf(CliInstance,"  Tx1RadioState\r\n" );
  Cli_Printf(CliInstance,"  Tx1SwapIq\r\n" );
  Cli_Printf(CliInstance,"  Tx1Attn\r\n" );
  Cli_Printf(CliInstance,"  Tx1Boost\r\n" );
  Cli_Printf(CliInstance,"  Tx1DataSrc\r\n" );
  Cli_Printf(CliInstance,"  Tx1ToRx1LoopBack\r\n" );
  Cli_Printf(CliInstance,"  Tx1EnableMode\r\n" );

  Cli_Printf(CliInstance,"  Tx2SampleRate\r\n" );
  Cli_Printf(CliInstance,"  Tx2CarrierFreq\r\n" );
  Cli_Printf(CliInstance,"  Tx2SsiClockDelay\r\n" );
  Cli_Printf(CliInstance,"  Tx2SsiDataDelay\r\n" );
  Cli_Printf(CliInstance,"  Tx2SsiStatus\r\n" );
  Cli_Printf(CliInstance,"  Tx2RadioState\r\n" );
  Cli_Printf(CliInstance,"  Tx2SwapIq\r\n" );
  Cli_Printf(CliInstance,"  Tx2Attn\r\n" );
  Cli_Printf(CliInstance,"  Tx2Boost\r\n" );
  Cli_Printf(CliInstance,"  Tx2DataSrc\r\n" );
  Cli_Printf(CliInstance,"  Tx2ToRx2LoopBack\r\n" );
  Cli_Printf(CliInstance,"  Tx2EnableMode\r\n" );

  Cli_Printf(CliInstance,"  Dac0Voltage\r\n" );
  Cli_Printf(CliInstance,"  Dac1Voltage\r\n" );
  Cli_Printf(CliInstance,"  Dac2Voltage\r\n" );
  Cli_Printf(CliInstance,"  Dac3Voltage\r\n" );

  Cli_Printf(CliInstance,"  Dac0Enable\r\n" );
  Cli_Printf(CliInstance,"  Dac1Enable\r\n" );
  Cli_Printf(CliInstance,"  Dac2Enable\r\n" );
  Cli_Printf(CliInstance,"  Dac3Enable\r\n" );

  Cli_Printf(CliInstance,"  Dgpio0\r\n" );
  Cli_Printf(CliInstance,"  Dgpio1\r\n" );
  Cli_Printf(CliInstance,"  Dgpio2\r\n" );
  Cli_Printf(CliInstance,"  Dgpio3\r\n" );
  Cli_Printf(CliInstance,"  Dgpio4\r\n" );
  Cli_Printf(CliInstance,"  Dgpio5\r\n" );
  Cli_Printf(CliInstance,"  Dgpio6\r\n" );
  Cli_Printf(CliInstance,"  Dgpio7\r\n" );
  Cli_Printf(CliInstance,"  Dgpio8\r\n" );
  Cli_Printf(CliInstance,"  Dgpio9\r\n" );
  Cli_Printf(CliInstance,"  Dgpio10\r\n" );
  Cli_Printf(CliInstance,"  Dgpio11\r\n" );
  Cli_Printf(CliInstance,"  Agpio0\r\n" );
  Cli_Printf(CliInstance,"  Agpio1\r\n" );
  Cli_Printf(CliInstance,"  Agpio2\r\n" );
  Cli_Printf(CliInstance,"  Agpio3\r\n" );
  Cli_Printf(CliInstance,"  Agpio4\r\n" );
  Cli_Printf(CliInstance,"  Agpio5\r\n" );
  Cli_Printf(CliInstance,"  Agpio6\r\n" );
  Cli_Printf(CliInstance,"  Agpio7\r\n" );
  Cli_Printf(CliInstance,"  Agpio8\r\n" );
  Cli_Printf(CliInstance,"  Agpio9\r\n" );
  Cli_Printf(CliInstance,"  Agpio10\r\n" );
  Cli_Printf(CliInstance,"  Agpio11\r\n" );

}

static void Adrv9001Cli_ToRfCalibrated(cli_t *CliInstance, const char *cmd, adrv9001_t *Adrv9001)
{
  int32_t status = 0;
  adi_common_ChannelNumber_e Channel;
  adi_common_Port_e Port;

  Adrv9001_ClearError( Adrv9001 );

  Adrv9001Cli_GetPortChannelParameter(CliInstance, cmd, 1, &Channel, &Port);

  status = Adrv9001_ToCalibrated( Adrv9001, Port, Channel );

  Cli_Printf(CliInstance,"%s\r\n",StatusString( status ));
}

static void Adrv9001Cli_ToRfPrimed(cli_t *CliInstance, const char *cmd, adrv9001_t *Adrv9001)
{
  int32_t status = 0;
  adi_common_ChannelNumber_e Channel;
  adi_common_Port_e Port;

  Adrv9001_ClearError( Adrv9001 );

  Adrv9001Cli_GetPortChannelParameter(CliInstance, cmd, 1, &Channel, &Port);

  status = Adrv9001_ToPrimed( Adrv9001, Port, Channel );

  Cli_Printf(CliInstance,"%s\r\n",StatusString( status ));
}

static void Adrv9001Cli_ToRfEnabled(cli_t *CliInstance, const char *cmd, adrv9001_t *Adrv9001)
{
  int32_t status = 0;
  adi_common_ChannelNumber_e Channel;
  adi_common_Port_e Port;

  Adrv9001_ClearError( Adrv9001 );

  Adrv9001Cli_GetPortChannelParameter(CliInstance, cmd, 1, &Channel, &Port);

  status = Adrv9001_ToRfEnabled( Adrv9001, Port, Channel );

  Cli_Printf(CliInstance,"%s\r\n",StatusString( status ));
}

static void Adrv9001Cli_CalibrateSsi(cli_t *CliInstance, const char *cmd, adrv9001_t *Adrv9001)
{
  int32_t status = 0;
  adi_common_ChannelNumber_e Channel;
  adi_common_Port_e Port;

  Adrv9001_ClearError( Adrv9001 );

  Adrv9001Cli_GetPortChannelParameter(CliInstance, cmd, 1, &Channel, &Port);

  status = Adrv9001_CalibrateSsiDelay( Adrv9001, Port, Channel, ADI_ADRV9001_SSI_TESTMODE_DATA_RAMP_16_BIT, 0x00 );

  Cli_Printf(CliInstance,"%s\r\n",StatusString( status ));
}

static void Adrv9001Cli_SweepSsi(cli_t *CliInstance, const char *cmd, adrv9001_t *Adrv9001)
{
  int32_t status = 0;
  adi_common_ChannelNumber_e Channel;
  adi_common_Port_e Port;

  Adrv9001_ClearError( Adrv9001 );

  Adrv9001Cli_GetPortChannelParameter(CliInstance, cmd, 1, &Channel, &Port);

  uint16_t results[8][8];
  if((status = Adrv9001_PerformSsiSweep( Adrv9001, Port, Channel, ADI_ADRV9001_SSI_TESTMODE_DATA_RAMP_16_BIT, 0x00, results )) == 0)
  {
    printf("SSI Sweep Test Results:\r\n");

    for(uint8_t clkDelay = 0; clkDelay < 8; clkDelay++)
    {
      printf("  ClkDelay %u: ",clkDelay);

      for(uint8_t dataDelay = 0; dataDelay < 8; dataDelay++)
      {
        printf("  %u ",results[clkDelay][dataDelay]);
      }
      printf("%s","\r\n");
    }
  }
  else
  {
    Cli_Printf(CliInstance,"%s\r\n",StatusString( status ));
  }
}

cli_cmd_t Adrv9001CliSweepSsiDef =
{
    "Adrv9001SweepSsi",
    "Adrv9001SweepSsi:  Sweep ADRV9001 SSI Delay\r\n"
    "Adrv9001SweepSsi < Port (Tx1, Tx2, Rx1, Rx2) >\r\n\r\n",
    (CliCmdFn_t)Adrv9001Cli_SweepSsi,
    1,
    NULL
};

cli_cmd_t Adrv9001CliCalibrateSsiDef =
{
    "Adrv9001CalibrateSsi",
    "Adrv9001CalibrateSsi:  Calibrate ADRV9001 SSI Delay\r\n"
    "Adrv9001CalibrateSsi < Port (Tx1, Tx2, Rx1, Rx2) >\r\n\r\n",
    (CliCmdFn_t)Adrv9001Cli_CalibrateSsi,
    1,
    NULL
};

cli_cmd_t Adrv9001CliToRfCalibratedDef =
{
    "Adrv9001ToRfCalibrated",
    "Adrv9001ToRfCalibrated:  Changes ADRV9001 state to calibrated\r\n"
    "Adrv9001ToRfCalibrated < Port (Tx1, Tx2, Rx1, Rx2) >\r\n\r\n",
    (CliCmdFn_t)Adrv9001Cli_ToRfCalibrated,
    1,
    NULL
};

cli_cmd_t Adrv9001CliToRfPrimedDef =
{
    "Adrv9001ToRfPrimed",
    "Adrv9001ToRfPrimed:  Changes ADRV9001 state to primed\r\n"
    "Adrv9001ToRfPrimed < Port (Tx1, Tx2, Rx1, Rx2) >\r\n\r\n",
    (CliCmdFn_t)Adrv9001Cli_ToRfPrimed,
    1,
    NULL
};

cli_cmd_t Adrv9001CliToRfEnabledDef =
{
    "Adrv9001ToRfEnabled",
    "Adrv9001ToRfEnabled:  Changes ADRV9001 state to enabled\r\n"
    "Adrv9001ToRfEnabled < Port (Tx1, Tx2, Rx1, Rx2) >\r\n\r\n",
    (CliCmdFn_t)Adrv9001Cli_ToRfEnabled,
    1,
    NULL
};

cli_cmd_t Adrv9001CliListParamsDef =
{
    "Adrv9001ListParams",
    "Adrv9001ListParams:  List PHY parameter names for use with Adrv9001SetParms and Adrv9001GetParams.\r\n"
    "Adrv9001ListParams < keyword (optional) >\r\n\r\n",
    (CliCmdFn_t)Adrv9001Cli_ListParams,
    -1,
    NULL
};

cli_cmd_t Adrv9001CliSetParamDef =
{
    "Adrv9001SetParam",
    "Adrv9001SetParam:  Set parameter value \r\n"
    "Adrv9001SetParam < ParameterName, Value >\r\n\r\n",
    (CliCmdFn_t)Adrv9001Cli_SetParam,
    2,
    NULL
};

cli_cmd_t Adrv9001CliGetParamDef =
{
    "Adrv9001GetParam",
    "Adrv9001GetParam:  Get parameter value \r\n"
    "Adrv9001GetParam < ParameterName, >\r\n\r\n",
    (CliCmdFn_t)Adrv9001Cli_GetParam,
    1,
    NULL
};

int32_t Adrv9001Cli_Initialize( cli_t *Cli, adrv9001_t *Adrv9001 )
{
  Cli->CallbackRef = Cli;

  Adrv9001CliGetParamDef.userData = Adrv9001;
  Adrv9001CliSetParamDef.userData = Adrv9001;
  Adrv9001CliListParamsDef.userData = Adrv9001;
  Adrv9001CliToRfCalibratedDef.userData = Adrv9001;
  Adrv9001CliToRfPrimedDef.userData = Adrv9001;
  Adrv9001CliToRfEnabledDef.userData = Adrv9001;
  Adrv9001CliCalibrateSsiDef.userData = Adrv9001;
  Adrv9001CliSweepSsiDef.userData = Adrv9001;

  Cli_RegisterCommand(Cli, &Adrv9001CliGetParamDef);
  Cli_RegisterCommand(Cli, &Adrv9001CliSetParamDef);
  Cli_RegisterCommand(Cli, &Adrv9001CliListParamsDef);
  Cli_RegisterCommand(Cli, &Adrv9001CliToRfCalibratedDef);
  Cli_RegisterCommand(Cli, &Adrv9001CliToRfPrimedDef);
  Cli_RegisterCommand(Cli, &Adrv9001CliToRfEnabledDef);
  Cli_RegisterCommand(Cli, &Adrv9001CliCalibrateSsiDef);
  Cli_RegisterCommand(Cli, &Adrv9001CliSweepSsiDef);

  return Adrv9001Status_Success;
}
