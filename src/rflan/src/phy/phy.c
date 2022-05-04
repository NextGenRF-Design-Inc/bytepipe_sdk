/***************************************************************************//**
*  \addtogroup PHY
*   @{
*******************************************************************************/
/***************************************************************************//**
*  \file       phy.c
*
*  \details    This file contains the RFLAN PHY layer implementation.
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
#include <stdlib.h>
#include <string.h>
#include "phy.h"
#include "phy_cli.h"
#include "phy_stream.h"
#include "phy_adrv9001.h"
#include "phy_task.h"
#include "phy_profile.h"

phy_status_t Phy_ToRfEnabled( phy_t *Instance, phy_port_t Port )
{
  if( (Port >= PhyPort_Num) && (Port != PhyPort_All) )
    return PhyStatus_InvalidPort;

  /* Create Request */
  phy_task_req_t req = {.v = Port};

  return PhyTask_Send( Instance, (phy_task_t)PhyAdrv9001_ToRfEnabled, req );
}

phy_status_t Phy_ToRfPrimied( phy_t *Instance, phy_port_t Port )
{
  if( (Port >= PhyPort_Num) && (Port != PhyPort_All) )
    return PhyStatus_InvalidPort;

  /* Create Request */
  phy_task_req_t req = {.v = Port};

  return PhyTask_Send( Instance, (phy_task_t)PhyAdrv9001_ToRfPrimed, req );
}

phy_status_t Phy_ToRfCalibrated( phy_t *Instance, phy_port_t Port )
{
  if( (Port >= PhyPort_Num) && (Port != PhyPort_All) )
    return PhyStatus_InvalidPort;

  /* Create Request */
  phy_task_req_t req = {.v = Port};

  return PhyTask_Send( Instance, (phy_task_t)PhyAdrv9001_ToRfCalibrated, req );
}

phy_status_t Phy_StreamDisable( phy_t *Instance, phy_port_t Port )
{
  if( (Port >= PhyPort_Num) && (Port != PhyPort_All) )
    return PhyStatus_InvalidPort;

  int32_t status;

  if((status = Phy_ToRfPrimied( Instance, Port)) != 0)
    return status;

  /* Create Request */
  phy_task_req_t req = {.v = Port};

  return PhyTask_Send( Instance, (phy_task_t)PhyStream_Disable, req );
}

phy_status_t Phy_StreamEnable( phy_t *Instance, phy_stream_req_t *Stream )
{
  int32_t status;

  if( Stream->Port >= PhyPort_Num )
    return PhyStatus_InvalidPort;

  /* Disable Potential Stream */
  if(( status = Phy_StreamDisable( Instance, Stream->Port )) != 0)
      return status;

  /* Enable RF */
  if(( status = Phy_ToRfEnabled( Instance, Stream->Port )) != 0)
      return status;

  phy_task_req_t req;

  /* Create Memory for Stream Request */
  if((req.p = calloc(1, sizeof(phy_stream_req_t))) == NULL)
    return PhyStatus_MemoryError;

  memcpy((void*)req.v, (void*)Stream, sizeof(phy_stream_req_t));

  if((status = PhyTask_Send( Instance, (phy_task_t)PhyStream_Enable, req )) != 0)
  {
    free(req.p);
  }

  return status;
}

phy_status_t Phy_LoadProfile( phy_t *Instance, uint32_t Index )
{
  /* Disable Streams */
  Phy_StreamDisable( Instance, PhyPort_All );

  /* Goto Calibrated State */
  Phy_ToRfCalibrated( Instance, PhyPort_All );

  if( Index >= Instance->ProfileCnt )
    return PhyStatus_InvalidParameter;

  /* Create Request */
  phy_task_req_t req = {.v = Index};

  return PhyTask_Send( Instance, (phy_task_t)PhyProfile_Load, req );
}

phy_status_t Phy_Initialize( phy_t *Instance, phy_cfg_t *Cfg )
{
  int32_t status;

  Instance->ProfileCnt = 1;

  /* Determine Number of Profiles */
  for(int i = 0; i < strlen(Cfg->ProfilePaths); i++)
  {
    if( strcmp(&Cfg->ProfilePaths[i], PHY_PROFILE_NAME_LIST_DELIMITER) == 0 )
    {
      Instance->ProfileCnt++;
    }
  }

  /* Allocate Profile List */
  Instance->ProfileList = calloc(Instance->ProfileCnt, sizeof(phy_profile_t));

  /* Initialize Active Profile from list */
  Instance->Profile = &Instance->ProfileList[0];

  /* Get first Profile Name */
  char *ProfilePath = strtok( Cfg->ProfilePaths, PHY_PROFILE_NAME_LIST_DELIMITER);

  /* Initialize Each Profile */
  while( ProfilePath != NULL )
  {
    /* Copy Profile Path */
    strncpy(Instance->Profile->Path, ProfilePath, PHY_PROFILE_NAME_MAX_LEN );

    /* Initialize Profile */
    if((status = PhyProfile_Initialize( Instance, Instance->Profile )) != 0)
    {
      free(Instance->ProfileList);
      return status;
    }

    /* Get Next Profile Path */
    ProfilePath = strtok( NULL, PHY_PROFILE_NAME_LIST_DELIMITER);

    /* Goto Next Profile */
    Instance->Profile++;
  }

  /* Default to first Profile */
  Instance->Profile = &Instance->ProfileList[0];

  /* Initialize Task */
  if((status = PhyTask_Initialize( Instance )) != PhyStatus_Success)
    return status;

  /* Initialize Stream */
  if((status = PhyStream_Initialize( Instance )) != PhyStatus_Success)
    return status;

  /* Initialize ADRV9001 */
  if((status = PhyAdrv9001_Initialize( Instance )) != 0)
    return status;

  return PhyStatus_Success;
}
