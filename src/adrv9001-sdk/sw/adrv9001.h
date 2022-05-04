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
#include "config.h"

#include "adi_adrv9001.h"
#include "adi_adrv9001_error.h"

#ifndef ADRV9001_STATUS_OFFSET
#define ADRV9001_STATUS_OFFSET    (-2200)
#endif

/**
* \brief Code indicated status of request
*/
typedef enum
{
  Adrv9001Status_Success              = (ADI_COMMON_ACT_NO_ACTION),
  Adrv9001Status_TimerError           = (ADRV9001_STATUS_OFFSET + ADI_COMMON_ACT_ERR_CHECK_TIMER),
  Adrv9001Status_InvalidParameter     = (ADRV9001_STATUS_OFFSET + ADI_COMMON_ACT_ERR_CHECK_PARAM),
  Adrv9001Status_InterfaceError       = (ADRV9001_STATUS_OFFSET + ADI_COMMON_ACT_ERR_RESET_INTERFACE),
  Adrv9001Status_FeatureError         = (ADRV9001_STATUS_OFFSET + ADI_COMMON_ACT_ERR_RESET_FEATURE),
  Adrv9001Status_ModuleError          = (ADRV9001_STATUS_OFFSET + ADI_COMMON_ACT_ERR_RESET_MODULE),
  Adrv9001Status_Error                = (ADRV9001_STATUS_OFFSET + ADI_COMMON_ACT_ERR_RESET_FULL),
  Adrv9001Status_NotSupported         = (ADRV9001_STATUS_OFFSET + ADI_COMMON_ACT_ERR_API_NOT_IMPLEMENTED),
  Adrv9001Status_ResetArm             = (ADRV9001_STATUS_OFFSET + ADI_ADRV9001_ACT_ERR_RESET_ARM),
  Adrv9001Status_InitCals             = (ADRV9001_STATUS_OFFSET + ADI_ADRV9001_ACT_ERR_RERUN_INIT_CALS),
  Adrv9001Status_GpioError            = (ADRV9001_STATUS_OFFSET + ADI_ADRV9001_ACT_ERR_RESET_GPIO),
  Adrv9001Status_LogError             = (ADRV9001_STATUS_OFFSET + ADI_ADRV9001_ACT_ERR_BBIC_LOG_ERROR),
  Adrv9001Status_CacheError           = (ADRV9001_STATUS_OFFSET + ADI_ADRV9001_ACT_ERR_RESET_CACHE),
  Adrv9001Status_FlushCache           = (ADRV9001_STATUS_OFFSET + ADI_ADRV9001_ACT_ERR_FLUSH_CACHE),
} adrv9001_status_t;

/**
* \brief Converts status code to string.
*/
#define ADRV9001_STATUS_2_STR(p) (                                                    \
  ( p == Adrv9001Status_Success )?                "Success" :                         \
  ( p == Adrv9001Status_TimerError )?             "Adrv9001 Timer Error" :            \
  ( p == Adrv9001Status_InvalidParameter )?       "Adrv9001 Invalid Parameter" :      \
  ( p == Adrv9001Status_InterfaceError )?         "Adrv9001 Interface Error" :        \
  ( p == Adrv9001Status_FeatureError )?           "Adrv9001 Feature Error" :          \
  ( p == Adrv9001Status_ModuleError )?            "Adrv9001 Module Error" :           \
  ( p == Adrv9001Status_Error )?                  "Adrv9001 Error" :                  \
  ( p == Adrv9001Status_NotSupported )?           "Adrv9001 Not Supported" :          \
  ( p == Adrv9001Status_ResetArm )?               "Adrv9001 Reset Arm" :              \
  ( p == Adrv9001Status_InitCals )?               "Adrv9001 InitCals Error" :         \
  ( p == Adrv9001Status_GpioError )?              "Adrv9001 Gpio Error" :             \
  ( p == Adrv9001Status_LogError )?               "Adrv9001 Log Error" :              \
  ( p == Adrv9001Status_CacheError )?             "Adrv9001 Cache Error" :            \
  ( p == Adrv9001Status_FlushCache )?             "Adrv9001 Flush Cache" : "Unknown" )



int32_t Adrv9001_Initialize( adi_adrv9001_Device_t *adrv9001Device_0 );


#endif
