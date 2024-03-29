#ifndef RFLAN_CLI_H_
#define RFLAN_CLI_H_
/***************************************************************************//**
*  \ingroup    RFLAN
*  \defgroup   RFLAN_CLI RFLAN Command Line Interface
*  @{
*******************************************************************************/
/***************************************************************************//**
*  \file       rflan_cli.h
*
*  \details    RFLAN Command Line Interface Definitions
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
#include "rflan_pib.h"
#include "rflan_gpio.h"

#ifdef RFLAN_STREAM_ENABLE
#include "rflan_stream.h"
#endif

/**
**  Initialization Structure
**
**  This structure is used to initialization the module.  The application can
**  destroy the corresponding parameter after calling initializing the module.
*/
typedef struct {
  cli_t              *Cli;                   ///< CLI Library Instance
  rflan_pib_t        *RflanPib;              ///< Reference to RFLAN Parameters
  XGpioPs            *Gpio;                  ///< Reference to RFLAN GPIO
#ifdef RFLAN_STREAM_ENABLE
  rflan_stream_t     *RflanStream;           ///< Reference to RFLAN Stream
#endif
  char               *BasePath;
} rflan_cli_init_t;

/**
**  Instance structure
**
**  This structure holds the variables associated with this module.  This
**  structure must be allocated and maintained by the application.  The application
**  should not access this structure directly.  The application must pass this
**  variable when calling all APIs.
*/
typedef struct {
  cli_t              *Cli;                   ///< CLI Library Instance
  rflan_pib_t        *RflanPib;              ///< Reference to RFLAN Parameters
  XGpioPs            *Gpio;                  ///< Reference to RFLAN GPIO
#ifdef RFLAN_STREAM_ENABLE
  rflan_stream_t     *RflanStream;           ///< Reference to RFLAN Stream
  char               *BasePath;
#endif
} rflan_cli_t;

/***************************************************************************//**
*
* \details  Initialize Driver
*
* \param    Instance [in]  Driver Instance
* \param    Init     [in]  Initialization structure
*
* \return   status
*
*******************************************************************************/
int32_t RflanCli_Initialize( rflan_cli_t *Instance, rflan_cli_init_t *Init );



#endif /* RFLAN_CLI_H_ */
