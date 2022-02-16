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
#include "adrv9001.h"
#include "phy_cli.h"
#include "app_cli.h"
#include "phy.h"
#include "parameters.h"
#include "iq_file.h"

#define PHY_STREAM_RX_SAMPLE_CNT          (16384)

static const char* PhyCli_ParsePort(const char *cmd, uint16_t pNum, adrv9001_port_t *port)
{
  const char         *s = NULL;
  uint16_t            len;

  s = Cli_FindParameter( cmd, pNum, &len );

  if(!strncmp(s, "Rx1",len))
    *port = Adrv9001Port_Rx1;
  else if(!strncmp(s, "Rx2",len))
    *port = Adrv9001Port_Rx2;
  else if(!strncmp(s, "Tx1",len))
    *port = Adrv9001Port_Tx1;
  else if(!strncmp(s, "Tx2",len))
    *port = Adrv9001Port_Tx2;

  return s;
}

static void PhyCli_PhyCallback( phy_evt_type_t EvtType, phy_evt_data_t EvtData, void *param)
{
  if( EvtType == PhyEvtType_StreamDone )
  {
    /* Indicate Event to User */
    printf("%s stream done\r\n", ADRV9001_PORT_2_STR( EvtData.Stream.Port ));

    /* Process Rx Stream */
    if( PHY_IS_PORT_RX( EvtData.Stream.Port ) )
    {
      /* Delete Existing File */
      f_unlink(EvtData.Stream.CallbackRef);

      /* Write To File */
      if( IqFile_Write(EvtData.Stream.CallbackRef, EvtData.Stream.SampleBuf, EvtData.Stream.SampleCnt) != XST_SUCCESS)
        printf("%s stream file write error\r\n", ADRV9001_PORT_2_STR( EvtData.Stream.Port ));

      /* Free Filename */
      free(EvtData.Stream.CallbackRef);
    }

    /* Free Sample Buffer */
    free(EvtData.Stream.SampleBuf);
  }
  else if( EvtType == PhyEvtType_StreamStart )
  {
    /* Indicate Event to User */
    printf("%s stream start\r\n", ADRV9001_PORT_2_STR( EvtData.Stream.Port ));
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

static void PhyCli_IqFileStreamEnable(Cli_t *CliInstance, const char *cmd, void *userData)
{
  phy_stream_t Stream = {.Callback = PhyCli_PhyCallback};
  int32_t status;

  /* Parse Port */
  if(PhyCli_ParsePort(cmd, 1, &Stream.Port) == NULL)
  {
    printf("Invalid Parameter\r\n");
    return;
  }

  /* Get Filename */
  char *filename = calloc(1, FF_FILENAME_MAX_LEN );
  strcpy(filename,FF_LOGICAL_DRIVE_PATH);
  Cli_GetParameter(cmd, 2, CliParamTypeStr, &filename[strlen(filename)]);

  int32_t SampleCnt;
  Cli_GetParameter(cmd, 3, CliParamTypeS32, &SampleCnt);

  Adrv9001_ClearError( );

  if( SampleCnt == -1 )
    Stream.Cyclic = true;
  else
    Stream.Cyclic = false;


  if( (Stream.Port == Adrv9001Port_Tx1) || (Stream.Port == Adrv9001Port_Tx2) )
  {
    if(IqFile_Read( filename, &Stream.SampleBuf, &Stream.SampleCnt ) != XST_SUCCESS)
    {
      printf("Invalid Parameter\r\n");
      return;
    }

    if( SampleCnt > 0 )
      Stream.SampleCnt = SampleCnt;
  }
  else if( (Stream.Port == Adrv9001Port_Rx1) || (Stream.Port == Adrv9001Port_Rx2) )
  {
    if( SampleCnt <= 0 )
    {
      printf("Invalid Parameter. Sample Count must be greater than zero for receive\r\n");
      return;
    }

    Stream.SampleCnt = SampleCnt;

    /* Store Filename for future reference */
    if((Stream.CallbackRef = malloc(strlen(filename))) == NULL)
    {
      printf("Memory Error\r\n");
      return;
    }
    else
    {
      strcpy((char*)Stream.CallbackRef, filename);
    }

    /* Allocate Buffer */
    if((Stream.SampleBuf = calloc(1, SampleCnt * sizeof(uint32_t))) == NULL)
    {
      printf("Memory Error\r\n");
      return;
    }
  }

  /* Enable Streaming */
  if((status = Phy_IqStreamEnable( &Stream )) != PhyStatus_Success)
  {
    printf("%s\r\n",PHY_STATUS_2_STR(status));
  }
}

static const CliCmd_t PhyCliIqFileStreamEnableDef =
{
  "PhyIqFileStreamEnable",
  "PhyIqFileStreamEnable:  Enable IQ stream to or from a file. \r\n"
  "PhyIqFileStreamEnable <port (Rx1,Rx2,Tx1,Tx2), filename, SampleCnt (-1=cyclic, 0=file size)>\r\n\r\n",
  (CliCmdFn_t)PhyCli_IqFileStreamEnable,
  3,
  NULL
};

static void PhyCli_IqFileStreamDisable(Cli_t *CliInstance, const char *cmd, void *userData)
{
  adrv9001_port_t     port;
  int32_t status;

  /* Parse Port */
  if(PhyCli_ParsePort(cmd, 1, &port) == NULL)
  {
    printf("Invalid Parameter\r\n");
    return;
  }

  /* Disable Stream */
  if((status = Phy_IqStreamDisable( port )) != PhyStatus_Success )
  {
    printf("%s\r\n",PHY_STATUS_2_STR(status));
  }
}

static const CliCmd_t PhyCliIqFileStreamDisableDef =
{
  "PhyIqFileStreamDisable",
  "PhyIqFileStreamDisable:  Disable IQ stream. \r\n"
  "PhyIqFileStreamDisable < port ( Rx1,Rx2,Tx1,Tx2 ) >\r\n\r\n",
  (CliCmdFn_t)PhyCli_IqFileStreamDisable,
  1,
  NULL
};

static void PhyCli_IqFileSize(Cli_t *CliInstance, const char *cmd, void *userData)
{
  int32_t status;

  /* Get Filename */
  char *filename = calloc(1, FF_FILENAME_MAX_LEN );
  strcpy(filename,FF_LOGICAL_DRIVE_PATH);
  Cli_GetParameter(cmd, 1, CliParamTypeStr, &filename[strlen(filename)]);

  uint32_t SampleCnt;
  FIL fil;

  if((status = f_open(&fil, filename, FA_OPEN_EXISTING | FA_READ)) == FR_OK)
  {
    status = IqFile_GetSampleCnt( &fil, &SampleCnt);
  }

  f_close(&fil);

  if(status == 0)
  {
    printf("%lu\r\n", SampleCnt);
  }
  else
  {
    printf("%s\r\n",PHY_STATUS_2_STR(status));
  }
}

static const CliCmd_t PhyCliIqFileSizeDef =
{
  "PhyIqFileSize",
  "PhyIqFileSize:  Returns number of IQ samples in file \r\n"
  "PhyIqFileSize < filename >\r\n\r\n",
  (CliCmdFn_t)PhyCli_IqFileSize,
  1,
  NULL
};

static void PhyCli_Adrv9001Init(Cli_t *CliInstance, const char *cmd, void *userData)
{
  int32_t status = Phy_Adrv9001Initialize( );

  printf("%s\r\n",PHY_STATUS_2_STR(status));
}

static const CliCmd_t PhyCliAdrv9001InitDef =
{
  "PhyAdrv9001Init",
  "PhyAdrv9001Init:  Initialize ADRV9001 \r\n"
  "PhyAdrv9001Init <  >\r\n\r\n",
  (CliCmdFn_t)PhyCli_Adrv9001Init,
  0,
  NULL
};

static void PhyCli_Adrv9001LoadProfile(Cli_t *CliInstance, const char *cmd, void *userData)
{
  /* Get Filename */
  char *ProfileName = calloc(1, FF_FILENAME_MAX_LEN );
  Cli_GetParameter(cmd, 1, CliParamTypeStr, ProfileName);

  char *StreamImageName = calloc(1, FF_FILENAME_MAX_LEN );
  Cli_GetParameter(cmd, 2, CliParamTypeStr, StreamImageName);

  /* Load Profile */
  int32_t status = Phy_Adrv9001LoadProfileReq( ProfileName, StreamImageName);

  free(StreamImageName);
  free(ProfileName);

  printf("%s\r\n",PHY_STATUS_2_STR(status));
}

static const CliCmd_t PhyCliAdrv9001LoadProfileDef =
{
  "PhyAdrv9001LoadProfile",
  "PhyAdrv9001LoadProfile:  Load ADRV9001 Profile \r\n"
  "PhyAdrv9001LoadProfile < ProfileName, StreamImageName >\r\n\r\n",
  (CliCmdFn_t)PhyCli_Adrv9001LoadProfile,
  2,
  NULL
};

int PhyCli_Initialize( void )
{
  Cli_t *Instance = AppCli_GetInstance( );

  Cli_RegisterCommand(Instance, &PhyCliIqFileStreamEnableDef);
  Cli_RegisterCommand(Instance, &PhyCliIqFileStreamDisableDef);
  Cli_RegisterCommand(Instance, &PhyCliIqFileSizeDef);
  Cli_RegisterCommand(Instance, &PhyCliAdrv9001InitDef);
  Cli_RegisterCommand(Instance, &PhyCliAdrv9001LoadProfileDef);

	return PhyStatus_Success;
}
