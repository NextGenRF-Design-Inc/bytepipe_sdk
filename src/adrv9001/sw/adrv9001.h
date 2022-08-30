#ifndef SRC_ADRV9001_H_
#define SRC_ADRV9001_H_
/***************************************************************************//**
*  \ingroup    RFLAN
*  \defgroup   ADRV9001 ADRV9001 Interface Driver
*  @{
*******************************************************************************/
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


#include "adi_adrv9001.h"
#include "adi_adrv9001_error.h"
#include "adi_adrv9001_radio_types.h"
#include "adi_adrv9001_rx_gaincontrol_types.h"
#include "adi_adrv9001.h"
#include "adi_adrv9001_arm.h"
#include "adi_adrv9001_radio.h"
#include "adi_adrv9001_profileutil.h"
#include "adi_adrv9001_gpio.h"
#include "adi_adrv9001.h"
#include "adi_adrv9001_tx.h"
#include "adi_adrv9001_rx.h"
#include "adi_adrv9001_arm.h"
#include "adi_adrv9001_auxdac.h"
#include "adi_adrv9001_ssi.h"
#include "axi_adrv9001.h"
#include "adi_adrv9001_dpd.h"
#include "ff.h"
#include "xspips.h"
#include "xparameters.h"
#include "pib.h"

#ifndef ADRV9001_STATUS_OFFSET
#define ADRV9001_STATUS_OFFSET    (0)
#endif

#ifndef ADRV9001_LOG_FILENAME
#define ADRV9001_LOG_FILENAME           ("1:/adi_adrv9001_log.txt")
#endif

#ifndef ADRV9001_SPI_DEVICE_ID
#define ADRV9001_SPI_DEVICE_ID          (XPAR_PSU_SPI_0_DEVICE_ID)
#endif

#ifndef ADRV9001_SPI_CS
#define ADRV9001_SPI_CS                 (0)
#endif

#ifndef ADRV9001_CTRL_BASE_ADDR
#define ADRV9001_CTRL_BASE_ADDR         (XPAR_ADRV9002_0_BASEADDR)
#endif

#define ADRV9001_TDD_ENABLE_DUR_FOREVER (0xffffffff)

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
  Adrv9001Status_InvalidPib           = (ADRV9001_STATUS_OFFSET - 14),
  Adrv9001Status_SpiErr               = (ADRV9001_STATUS_OFFSET - 15),
  Adrv9001Status_InvalidPaEnable      = (ADRV9001_STATUS_OFFSET - 16),
  Adrv9001Status_SsiSweepErr          = (ADRV9001_STATUS_OFFSET - 17),
  Adrv9001Status_SsiTestModeErr       = (ADRV9001_STATUS_OFFSET - 18),
  Adrv9001Status_SsiSetErr            = (ADRV9001_STATUS_OFFSET - 19),
  Adrv9001Status_InvalidState         = (ADRV9001_STATUS_OFFSET - 20),
  Adrv9001Status_CarrierFreqErr       = (ADRV9001_STATUS_OFFSET - 21),
  Adrv9001Status_ReadErr              = (ADRV9001_STATUS_OFFSET - 22),
  Adrv9001Status_EnableModeErr        = (ADRV9001_STATUS_OFFSET - 23),
} adrv9001_status_t;

#define ADRV9001_LOG_PATH_SIZE      (64)

/**
** ADRV9001 parameters
*/
typedef struct {
  adi_adrv9001_ClockSettings_t          clocks;
  adi_adrv9001_RxSettings_t             rx;
  adi_adrv9001_TxSettings_t             tx;
  adi_adrv9001_DeviceSysConfig_t        sysConfig;
  adi_adrv9001_PfirBuffer_t             pfirBuffer;
  adi_adrv9001_Carrier_t                Lo1Carrier;
  adi_adrv9001_Carrier_t                Lo2Carrier;
  uint16_t                              Tx1Attn;
  uint16_t                              Tx2Attn;
  uint8_t                               Tx1Boost;
  uint8_t                               Tx2Boost;
  uint32_t                              Tx1ExternalPathDelay;
  uint32_t                              Tx2ExternalPathDelay;
  adi_adrv9001_GainControlCfg_t         Rx1Agc;
  adi_adrv9001_GainControlCfg_t         Rx2Agc;
  uint32_t                              Tx1DpdExternalPathDelay;
  uint32_t                              Tx2DpdExternalPathDelay;
  adi_adrv9001_DpdInitCfg_t             Tx1DpdInitCfg;
  adi_adrv9001_DpdInitCfg_t             Tx2DpdInitCfg;
  adi_adrv9001_DpdCfg_t                 Tx1DpdCfg;
  adi_adrv9001_DpdCfg_t                 Tx2DpdCfg;
  adi_adrv9001_RxInterfaceGainCtrl_t    Rx1InterfaceGain;
  adi_adrv9001_RxInterfaceGainCtrl_t    Rx2InterfaceGain;
  uint32_t                              Tx1TestModeData;
  uint32_t                              Tx2TestModeData;
  uint32_t                              Rx1TestModeData;
  uint32_t                              Rx2TestModeData;
  uint32_t                              Tx1SsiEnableDly;
  uint32_t                              Tx2SsiEnableDly;
  uint32_t                              Rx1SsiEnableDly;
  uint32_t                              Rx2SsiEnableDly;
  uint32_t                              Tx1DisableDly;
  uint32_t                              Tx2DisableDly;
  uint32_t                              Rx1SsiDisableDly;
  uint32_t                              Rx2SsiDisableDly;
  uint32_t                              HwVer;
  char                                  LogPath[ ADRV9001_LOG_PATH_SIZE ];
} adrv9001_params_t;

extern adrv9001_params_t Adrv9001Params;

typedef struct {
	adi_adrv9001_Device_t	      Device;
  adrv9001_params_t          *Params;
  uint32_t                    CtrlBase;             ///< Control Base
  XSpiPs                      Spi;                  ///< SPI Instance
  FIL                         LogFil;               ///< Log File
  pib_t                       Pib;
  uint8_t                     PendingReboot;
}adrv9001_t;

typedef struct {
  uint32_t                    HwVer;
  char                       *LogFilename;
  uint32_t                    CtrlBase;
}adrv9001_init_t;

int32_t Adrv9001_LoadProfile            ( adrv9001_t *Instance );
int32_t Adrv9001_Initialize             ( adrv9001_t *Instance, adrv9001_init_t *Init );
int32_t Adrv9001_ToRfEnabled            ( adrv9001_t *Instance, adi_common_Port_e port, adi_common_ChannelNumber_e channel, uint32_t SampleCnt );
int32_t Adrv9001_ToPrimed               ( adrv9001_t *Instance, adi_common_Port_e port, adi_common_ChannelNumber_e channel );
int32_t Adrv9001_ToCalibrated           ( adrv9001_t *Instance, adi_common_Port_e port, adi_common_ChannelNumber_e channel );
int32_t Adrv9001_SetPaEnable            ( adrv9001_t *Instance, adi_common_Port_e port, adi_common_ChannelNumber_e channel, bool Enable );
int32_t Adrv9001_SetSsiDataDelay        ( adrv9001_t *Instance, adi_common_Port_e port, adi_common_ChannelNumber_e channel, uint8_t Delay );
int32_t Adrv9001_SetSsiClkDelay         ( adrv9001_t *Instance, adi_common_Port_e port, adi_common_ChannelNumber_e channel, uint8_t Delay );
int32_t Adrv9001_SetVcTcxo              ( adrv9001_t *Instance, float Voltage );
int32_t Adrv9001_GetVcTcxo              ( adrv9001_t *Instance, float *Voltage );
int32_t Adrv9001_PerformSsiSweep        ( adrv9001_t *Instance, adi_common_Port_e port, adi_common_ChannelNumber_e channel, uint16_t results[8][8] );
int32_t Adrv9001_ClearError             ( adrv9001_t *Instance );
int32_t Adrv9001_EnableDac              ( adrv9001_t *Instance, uint8_t Id, bool Enable );
int32_t Adrv9001_CalibrateSsiDelay      ( adrv9001_t *Instance, adi_common_Port_e port, adi_common_ChannelNumber_e channel );

#endif
