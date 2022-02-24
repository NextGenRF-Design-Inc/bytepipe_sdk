#ifndef SRC_PHY_TYPES_H_
#define SRC_PHY_TYPES_H_
/***************************************************************************//**
*  \ingroup    APP
*  \defgroup   PHY RFLAN PHY Layer
*  @{
*******************************************************************************/
/***************************************************************************//**
*  \file       phy_types.h
*
*  \details
*
*  This file contains the RFLAN PHY layer type definitions.
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
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "parameters.h"
#include "adrv9001.h"
#include "adrv9001_hal.h"
#include "axi_dmac.h"

/**
**  ADRV9001 Status
*/
typedef enum
{
  PhyStatus_Success             = (0),
  PhyStatus_InvalidPort         = (PHY_STATUS_OFFSET - 1),
  PhyStatus_MemoryError         = (PHY_STATUS_OFFSET - 2),
  PhyStatus_NotSupported        = (PHY_STATUS_OFFSET - 3),
  PhyStatus_OsError             = (PHY_STATUS_OFFSET - 4),
  PhyStatus_Busy                = (PHY_STATUS_OFFSET - 5),
  PhyStatus_InvalidParameter    = (PHY_STATUS_OFFSET - 6),
  PhyStatus_Adrv9001Error       = (PHY_STATUS_OFFSET - 7),
  PhyStatus_RadioStateError     = (PHY_STATUS_OFFSET - 8),
  PhyStatus_IqStreamAbort       = (PHY_STATUS_OFFSET - 9),
  PhyStatus_DmaError            = (PHY_STATUS_OFFSET - 10),
  PhyStatus_StreamStart         = (PHY_STATUS_OFFSET - 11),
  PhyStatus_StreamDone          = (PHY_STATUS_OFFSET - 12),
  PhyStatus_StreamError         = (PHY_STATUS_OFFSET - 13),
  PhyStatus_InvalidRadioState   = (PHY_STATUS_OFFSET - 14),
  PhyStatus_GpioError           = (PHY_STATUS_OFFSET - 15),
  PhyStatus_DacError            = (PHY_STATUS_OFFSET - 16),
  PhyStatus_TestModeFailure     = (PHY_STATUS_OFFSET - 17),
  PhyStatus_SsiFailure          = (PHY_STATUS_OFFSET - 18),
} phy_status_t;


/**
 **  PHY Port
 */
typedef enum
{
  PhyPort_Rx1    = (0),
  PhyPort_Rx2    = (1),
  PhyPort_Tx1    = (2),
  PhyPort_Tx2    = (3),
  PhyPort_Num    = (4),
} phy_port_t;

/**
 **  PHY Event Type
 */
typedef enum
{
  PhyEvtType_StreamInd        = 0,    ///< Indicates stream is done. EvtData = phy_stream_t
  PhyEvtType_ProfileUpdated   = 1,    ///< Indicates profile has been updated. No Evt data
  PhyEvtType_LogWrite         = 2,    ///< Log Data
} phy_evt_type_t;

/**
** PHY Callback
*/
typedef void (*phy_callback_t)( phy_evt_type_t EvtType, void *EvtData, void *param );

/**
 **  PHY Stream Request
 */
typedef struct
{
  phy_port_t        Port;             ///< Port
  uint32_t         *SampleBuf;        ///< Sample Buffer
  uint32_t          SampleCnt;        ///< Length in samples
  void             *CallbackRef;      ///< User Data
  phy_callback_t    Callback;         ///< Callback
  bool              Cyclic;           ///< Flag indicates the stream will continue Indefinitely
  bool              Blocking;         ///< Indicates if stream is blocking or non-blocking
} phy_stream_req_t;

/**
 **  PHY Stream Indication
 */
typedef struct
{
  phy_port_t        Port;             ///< Port
  uint32_t         *SampleBuf;        ///< Sample Buffer
  uint32_t          SampleCnt;        ///< Length in samples
  phy_status_t      Status;           ///< Status
} phy_stream_ind_t;

/**
**  PHY Stream
*/
typedef struct{
  uint32_t         *SampleBuf;        ///< Sample Buffer
  uint32_t          SampleCnt;        ///< Length in samples
  phy_port_t        Port;             ///< Port
  phy_callback_t    Callback;         ///< Callback
  void             *CallbackRef;      ///< User Data
  phy_status_t      Status;           ///< Status
  bool              Cyclic;           ///< Flag indicates the stream will continue Indefinitely
  axi_dmac_t       *Dma;              ///< DMA Instance
  uint32_t          DmaBufAddr;       ///< DMA Buffer Address
}phy_stream_t;

/**
**  PHY Configuration
*/
typedef struct{
}phy_cfg_t;

/**
**  PHY Stream Configuration
*/
typedef struct{
}phy_stream_cfg_t;

/**
**  PHY Stream Configuration
*/
typedef struct{
}phy_adrv9001_cfg_t;


/**
**  PHY Instance
*/
typedef struct{
  void             *Queue;                 ///< PHY Queue
  adrv9001_t        Adrv9001;              ///< ADRV9001 Instance
  adrv9001_hal_t    Adrv9001Hal;           ///< ADRV9001 Instance
  phy_stream_t      Stream[PhyPort_Num];   ///< Stream Data
}phy_t;

/**
**  PHY Profile
*/
typedef struct{
  const char        JsonFilename[FF_FILENAME_MAX_LEN];
  const char        StreamFilename[FF_FILENAME_MAX_LEN];
}phy_profile_t;

typedef union{
  void             *p;
  uint32_t          v;
} phy_task_req_t;

typedef int32_t (*phy_task_t)( phy_t *Instance, phy_task_req_t req );

/**
 **  PHY Message
 */
typedef struct
{
  phy_task_t          task;
  phy_task_req_t      req;
} phy_msg_t;


#endif /* SRC_PHY_TYPES_H_ */
