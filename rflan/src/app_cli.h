#ifndef APP_CLI_H_
#define APP_CLI_H_
/***************************************************************************//**
*  \ingroup    APP
*  \defgroup   APP_CLI APP Command Line Interface
*  @{
*******************************************************************************/
/***************************************************************************//**
*  \file       app_cli.h
*
*  \details    APP Command Line Interface Definitions
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
extern "C"
{
#endif

#include <stdint.h>
#include "cli.h"
#include "ff.h"


/******************************************************************************/
/**
* \details
*
* This function initializes the application CLI functionality which includes
* serial port processing and basic CLI commands.  Additional CLI commands can be
* added in adjacent application modules.
*
* \return     Status
*
*******************************************************************************/
int32_t AppCli_Initialize( FIL *LogFil );

/******************************************************************************/
/**
* \details
*
* This function returns the CLI instance allowing other application modules to
* add CLI commands.
*
* \return     Cli_t is the CLI instance.
*
*******************************************************************************/
Cli_t *AppCli_GetInstance( void );

/******************************************************************************/
/**
* \details
*
* This function is used for sending a character out to the serial port.
*
* \return     Nothing
*
*******************************************************************************/
void outbyte(char c);

#endif /* APP_CLI_H_ */
