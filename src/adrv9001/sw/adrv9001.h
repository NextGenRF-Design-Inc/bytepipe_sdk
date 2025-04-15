#ifndef SRC_ADRV9001_H_
#define SRC_ADRV9001_H_
/***************************************************************************//**
*  \file       adrv9001.h
*
*  \details
*
*  This file contains definitions for the adrv9001 interface driver.  The
*  adrv9001 interface driver implements algorithms that call one or several
*  adi_adrv9001 API functions.
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

#include "adi_adrv9001_rx_types.h"
#include "adi_adrv9001_tx_types.h"
#include "adi_adrv9001.h"
#include "adi_adrv9001_error.h"
#include "adi_adrv9001_radio_types.h"
#include "adi_adrv9001_rx_gaincontrol_types.h"
#include "adi_adrv9001_rx_gaincontrol.h"
#include "adi_adrv9001_arm.h"
#include "adi_adrv9001_radio.h"
#include "adi_adrv9001_profileutil.h"
#include "adi_adrv9001_gpio.h"
#include "adi_adrv9001_tx.h"
#include "adi_adrv9001_rx.h"
#include "adi_adrv9001_auxdac.h"
#include "adi_adrv9001_auxadc.h"
#include "adi_adrv9001_utilities.h"
#include "adi_adrv9001_ssi.h"
#include "adi_adrv9001_fh.h"
#include "ff.h"
#include "axi_adrv9001.h"
#include "jsmn.h"
#include "adi_adrv9001_profileutil.h"
#include "adrv9001_Init_t_parser.h"
#include "adi_adrv9001_arm.h"
#include "adi_adrv9001_cals.h"
#include "adi_adrv9001_gpio.h"
#include "adi_adrv9001_mcs.h"
#include "adi_adrv9001_stream.h"
#include "adi_adrv9001_powermanagement.h"
#include "adi_adrv9001_powersavingandmonitormode.h"
#include "adi_adrv9001_bbdc.h"


#ifndef ADRV9001_STATUS_OFFSET
#define ADRV9001_STATUS_OFFSET    (0)
#endif

#define ADRV9001_MAX_DAC_VOLTAGE    (1.75)
#define ADRV9001_MIN_DAC_VOLTAGE    (0.05)
#define ADRV9001_TDD_ENABLE_DUR_FOREVER (0xffffffff)

#define ADRV9001_MAX_HOP_TABLE_SIZE     (64)
#define ADRV9001_HOP_TABLE_START_FREQ   (2800000000)
#define ADRV9001_HOP_TABLE_STEP_FREQ    (40000000)

#define ADRV9001_TEST_MODE_PATTERN        (0xABCDABCD)

extern adi_adrv9001_Init_t initialize_init_8;

/**
* \brief Code indicated status of request
*/
typedef enum
{
  Adrv9001Status_Success              = (0),
  Adrv9001Status_ProfileInitErr       = (ADRV9001_STATUS_OFFSET - 1),
  Adrv9001Status_ProfileCalErr        = (ADRV9001_STATUS_OFFSET - 2),
  Adrv9001Status_ProfileCfgErr        = (ADRV9001_STATUS_OFFSET - 3),
  Adrv9001Status_ToPrimedErr          = (ADRV9001_STATUS_OFFSET - 4),
  Adrv9001Status_ToCalErr             = (ADRV9001_STATUS_OFFSET - 5),
  Adrv9001Status_ToEnabledErr         = (ADRV9001_STATUS_OFFSET - 6),
  Adrv9001Status_NotSupported         = (ADRV9001_STATUS_OFFSET - 7),
  Adrv9001Status_DacErr               = (ADRV9001_STATUS_OFFSET - 8),
  Adrv9001Status_TxAttnErr            = (ADRV9001_STATUS_OFFSET - 9),
  Adrv9001Status_TxBoostErr           = (ADRV9001_STATUS_OFFSET - 10),
  Adrv9001Status_GpioErr              = (ADRV9001_STATUS_OFFSET - 11),
  Adrv9001Status_LogErr               = (ADRV9001_STATUS_OFFSET - 12),
  Adrv9001Status_Busy                 = (ADRV9001_STATUS_OFFSET - 13),
  Adrv9001Status_RxGainTableErr       = (ADRV9001_STATUS_OFFSET - 14),
  Adrv9001Status_SpiErr               = (ADRV9001_STATUS_OFFSET - 15),
  Adrv9001Status_InvalidPaEnable      = (ADRV9001_STATUS_OFFSET - 16),
  Adrv9001Status_SsiSweepErr          = (ADRV9001_STATUS_OFFSET - 17),
  Adrv9001Status_SsiTestModeErr       = (ADRV9001_STATUS_OFFSET - 18),
  Adrv9001Status_SsiSetErr            = (ADRV9001_STATUS_OFFSET - 19),
  Adrv9001Status_InvalidState         = (ADRV9001_STATUS_OFFSET - 20),
  Adrv9001Status_CarrierFreqErr       = (ADRV9001_STATUS_OFFSET - 21),
  Adrv9001Status_ReadErr              = (ADRV9001_STATUS_OFFSET - 22),
  Adrv9001Status_EnableModeErr        = (ADRV9001_STATUS_OFFSET - 23),
  Adrv9001Status_InvalidEnableMode    = (ADRV9001_STATUS_OFFSET - 24),
  Adrv9001Status_IrqErr               = (ADRV9001_STATUS_OFFSET - 25),
  Adrv9001Status_Tx1SsiCalErr         = (ADRV9001_STATUS_OFFSET - 26),
  Adrv9001Status_Tx2SsiCalErr         = (ADRV9001_STATUS_OFFSET - 27),
  Adrv9001Status_Rx1SsiCalErr         = (ADRV9001_STATUS_OFFSET - 28),
  Adrv9001Status_Rx2SsiCalErr         = (ADRV9001_STATUS_OFFSET - 29),
  Adrv9001Status_RxGainSetErr         = (ADRV9001_STATUS_OFFSET - 30),
  Adrv9001Status_WriteErr             = (ADRV9001_STATUS_OFFSET - 31),
  Adrv9001Status_RxSsiErr             = (ADRV9001_STATUS_OFFSET - 32),
  Adrv9001Status_InvalidParameter     = (ADRV9001_STATUS_OFFSET - 33),
  Adrv9001Status_PfirErr              = (ADRV9001_STATUS_OFFSET - 34),
  Adrv9001Status_MemoryErr            = (ADRV9001_STATUS_OFFSET - 35),
  Adrv9001Status_InvalidHopFreq       = (ADRV9001_STATUS_OFFSET - 36),
  Adrv9001Status_AdcErr               = (ADRV9001_STATUS_OFFSET - 37),
  Adrv9001Status_FileSystemErr        = (ADRV9001_STATUS_OFFSET - 38),
  Adrv9001Status_InitAnalogErr        = (ADRV9001_STATUS_OFFSET - 39),
  Adrv9001Status_StreamImageWriteErr  = (ADRV9001_STATUS_OFFSET - 40),
  Adrv9001Status_ArmImageWriteErr     = (ADRV9001_STATUS_OFFSET - 41),
  Adrv9001Status_ProfileWriteErr      = (ADRV9001_STATUS_OFFSET - 42),
  Adrv9001Status_PowerMgmtErr         = (ADRV9001_STATUS_OFFSET - 43),
  Adrv9001Status_ArmErr               = (ADRV9001_STATUS_OFFSET - 44),
  Adrv9001Status_PllErr               = (ADRV9001_STATUS_OFFSET - 45),
  Adrv9001Status_PathDelayErr         = (ADRV9001_STATUS_OFFSET - 46),
  Adrv9001Status_SlewRateLimiterErr   = (ADRV9001_STATUS_OFFSET - 47),
  Adrv9001Status_InitCalsErr          = (ADRV9001_STATUS_OFFSET - 48),
  Adrv9001Status_FreqHopErr           = (ADRV9001_STATUS_OFFSET - 49),
  Adrv9001Status_AgcErr               = (ADRV9001_STATUS_OFFSET - 50),
  Adrv9001Status_LoidErr              = (ADRV9001_STATUS_OFFSET - 51),
  Adrv9001Status_BbdcErr              = (ADRV9001_STATUS_OFFSET - 52),
  Adrv9001Status_TrackingCalsErr      = (ADRV9001_STATUS_OFFSET - 53),
  Adrv9001Status_LoopBackErr          = (ADRV9001_STATUS_OFFSET - 54),
  Adrv9001Status_LogPathTooLong       = (ADRV9001_STATUS_OFFSET - 55),

} adrv9001_status_t;

typedef enum{
  Adrv9001TxDataSrc_Ones = 0,
  Adrv9001TxDataSrc_Zeros,
  Adrv9001TxDataSrc_Ramp,
  Adrv9001TxDataSrc_Pn15,
  Adrv9001TxDataSrc_FixedPattern,
  Adrv9001TxDataSrc_Axis,
  Adrv9001TxDataSrc_RxLoopback,
}adrv9001_tx_data_src_t;

typedef enum{
  Adrv9001RxDataSrc_OTA,
  Adrv9001RxDataSrc_TxLoopback,
}adrv9001_rx_data_src_t;

#define ADRV9001_LOG_PATH_SIZE      (64)

typedef void (*adrv9001_rf_state_cb_t)( void *CallbackRef, adi_adrv9001_ChannelState_e state, adi_common_Port_e port, adi_common_ChannelNumber_e channel );

typedef void (*adrv9001_hop_irq_cb_t)( void *CallbackRef );

typedef int  (*adrv9001_initialize_fn_t)( adi_adrv9001_Device_t * adrv9001Device_0 );
typedef int  (*adrv9001_calibrate_fn_t)( adi_adrv9001_Device_t * adrv9001Device_0 );
typedef int  (*adrv9001_configure_fn_t)( adi_adrv9001_Device_t * adrv9001Device_0 );
typedef void* (*adrv9001_malloc_t)(size_t);
typedef void  (*adrv9001_free_t)(void *);

typedef struct {
  adi_adrv9001_Init_t                      *Init;
  uint8_t                                  *StreamImageBuf;
  adi_adrv9001_RxGainTableRow_t            *Rx1GainTable;
  adi_adrv9001_RxGainTableRow_t            *Rx2GainTable;
  adi_adrv9001_RxGainTableRow_t            *oRx1GainTable;
  adi_adrv9001_RxGainTableRow_t            *oRx2GainTable;
  adi_adrv9001_TxAttenTableRow_t           *TxAttnTable;
  adi_adrv9001_LoGenOptimization_e          Rx1LoGenOptimization;
  adi_adrv9001_LoGenOptimization_e          Rx2LoGenOptimization;
  adi_adrv9001_LoGenOptimization_e          Tx1LoGenOptimization;
  adi_adrv9001_LoGenOptimization_e          Tx2LoGenOptimization;
  uint64_t                                  Rx1CarrierFreqHz;
  uint64_t                                  Rx2CarrierFreqHz;
  uint64_t                                  Tx1CarrierFreqHz;
  uint64_t                                  Tx2CarrierFreqHz;
  int32_t                                   Rx1IfFreqHz;
  int32_t                                   Rx2IfFreqHz;
  int32_t                                   Tx1IfFreqHz;
  int32_t                                   Tx2IfFreqHz;
  adi_adrv9001_PllCalibration_e             Lo1PllCalibration;
  adi_adrv9001_PllPower_e                   Lo1PllPower;
  adi_adrv9001_PllCalibration_e             Lo2PllCalibration;
  adi_adrv9001_PllPower_e                   Lo2PllPower;
  bool                                      Tx1Boost;
  bool                                      Tx2Boost;
  adi_adrv9001_GainControlCfg_t            *Rx1Agc;
  adi_adrv9001_GainControlCfg_t            *Rx2Agc;
  adi_adrv9001_RxGainControlMode_e          Rx1AgcCtrlMode;
  adi_adrv9001_RxGainControlMode_e          Rx2AgcCtrlMode;
  adi_adrv9001_RxGainControlPinCfg_t        Rx1AgcPinMode;
  adi_adrv9001_RxGainControlPinCfg_t        Rx2AgcPinMode;
  adi_adrv9001_TxAttenuationPinControlCfg_t Tx1AttnPinMode;
  adi_adrv9001_TxAttenuationPinControlCfg_t Tx2AttnPinMode;
}adrv9001_profile_t;

/**
** SSI Delay
*/
typedef struct {
  uint32_t            Clk    : 8;
  uint32_t            Strobe : 8;
  uint32_t            Idata  : 8;
  uint32_t            Qdata  : 8;
}adrv9001_ssi_port_delay_t;

typedef struct {
  adi_adrv9001_Device_t	                Device;            ///< ADI ADRV9001 Instance
  adi_adrv9001_Init_t                  *Params;            ///< ADRV9001 Parameters
  axi_adrv9001_t                        Axi;               ///< AXI Instance
  FIL                                   LogFil;            ///< Log File
  uint8_t                               Initialized;
  XScuGic                              *IrqInstance;       ///< Processor Interrupt Controller Instance
  adrv9001_rf_state_cb_t                StateCallback;
  void                                 *StateCallbackRef;
  adrv9001_hop_irq_cb_t                 HopIrqCallback;
  void                                 *HopIrqCallbackRef;
  uint8_t                               Rx1SsiSweep[64];
  uint8_t                               Rx2SsiSweep[64];
  uint8_t                               Tx1SsiSweep[64];
  uint8_t                               Tx2SsiSweep[64];
  bool                                  Rx1SsiSweepStatus;
  bool                                  Rx2SsiSweepStatus;
  bool                                  Tx1SsiSweepStatus;
  bool                                  Tx2SsiSweepStatus;
  bool                                  Tx1ToRx1Loopback;
  bool                                  Tx2ToRx2Loopback;
  bool                                  Rx1ToTx1Loopback;
  bool                                  Rx2ToTx2Loopback;
  bool                                  UseExtClock;
  char                                  LogPath[ ADRV9001_LOG_PATH_SIZE ];
  float                                 TxAttn[2];
  bool                                  TxBoost[2];
  adi_adrv9001_FhHopFrame_t            *HopTable;
  uint8_t                               HopTableSize;
  adi_adrv9001_ChannelEnableMode_e      TxEnableMode;
  adi_adrv9001_ChannelEnableMode_e      RxEnableMode;
  float                                 Rx1RssiOffsetdB;
  float                                 Rx2RssiOffsetdB;
  adi_adrv9001_SsiTestModeData_e        Rx1TestMode;
  adi_adrv9001_SsiTestModeData_e        Rx2TestMode;
  adi_adrv9001_SsiTestModeData_e        Tx1TestMode;
  adi_adrv9001_SsiTestModeData_e        Tx2TestMode;
  adrv9001_initialize_fn_t              InitializeFn;
  adrv9001_calibrate_fn_t               CalibrateFn;
  adrv9001_configure_fn_t               ConfigureFn;
  adrv9001_malloc_t                     Malloc;
  adrv9001_free_t                       Free;
}adrv9001_t;

typedef struct {
  adi_adrv9001_ChannelEnableMode_e      TxEnableMode;
  adi_adrv9001_ChannelEnableMode_e      RxEnableMode;
  char                                 *LogFilename;           ///< Log Filename
  char                                 *BasePath;              ///< File System Path
  uint32_t                              AxiBase;               ///< AXI bus base address
  uint32_t                              AxiIrqId;              ///< Processor interrupt ID.
  uint32_t                              HopIrqId;              ///< Processor interrupt ID.
  XScuGic                              *IrqInstance;           ///< Processor Interrupt Controller Instance
  adrv9001_rf_state_cb_t                StateCallback;
  void                                 *StateCallbackRef;
  adrv9001_hop_irq_cb_t                 HopIrqCallback;
  void                                 *HopIrqCallbackRef;
  int32_t                              *Rx1ChfCoeff;
  int32_t                              *Rx2ChfCoeff;
  float                                 TxAttn[2];
  bool                                  TxBoost[2];
  uint32_t                              AxiClockFreqHz;
  float                                 Rx1RssiOffsetdB;
  float                                 Rx2RssiOffsetdB;
  adi_adrv9001_Init_t                  *Init;
  adrv9001_initialize_fn_t              InitializeFn;
  adrv9001_calibrate_fn_t               CalibrateFn;
  adrv9001_configure_fn_t               ConfigureFn;
  adrv9001_malloc_t                     Malloc;
  adrv9001_free_t                       Free;
  bool                                  UseExtClock;
}adrv9001_init_t;


int32_t Adrv9001_LoadProfile            ( adrv9001_t *Instance, adrv9001_profile_t *Profile  );
int32_t Adrv9001_LoadDefaultProfile     ( adrv9001_t *Instance );
int32_t Adrv9001_Initialize             ( adrv9001_t *Instance, adrv9001_init_t *Init );
int32_t Adrv9001_ClearError             ( adrv9001_t *Instance );
void    Adrv9001_RegisterRadioStateCallback( adrv9001_t *Instance, adrv9001_rf_state_cb_t Callback, void *CallbackRef );
int32_t Adrv9001_GetLogPath             (adrv9001_t *Instance, char* LogPath);
int32_t Adrv9001_SetLogPath             (adrv9001_t *Instance, char* LogPath);
bool    Adrv9001_IsPortEnabled          ( adrv9001_t *Instance, adi_common_Port_e port, adi_common_ChannelNumber_e channel );
int32_t Adrv9001_SetEnableMode          ( adrv9001_t *Instance, adi_common_Port_e port, adi_common_ChannelNumber_e channel, adi_adrv9001_ChannelEnableMode_e mode );
int32_t Adrv9001_GetEnableMode          ( adrv9001_t *Instance, adi_common_Port_e port, adi_common_ChannelNumber_e channel, adi_adrv9001_ChannelEnableMode_e *mode );

int32_t Adrv9001_ToRfEnabled            ( adrv9001_t *Instance, adi_common_Port_e port, adi_common_ChannelNumber_e channel );
int32_t Adrv9001_ToPrimed               ( adrv9001_t *Instance, adi_common_Port_e port, adi_common_ChannelNumber_e channel );
int32_t Adrv9001_ToCalibrated           ( adrv9001_t *Instance, adi_common_Port_e port, adi_common_ChannelNumber_e channel );
bool    Adrv9001_IsRfEnabled            ( adrv9001_t *Instance, adi_common_Port_e port, adi_common_ChannelNumber_e channel );
int32_t Adrv9001_GetRadioState          ( adrv9001_t *Instance, adi_common_Port_e port, adi_common_ChannelNumber_e channel, adi_adrv9001_ChannelState_e *Value );

int32_t Adrv9001_SetSsiDelay            ( adrv9001_t *Instance, adi_common_Port_e port, adi_common_ChannelNumber_e channel, adrv9001_ssi_port_delay_t Delay );
int32_t Adrv9001_PerformSsiSweep        ( adrv9001_t *Instance, adi_common_Port_e port, adi_common_ChannelNumber_e channel, adi_adrv9001_SsiTestModeData_e testMode, uint32_t fixedPattern, uint16_t results[8][8] );
int32_t Adrv9001_CalibrateSsiDelay      ( adrv9001_t *Instance, adi_common_Port_e port, adi_common_ChannelNumber_e channel, adi_adrv9001_SsiTestModeData_e testMode, uint32_t fixedPattern );
int32_t Adrv9001_SetSsiClkDelay         ( adrv9001_t *Instance, adi_common_Port_e port, adi_common_ChannelNumber_e channel, uint16_t Value );
int32_t Adrv9001_SetSsiDataDelay        ( adrv9001_t *Instance, adi_common_Port_e port, adi_common_ChannelNumber_e channel, uint16_t Value );
int32_t Adrv9001_GetSsiClockDelay       ( adrv9001_t *Instance, adi_common_Port_e port, adi_common_ChannelNumber_e channel, uint16_t *Value );
int32_t Adrv9001_GetSsiDataDelay        ( adrv9001_t *Instance, adi_common_Port_e port, adi_common_ChannelNumber_e channel, uint16_t *Value );
int32_t Adrv9001_GetSsiStatus           ( adrv9001_t *Instance, adi_common_Port_e port, adi_common_ChannelNumber_e channel, bool *Value );

int32_t Adrv9001_SetTxTestMode          ( adrv9001_t *Instance, adi_common_ChannelNumber_e channel, adi_adrv9001_SsiTestModeData_e testMode );
int32_t Adrv9001_GetTxTestMode          ( adrv9001_t *Instance, adi_common_ChannelNumber_e channel, adi_adrv9001_SsiTestModeData_e *testMode );
int32_t Adrv9001_TxTestModeCheck        ( adrv9001_t *Instance, adi_common_ChannelNumber_e channel, adi_adrv9001_SsiTestModeData_e testMode, uint32_t fixedPattern );
int32_t Adrv9001_GetRxTestMode          ( adrv9001_t *Instance, adi_common_ChannelNumber_e channel, adi_adrv9001_SsiTestModeData_e *testMode );
int32_t Adrv9001_SetRxTestMode          ( adrv9001_t *Instance, adi_common_ChannelNumber_e channel, adi_adrv9001_SsiTestModeData_e testMode );

int32_t Adrv9001_SetDacEnable           ( adrv9001_t *Instance, adi_adrv9001_AuxDac_e Id, bool Enable );
int32_t Adrv9001_GetDacEnable           ( adrv9001_t *Instance, adi_adrv9001_AuxDac_e Id, bool *Enable );
int32_t Adrv9001_SetDacVoltage          ( adrv9001_t *Instance, adi_adrv9001_AuxDac_e Channel, float Voltage );
int32_t Adrv9001_GetDacVoltage          ( adrv9001_t *Instance, adi_adrv9001_AuxDac_e Channel, float *Voltage );

int32_t Adrv9001_GetAdcVoltage          ( adrv9001_t *Instance, adi_adrv9001_AuxAdc_e Channel, float *Value );
int32_t Adrv9001_GetAdcEnable           ( adrv9001_t *Instance, adi_adrv9001_AuxAdc_e Channel, bool *Value );
int32_t Adrv9001_SetAdcEnable           ( adrv9001_t *Instance, adi_adrv9001_AuxAdc_e Channel, bool Value );

int32_t Adrv9001_LoadHopTable           ( adrv9001_t *Instance, uint8_t size );
int32_t Adrv9001_SetHopTableFrequency   ( adrv9001_t *Instance, uint8_t index, uint64_t Freq );
int32_t Adrv9001_GetHopTableFrequency   ( adrv9001_t *Instance, uint8_t index, uint64_t *Freq );
int32_t Adrv9001_SetHopTableAttn        ( adrv9001_t *Instance, uint8_t index, float Attn );
int32_t Adrv9001_GetHopTableAttn        ( adrv9001_t *Instance, uint8_t index, float *Attn );
int32_t Adrv9001_GetHopFrequency        ( adrv9001_t *Instance, uint64_t *Value );
int32_t Adrv9001_GetNextHopFrequency    ( adrv9001_t *Instance, uint64_t *Value );

int32_t Adrv9001_SetTxAttn              ( adrv9001_t *Instance, adi_common_ChannelNumber_e channel, float Value );
int32_t Adrv9001_GetTxAttn              ( adrv9001_t *Instance, adi_common_ChannelNumber_e channel, float *Value );
int32_t Adrv9001_SetTxBoost             ( adrv9001_t *Instance, adi_common_ChannelNumber_e channel, bool Value );
int32_t Adrv9001_GetTxBoost             ( adrv9001_t *Instance, adi_common_ChannelNumber_e channel, bool *Value );

int32_t Adrv9001_GetSampleRate          ( adrv9001_t *Instance, adi_common_Port_e port, adi_common_ChannelNumber_e channel, uint32_t *Value );

int32_t Adrv9001_SetCarrierFrequency    ( adrv9001_t *Instance, adi_common_Port_e port, adi_common_ChannelNumber_e channel, uint64_t FreqHz );
int32_t Adrv9001_GetCarrierFrequency    ( adrv9001_t *Instance, adi_common_Port_e port, adi_common_ChannelNumber_e channel, uint64_t *FreqHz );

int32_t Adrv9001_LoadRxChannelFilter    ( adrv9001_t *Instance, adi_common_ChannelNumber_e channel, int32_t *Coeff );

int32_t Adrv9001_SetRxToTxLoopBack      (adrv9001_t *Instance, adi_common_ChannelNumber_e channel, bool Value);
int32_t Adrv9001_GetRxToTxLoopBack      (adrv9001_t *Instance, adi_common_ChannelNumber_e channel, bool *Value);
int32_t Adrv9001_SetTxToRxLoopBack      ( adrv9001_t *Instance, adi_common_ChannelNumber_e channel, bool Value );
int32_t Adrv9001_GetTxToRxLoopBack      ( adrv9001_t *Instance, adi_common_ChannelNumber_e channel, bool *Value );

int32_t Adrv9001_SetGainControlMode     ( adrv9001_t *Instance, adi_common_ChannelNumber_e channel, adi_adrv9001_RxGainControlMode_e mode );
int32_t Adrv9001_GetGainControlMode     ( adrv9001_t *Instance, adi_common_ChannelNumber_e channel, adi_adrv9001_RxGainControlMode_e *mode );
int32_t Adrv9001_GetRxRssi              ( adrv9001_t *Instance, adi_common_ChannelNumber_e channel, float *Value );
int32_t Adrv9001_GetRxCurGainIndex      ( adrv9001_t *Instance, adi_common_ChannelNumber_e channel, uint8_t *Value );
int32_t Adrv9001_SetRxCurGainIndex      ( adrv9001_t *Instance, adi_common_ChannelNumber_e channel, uint8_t Value );
int32_t Adrv9001_GetRxGainTableRow      ( adrv9001_t *Instance, adi_common_ChannelNumber_e channel, uint8_t TableIndex, adi_adrv9001_RxGainTableRow_t *Value );
int32_t Adrv9001_GetRxGainTableFeAttn   ( adrv9001_t *Instance, adi_common_ChannelNumber_e channel, uint8_t TableIndex, float *Value );
int32_t Adrv9001_GetRxGainTableDigAttn  ( adrv9001_t *Instance, adi_common_ChannelNumber_e channel, uint8_t TableIndex, float *Value );
int32_t Adrv9001_GetRxGainTableExtCtrl  ( adrv9001_t *Instance, adi_common_ChannelNumber_e channel, uint8_t TableIndex, uint8_t *Value );
int32_t Adrv9001_SetRxRssiOffset        ( adrv9001_t *Instance, adi_common_ChannelNumber_e channel, float Value );
int32_t Adrv9001_GetRxRssiOffset        ( adrv9001_t *Instance, adi_common_ChannelNumber_e channel, float *Value );

int32_t Adrv9001_GetTemperature         ( adrv9001_t *Instance, int16_t *Value );
int32_t Adrv9001_GetApiVersion          ( adrv9001_t *Instance, adi_common_ApiVersion_t *Value );
int32_t Adrv9001_GetFirmwareVersion     ( adrv9001_t *Instance, adi_adrv9001_ArmVersion_t *Value );
int32_t Adrv9001_GetSiliconVersion      ( adrv9001_t *Instance, adi_adrv9001_SiliconVersion_t *Value );

int32_t Adrv9001_GetTxDataSrc           ( adrv9001_t *Instance, adi_common_ChannelNumber_e channel, adrv9001_tx_data_src_t *Value );
int32_t Adrv9001_SetTxDataSrc           ( adrv9001_t *Instance, adi_common_ChannelNumber_e channel, adrv9001_tx_data_src_t Value );
int32_t Adrv9001_GetRxDataSrc           ( adrv9001_t *Instance, adi_common_ChannelNumber_e channel, adrv9001_rx_data_src_t *Value );
int32_t Adrv9001_SetRxDataSrc           ( adrv9001_t *Instance, adi_common_ChannelNumber_e channel, adrv9001_rx_data_src_t Value );

int32_t Adrv9001_SetDgitalGpioDirection ( adrv9001_t *Instance, adi_adrv9001_GpioPinCrumbSel_e Bank, adi_adrv9001_GpioPinDirection_e Dir );
int32_t Adrv9001_SetAnalogGpioDirection ( adrv9001_t *Instance, adi_adrv9001_GpioAnalogPinNibbleSel_e Bank, adi_adrv9001_GpioPinDirection_e Dir );
int32_t Adrv9001_SetGpioDirection       ( adrv9001_t *Instance, adi_adrv9001_GpioPin_e Pin, adi_adrv9001_GpioPinDirection_e Dir );
int32_t Adrv9001_GetGpioDirection       ( adrv9001_t *Instance, adi_adrv9001_GpioPin_e Pin, adi_adrv9001_GpioPinDirection_e *Dir );
int32_t Adrv9001_SetGpioPinLevel        ( adrv9001_t *Instance, adi_adrv9001_GpioPin_e Pin, adi_adrv9001_GpioPinLevel_e Level );
int32_t Adrv9001_GetGpioPinLevel        ( adrv9001_t *Instance, adi_adrv9001_GpioPin_e Pin, adi_adrv9001_GpioPinLevel_e *Level );

int32_t Adrv9001_SetRxInputPort         ( adrv9001_t *Instance, adi_common_ChannelNumber_e channel, adi_adrv9001_RxRfInputSel_e InputPort );
int32_t Adrv9001_GetRxInputPort         ( adrv9001_t *Instance, adi_common_ChannelNumber_e channel, adi_adrv9001_RxRfInputSel_e *InputPort );

int32_t Adrv9001_EnableManualInputPorts ( adrv9001_t *Instance, bool Enable );


#endif
