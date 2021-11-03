#ifndef SRC_APP_H_
#define SRC_APP_H_
/***************************************************************************//**
*  \ingroup    RFLAN
*  \defgroup   APP RFLAN Application
*  @{
*******************************************************************************/
/***************************************************************************//**
*  \file       app.h
*
*  \details    This file contains the RFLAN application definitions.
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


/******************************************************************************/
/**
*   Build Version
*
*   The build version is based on the year, month, and day of which the code
*   was compiled.
*
*******************************************************************************/
#define COMPUTE_BUILD_YEAR  ((__DATE__[ 9] - '0') * 10 +  (__DATE__[10] - '0'))
#define COMPUTE_BUILD_DAY   (((__DATE__[4] >= '0') ? (__DATE__[4] - '0') * 10 : 0) + (__DATE__[5] - '0'))

#define BUILD_MONTH_IS_JAN  (__DATE__[0] == 'J' && __DATE__[1] == 'a' && __DATE__[2] == 'n')
#define BUILD_MONTH_IS_FEB  (__DATE__[0] == 'F')
#define BUILD_MONTH_IS_MAR  (__DATE__[0] == 'M' && __DATE__[1] == 'a' && __DATE__[2] == 'r')
#define BUILD_MONTH_IS_APR  (__DATE__[0] == 'A' && __DATE__[1] == 'p')
#define BUILD_MONTH_IS_MAY  (__DATE__[0] == 'M' && __DATE__[1] == 'a' && __DATE__[2] == 'y')
#define BUILD_MONTH_IS_JUN  (__DATE__[0] == 'J' && __DATE__[1] == 'u' && __DATE__[2] == 'n')
#define BUILD_MONTH_IS_JUL  (__DATE__[0] == 'J' && __DATE__[1] == 'u' && __DATE__[2] == 'l')
#define BUILD_MONTH_IS_AUG  (__DATE__[0] == 'A' && __DATE__[1] == 'u')
#define BUILD_MONTH_IS_SEP  (__DATE__[0] == 'S')
#define BUILD_MONTH_IS_OCT  (__DATE__[0] == 'O')
#define BUILD_MONTH_IS_NOV  (__DATE__[0] == 'N')
#define BUILD_MONTH_IS_DEC  (__DATE__[0] == 'D')


#define COMPUTE_BUILD_MONTH \
  ( \
    (BUILD_MONTH_IS_JAN) ?  1 : \
    (BUILD_MONTH_IS_FEB) ?  2 : \
    (BUILD_MONTH_IS_MAR) ?  3 : \
    (BUILD_MONTH_IS_APR) ?  4 : \
    (BUILD_MONTH_IS_MAY) ?  5 : \
    (BUILD_MONTH_IS_JUN) ?  6 : \
    (BUILD_MONTH_IS_JUL) ?  7 : \
    (BUILD_MONTH_IS_AUG) ?  8 : \
    (BUILD_MONTH_IS_SEP) ?  9 : \
    (BUILD_MONTH_IS_OCT) ? 10 : \
    (BUILD_MONTH_IS_NOV) ? 11 : \
    (BUILD_MONTH_IS_DEC) ? 12 : \
     /* error default */  99 \
  )

#define BUILD_DATE_IS_BAD (__DATE__[0] == '?')
#define BUILD_YEAR  ((BUILD_DATE_IS_BAD) ? 99 : COMPUTE_BUILD_YEAR)
#define BUILD_MONTH ((BUILD_DATE_IS_BAD) ? 99 : COMPUTE_BUILD_MONTH)
#define BUILD_DAY   ((BUILD_DATE_IS_BAD) ? 99 : COMPUTE_BUILD_DAY)

#define APP_FW_VER_MAJOR_MASK     (0x00ff0000)
#define APP_FW_VER_MAJOR_SHIFT    (16)
#define APP_FW_VER_MINOR_MASK     (0x0000ff00)
#define APP_FW_VER_MINOR_SHIFT    (8)
#define APP_FW_VER_REV_MASK       (0x000000ff)
#define APP_FW_VER_REV_SHIFT      (0)

#define APP_FW_VER_MAJOR    BUILD_YEAR
#define APP_FW_VER_MINOR    BUILD_MONTH
#define APP_FW_VER_REV      BUILD_DAY

#define APP_FW_VER \
  ( \
    ((APP_FW_VER_MAJOR << APP_FW_VER_MAJOR_SHIFT) & APP_FW_VER_MAJOR_MASK) + \
		((APP_FW_VER_MINOR << APP_FW_VER_MINOR_SHIFT) & APP_FW_VER_MINOR_MASK) + \
	  ((APP_FW_VER_REV << APP_FW_VER_REV_SHIFT) & APP_FW_VER_REV_MASK) \
	)
#endif /* SRC_APP_H_ */
