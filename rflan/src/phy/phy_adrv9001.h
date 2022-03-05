#ifndef SRC_PHY_ADRV9001_H_
#define SRC_PHY_ADRV9001_H_
/***************************************************************************//**
*  \ingroup    RFLAN
*  \defgroup   ADRV9001 ADRV9001 Interface Driver
*  @{
*******************************************************************************/
/***************************************************************************//**
*  \file       phy_adrv9001.h
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

#include "phy_types.h"

phy_status_t PhyAdrv9001_EnableDac            ( phy_t *Instance, uint8_t Id, bool Enable );
phy_status_t PhyAdrv9001_SetDac               ( phy_t *Instance, uint8_t Id, float Voltage );
phy_status_t PhyAdrv9001_GetDac               ( phy_t *Instance, uint8_t Id, float *Voltage );
phy_status_t PhyAdrv9001_GetTemperature       ( phy_t *Instance, int16_t *Temp_C );


phy_status_t PhyAdrv9001_GetSsiDelay          ( phy_t *Instance, phy_port_t port, uint8_t *clkDelay, uint8_t *dataDelay );
phy_status_t PhyAdrv9001_SetSsiDelay          ( phy_t *Instance, phy_port_t port, uint8_t clkDelay, uint8_t dataDelay );
phy_status_t PhyAdrv9001_InspectTestMode      ( phy_t *Instance, phy_port_t Port );
phy_status_t PhyAdrv9001_SetTestMode          ( phy_t *Instance, phy_port_t Port );
phy_status_t PhyAdrv9001_RunSsiTestMode       ( phy_t *Instance, phy_port_t Port, uint8_t clkDelay, uint8_t dataDelay );
phy_status_t PhyAdrv9001_CalibrateSsiDelay    ( phy_t *Instance, phy_port_t Port );
phy_status_t PhyAdrv9001_PerformSsiSweep      ( phy_t *Instance, phy_port_t Port, uint8_t results[8][8] );
phy_status_t PhyAdrv9001_SetEnablePin         ( phy_t *Instance, phy_port_t Port, uint8_t Enabled );
phy_status_t PhyAdrv9001_LnaEnable            ( phy_t *Instance, phy_port_t Port, bool Enable );
phy_status_t PhyAdrv9001_GetGainControlMode   ( phy_t *Instance, phy_port_t Port, uint32_t *Rssi_mdB );
phy_status_t PhyAdrv9001_ResetSsiPort         ( phy_t *Instance, phy_port_t Port );
phy_status_t PhyAdrv9001_ToRfCalibrated       ( phy_t *Instance, phy_port_t Port );
phy_status_t PhyAdrv9001_ToRfEnabled          ( phy_t *Instance, phy_port_t Port );
phy_status_t PhyAdrv9001_ToRfPrimed           ( phy_t *Instance, phy_port_t Port );
phy_status_t PhyAdrv9001_GetRssi              ( phy_t *Instance, phy_port_t Port, uint32_t *Rssi_mdB );
phy_status_t PhyAdrv9001_SetTxBoost           ( phy_t *Instance, phy_port_t Port, bool Enable );
phy_status_t PhyAdrv9001_GetTxBoost           ( phy_t *Instance, phy_port_t Port, bool *Enable );
phy_status_t PhyAdrv9001_SetTxAttenuation     ( phy_t *Instance, phy_port_t Port, uint16_t Attn_mdB );
phy_status_t PhyAdrv9001_GetTxAttenuation     ( phy_t *Instance, phy_port_t Port, uint16_t *Attn_mdB );
phy_status_t PhyAdrv9001_GetSampleRate        ( phy_t *Instance, phy_port_t Port, uint32_t *FreqHz );
phy_status_t PhyAdrv9001_GetCarrierFrequency  ( phy_t *Instance, phy_port_t Port, uint64_t *FreqHz );
phy_status_t PhyAdrv9001_SetCarrierFrequency  ( phy_t *Instance, phy_port_t Port, uint64_t FreqHz );
phy_status_t PhyAdrv9001_SetInternalLoopBack  ( phy_t *Instance, phy_port_t Port, bool Enabled );

phy_status_t PhyAdrv9001_ConfigureGpio        ( phy_t *Instance );
phy_status_t PhyAdrv9001_LoadProfile          ( phy_t *Instance );
phy_status_t PhyAdrv9001_ClearError           ( phy_t *Instance );
phy_status_t PhyAdrv9001_Initialize           ( phy_t *Instance );

#endif
