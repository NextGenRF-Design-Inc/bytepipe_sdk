#ifndef TCA9548_H_
#define TCA9548_H_
/***************************************************************************//**
*  \ingroup    CSL
*  \defgroup   TCA9548 Driver
*  @{
*******************************************************************************/
/***************************************************************************//**
*  \file       tca9548.h
*
*  \details
*
*  This file contains the TCA9548 driver.
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

#define TCA9548_STATUS_OFFSET      (-700)

/**
* \brief Code indicated status of request
*/
typedef enum
{
  Tca95485Status_Success                 = (0),
  Tca9548Status_InvalidParameter        = (TCA9548_STATUS_OFFSET - 1),
  Tca9548Status_IicError                = (TCA9548_STATUS_OFFSET - 2),
  Tca9548Status_WriteError              = (TCA9548_STATUS_OFFSET - 3),
} tca9548_status_t;


typedef struct
{
  XIicPs                 *Iic;
  uint8_t                 Addr;
} tca9548_t;

typedef struct
{
  XIicPs                 *Iic;
  uint8_t                 Addr;
} tca9548_init_t;



int32_t Tca9548_Initialize( tca9548_t *Instance, tca9548_init_t *Init );
int32_t Tca9548_GetChannel( tca9548_t *Instance, uint8_t *Channel);
int32_t Tca9548_SetChannel( tca9548_t *Instance, uint8_t Channel);

#endif
