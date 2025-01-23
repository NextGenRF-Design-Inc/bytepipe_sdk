/***************************************************************************//**
 *  \file       adrv9001_params.c
 *
 *  \details    This file contains the ADRV9001_PARAMS implementation.
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

#include "adrv9001.h"
#include "adrv9001_params.h"

int32_t Adrv9001Params_GetParamName( adrv9001_param_id_t Id, char *Name )
{
  strcpy(Name, Adrv9001ParamNames[Id]);

  return Adrv9001Status_Success;
}

int32_t Adrv9001Params_GetParamId( char *Name, adrv9001_param_id_t *Id)
{
  for( int i = 0; i < Adrv9001ParamId_Num; i++)
  {
    if( strcmp(Name, Adrv9001ParamNames[i]) == 0 )
    {
      *Id = (adrv9001_param_id_t)i;

      return Adrv9001Status_Success;
    }
  }

  return Adrv9001Status_InvalidParameter;
}

int32_t Adrv9001Pib_SetByNameByString( adrv9001_params_t *Instance, char *Name, char *Value )
{
  int32_t status;

  adrv9001_param_id_t Id;

  if(( status = Adrv9001Params_GetParamId( Name, &Id )) != 0)
    return status;

  if(( status = Adrv9001Params_SetByIdByString( Instance, Id, Value )) != 0)
    return status;

  return status;
}

int32_t Adrv9001Params_SetByIdByString( adrv9001_params_t *Instance, adrv9001_param_id_t Id, char *Value )
{
  int32_t status;

  adrv9001_t *Adrv9001 = Instance->Adrv9001;

  if( Id == Adrv9001ParamId_HwVer )
  {
    return Adrv9001Status_InvalidParameter;
  }
  else if( Id == Adrv9001ParamId_LogPath )
  {
    sprintf( Adrv9001->LogPath, "%s", Value );
  }
  else if( Id == Adrv9001ParamId_Tx1Attn )
  {
    float tmp;
    sscanf(Value, "%f", &tmp);
    status = Adrv9001_SetTxAttn( Adrv9001, ADI_CHANNEL_1, tmp );
  }
  else if( Id == Adrv9001ParamId_Tx2Attn )
  {
    float tmp;
    sscanf(Value, "%f", &tmp);
    status = Adrv9001_SetTxAttn( Adrv9001, ADI_CHANNEL_2, tmp );
  }
  else if( Id == Adrv9001ParamId_Tx1Boost )
  {
    uint8_t tmp;
    sscanf(Value, "%hhu", &tmp);
    status = Adrv9001_SetTxBoost( Adrv9001, ADI_CHANNEL_1, (bool)tmp );
  }
  else if( Id == Adrv9001ParamId_Tx2Boost )
  {
    uint8_t tmp;
    sscanf(Value, "%hhu", &tmp);
    status = Adrv9001_SetTxBoost( Adrv9001, ADI_CHANNEL_2, (bool)tmp );
  }
  

  else if( Id == Adrv9001ParamId_Tx1CarrierFrequency )
  {
    uint64_t tmp;
    sscanf(Value, "%llu", &tmp);
    status = Adrv9001_SetCarrierFrequency( Adrv9001, ADI_TX, ADI_CHANNEL_1, tmp );
  }
  else if( Id == Adrv9001ParamId_Tx2CarrierFrequency )
  {
    uint64_t tmp;
    sscanf(Value, "%llu", &tmp);
    status = Adrv9001_SetCarrierFrequency( Adrv9001, ADI_TX, ADI_CHANNEL_2, tmp );
  }
  else if( Id == Adrv9001ParamId_Rx1CarrierFrequency )
  {
    uint64_t tmp;
    sscanf(Value, "%llu", &tmp);
    status = Adrv9001_SetCarrierFrequency( Adrv9001, ADI_RX, ADI_CHANNEL_1, tmp );
  }
  else if( Id == Adrv9001ParamId_Rx2CarrierFrequency )
  {
    uint64_t tmp;
    sscanf(Value, "%llu", &tmp);
    status = Adrv9001_SetCarrierFrequency( Adrv9001, ADI_RX, ADI_CHANNEL_2, tmp );
  }
  else if( Id == Adrv9001ParamId_Tx1IqData )
  {
    uint32_t tmp;
    sscanf(Value, "%x", &tmp);
    AxiAdrv9001_SetFixedPattern( &Adrv9001->Axi, ADI_TX, ADI_CHANNEL_1, tmp );
    status = (int32_t)Adrv9001Status_Success;
  }
  else if( Id == Adrv9001ParamId_Tx2IqData )
  {
    uint32_t tmp;
    sscanf(Value, "%x", &tmp);
    AxiAdrv9001_SetFixedPattern( &Adrv9001->Axi, ADI_TX, ADI_CHANNEL_2, tmp );
    status = (int32_t)Adrv9001Status_Success;
  }
  else if( Id == Adrv9001ParamId_Rx1IqData )
  {
    uint32_t tmp;
    sscanf(Value, "%x", &tmp);
    AxiAdrv9001_SetFixedPattern( &Adrv9001->Axi, ADI_RX, ADI_CHANNEL_1, tmp );
    status = (int32_t)Adrv9001Status_Success;
  }
  else if( Id == Adrv9001ParamId_Rx2IqData )
  {
    uint32_t tmp;
    sscanf(Value, "%x", &tmp);
    AxiAdrv9001_SetFixedPattern( &Adrv9001->Axi, ADI_RX, ADI_CHANNEL_2, tmp );
    status = (int32_t)Adrv9001Status_Success;
  }
  else if( Id == Adrv9001ParamId_Tx1IqDataPath )
  {
    axi_adrv9001_data_src_t tmp;
    sscanf(Value, "%hhu", &tmp);
    AxiAdrv9001_SetTxDataSrc(&Adrv9001->Axi, tmp, ADI_CHANNEL_1 );
    status = (int32_t)Adrv9001Status_Success;
  }
  else if( Id == Adrv9001ParamId_Tx2IqDataPath )
  {
    axi_adrv9001_data_src_t tmp;
    sscanf(Value, "%hhu", &tmp);
    AxiAdrv9001_SetTxDataSrc(&Adrv9001->Axi, tmp, ADI_CHANNEL_2 );
    status = (int32_t)Adrv9001Status_Success;
  }
  else if( Id == Adrv9001ParamId_Dgpio )
  {
    uint32_t tmp;
    sscanf(Value, "%lu", &tmp);
    AxiAdrv9001_SetDgpio( &Adrv9001->Axi, tmp);
    status = (int32_t)Adrv9001Status_Success;
  }
  else if( Id == Adrv9001ParamId_DgpioDir )
  {
    uint32_t tmp;
    sscanf(Value, "%lu", &tmp);
    AxiAdrv9001_SetDgpioDir( &Adrv9001->Axi, tmp);
    status = (int32_t)Adrv9001Status_Success;
  }
  else if( Id == Adrv9001ParamId_VcTcxo )
  {
	  float tmp;
    sscanf(Value, "%f", &tmp);
	  status = Adrv9001_SetDacVoltage(Adrv9001, Instance->VcTcxoDac, tmp);
  }
  else if( Id == Adrv9001ParamId_Tx1RadioState )
  {
    adi_adrv9001_ChannelState_e tmp;
    sscanf(Value, "%hhu", &tmp);

    if( tmp == ADI_ADRV9001_CHANNEL_CALIBRATED )
      status = Adrv9001_ToCalibrated( Adrv9001, ADI_TX, ADI_CHANNEL_1 );
    else if( tmp == ADI_ADRV9001_CHANNEL_PRIMED)
      status = Adrv9001_ToPrimed( Adrv9001, ADI_TX, ADI_CHANNEL_1 );
    else if( tmp == ADI_ADRV9001_CHANNEL_RF_ENABLED)
      status = Adrv9001_ToRfEnabled( Adrv9001, ADI_TX, ADI_CHANNEL_1 );
  }
  else if( Id == Adrv9001ParamId_Tx2RadioState )
  {
    adi_adrv9001_ChannelState_e tmp;
    sscanf(Value, "%hhu", &tmp);

    if( tmp == ADI_ADRV9001_CHANNEL_CALIBRATED )
      status = Adrv9001_ToCalibrated( Adrv9001, ADI_TX, ADI_CHANNEL_2 );
    else if( tmp == ADI_ADRV9001_CHANNEL_PRIMED)
      status = Adrv9001_ToPrimed( Adrv9001, ADI_TX, ADI_CHANNEL_2 );
    else if( tmp == ADI_ADRV9001_CHANNEL_RF_ENABLED)
      status = Adrv9001_ToRfEnabled( Adrv9001, ADI_TX, ADI_CHANNEL_2 );
  }
  else if( Id == Adrv9001ParamId_Rx1RadioState )
  {
    adi_adrv9001_ChannelState_e tmp;
    sscanf(Value, "%hhu", &tmp);

    if( tmp == ADI_ADRV9001_CHANNEL_CALIBRATED )
      status = Adrv9001_ToCalibrated( Adrv9001, ADI_RX, ADI_CHANNEL_1 );
    else if( tmp == ADI_ADRV9001_CHANNEL_PRIMED)
      status = Adrv9001_ToPrimed( Adrv9001, ADI_RX, ADI_CHANNEL_1 );
    else if( tmp == ADI_ADRV9001_CHANNEL_RF_ENABLED)
      status = Adrv9001_ToRfEnabled( Adrv9001, ADI_RX, ADI_CHANNEL_1 );
  }
  else if( Id == Adrv9001ParamId_Rx2RadioState )
  {
    adi_adrv9001_ChannelState_e tmp;
    sscanf(Value, "%hhu", &tmp);

    if( tmp == ADI_ADRV9001_CHANNEL_CALIBRATED )
      status = Adrv9001_ToCalibrated( Adrv9001, ADI_RX, ADI_CHANNEL_2 );
    else if( tmp == ADI_ADRV9001_CHANNEL_PRIMED)
      status = Adrv9001_ToPrimed( Adrv9001, ADI_RX, ADI_CHANNEL_2 );
    else if( tmp == ADI_ADRV9001_CHANNEL_RF_ENABLED)
      status = Adrv9001_ToRfEnabled( Adrv9001, ADI_RX, ADI_CHANNEL_2 );
  }
  else if( Id == Adrv9001ParamId_TxRx1SsiLoopBack )
  {
    uint8_t tmp;
    sscanf(Value, "%hhu", &tmp);
    status = Adrv9001_SetTxToRxLoopBack( Adrv9001, ADI_CHANNEL_1, (bool)tmp );
  }
  else if( Id == Adrv9001ParamId_TxRx2SsiLoopBack )
  {
    uint8_t tmp;
    sscanf(Value, "%hhu", &tmp);
    status = Adrv9001_SetTxToRxLoopBack( Adrv9001, ADI_CHANNEL_2, (bool)tmp );
  }
  else if( Id == Adrv9001ParamId_Rx1CurGainIndex )
  {
    uint8_t tmp;
    sscanf(Value, "%hhu", &tmp);
    status = Adrv9001_SetRxCurGainIndex( Adrv9001, ADI_CHANNEL_1, tmp );
  }
  else if( Id == Adrv9001ParamId_Rx2CurGainIndex )
  {
    uint8_t tmp;
    sscanf(Value, "%hhu", &tmp);
    status = Adrv9001_SetRxCurGainIndex( Adrv9001, ADI_CHANNEL_2, tmp );
  }
  else if( Id == Adrv9001ParamId_Tx1TestMode )
  {
    uint8_t tmp;
    sscanf(Value, "%hhu", &tmp);
    status = Adrv9001_SetTxTestMode( Adrv9001, ADI_CHANNEL_1, (adi_adrv9001_SsiTestModeData_e)tmp );
  }
  else if( Id == Adrv9001ParamId_Tx2TestMode )
  {
    uint8_t tmp;
    sscanf(Value, "%hhu", &tmp);
    status = Adrv9001_SetTxTestMode( Adrv9001, ADI_CHANNEL_2, (adi_adrv9001_SsiTestModeData_e)tmp );
  }
  else if( Id == Adrv9001ParamId_Rx1TestMode )
  {
    uint8_t tmp;
    sscanf(Value, "%hhu", &tmp);
    status = Adrv9001_SetRxTestMode( Adrv9001, ADI_CHANNEL_1, (adi_adrv9001_SsiTestModeData_e)tmp );
  }
  else if( Id == Adrv9001ParamId_Rx2TestMode )
  {
    uint8_t tmp;
    sscanf(Value, "%hhu", &tmp);
    status = Adrv9001_SetRxTestMode( Adrv9001, ADI_CHANNEL_2, (adi_adrv9001_SsiTestModeData_e)tmp );
  }
  else if( Id == Adrv9001ParamId_Tx1TestData )
  {

  }
  else if( Id == Adrv9001ParamId_Tx2TestData )
  {

  }
  else if( Id == Adrv9001ParamId_Rx1TestData )
  {

  }
  else if( Id == Adrv9001ParamId_Rx2TestData )
  {

  }
  else if( Id == Adrv9001ParamId_Rx1InputPort )
  {
    uint8_t tmp;
    sscanf(Value, "%hhu", &tmp);
    status = Adrv9001_SetRxInputPort( Adrv9001, ADI_CHANNEL_1, (adi_adrv9001_RxRfInputSel_e)tmp );
  }
  else if( Id == Adrv9001ParamId_Rx2InputPort )
  {
    uint8_t tmp;
    sscanf(Value, "%hhu", &tmp);
    status = Adrv9001_SetRxInputPort( Adrv9001, ADI_CHANNEL_2, (adi_adrv9001_RxRfInputSel_e)tmp );
  }  

  return status;
}

int32_t Adrv9001Params_GetStringByName( adrv9001_params_t *Instance, char *Name, char *Value )
{
  int32_t status;

  adrv9001_param_id_t Id;

  if(( status = Adrv9001Params_GetParamId( Name, &Id )) != 0)
    return status;

  if(( status = Adrv9001Params_GetStringById( Instance, Id, Value )) != 0)
    return status;

  return status;
}

int32_t Adrv9001Params_GetStringById( adrv9001_params_t *Instance, uint16_t Id, char *Value )
{
  int32_t status;

  adrv9001_t *Adrv9001 = Instance->Adrv9001;

  if( Id == Adrv9001ParamId_HwVer )
  {
    return Adrv9001Status_InvalidParameter;
  }
  else if( Id == Adrv9001ParamId_LogPath )
  {
    strcpy(Value, Adrv9001->LogPath );
  }
  else if( Id == Adrv9001ParamId_Tx1Attn )
  {
    sprintf(Value, "%2.2f", Adrv9001->TxAttn[0] );
  }
  else if( Id == Adrv9001ParamId_Tx2Attn )
  {
    sprintf(Value, "%2.2f", Adrv9001->TxAttn[1] );
  }
  else if( Id == Adrv9001ParamId_Tx1Boost )
  {
    sprintf(Value, "%hhu", (uint8_t)Adrv9001->TxBoost[0]);
  }
  else if( Id == Adrv9001ParamId_Tx2Boost )
  {
    sprintf(Value, "%hhu", (uint8_t)Adrv9001->TxBoost[1]);
  }
  else if( Id == Adrv9001ParamId_Tx1CarrierFrequency )
  {
    uint64_t tmp;
    if((status = Adrv9001_GetCarrierFrequency( Adrv9001, ADI_TX, ADI_CHANNEL_1, &tmp )) != 0)
      return status;

    sprintf(Value, "%llu", tmp);
  }
  else if( Id == Adrv9001ParamId_Tx2CarrierFrequency )
  {
    uint64_t tmp;
    if((status = Adrv9001_GetCarrierFrequency( Adrv9001, ADI_TX, ADI_CHANNEL_2, &tmp )) != 0)
      return status;

    sprintf(Value, "%llu", tmp);
  }
  else if( Id == Adrv9001ParamId_Rx1CarrierFrequency )
  {
    uint64_t tmp;
    if((status = Adrv9001_GetCarrierFrequency( Adrv9001, ADI_RX, ADI_CHANNEL_1, &tmp )) != 0)
      return status;

    sprintf(Value, "%llu", tmp);
  }
  else if( Id == Adrv9001ParamId_Rx2CarrierFrequency )
  {
    uint64_t tmp;
    if((status = Adrv9001_GetCarrierFrequency( Adrv9001, ADI_RX, ADI_CHANNEL_2, &tmp )) != 0)
      return status;

    sprintf(Value, "%llu", tmp);
  }
  else if( Id == Adrv9001ParamId_Tx1IqData )
  {
    uint32_t tmp = AxiAdrv9001_GetFixedPattern( &Adrv9001->Axi, ADI_TX, ADI_CHANNEL_1 );
    sprintf(Value, "%x", tmp);
  }
  else if( Id == Adrv9001ParamId_Tx2IqData )
  {
    uint32_t tmp = AxiAdrv9001_GetFixedPattern( &Adrv9001->Axi, ADI_TX, ADI_CHANNEL_2 );
    sprintf(Value, "%x", tmp);
  }
  else if( Id == Adrv9001ParamId_Rx1IqData )
  {
    uint32_t tmp = AxiAdrv9001_GetFixedPattern( &Adrv9001->Axi, ADI_RX, ADI_CHANNEL_1 );
    sprintf(Value, "%x", tmp);
  }
  else if( Id == Adrv9001ParamId_Rx2IqData )
  {
    uint32_t tmp = AxiAdrv9001_GetFixedPattern( &Adrv9001->Axi, ADI_RX, ADI_CHANNEL_2 );
    sprintf(Value, "%x", tmp);
  }
  else if( Id == Adrv9001ParamId_Tx1IqDataPath )
  {
    axi_adrv9001_data_src_t tmp = AxiAdrv9001_GetTxDataSrc(&Adrv9001->Axi, ADI_CHANNEL_1 );
    sprintf(Value, "%hhu", tmp);
  }
  else if( Id == Adrv9001ParamId_Tx2IqDataPath )
  {
    axi_adrv9001_data_src_t tmp = AxiAdrv9001_GetTxDataSrc(&Adrv9001->Axi, ADI_CHANNEL_2 );
    sprintf(Value, "%hhu", tmp);
  }
  else if( Id == Adrv9001ParamId_Dgpio )
  {
    uint32_t tmp;
    AxiAdrv9001_GetDgpio( &Adrv9001->Axi, &tmp);
    sprintf(Value, "%lu", tmp);
  }
  else if( Id == Adrv9001ParamId_DgpioDir )
  {
    uint32_t tmp;
    AxiAdrv9001_GetDgpioDir( &Adrv9001->Axi, &tmp);
    sprintf(Value, "%lu", tmp);
  }
  else if( Id == Adrv9001ParamId_VcTcxo )
  {
    float tmp;
    Adrv9001_GetDacVoltage(Adrv9001, Instance->VcTcxoDac, &tmp);
    sprintf(Value, "%2.2f", tmp);
  }
  else if( Id == Adrv9001ParamId_Tx1RadioState )
  {
    adi_adrv9001_ChannelState_e tmp;
    if((status = Adrv9001_GetRadioState( Adrv9001, ADI_TX, ADI_CHANNEL_1, &tmp )) != 0)
      return status;

    sprintf(Value, "%hhu", (uint8_t)tmp );
  }
  else if( Id == Adrv9001ParamId_Tx2RadioState )
  {
    adi_adrv9001_ChannelState_e tmp;
    if((status = Adrv9001_GetRadioState( Adrv9001, ADI_TX, ADI_CHANNEL_2, &tmp )) != 0)
      return status;

    sprintf(Value, "%hhu", (uint8_t)tmp );
  }
  else if( Id == Adrv9001ParamId_Rx1RadioState )
  {
    adi_adrv9001_ChannelState_e tmp;
    if((status = Adrv9001_GetRadioState( Adrv9001, ADI_RX, ADI_CHANNEL_1, &tmp )) != 0)
      return status;

    sprintf(Value, "%hhu", (uint8_t)tmp );
  }
  else if( Id == Adrv9001ParamId_Rx2RadioState )
  {
    adi_adrv9001_ChannelState_e tmp;
    if((status = Adrv9001_GetRadioState( Adrv9001, ADI_RX, ADI_CHANNEL_2, &tmp )) != 0)
      return status;

    sprintf(Value, "%hhu", (uint8_t)tmp );
  }
  else if( Id == Adrv9001ParamId_TxRx1SsiLoopBack )
  {
    bool tmp;
    if((status = Adrv9001_GetTxToRxLoopBack( Adrv9001, ADI_CHANNEL_1, &tmp )) != 0)
      return status;

    sprintf(Value, "%s", tmp? "True" : "False");
  }
  else if( Id == Adrv9001ParamId_TxRx2SsiLoopBack )
  {
    bool tmp;
    if((status = Adrv9001_GetTxToRxLoopBack( Adrv9001, ADI_CHANNEL_2, &tmp )) != 0)
      return status;

    sprintf(Value, "%s", tmp? "True" : "False");
  }
  else if( Id == Adrv9001ParamId_FirmwareVersion )
  {
    adi_adrv9001_ArmVersion_t tmp;
    if((status = Adrv9001_GetFirmwareVersion( Adrv9001, &tmp )) != 0)
      return status;

    sprintf(Value, "%hhu.%hhu.%hhu.%hhu", tmp.majorVer, tmp.minorVer, tmp.maintVer, tmp.armBuildType);
  }
  else if( Id == Adrv9001ParamId_APIVersion )
  {
    adi_common_ApiVersion_t tmp;
    if((status = Adrv9001_GetApiVersion( Adrv9001, &tmp )) != 0)
      return status;

    sprintf(Value, "%lu.%lu.%lu", tmp.major, tmp.minor, tmp.patch );
  }
  else if( Id == Adrv9001ParamId_SiliconVersion )
  {
    adi_adrv9001_SiliconVersion_t tmp;
    if((status = Adrv9001_GetSiliconVersion( Adrv9001, &tmp )) != 0)
      return status;

    sprintf(Value, "%X%X", tmp.major, tmp.minor);
  }
  else if( Id == Adrv9001ParamId_Temp )
  {
    int16_t tmp;
    if((status = Adrv9001_GetTemperature( Adrv9001, &tmp )) != 0)
      return status;

    sprintf(Value, "%d", tmp );
  }
  else if( Id == Adrv9001ParamId_Rx1Rssi )
  {
    float tmp;
    if((status = Adrv9001_GetRxRssi( Adrv9001, ADI_CHANNEL_1, &tmp )) != 0)
      return status;

    sprintf(Value, "%2.2f", tmp );
  }
  else if( Id == Adrv9001ParamId_Rx2Rssi )
  {
    float tmp;
    if((status = Adrv9001_GetRxRssi( Adrv9001, ADI_CHANNEL_2, &tmp )) != 0)
      return status;

    sprintf(Value, "%2.2f", tmp );
  }
  else if( Id == Adrv9001ParamId_Rx1CurGainIndex )
  {
    uint8_t tmp;
    if((status = Adrv9001_GetRxCurGainIndex( Adrv9001, ADI_CHANNEL_1, &tmp )) != 0)
      return status;

    sprintf(Value, "%hhu", (uint8_t)tmp );
  }
  else if( Id == Adrv9001ParamId_Rx2CurGainIndex )
  {
    uint8_t tmp;
    if((status = Adrv9001_GetRxCurGainIndex( Adrv9001, ADI_CHANNEL_2, &tmp )) != 0)
      return status;

    sprintf(Value, "%hhu", (uint8_t)tmp );
  }
  else if( Id == Adrv9001ParamId_DeviceClock_kHz )
  {
    sprintf(Value, "%lu", Adrv9001->Params->clocks.deviceClock_kHz );
  }
  else if( Id == Adrv9001ParamId_ClkPllVcoFreq_daHz )
  {
    sprintf(Value, "%lu", Adrv9001->Params->clocks.clkPllVcoFreq_daHz );
  }
  else if( Id == Adrv9001ParamId_ClkPllHsDiv )
  {
    sprintf(Value, "%hhu", (uint8_t)Adrv9001->Params->clocks.clkPllHsDiv );
  }
  else if( Id == Adrv9001ParamId_ClkPllMode )
  {
    sprintf(Value, "%hhu", (uint8_t)Adrv9001->Params->clocks.clkPllMode );
  }
  else if( Id == Adrv9001ParamId_Clk1105Div )
  {
    sprintf(Value, "%hhu", (uint8_t)Adrv9001->Params->clocks.clk1105Div );
  }
  else if( Id == Adrv9001ParamId_ArmClkDiv )
  {
    sprintf(Value, "%hhu", (uint8_t)Adrv9001->Params->clocks.armClkDiv );
  }
  else if( Id == Adrv9001ParamId_ArmPowerSavingClkDiv )
  {
    sprintf(Value, "%hhu", (uint8_t)Adrv9001->Params->clocks.armPowerSavingClkDiv );
  }
  else if( Id == Adrv9001ParamId_RefClockOutEnable )
  {
    sprintf(Value, "%hhu", (uint8_t)Adrv9001->Params->clocks.refClockOutEnable );
  }
  else if( Id == Adrv9001ParamId_AuxPllPower )
  {
    sprintf(Value, "%hhu", (uint8_t)Adrv9001->Params->clocks.auxPllPower );
  }
  else if( Id == Adrv9001ParamId_ClkPllPower )
  {
    sprintf(Value, "%hhu", (uint8_t)Adrv9001->Params->clocks.clkPllPower );
  }
  else if( Id == Adrv9001ParamId_PadRefClkDrv )
  {
    sprintf(Value, "%hhu", Adrv9001->Params->clocks.padRefClkDrv );
  }
  else if( Id == Adrv9001ParamId_ExtLo1OutFreq_kHz )
  {
    sprintf(Value, "%lu", Adrv9001->Params->clocks.extLo1OutFreq_kHz );
  }
  else if( Id == Adrv9001ParamId_ExtLo2OutFreq_kHz )
  {
    sprintf(Value, "%lu", Adrv9001->Params->clocks.extLo2OutFreq_kHz );
  }
  else if( Id == Adrv9001ParamId_RfPll1LoMode )
  {
    sprintf(Value, "%hhu", (uint8_t)Adrv9001->Params->clocks.rfPll1LoMode );
  }
  else if( Id == Adrv9001ParamId_RfPll2LoMode )
  {
    sprintf(Value, "%hhu", (uint8_t)Adrv9001->Params->clocks.rfPll2LoMode );
  }
  else if( Id == Adrv9001ParamId_Ext1LoType )
  {
    sprintf(Value, "%hhu", (uint8_t)Adrv9001->Params->clocks.ext1LoType );
  }
  else if( Id == Adrv9001ParamId_Ext2LoType )
  {
    sprintf(Value, "%hhu", (uint8_t)Adrv9001->Params->clocks.ext2LoType );
  }
  else if( Id == Adrv9001ParamId_Rx1RfInputSel )
  {
    sprintf(Value, "%hhu", (uint8_t)Adrv9001->Params->clocks.rx1RfInputSel );
  }
  else if( Id == Adrv9001ParamId_Rx2RfInputSel )
  {
    sprintf(Value, "%hhu", (uint8_t)Adrv9001->Params->clocks.rx2RfInputSel );
  }
  else if( Id == Adrv9001ParamId_ExtLo1Divider )
  {
    sprintf(Value, "%hu", Adrv9001->Params->clocks.extLo1Divider );
  }
  else if( Id == Adrv9001ParamId_ExtLo2Divider )
  {
    sprintf(Value, "%hu", Adrv9001->Params->clocks.extLo2Divider );
  }
  else if( Id == Adrv9001ParamId_RfPllPhaseSyncMode )
  {
    sprintf(Value, "%hhu", (uint8_t)Adrv9001->Params->clocks.rfPllPhaseSyncMode );
  }
  else if( Id == Adrv9001ParamId_Rx1LoSelect )
  {
    sprintf(Value, "%hhu", (uint8_t)Adrv9001->Params->clocks.rx1LoSelect );
  }
  else if( Id == Adrv9001ParamId_Rx2LoSelect )
  {
    sprintf(Value, "%hhu", (uint8_t)Adrv9001->Params->clocks.rx2LoSelect );
  }
  else if( Id == Adrv9001ParamId_Tx1LoSelect )
  {
    sprintf(Value, "%hhu", (uint8_t)Adrv9001->Params->clocks.tx1LoSelect );
  }
  else if( Id == Adrv9001ParamId_Tx2LoSelect )
  {
    sprintf(Value, "%hhu", (uint8_t)Adrv9001->Params->clocks.tx2LoSelect );
  }
  else if( Id == Adrv9001ParamId_Rx1LoDivMode )
  {
    sprintf(Value, "%hhu", (uint8_t)Adrv9001->Params->clocks.rx1LoDivMode );
  }
  else if( Id == Adrv9001ParamId_Rx2LoDivMode )
  {
    sprintf(Value, "%hhu", (uint8_t)Adrv9001->Params->clocks.rx2LoDivMode );
  }
  else if( Id == Adrv9001ParamId_Tx1LoDivMode )
  {
    sprintf(Value, "%hhu", (uint8_t)Adrv9001->Params->clocks.tx1LoDivMode );
  }
  else if( Id == Adrv9001ParamId_Tx2LoDivMode )
  {
    sprintf(Value, "%hhu", (uint8_t)Adrv9001->Params->clocks.tx2LoDivMode );
  }
  else if( Id == Adrv9001ParamId_LoGen1Select )
  {
    sprintf(Value, "%hhu", (uint8_t)Adrv9001->Params->clocks.loGen1Select );
  }
  else if( Id == Adrv9001ParamId_LoGen2Select )
  {
    sprintf(Value, "%hhu", (uint8_t)Adrv9001->Params->clocks.loGen2Select );
  }
  else if( Id == Adrv9001ParamId_Tx1SampleRate )
  {
    uint32_t tmp;
    if((status = Adrv9001_GetSampleRate( Adrv9001, ADI_TX, ADI_CHANNEL_1, &tmp)) != 0)
      return status;

    sprintf(Value, "%lu", tmp);
  }
  else if( Id == Adrv9001ParamId_Tx2SampleRate )
  {
    uint32_t tmp;
    if((status = Adrv9001_GetSampleRate( Adrv9001, ADI_TX, ADI_CHANNEL_2, &tmp)) != 0)
      return status;

    sprintf(Value, "%lu", tmp);
  }
  else if( Id == Adrv9001ParamId_Rx1SampleRate )
  {
    uint32_t tmp;
    if((status = Adrv9001_GetSampleRate( Adrv9001, ADI_RX, ADI_CHANNEL_1, &tmp)) != 0)
      return status;

    sprintf(Value, "%lu", tmp);
  }
  else if( Id == Adrv9001ParamId_Rx2SampleRate )
  {
    uint32_t tmp;
    if((status = Adrv9001_GetSampleRate( Adrv9001, ADI_RX, ADI_CHANNEL_2, &tmp)) != 0)
      return status;

    sprintf(Value, "%lu", tmp);
  }
  else if( Id == Adrv9001ParamId_Tx1TestMode )
  {
    adi_adrv9001_SsiTestModeData_e tmp;

    if((status = Adrv9001_GetTxTestMode( Adrv9001, ADI_CHANNEL_1, &tmp )) != 0)
      return status;

    sprintf(Value, "%hhu", tmp);
  }
  else if( Id == Adrv9001ParamId_Tx2TestMode )
  {
    adi_adrv9001_SsiTestModeData_e tmp;

    if((status = Adrv9001_GetTxTestMode( Adrv9001, ADI_CHANNEL_2, &tmp )) != 0)
      return status;

    sprintf(Value, "%hhu", tmp);
  }
  else if( Id == Adrv9001ParamId_Rx1TestMode )
  {
	  adi_adrv9001_SsiTestModeData_e tmp;

	  if((status = Adrv9001_GetRxTestMode( Adrv9001, ADI_CHANNEL_1, &tmp )) != 0)
      return status;

    sprintf(Value, "%hhu", tmp);

  }
  else if( Id == Adrv9001ParamId_Rx2TestMode )
  {
    adi_adrv9001_SsiTestModeData_e tmp;

    if((status = Adrv9001_GetRxTestMode( Adrv9001, ADI_CHANNEL_2, &tmp )) != 0)
      return status;

    sprintf(Value, "%hhu", tmp);
  }
  else if( Id == Adrv9001ParamId_Tx1TestData )
  {

  }
  else if( Id == Adrv9001ParamId_Tx2TestData )
  {

  }
  else if( Id == Adrv9001ParamId_Rx1TestData )
  {

  }
  else if( Id == Adrv9001ParamId_Rx2TestData )
  {

  }
  else if( Id == Adrv9001ParamId_Rx1InputPort )
  {
    adi_adrv9001_RxRfInputSel_e tmp;

    if((status = Adrv9001_GetRxInputPort( Adrv9001, ADI_CHANNEL_1, &tmp )) != 0)
      return status;

    sprintf(Value, "%hhu", tmp);
  }
  else if( Id == Adrv9001ParamId_Rx2InputPort )
  {
    adi_adrv9001_RxRfInputSel_e tmp;

    if((status = Adrv9001_GetRxInputPort( Adrv9001, ADI_CHANNEL_2, &tmp )) != 0)
      return status;

    sprintf(Value, "%hhu", tmp);
  }    

  return Adrv9001Status_Success;
}

int32_t Adrv9001Params_Initialize( adrv9001_params_t *Instance, adrv9001_params_init_t *Init )
{
  Instance->Adrv9001 = Init->Adrv9001;
  Instance->Rx1LnaEnablePin = Init->Rx1LnaEnablePin;
  Instance->Rx2LnaEnablePin = Init->Rx2LnaEnablePin;
  Instance->VcTcxoDac = Init->VcTcxoDac;
  Instance->VcTcxoEnablePin = Init->VcTcxoEnablePin;

  Adrv9001_EnableManualInputPorts( Instance->Adrv9001, true );

  return Adrv9001Status_Success;
}
