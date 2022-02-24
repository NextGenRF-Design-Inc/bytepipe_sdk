/***************************************************************************//**
*  \addtogroup PHY_CLI
*   @{
*******************************************************************************/
/***************************************************************************//**
*  \file       phy_cli.c
*
*  \details    This file contains the RFLAN PHY layer CLI commands.
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
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "phy_cli.h"
#include "app_cli.h"
#include "phy.h"
#include "iq_file.h"

static phy_t *PhyCliInstance = NULL;

static const char* PhyCli_ParsePort(const char *cmd, uint16_t pNum, phy_port_t *port)
{
  const char         *s = NULL;
  uint16_t            len;

  s = Cli_FindParameter( cmd, pNum, &len );

  if(!strncmp(s, "Rx1",len))
    *port = PhyPort_Rx1;
  else if(!strncmp(s, "Rx2",len))
    *port = PhyPort_Rx2;
  else if(!strncmp(s, "Tx1",len))
    *port = PhyPort_Tx1;
  else if(!strncmp(s, "Tx2",len))
    *port = PhyPort_Tx2;

  return s;
}

static void PhyCli_Callback( phy_evt_type_t EvtType, void *EvtData, void *param)
{
  if( EvtType == PhyEvtType_StreamInd )
  {
    phy_stream_ind_t *Stream = (phy_stream_ind_t*)EvtData;
    char *Filename = (char*)param;

    if( Stream->Status == PhyStatus_StreamDone )
    {
      /* Indicate Event to User */
      printf("%s stream done\r\n", PHY_PORT_2_STR( Stream->Port ));

      /* Process Rx Stream */
      if( PHY_IS_PORT_RX( Stream->Port ) )
      {
        /* Delete Existing File */
        f_unlink(Filename);

        /* Write To File */
        if( IqFile_Write(Filename, Stream->SampleBuf, Stream->SampleCnt) != 0)
          printf("%s stream file write error\r\n", PHY_PORT_2_STR( Stream->Port ));

        /* Free Filename */
        free(Filename);
      }

      /* Free Sample Buffer */
      free(Stream->SampleBuf);
    }
    else if(  Stream->Status == PhyStatus_StreamStart )
    {
      /* Indicate Event to User */
      printf("%s stream start\r\n", PHY_PORT_2_STR( Stream->Port ));
    }
  }
  else if( EvtType == PhyEvtType_ProfileUpdated )
  {
    /* Indicate Event to User */
    printf("Profile Update Success\r\n");
  }
  else
  {
    /* Indicate Event to User */
    printf("Unknown PHY Event Callback\r\n");
  }
}


void PhyCli_StreamEnable(Cli_t *CliInstance, const char *cmd, void *userData)
{
  phy_stream_req_t StreamReq = {.Callback = PhyCli_Callback};
  int32_t status;
  char *filename;

  /* Parse Port */
  if(PhyCli_ParsePort(cmd, 1, &StreamReq.Port) == NULL)
  {
    printf("Invalid Parameter\r\n");
    return;
  }

  /* Get Filename */
  if((filename = calloc(1, FF_FILENAME_MAX_LEN )) == NULL)
  {
    printf("Memory Error\r\n");
    return;
  }

  strcpy(filename,FF_LOGICAL_DRIVE_PATH);
  Cli_GetParameter(cmd, 2, CliParamTypeStr, &filename[strlen(filename)]);

  int32_t SampleCnt;
  Cli_GetParameter(cmd, 3, CliParamTypeS32, &SampleCnt);

  if( SampleCnt == -1 )
    StreamReq.Cyclic = true;
  else
    StreamReq.Cyclic = false;


  if( PHY_IS_PORT_TX(StreamReq.Port) )
  {
    if(IqFile_Read( filename, &StreamReq.SampleBuf, &StreamReq.SampleCnt ) != 0)
    {
      free(filename);
      printf("Invalid Parameter\r\n");
      return;
    }

    free(filename);

    if( SampleCnt > 0 )
      StreamReq.SampleCnt = SampleCnt;
  }
  else if( PHY_IS_PORT_RX(StreamReq.Port) )
  {
    if( SampleCnt <= 0 )
    {
      printf("Invalid Parameter. Sample Count must be greater than zero for receive\r\n");
      return;
    }

    StreamReq.SampleCnt = SampleCnt;

    /* Store Filename for future reference.  This is freed in callback when stream done */
    if((StreamReq.CallbackRef = malloc(strlen(filename))) == NULL)
    {
      free(filename);
      printf("Memory Error\r\n");
      return;
    }
    else
    {
      strcpy((char*)StreamReq.CallbackRef, filename);
      free(filename);
    }

    /* Allocate Buffer.  This is freed in callback when stream done */
    if((StreamReq.SampleBuf = calloc(1, SampleCnt * sizeof(uint32_t))) == NULL)
    {
      printf("Memory Error\r\n");
      return;
    }
  }

  /* Enable Streaming */
  if((status = Phy_StreamEnable( PhyCliInstance, &StreamReq )) != PhyStatus_Success)
  {
    printf("%s\r\n",PHY_STATUS_2_STR(status));
  }
}

void PhyCli_StreamDisable(Cli_t *CliInstance, const char *cmd, void *userData)
{
  phy_port_t     port;
  int32_t status;

  /* Parse Port */
  if(PhyCli_ParsePort(cmd, 1, &port) == NULL)
  {
    printf("Invalid Parameter\r\n");
    return;
  }

  /* Disable Stream */
  if((status = Phy_StreamDisable( PhyCliInstance, port )) != PhyStatus_Success )
  {
    printf("%s\r\n",PHY_STATUS_2_STR(status));
  }
}

void PhyCli_LoadProfile(Cli_t *CliInstance, const char *cmd, void *userData)
{
  phy_profile_t PhyProfile = {
      .JsonFilename = FF_LOGICAL_DRIVE_PATH,
      .StreamFilename = FF_LOGICAL_DRIVE_PATH
  };


  /* Get Filename */
  Cli_GetParameter(cmd, 1, CliParamTypeStr, (void*)&PhyProfile.JsonFilename[strlen(FF_LOGICAL_DRIVE_PATH)]);

  Cli_GetParameter(cmd, 2, CliParamTypeStr, (void*)&PhyProfile.StreamFilename[strlen(FF_LOGICAL_DRIVE_PATH)]);

  /* Load Profile */
  int32_t status = Phy_LoadProfile( PhyCliInstance, &PhyProfile );

  if( status != 0 )
    printf("%s\r\n",PHY_STATUS_2_STR(status));
}


int PhyCli_Initialize( phy_t *Instance, Cli_t *Cli )
{
  PhyCliInstance = Instance;

  Cli_RegisterCommand(Cli, &PhyCliStreamEnableDef);
  Cli_RegisterCommand(Cli, &PhyCliStreamDisableDef);
  Cli_RegisterCommand(Cli, &PhyCliLoadProfileDef);

	return PhyStatus_Success;
}
