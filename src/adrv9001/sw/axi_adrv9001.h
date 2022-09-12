#ifndef SRC_AXI_ADRV9001_H_
#define SRC_AXI_ADRV9001_H_
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
#include "adi_adrv9001_common_types.h"

typedef enum
{
  AXI_ADRV9001_DATA_PATH_AXIS = 0,
  AXI_ADRV9001_DATA_PATH_AXI = 1
} axi_adrv9001_data_path_t;


int32_t AxiAdrv9001_Initialize      ( uint32_t Base );

void AxiAdrv9001_ResetbPinSet       ( uint32_t Base, uint8_t Level );
void AxiAdrv9001_SetDgpio           ( uint32_t Base, uint32_t Value );
void AxiAdrv9001_GetDgpio           ( uint32_t Base, uint32_t *Value );
void AxiAdrv9001_GetDgpioDir        ( uint32_t Base, uint32_t *Value );
void AxiAdrv9001_SetDgpioDir        ( uint32_t Base, uint32_t Value );
void AxiAdrv9001_SetTxDataPath      ( uint32_t Base, adi_common_ChannelNumber_e Channel, axi_adrv9001_data_path_t Value );
void AxiAdrv9001_GetTxDataPath      ( uint32_t Base, adi_common_ChannelNumber_e Channel, axi_adrv9001_data_path_t *Value );
void AxiAdrv9001_SetTxData          ( uint32_t Base, adi_common_ChannelNumber_e Channel, uint32_t Value );
void AxiAdrv9001_GetRxData          ( uint32_t Base, adi_common_ChannelNumber_e Channel, uint32_t *Value );

void AxiAdrv9001_SetEnable          ( uint32_t Base, adi_common_Port_e Port, adi_common_ChannelNumber_e Channel, bool Enabled );
void AxiAdrv9001_SetDisableCnt      ( uint32_t Base, adi_common_Port_e Port, adi_common_ChannelNumber_e Channel, uint32_t SampleCnt );
void AxiAdrv9001_SetSsiEnableCnt    ( uint32_t Base, adi_common_Port_e Port, adi_common_ChannelNumber_e Channel, uint32_t SampleCnt );
void AxiAdrv9001_SetSsiDisableCnt   ( uint32_t Base, adi_common_Port_e Port, adi_common_ChannelNumber_e Channel, uint32_t SampleCnt );

int32_t AxiAdrv9001_MspiTransfer    ( uint32_t Base, uint8_t *TxBuf, uint8_t *RxBuf, uint32_t Length);

#endif
