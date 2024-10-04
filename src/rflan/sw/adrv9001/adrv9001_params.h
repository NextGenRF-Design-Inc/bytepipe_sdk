#ifndef SRC_ADRV9001_PARAMS_H_
#define SRC_ADRV9001_PARAMS_H_
/***************************************************************************//**
*  \file       adrv9001_params.h
*
*  \details
*
*  This file contains definitions for the adrv9001 parameters.
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
#include <stdint.h>
#include <stdbool.h>


typedef enum
{
  Adrv9001ParamId_HwVer                                         ,
  Adrv9001ParamId_LogPath                                       ,
  Adrv9001ParamId_Tx1Attn                                       ,
  Adrv9001ParamId_Tx2Attn                                       ,
  Adrv9001ParamId_Tx1Boost                                      ,
  Adrv9001ParamId_Tx2Boost                                      ,
  Adrv9001ParamId_Tx1CarrierFrequency                           ,
  Adrv9001ParamId_Tx2CarrierFrequency                           ,
  Adrv9001ParamId_Rx1CarrierFrequency                           ,
  Adrv9001ParamId_Rx2CarrierFrequency                           ,
  Adrv9001ParamId_Tx1IqData                                     ,
  Adrv9001ParamId_Tx2IqData                                     ,
  Adrv9001ParamId_Rx1IqData                                     ,
  Adrv9001ParamId_Rx2IqData                                     ,
  Adrv9001ParamId_Tx1IqDataPath                                 ,
  Adrv9001ParamId_Tx2IqDataPath                                 ,
  Adrv9001ParamId_Dgpio                                         ,
  Adrv9001ParamId_DgpioDir                                      ,
  Adrv9001ParamId_VcTcxo                                        ,
  Adrv9001ParamId_Tx1RadioState                                 ,
  Adrv9001ParamId_Tx2RadioState                                 ,
  Adrv9001ParamId_Rx1RadioState                                 ,
  Adrv9001ParamId_Rx2RadioState                                 ,
  Adrv9001ParamId_TxRx1SsiLoopBack                              ,
  Adrv9001ParamId_TxRx2SsiLoopBack                              ,
  Adrv9001ParamId_FirmwareVersion                               ,
  Adrv9001ParamId_APIVersion                                    ,
  Adrv9001ParamId_SiliconVersion                                ,
  Adrv9001ParamId_Temp                                          ,
  Adrv9001ParamId_Rx1Rssi                                       ,
  Adrv9001ParamId_Rx2Rssi                                       ,
  Adrv9001ParamId_Rx1CurGainIndex                               ,
  Adrv9001ParamId_Rx2CurGainIndex                               ,
  Adrv9001ParamId_DeviceClock_kHz                               ,
  Adrv9001ParamId_ClkPllVcoFreq_daHz                            ,
  Adrv9001ParamId_ClkPllHsDiv                                   ,
  Adrv9001ParamId_ClkPllMode                                    ,
  Adrv9001ParamId_Clk1105Div                                    ,
  Adrv9001ParamId_ArmClkDiv                                     ,
  Adrv9001ParamId_ArmPowerSavingClkDiv                          ,
  Adrv9001ParamId_RefClockOutEnable                             ,
  Adrv9001ParamId_AuxPllPower                                   ,
  Adrv9001ParamId_ClkPllPower                                   ,
  Adrv9001ParamId_PadRefClkDrv                                  ,
  Adrv9001ParamId_ExtLo1OutFreq_kHz                             ,
  Adrv9001ParamId_ExtLo2OutFreq_kHz                             ,
  Adrv9001ParamId_RfPll1LoMode                                  ,
  Adrv9001ParamId_RfPll2LoMode                                  ,
  Adrv9001ParamId_Ext1LoType                                    ,
  Adrv9001ParamId_Ext2LoType                                    ,
  Adrv9001ParamId_Rx1RfInputSel                                 ,
  Adrv9001ParamId_Rx2RfInputSel                                 ,
  Adrv9001ParamId_ExtLo1Divider                                 ,
  Adrv9001ParamId_ExtLo2Divider                                 ,
  Adrv9001ParamId_RfPllPhaseSyncMode                            ,
  Adrv9001ParamId_Rx1LoSelect                                   ,
  Adrv9001ParamId_Rx2LoSelect                                   ,
  Adrv9001ParamId_Tx1LoSelect                                   ,
  Adrv9001ParamId_Tx2LoSelect                                   ,
  Adrv9001ParamId_Rx1LoDivMode                                  ,
  Adrv9001ParamId_Rx2LoDivMode                                  ,
  Adrv9001ParamId_Tx1LoDivMode                                  ,
  Adrv9001ParamId_Tx2LoDivMode                                  ,
  Adrv9001ParamId_LoGen1Select                                  ,
  Adrv9001ParamId_LoGen2Select                                  ,
  Adrv9001ParamId_Tx1SampleRate                                 ,
  Adrv9001ParamId_Tx2SampleRate                                 ,
  Adrv9001ParamId_Rx1SampleRate                                 ,
  Adrv9001ParamId_Rx2SampleRate                                 ,
  Adrv9001ParamId_Tx1TestMode                                   ,
  Adrv9001ParamId_Tx2TestMode                                   ,
  Adrv9001ParamId_Rx1TestMode                                   ,
  Adrv9001ParamId_Rx2TestMode                                   ,
  Adrv9001ParamId_Tx1TestData                                   ,
  Adrv9001ParamId_Tx2TestData                                   ,
  Adrv9001ParamId_Rx1TestData                                   ,
  Adrv9001ParamId_Rx2TestData                                   ,
                                                                
  Adrv9001ParamId_Num                                           ,

}adrv9001_param_id_t;

static const char* Adrv9001ParamNames[] = {
  "HwVer",
  "LogPath",
  "Tx1Attn",
  "Tx2Attn",
  "Tx1Boost",
  "Tx2Boost",
  "Tx1CarrierFrequency",
  "Tx2CarrierFrequency",
  "Rx1CarrierFrequency",
  "Rx2CarrierFrequency",
  "Tx1IqData",
  "Tx2IqData",
  "Rx1IqData",
  "Rx2IqData",
  "Tx1IqDataPath",
  "Tx2IqDataPath",
  "Dgpio",
  "DgpioDir",
  "VcTcxo",
  "Tx1RadioState",
  "Tx2RadioState",
  "Rx1RadioState",
  "Rx2RadioState",
  "TxRx1SsiLoopBack",
  "TxRx2SsiLoopBack",
  "FirmwareVersion",
  "APIVersion",
  "SiliconVersion",
  "Temp",
  "Rx1Rssi",
  "Rx2Rssi",
  "Rx1CurGainIndex",
  "Rx2CurGainIndex",
  "DeviceClock_kHz",
  "ClkPllVcoFreq_daHz",
  "ClkPllHsDiv",
  "ClkPllMode",
  "Clk1105Div",
  "ArmClkDiv",
  "ArmPowerSavingClkDiv",
  "RefClockOutEnable",
  "AuxPllPower",
  "ClkPllPower",
  "PadRefClkDrv",
  "ExtLo1OutFreq_kHz",
  "ExtLo2OutFreq_kHz",
  "RfPll1LoMode",
  "RfPll2LoMode",
  "Ext1LoType",
  "Ext2LoType",
  "Rx1RfInputSel",
  "Rx2RfInputSel",
  "ExtLo1Divider",
  "ExtLo2Divider",
  "RfPllPhaseSyncMode",
  "Rx1LoSelect",
  "Rx2LoSelect",
  "Tx1LoSelect",
  "Tx2LoSelect",
  "Rx1LoDivMode",
  "Rx2LoDivMode",
  "Tx1LoDivMode",
  "Tx2LoDivMode",
  "LoGen1Select",
  "LoGen2Select",
  "Tx1SampleRate",
  "Tx2SampleRate",
  "Rx1SampleRate",
  "Rx2SampleRate",
  "Tx1TestMode",
  "Tx2TestMode",
  "Rx1TestMode",
  "Rx2TestMode",
  "Tx1TestData",
  "Tx2TestData",
  "Rx1TestData",
  "Rx2TestData"
};

typedef struct{
  adrv9001_t              *Adrv9001;
  adi_adrv9001_GpioPin_e   Rx1LnaEnablePin;
  adi_adrv9001_GpioPin_e   Rx2LnaEnablePin;
  adi_adrv9001_GpioPin_e   VcTcxoEnablePin;
  adi_adrv9001_AuxDac_e    VcTcxoDac;
}adrv9001_params_init_t;


typedef struct{
  adrv9001_t              *Adrv9001;
  adi_adrv9001_GpioPin_e   Rx1LnaEnablePin;
  adi_adrv9001_GpioPin_e   Rx2LnaEnablePin;
  adi_adrv9001_GpioPin_e   VcTcxoEnablePin;
  adi_adrv9001_AuxDac_e    VcTcxoDac;
}adrv9001_params_t;

int32_t Adrv9001Params_Initialize( adrv9001_params_t *Instance, adrv9001_params_init_t *Init );
int32_t Adrv9001Params_SetByIdByString( adrv9001_params_t *Instance, adrv9001_param_id_t Id, char *Value );
int32_t Adrv9001Pib_SetByNameByString( adrv9001_params_t *Instance, char *Name, char *Value );
int32_t Adrv9001Params_GetStringById( adrv9001_params_t *Instance, uint16_t Id, char *Value );
int32_t Adrv9001Params_GetStringByName( adrv9001_params_t *Instance, char *Name, char *Value );

#endif
