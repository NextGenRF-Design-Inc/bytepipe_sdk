#ifndef VERSA_CLOCK5_H_
#define VERSA_CLOCK5_H_
/***************************************************************************//**
*  \ingroup    CSL
*  \defgroup   VERSA_CLOCK5 Versa Clock 5 Driver
*  @{
*******************************************************************************/
/***************************************************************************//**
*  \file       versa_clock5.h
*
*  \details
*
*  This file contains the Versa Clock5 driver.
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
#include "xiicps.h"
//#include "cli.h"

#define GTR0_REFCLK_FREQ_HZ             (52000000)
#define GTR1_REFCLK_FREQ_HZ             (125000000)
#define GTR2_REFCLK_FREQ_HZ             (27000000)
#define GTR3_REFCLK_FREQ_HZ             (300000000)
#define GTR0_REFCLK_VERSACLK5_PORT      (2)
#define GTR1_REFCLK_VERSACLK5_PORT      (3)
#define GTR2_REFCLK_VERSACLK5_PORT      (1)
#define GTR3_REFCLK_VERSACLK5_PORT      (0)
#define VERSA_CLOCK5_STATUS_OFFSET      (-600)

typedef struct
{
  XIicPs                 *Iic;
  uint8_t                 Addr;
} versa_clock5_t;

typedef struct
{
  XIicPs                 *Iic;
  uint8_t                 Addr;
} versa_clock5_init_t;

/**
* \brief Code indicated status of request
*/
typedef enum
{
  VersaClock5Status_Success                 = (0),
  VersaClock5Status_InvalidParameter        = (VERSA_CLOCK5_STATUS_OFFSET - 1),
  VersaClock5Status_IicError                = (VERSA_CLOCK5_STATUS_OFFSET - 2),
} versa_clock_status_t;

int32_t VersaClock5_Initialize( versa_clock5_t *Instance, versa_clock5_init_t *Init );
int32_t VersaClock5_ReadRegister(versa_clock5_t *Instance, uint16_t Address, uint8_t *Value);
int32_t VersaClock5_GlobalReset( versa_clock5_t *Instance );
int32_t VersaClock5_SetClockFreq(versa_clock5_t *Instance, uint8_t Port, uint32_t FreqHz );
int32_t VersaClock5_GetClockFreq(versa_clock5_t *Instance, uint8_t Port, uint32_t *FreqHz );
int32_t VersaClock5_Read(versa_clock5_t *Instance, uint8_t *Buf, uint16_t Length);
int32_t VersaClock5_Write(versa_clock5_t *Instance, uint8_t *Buf, uint16_t Length);
int32_t VersaClock5_WriteRegister(versa_clock5_t *Instance, uint16_t Address, uint8_t Value);

#endif
