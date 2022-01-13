#ifndef SRC_PHY_H_
#define SRC_PHY_H_
/***************************************************************************//**
*  \ingroup    APP
*  \defgroup   PHY RFLAN PHY Layer
*  @{
*******************************************************************************/
/***************************************************************************//**
*  \file       phy.h
*
*  \details
*
*  This file contains the RFLAN PHY layer for interfacing to the ADRV900X.
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
#include "parameters.h"
#include "xstatus.h"
#include "adrv9001.h"


#define PHY_STATUS_OFFSET           (-2000)

#define PHY_IS_PORT_TX(p)           ADRV9001_IS_PORT_TX(p)
#define PHY_IS_PORT_RX(p)           ADRV9001_IS_PORT_RX(p)

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
} phy_status_t;

#define PHY_STATUS_2_STR(p)   (( p == PhyStatus_Success )?                  "Success" :                         \
                               ( p == PhyStatus_InvalidPort )?              "PHY InvalidPort" :                 \
                               ( p == PhyStatus_InvalidParameter )?         "PHY InvalidParameter" :            \
                               ( p == PhyStatus_MemoryError )?              "PHY MemoryError" :                 \
                               ( p == PhyStatus_RadioStateError )?          "PHY InvalidRadioState" :           \
                               ( p == PhyStatus_NotSupported )?             "PHY Functionality Not supported" : \
                               ( p == PhyStatus_OsError )?                  "PHY OS Error" :                    \
                               ( p == PhyStatus_Busy )?                     "PHY Busy" :                        \
                               ( p == PhyStatus_Adrv9001Error )?            "PHY ADRV9001 Error" :              \
                               ( p == PhyStatus_IqStreamAbort )?            "PHY IQ Stream Abort" : "Unknown")
/**
** PHY Profile
*/
typedef uint8_t profile_t[ADRV9001_PROFILE_SIZE];

/**
 **  PHY Event Data
 */
typedef union
{
  struct
  {
    adrv9001_port_t       Port;         ///< Port
    uint32_t             *SampleBuf;    ///< Sample Buffer
    uint32_t              SampleCnt;    ///< Length in samples
    phy_status_t          Status;       ///< Status
    void                 *CallbackRef;  ///< User Data
  }Stream;
  char                   *Message;      ///< Message
} phy_evt_data_t;


/**
 **  PHY Event Type
 */
typedef enum
{
  PhyEvtType_StreamDone       = 0,    ///< Indicates stream is done. EvtData = phy_stream_t
  PhyEvtType_StreamStart      = 1,    ///< Indicates stream has started. EvtData = phy_stream_t
  PhyEvtType_ProfileUpdated   = 2,    ///< Indicates profile has been updated. No Evt data
  PhyEvtType_LogWrite         = 3,    ///< Log Data
} phy_evt_type_t;

/**
** PHY Callback
*/
typedef void (*phy_callback_t)( phy_evt_type_t EvtType, phy_evt_data_t EvtData, void *param );

/**
**  PHY Stream
*/
typedef struct{
  uint32_t         *SampleBuf;        ///< Sample Buffer
  uint32_t          SampleCnt;        ///< Length in samples
  adrv9001_port_t   Port;             ///< Port
  phy_callback_t    Callback;         ///< Callback
  void             *CallbackRef;      ///< User Data
  phy_status_t      Status;           ///< Status
  bool              Cyclic;           ///< Flag indicates the stream will continue Indefinitely
}phy_stream_t;

/**
**  PHY Configuration
*/
typedef struct{
  phy_callback_t    Callback;         ///< Callback
  void             *CallbackRef;      ///< User Data
}phy_cfg_t;

/*******************************************************************************
*
* \details
*
* This function enables continuous streaming of IQ data.
*
* \param[in]  Stream is the stream configuration.  This parameter is provided
*             by the caller and can be released once the function is returned.
*             However some of the parameters making up this variable must be
*             maintained in memory for parts or the duration the stream.
*
*             All streams are non-blocking.  The caller will receive event
*             callbacks indicating when the stream starts, stops, there is an
*             error, or it is ready for more data.  The callback function is
*             unique to each stream.  The caller also has the option of
*             providing a CallbackRef which points to user specific data.
*
*            -Transmit Stream
*             When streaming to a transmit port the SampleBuf must be maintained
*             until the Stream Start event is triggered and the caller receives
*             a callback with PhyEvtType_StreamStart.  At this point the caller
*             can free or re-purpose memory associated with SampleBuf.  This
*             applies to all modes including cyclic mode.  If the cyclic flag
*             is set the SampleBuf will be streamed continuously until the caller
*             executes Phy_IqStreamDisable.  If the cyclic flag is not set the
*             stream will stop once a number of SampleCnt samples have been
*             streamed.  Once the stream is done a callback of
*             PhyEvtType_StreamStop will be executed informing the caller.
*
*            -Receive Stream
*             When streaming to a receive port the caller will receive a
*             callback of PhyEvtType_StreamStart once streaming has started.
*             Once SampleCnt number of samples have been streamed to memory
*             the user will receive a callback .
*
*
* \return     Status
*
*******************************************************************************/
phy_status_t Phy_IqStreamEnable( phy_stream_t *Stream );

/*******************************************************************************
*
* \details
*
* This function enables continuous streaming of IQ data.
*
* \param[in]  Stream is the stream configuration
*
* \return     Status
*
*******************************************************************************/
phy_status_t Phy_IqStreamDisable( adrv9001_port_t Port );

/*******************************************************************************
*
* \details
*
* This function loads a new ADRV9001 profile from the file system
*
* \param[in]  Buf is a buffer containing the profile
*
* \param[in]  Length is the length of the buffer in bytes
*
* \return     Status
*
*******************************************************************************/
phy_status_t Phy_UpdateProfile( profile_t *Profile );

/*******************************************************************************
*
* \details
* This function initializes the PHY layer which includes the ADRV9002
*
* \return   int32_t - status
*
*******************************************************************************/
phy_status_t Phy_Initialize( phy_cfg_t *Cfg );



#endif /* SRC_PHY_H_ */
