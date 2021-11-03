/***************************************************************************//**
*  \addtogroup IQ_FILE
*   @{
*******************************************************************************/
/***************************************************************************//**
*  \file       iq_file.c
*
*  \details    This file contains functions for reading and writing IQ data
*              to a fat file system.
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
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "iq_file.h"
#include "ff.h"
#include "xstatus.h"

#define IQ_FILE_MAX_LINE_SIZE       (64)

static int32_t IqFile_ReadLine( FIL *fil, char *line, int size )
{
  uint32_t len = 1;
  char c;

  while((len > 0) && (size > 0))
  {
    f_read(fil, (void*)&c, 1, &len);

    if(len == 0)
    {
      return XST_FAILURE;
    }
    else
    {
      *line = c;
      line++;
      size--;

      if( c == '\n' )
      {
        *line = 0;
        return XST_SUCCESS;
      }
    }
  }

  return XST_FAILURE;
}

static int32_t IqFile_GetSampleCnt( FIL *fil, uint32_t *SampleCnt )
{
  uint32_t len = 1;
  char c;
  uint32_t cnt = 0;

  if(f_lseek(fil, 0) != FR_OK)
    return XST_FAILURE;

  while( len > 0 )
  {
    f_read(fil, (void*)&c, 1, &len);

    if( c == ',' )
      cnt++;
  }

  *SampleCnt = cnt;

  return XST_SUCCESS;
}

static int32_t IqFile_ReadSample( FIL *fil, uint32_t *Sample )
{
  char line[IQ_FILE_MAX_LINE_SIZE] = {0};

  if(IqFile_ReadLine( fil, line, sizeof(line) ) != XST_SUCCESS)
    return XST_FAILURE;

  int32_t idata;
  int32_t qdata;

  char *iq = strtok(line,",");
  sscanf( iq, "%i", (int32_t*)&idata );

  iq = strtok(NULL,",");
  sscanf( iq, "%i", (int32_t*)&qdata );
  *Sample = (((uint32_t)idata & 0xffff) << 16) | ((uint32_t)qdata & 0xffff);

  return XST_SUCCESS;
}

int32_t IqFile_Read( const char* filename, uint32_t **Buf, uint32_t *Length )
{
  FIL fil;
  int32_t status;
  uint32_t cnt = 0;
  uint32_t *SampleBuf = NULL;
  uint32_t SampleCnt = 0;

  *Length = SampleCnt;
  *Buf = SampleBuf;

  do
  {
    /* Open File */
    if((status = f_open(&fil, filename, FA_OPEN_EXISTING | FA_READ)) != FR_OK) break;

    /* Get Number of Samples */
    if((status = IqFile_GetSampleCnt( &fil, &SampleCnt )) != XST_SUCCESS) break;

    /* Allocate Buffer */
    if((SampleBuf = malloc(SampleCnt * sizeof(uint32_t))) == NULL)
    {
      status = XST_FAILURE;
      break;
    }

    /* Pointer to beginning of file */
    if((status = f_lseek(&fil, 0)) != FR_OK) break;

    while(cnt < SampleCnt)
    {
      if(( status = IqFile_ReadSample( &fil, &SampleBuf[cnt] )) != XST_SUCCESS )
        break;

      cnt++;
    }

  }while(0);

  f_close(&fil);

  if( status != XST_SUCCESS)
  {
    free(SampleBuf);
  }
  else
  {
    *Length = SampleCnt;
    *Buf = SampleBuf;
  }

  return status;
}

int32_t IqFile_Write( const char* filename, uint32_t *Buf, uint32_t Length )
{
  FIL fil;
  UINT len = 0;
  int32_t status;

  do
  {
    /* Open File */
    if((status = f_open(&fil, filename, FA_CREATE_ALWAYS | FA_WRITE)) != FR_OK) break;

    /* Pointer to beginning of file */
    if((status = f_lseek(&fil, 0)) != FR_OK) break;

    char line[IQ_FILE_MAX_LINE_SIZE];
    int16_t idata, qdata;

    /* Write data to file */
    for(int i = 0; i < Length; i++)
    {
      line[0] = 0;
      idata = (int16_t)(Buf[i] >> 16);
      qdata = (int16_t)Buf[i];

      snprintf(line, IQ_FILE_MAX_LINE_SIZE, "%d, %d\r\n", idata, qdata );

      if((status = f_write(&fil, (const void*)line, strlen(line), (UINT*)&len)) != FR_OK) break;

      if(strlen(line) != len)
      {
        status = XST_FAILURE;
        break;
      }
    }
  }while(0);

  f_close(&fil);

  return status;
}


