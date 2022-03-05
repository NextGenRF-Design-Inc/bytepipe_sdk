/***************************************************************************//**
*  \addtogroup PHY PROFILE
*   @{
*******************************************************************************/
/***************************************************************************//**
*  \file       phy_profile.c
*
*  \details    This file contains the RFLAN PHY layer profile parser.
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
#include <stdint.h>
#include <string.h>
#include "phy_profile.h"
#include "phy_adrv9001.h"
#include "ff.h"
#include "jsmn.h"

#define PHY_PROFILE_TOKEN_MAX_LENGTH 32


phy_status_t PhyProfile_Load( phy_t *Instance, uint32_t Index )
{
  int status;

  if( Index >= Instance->ProfileCnt )
    return PhyStatus_InvalidProfile;

  /* Set New Profile */
  Instance->Profile = &Instance->ProfileList[Index];

  if((status = PhyAdrv9001_LoadProfile( Instance )) != 0)
    return status;

  return PhyStatus_Success;
}

phy_status_t PhyProfile_Initialize( phy_t *Instance, phy_profile_t *Profile )
{
  int16_t numTokens = 0;
  jsmn_parser parser = { 0 };
  jsmntok_t * tokens = NULL;
  char parsingBuffer[PHY_PROFILE_TOKEN_MAX_LENGTH];
  char *JsonBuf = NULL;
  uint32_t JsonBufLength = 0;

  FIL fil;
  int32_t status;

  do
  {
    /* Open File */
    if((status = f_open(&fil, Profile->Path, FA_OPEN_EXISTING | FA_READ)) != FR_OK)
      break;

    JsonBufLength = f_size(&fil);

    /* Allocate Buffer */
    if((JsonBuf = malloc(JsonBufLength)) == NULL)
    {
      status = PhyStatus_MemoryError;
      break;
    }

    /* Pointer to beginning of file */
    if((status = f_lseek(&fil, 0)) != FR_OK)
      break;

    /* Read Entire File */
    if((status = f_read(&fil, JsonBuf, JsonBufLength, (UINT*)&JsonBufLength)) != FR_OK)
      break;

    /* Determine the number of JSON tokens */
    jsmn_init(&parser);
    if((numTokens = jsmn_parse(&parser, JsonBuf, JsonBufLength, NULL, 0)) < 1)
    {
      status = PhyStatus_InvalidParameter;
      break;
    }

    /* allocate space for tokens */
    if((tokens = (jsmntok_t*)calloc(numTokens, sizeof(jsmntok_t))) == NULL)
    {
      status = PhyStatus_MemoryError;
      break;
    }

    /* Parse buffer into the tokens */
    jsmn_init(&parser);
    numTokens = jsmn_parse(&parser, JsonBuf, JsonBufLength, tokens, numTokens);

    /* Make Sure valid tokens */
    if (numTokens < 1 || tokens[0].type != JSMN_OBJECT)
    {
      status = PhyStatus_InvalidParameter;
      break;
    }

    /* Loop over all keys of the root object, searching for matching fields. */
    for (int i = 1; i < numTokens; i++)
    {
        PHY_PROFILE_T(tokens, i, JsonBuf, parsingBuffer, (Profile));
    }

  }while(0);

  free(JsonBuf);
  free(tokens);
  f_close(&fil);

  return status;
}

