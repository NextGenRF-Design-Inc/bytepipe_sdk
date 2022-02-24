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
#include "phy_types.h"


#define PHY_IS_PORT_TX(p)           ((( p == PhyPort_Tx1 ) || ( p == PhyPort_Tx2 )) ? true : false)
#define PHY_IS_PORT_RX(p)           ((( p == PhyPort_Rx1 ) || ( p == PhyPort_Rx2 )) ? true : false)
#define PHY_PORT_2_STR(p)            (( p == PhyPort_Tx1 )? "Tx1" :                 \
                                      ( p == PhyPort_Tx2 )? "Tx2" :                 \
                                      ( p == PhyPort_Rx1 )? "Rx1" :                 \
                                      ( p == PhyPort_Rx2 )? "Rx2" : "Unknown")


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

phy_status_t Phy_ToRfEnabled    ( phy_t *Instance, phy_port_t Port );
phy_status_t Phy_ToRfPrimied    ( phy_t *Instance, phy_port_t Port );
phy_status_t Phy_ToRfCalibrated ( phy_t *Instance, phy_port_t Port );
phy_status_t Phy_StreamDisable  ( phy_t *Instance, phy_port_t Port );
phy_status_t Phy_StreamEnable   ( phy_t *Instance, phy_stream_req_t *Stream );
phy_status_t Phy_LoadProfile    ( phy_t *Instance, phy_profile_t *Profile );
phy_status_t Phy_Initialize     ( phy_t *Instance, phy_cfg_t *Cfg );



#endif /* SRC_PHY_H_ */
