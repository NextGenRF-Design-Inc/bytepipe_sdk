#ifndef RFLAN_PIB_H_
#define RFLAN_PIB_H_
/***************************************************************************//**
*  \ingroup    RFLAN
*  \defgroup   RFLAN_PIB RFLAN PIB Application
*  @{
*******************************************************************************/
/***************************************************************************//**
*  \file       rflan_pib.h
*
*  \details    This file contains the RFLAN PIB application definitions.
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
#ifdef __cplusplus
 extern "C" {
#endif

#include "cli.h"
#include "pib.h"

/**
** RFLAN parameters
*/
typedef struct {
  uint32_t            PibSize;              ///< Length of PIB in bytes
  uint32_t            FwVer;                ///< Firmware Version
  uint32_t            HwVer;                ///< Hardware Version
  char                IpAddr[20];           ///< IP Address
  char                IpMask[20];           ///< IP Mask
  char                IpGwAddr[20];         ///< IP Gateway Address
  uint8_t             LwipEnable;           ///< Enable LWIP
} rflan_params_t;

/**
** RFLAN PIB
*/
typedef struct {
  uint32_t            HwVer;
} rflan_pib_init_t;

/**
** RFLAN PIB
*/
typedef struct {
  pib_t               Pib;
  rflan_params_t      Params;
} rflan_pib_t;


int32_t RflanPib_Initialize     ( rflan_pib_t *Instance, rflan_pib_init_t *Init );
int32_t RflanPib_SetbyName      ( rflan_pib_t *Instance, char *name, char *value );
int32_t RflanPib_GetByName      ( rflan_pib_t *Instance, char *name, uint8_t *value );

#ifdef __cplusplus
}
#endif

#endif /* RFLAN_PIB_H_ */
