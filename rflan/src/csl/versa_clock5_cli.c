/***************************************************************************//**
*  \addtogroup APP_CLI
*   @{
*******************************************************************************/
/***************************************************************************//**
*  \file       versa_clock5_cli.c
*
*  \details    This file contains the RFLAN application CLI implementation.
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

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "cli.h"
#include "app_cli.h"
#include "xparameters.h"
#include "xil_types.h"
#include "xstatus.h"
#include "string.h"
#include "versa_clock5.h"


/*******************************************************************************
*
* \details Read VersaClock5 Register
*
*******************************************************************************/
static void AppCli_ReadVersaClock5(Cli_t *CliInstance, const char *cmd, void *userData)
{
  uint32_t addr;
  uint32_t Length;

  Cli_GetParameter(cmd, 1, CliParamTypeU32, &addr);
  Cli_GetParameter(cmd, 2, CliParamTypeU32, &Length);

  uint8_t *Buf = malloc(Length);

  Buf[0] = (uint8_t)addr;

  if(VersaClock5_Write( Buf, 1) == 0)
  {
    if(VersaClock5_Read(Buf, Length) == 0)
    {
      printf("0x");
      for( int i = 0; i < Length; i++)
      {
        printf("%02x",Buf[i]);
      }

      printf("\r\n");
    }
    else
    {
      printf("Failed\r\n");
    }
  }
  else
  {
    printf("Failed\r\n");
  }

}

static const CliCmd_t AppCliReadVersaClock5Def =
{
  "ReadVersaClock5",
  "ReadVersaClock5: Read VersaClock5 register \r\n"
  "ReadVersaClock5 < address, length >\r\n\n",
  (CliCmdFn_t)AppCli_ReadVersaClock5,
  2,
  NULL
};

/*******************************************************************************
*
* \details Write VersaClock5 Register
*
*******************************************************************************/
static void AppCli_WriteVersaClock5(Cli_t *CliInstance, const char *cmd, void *userData)
{
  uint32_t addr;
  uint16_t len;


  Cli_GetParameter(cmd, 1, CliParamTypeU32, &addr);

  const char *s = Cli_FindParameter( cmd, 2, &len );

  s += 2;
  len -= 2;

  uint8_t *Buf = malloc(len/2 + 1);

  Buf[0] = (uint8_t)addr;


  for( int i = 0; i < (len/2 - 1); i++ )
  {
    sscanf( (s + 2*i), "%02hhx", (Buf+i+1) );
  }

  if(VersaClock5_Write(Buf, len/2 + 1) == 0)
  {
    printf("Success\r\n");
  }
  else
  {
    printf("Failed\r\n");
  }

  free(Buf);

}

static const CliCmd_t AppCliWriteVersaClock5Def =
{
  "WriteVersaClock5",
  "WriteVersaClock5: Read VersaClock5 register \r\n"
  "WriteVersaClock5 < address, value >\r\n\n",
  (CliCmdFn_t)AppCli_WriteVersaClock5,
  2,
  NULL
};

/*******************************************************************************
*
* \details Write VersaClock5 Register
*
*******************************************************************************/
static void AppCli_GetVersaClock5Freq(Cli_t *CliInstance, const char *cmd, void *userData)
{
  uint32_t port;
  uint32_t freqHz;

  Cli_GetParameter(cmd, 1, CliParamTypeU32, &port);

  if(VersaClock5_GetClockFreq(port, &freqHz) == 0)
  {
    printf("Port %lu = %lu Hz\r\n", port, freqHz);
  }
  else
  {
    printf("Failed\r\n");
  }
}

static const CliCmd_t AppCliGetVersaClock5FreqDef =
{
  "GetVersaClock5Freq",
  "GetVersaClock5Freq: Read VersaClock5 frequency \r\n"
  "GetVersaClock5Freq < port >\r\n\n",
  (CliCmdFn_t)AppCli_GetVersaClock5Freq,
  1,
  NULL
};

/*******************************************************************************
*
* \details Write VersaClock5 Register
*
*******************************************************************************/
static void AppCli_SetVersaClock5Freq(Cli_t *CliInstance, const char *cmd, void *userData)
{
  uint32_t port;
  uint32_t freqHz;

  Cli_GetParameter(cmd, 1, CliParamTypeU32, &port);
  Cli_GetParameter(cmd, 2, CliParamTypeU32, &freqHz);

  if(VersaClock5_SetClockFreq(port, freqHz) == 0)
  {
    VersaClock5_GlobalReset( );

    printf("Success\r\n");
  }
  else
  {
    printf("Failed\r\n");
  }
}

static const CliCmd_t AppCliSetVersaClock5FreqDef =
{
  "SetVersaClock5Freq",
  "SetVersaClock5Freq: Set VersaClock5 frequency \r\n"
  "SetVersaClock5Freq < port, frequency >\r\n\n",
  (CliCmdFn_t)AppCli_SetVersaClock5Freq,
  2,
  NULL
};


int VersaClock5Cli_Initialize( void )
{
  Cli_t *Instance = AppCli_GetInstance( );

  /* Register APP Specific */
  Cli_RegisterCommand(Instance, &AppCliReadVersaClock5Def);
  Cli_RegisterCommand(Instance, &AppCliWriteVersaClock5Def);
  Cli_RegisterCommand(Instance, &AppCliGetVersaClock5FreqDef);
  Cli_RegisterCommand(Instance, &AppCliSetVersaClock5FreqDef);

	return XST_SUCCESS;
}
