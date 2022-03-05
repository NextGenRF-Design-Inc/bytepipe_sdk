#ifndef PHY_CLI_H_
#define PHY_CLI_H_

/***************************************************************************//**
*  \ingroup    PHY
*  \defgroup   PHY_CLI PHY Layer Command Line Interface
*  @{
*******************************************************************************/
/***************************************************************************//**
*  \file       phy_cli.h
*
*  \details    This file contains the RFLAN PHY layer CLI definitions.
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
#include "phy_types.h"
#include "cli.h"

void PhyCli_StreamEnable(Cli_t *CliInstance, const char *cmd, void *userData);
void PhyCli_LoadProfile(Cli_t *CliInstance, const char *cmd, void *userData);
void PhyCli_StreamDisable(Cli_t *CliInstance, const char *cmd, void *userData);

static const CliCmd_t PhyCliStreamEnableDef =
{
  "PhyStreamEnable",
  "PhyStreamEnable:  Enable PHY stream to or from a file. \r\n"
  "PhyStreamEnable <port (Rx1,Rx2,Tx1,Tx2), filename, SampleCnt (-1=cyclic, 0=file size)>\r\n\r\n",
  (CliCmdFn_t)PhyCli_StreamEnable,
  3,
  NULL
};

static const CliCmd_t PhyCliLoadProfileDef =
{
  "PhyLoadProfile",
  "PhyLoadProfile:  Load PHY Profile \r\n"
  "PhyLoadProfile < Index >\r\n\r\n",
  (CliCmdFn_t)PhyCli_LoadProfile,
  1,
  NULL
};

static const CliCmd_t PhyCliStreamDisableDef =
{
  "PhyStreamDisable",
  "PhyStreamDisable:  Disable PHY stream. \r\n"
  "PhyStreamDisable < port ( Rx1,Rx2,Tx1,Tx2 ) >\r\n\r\n",
  (CliCmdFn_t)PhyCli_StreamDisable,
  1,
  NULL
};


int PhyCli_Initialize( phy_t *Instance, Cli_t *Cli );


#endif /* PHY_CLI_H_ */
