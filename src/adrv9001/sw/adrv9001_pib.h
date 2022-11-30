#ifndef ADRV9001_PIB_H_
#define ADRV9001_PIB_H_
/***************************************************************************//**
*  \ingroup    ADRV9001
*  \defgroup   ADRV9001_PIB ADRV9001 PIB
*  @{
*******************************************************************************/
/***************************************************************************//**
*  \file       adrv9001_pib.h
*
*  \details    This file contains the ADRV9001 parameter information base
*              definitions.
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


#include "pib.h"
#include "adrv9001.h"

/***************************************************************************//**
*
* \details  Initialize Driver
*
* \param    Instance [in]  ADRV9001 Instance
*
* \return   status
*
*******************************************************************************/
int32_t Adrv9001Pib_Initialize( adrv9001_t *Instance );

/***************************************************************************//**
*
* \details  Set Parameter value using name of parameter and value as a string.
*
* \param    Instance [in]  Driver Instance
* \param    name     [in]  String containing the parameter name
* \param    str      [in]  String containing the parameter value
*
* \return   status
*
*******************************************************************************/
int32_t Adrv9001Pib_SetByNameByString( adrv9001_t *Instance, char *name, char *str );

/***************************************************************************//**
*
* \details  Get Parameter value as a string using name of parameter.
*
* \param    Instance [in]  Driver Instance
* \param    name     [in]  String containing the parameter name
* \param    str      [out] String containing the parameter value
*
* \return   status
*
*******************************************************************************/
int32_t Adrv9001Pib_GetStringByName( adrv9001_t *Instance, char *name, char *str );

#ifdef __cplusplus
}
#endif

#endif /* ADRV9001_PIB_H_ */
