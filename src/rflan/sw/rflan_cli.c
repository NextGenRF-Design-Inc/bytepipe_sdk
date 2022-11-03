/***************************************************************************//**
*  \addtogroup RFLAN_CLI
*   @{
*******************************************************************************/
/***************************************************************************//**
*  \file       rflan_cli.c
*
*  \details    This file contains the RFLAN CLI implementation.
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
#include <stdarg.h>

#include "xparameters.h"
#include "xil_types.h"
#include "xstatus.h"
#include "ff.h"
#include "cli.h"
#include "mem.h"
#include "rflan_cli.h"
#include "rflan.h"
#include "rflan_pib.h"
#include "rflan_gpio.h"
#include "rflan_stream.h"
#include "ff.h"
#include "status.h"

#define BOOT_BUF_SIZE   16384

#define PHY_CLI_IQ_FILE_MAX_LINE_SIZE       (64)

static int32_t RflanCli_IqFileReadLine( FIL *fil, char *line, int size )
{
  UINT len = 1;
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

static int32_t RflanCli_IqFileReadSample( FIL *fil, uint32_t *Sample )
{
  char line[PHY_CLI_IQ_FILE_MAX_LINE_SIZE] = {0};

  if(RflanCli_IqFileReadLine( fil, line, sizeof(line) ) != XST_SUCCESS)
    return XST_FAILURE;

  int32_t idata;
  int32_t qdata;

  char *iq = strtok(line,",");
  sscanf( iq, "%li", (int32_t*)&idata );

  iq = strtok(NULL,",");
  sscanf( iq, "%li", (int32_t*)&qdata );
  *Sample = (((uint32_t)idata & 0xffff) << 16) | ((uint32_t)qdata & 0xffff);

  return XST_SUCCESS;
}

static int32_t RflanCli_IqFileGetSampleCnt( FIL *fil, uint32_t *SampleCnt )
{
  UINT len = 1;
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

static int32_t RflanCli_IqFileRead( const char* filename, uint32_t Addr )
{
  FIL fil;
  int32_t status;
  uint32_t cnt = 0;
  uint32_t sample;
  uint32_t SampleCnt = 0;

  do
  {
    /* Open File */
    if((status = f_open(&fil, filename, FA_OPEN_EXISTING | FA_READ)) != FR_OK) break;

    /* Get Number of Samples */
    if((status = RflanCli_IqFileGetSampleCnt( &fil, &SampleCnt )) != XST_SUCCESS) break;

    /* Pointer to beginning of file */
    if((status = f_lseek(&fil, 0)) != FR_OK) break;

    while(cnt < SampleCnt)
    {
      if(( status = RflanCli_IqFileReadSample( &fil, &sample )) != XST_SUCCESS )
        break;

      Xil_Out32(Addr + (cnt << 2), sample);

      cnt++;
    }

  }while(0);

  f_close(&fil);
  return status;
}

static int32_t RflanCli_IqFileWrite( const char* filename, uint32_t Addr, uint32_t Length )
{
  FIL fil;
  UINT len = 0;
  int32_t status;
  uint32_t sample;

  do
  {
    /* Open File */
    if((status = f_open(&fil, filename, FA_CREATE_ALWAYS | FA_WRITE)) != FR_OK) break;

    /* Pointer to beginning of file */
    if((status = f_lseek(&fil, 0)) != FR_OK) break;

    char line[PHY_CLI_IQ_FILE_MAX_LINE_SIZE];
    int16_t idata, qdata;

    /* Write data to file */
    for(int i = 0; i < Length; i++)
    {
      sample = Xil_In32(Addr + (i << 2));

      line[0] = 0;
      idata = (int16_t)(sample >> 16);
      qdata = (int16_t)sample;

      snprintf(line, PHY_CLI_IQ_FILE_MAX_LINE_SIZE, "%d, %d\r\n", idata, qdata );

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

static void RflanCli_Reboot(cli_t *CliInstance, const char *cmd, void *userData)
{
  if( Cli_GetParameterCount( cmd ) > 0 )
  {
	  char *filename;

	  if((filename = calloc(1, FF_FILENAME_MAX_LEN )) == NULL)
    {
      Cli_Printf(CliInstance,"Memory Error\r\n");
      return;
    }

	  strcpy(filename,FF_LOGICAL_DRIVE_PATH);

	  Cli_GetParameter(cmd, 1, CliParamTypeStr, &filename[strlen(filename)]);

	  FIL fil1;
    FIL fil2;
    UINT len;
    void *Buf = malloc(BOOT_BUF_SIZE);

    do
    {
      if( f_unlink("1:/boot.bin") != FR_OK)
      {
        if( f_unlink("1:/BOOT.BIN") != FR_OK)
        {
          Cli_Printf(CliInstance,"Failed to delete boot.bin\r\n");
          break;
        }
      }

      Cli_Printf(CliInstance,"Rebooting...");

      /* Open File */
      if(f_open(&fil1, filename, FA_OPEN_EXISTING | FA_READ) != FR_OK)
        break;

      /* Get Size of file */
      uint32_t size = f_size(&fil1);

      /* Pointer to beginning of file */
      if(f_lseek(&fil1, 0) != FR_OK)
        break;

      /* Open File */
      if(f_open(&fil2, "1:/boot.bin", FA_CREATE_ALWAYS | FA_WRITE) != FR_OK)
        break;

      /* Pointer to beginning of file */
      if(f_lseek(&fil2, 0) != FR_OK)
        break;

      uint32_t head = 0;
      while( head < size )
      {

        if(( head + BOOT_BUF_SIZE ) > size)
          len = size - head;
        else
          len = BOOT_BUF_SIZE;

        if(f_read(&fil1, Buf, len, (UINT*)&len) != FR_OK)
          break;

        head += len;

        if(f_write(&fil2, Buf, len, (UINT*)&len) != FR_OK)
          break;
      }

    }while(0);

    f_sync(&fil2);

    f_close(&fil1);
    f_close(&fil2);

    free(filename);
    free(Buf);
  }


  Rflan_Reboot();
}

static void RflanCli_CliLs(cli_t *CliInstance, const char *cmd, void *userData)
{
  int status;
  DIR dp;

  FILINFO fno;

  if( Cli_GetParameterCount( cmd ) > 0 )
  {
    char *Path = NULL;

    if((Path = calloc(1, FF_FILENAME_MAX_LEN )) == NULL)
    {
      Cli_Printf(CliInstance,"Memory Error\r\n");
      return;
    }

    Cli_GetParameter(cmd, 1, CliParamTypeStr, Path);

    if((status = f_opendir(&dp, Path)) != XST_SUCCESS)
    {
      free(Path);
      Cli_Printf(CliInstance,"Failure\r\n");
      return;
    }
  }
  else
  {
    if((status = f_opendir(&dp, FF_LOGICAL_DRIVE_PATH)) != XST_SUCCESS)
    {
      Cli_Printf(CliInstance,"Failure\r\n");
      return;
    }
  }

  for (;;)
  {
    status = f_readdir(&dp, &fno);

    if ((status != FR_OK ) || (fno.fname[0] == 0))
      break;

    Cli_Printf(CliInstance,"%s (%ld bytes)\r\n", fno.fname, fno.fsize);
  }

  Cli_Printf(CliInstance,"\r\n\r\n");

  f_closedir(&dp);
}

static void RflanCli_TaskInfo(cli_t *CliInstance, const char *cmd, void *userData)
{
  Cli_Printf(CliInstance, "Name            Stack   Usage\r\n");
  Cli_Printf(CliInstance, "-----------------------------\r\n");


  uint32_t TotalTime;
  uint32_t Percentage;

  uint32_t TaskCnt = uxTaskGetNumberOfTasks();
  TaskStatus_t *TaskStatus = pvPortMalloc( TaskCnt * sizeof( TaskStatus_t ) );

  if( TaskStatus != NULL )
  {
    /* Generate the (binary) data. */
    TaskCnt = uxTaskGetSystemState( TaskStatus, TaskCnt, &TotalTime );

    /* For percentage calculations. */
    TotalTime /= 100UL;

    /* Avoid divide by zero errors. */

    /* Create a human readable table from the binary data. */
    for( int x = 0; x < TaskCnt; x++ )
    {
      if( TotalTime > 0UL )
      {
        Percentage = TaskStatus[x].ulRunTimeCounter / TotalTime;
      }
      else
      {
        Percentage = 0;
      }

      Cli_Printf(CliInstance,"%s", TaskStatus[x].pcTaskName);

      /* Pad the end of the string with spaces to ensure columns line up when printed out. */
      for(int i = strlen( TaskStatus[x].pcTaskName ); i < ( configMAX_TASK_NAME_LEN - 1 ); i++ )
      {
        Cli_Printf(CliInstance,"%s"," ");
      }

      Cli_Printf(CliInstance,"\t%lu\t%u%%\r\n", TaskStatus[x].usStackHighWaterMark*4, (uint16_t) Percentage );
    }

    vPortFree( TaskStatus );
  }

  size_t remainingHeap = xPortGetFreeHeapSize( );

  size_t minHeap = xPortGetMinimumEverFreeHeapSize( );

  Cli_Printf(CliInstance,"Remaining Heap = %d bytes\r\n", remainingHeap);
  Cli_Printf(CliInstance,"Minimum Heap = %d bytes\r\n", minHeap);

}

static void RflanCli_ListParamNames(cli_t *CliInstance, const char *cmd, rflan_pib_t *RflanPib)
{
  Cli_Printf(CliInstance,"RFLAN Parameter Names:\r\n");

  if( Cli_GetParameterCount( cmd ) > 0 )
  {
    char *key;

    if((key = calloc(1, PIB_NAME_SIZE )) == NULL)
    {
      Cli_Printf(CliInstance,"Memory Error\r\n");
      return;
    }

    Cli_GetParameter(cmd, 1, CliParamTypeStr, key);

    for(int i = 0; i < RflanPib->Pib.PibLen; i++)
    {
      if(strstr( RflanPib->Pib.Def[i].name, key ))
        Cli_Printf(CliInstance,"  - %s\r\n", RflanPib->Pib.Def[i].name);
    }
  }
  else
  {
    for(int i = 0; i < RflanPib->Pib.PibLen; i++)
      Cli_Printf(CliInstance,"  - %s\r\n", RflanPib->Pib.Def[i].name);
  }
}

static void RflanCli_SetRflanParam(cli_t *CliInstance, const char *cmd, rflan_pib_t *RflanPib)
{
  char *name;
  char *value;

  if((name = calloc(1, PIB_NAME_SIZE )) == NULL)
  {
    Cli_Printf(CliInstance,"Memory Error\r\n");
    return;
  }

  if((value = calloc(1, 1024 )) == NULL)
  {
    free(name);
    Cli_Printf(CliInstance,"Memory Error\r\n");
    return;
  }

  /* Get Name */
  Cli_GetParameter(cmd, 1, CliParamTypeStr, name);

  /* Get Value */
  Cli_GetParameter(cmd, 2, CliParamTypeStr, value);

  /* Set PIB parameter */
  int32_t status = RflanPib_SetbyNameByString( RflanPib, name, value );

  Cli_Printf(CliInstance,"SetParam %s\r\n",StatusString(status));

  free(name);
  free(value);
}

static void RflanCli_GetRflanParam(cli_t *CliInstance, const char *cmd, rflan_pib_t *RflanPib)
{
  char *name;
  char *value;
  int status;

  if((name = calloc(1, PIB_NAME_SIZE )) == NULL)
  {
    Cli_Printf(CliInstance,"Memory Error\r\n");
    return;
  }

  if((value = calloc(1, 1024 )) == NULL)
  {
    free(name);
    Cli_Printf(CliInstance,"Memory Error\r\n");
    return;
  }

  /* Get Name */
  Cli_GetParameter(cmd, 1, CliParamTypeStr, name);

  /* Get Value */
  if((status = RflanPib_GetStringByName( RflanPib, name, value )) != 0)
  {
    Cli_Printf(CliInstance,"GetParam Error - %s\r\n",StatusString(status));
  }
  else
  {
    Cli_Printf(CliInstance,"%s = %s\r\n",name, value);
  }

  free(name);
  free(value);
}

static void RflanCli_StreamBufGet(cli_t *CliInstance, const char *cmd, rflan_stream_t *Stream)
{
  uint32_t SampleOffset;
  uint16_t len;
  uint32_t BufAddr;
  uint32_t sample;

  const char *str = Cli_FindParameter( cmd, 1, &len );

  if(strncmp(str, "Tx1", 3) == 0 )
  {
    BufAddr = RFLAN_DMA_TX1_BUF_ADDR;
  }
  else if(strncmp(str, "Tx2", 3) == 0 )
  {
    BufAddr = RFLAN_DMA_TX2_BUF_ADDR;
  }
  else if(strncmp(str, "Rx1", 3) == 0 )
  {
    BufAddr = RFLAN_DMA_RX1_BUF_ADDR;
  }
  else if(strncmp(str, "Rx2", 3) == 0 )
  {
    BufAddr = RFLAN_DMA_RX2_BUF_ADDR;
  }
  else
  {
    Cli_Printf(CliInstance,"Invalid Parameter\r\n");
    return;
  }

  Cli_GetParameter(cmd, 2, CliParamTypeU32, &SampleOffset);
  Cli_GetParameter(cmd, 3, CliParamTypeU16, &len);


  BufAddr += (SampleOffset << 2);

  Cli_Printf(CliInstance,"%c%c%c Stream Buffer: ", str[0],str[1],str[2]);

  for( int i = 0; i < len-1; i++ )
  {
    sample = Xil_In32( BufAddr );

    BufAddr += 4;

    Cli_Printf(CliInstance,"%d,%d,",(int16_t)(sample >> 16), (int16_t)(sample));
  }

  sample = Xil_In32(BufAddr);
  Cli_Printf(CliInstance,"%d,%d\r\n",(int16_t)(sample >> 16), (int16_t)(sample));

}

static void RflanCli_StreamBufPut(cli_t *CliInstance, const char *cmd, rflan_stream_t *Stream)
{
  uint16_t len;
  uint32_t SampleOffset;
  uint32_t BufAddr;

  char *str = (char*)Cli_FindParameter( cmd, 1, &len );

  if(strncmp(str, "Tx1", 3) == 0 )
  {
    BufAddr = RFLAN_DMA_TX1_BUF_ADDR;
  }
  else if(strncmp(str, "Tx2", 3) == 0 )
  {
    BufAddr = RFLAN_DMA_TX2_BUF_ADDR;
  }
  else if(strncmp(str, "Rx1", 3) == 0 )
  {
    BufAddr = RFLAN_DMA_RX1_BUF_ADDR;
  }
  else if(strncmp(str, "Rx2", 3) == 0 )
  {
    BufAddr = RFLAN_DMA_RX2_BUF_ADDR;
  }
  else
  {
    Cli_Printf(CliInstance,"Invalid Parameter\r\n");
    return;
  }

  Cli_GetParameter(cmd, 2, CliParamTypeU32, &SampleOffset);
  BufAddr += SampleOffset * 4;

  str = (char*)Cli_FindParameter( cmd, 3, &len );

  int32_t idata;
  int32_t qdata;

  char *s = strtok(str,",");
  sscanf( s, "%li", (int32_t*)&idata );
  s = strtok(NULL,",");
  sscanf( s, "%li", (int32_t*)&qdata );
  Xil_Out32(BufAddr, (((uint32_t)idata & 0xffff) << 16) | ((uint32_t)qdata & 0xffff));
  BufAddr += 4;

  while( s != NULL )
  {
    if((s = strtok(NULL,",")) != NULL)
    {
      sscanf( s, "%li", (int32_t*)&idata );
      if((s = strtok(NULL,",")) != NULL)
      {
        sscanf( s, "%li", (int32_t*)&qdata );
        Xil_Out32(BufAddr, (((uint32_t)idata & 0xffff) << 16) | ((uint32_t)qdata & 0xffff));
        BufAddr += 4;
      }
    }
  }

  Cli_Printf(CliInstance,"Success\r\n");
}

static void RflanCli_StreamStart(cli_t *CliInstance, const char *cmd, rflan_stream_t *Stream)
{
  uint16_t len;
  int32_t status;
  int32_t SampleCnt;
  uint8_t Cyclic;

  const char *str = Cli_FindParameter( cmd, 1, &len );

  Cli_GetParameter(cmd, 2, CliParamTypeU8, &Cyclic);
  Cli_GetParameter(cmd, 3, CliParamTypeS32, &SampleCnt);

  if(strncmp(str, "Tx1", 3) == 0 )
  {
    status = RflanStream_StartTransfer( Stream, RFLAN_DMA_TX1_BUF_ADDR, SampleCnt, RflanStreamChannel_Tx1, Cyclic );
  }
  else if(strncmp(str, "Tx2", 3) == 0 )
  {
    status = RflanStream_StartTransfer( Stream, RFLAN_DMA_TX2_BUF_ADDR, SampleCnt, RflanStreamChannel_Tx2, Cyclic );
  }
  else if(strncmp(str, "Rx1", 3) == 0 )
  {
    status = RflanStream_StartTransfer( Stream, RFLAN_DMA_RX1_BUF_ADDR, SampleCnt, RflanStreamChannel_Rx1, Cyclic );
  }
  else if(strncmp(str, "Rx2", 3) == 0 )
  {
    status = RflanStream_StartTransfer( Stream, RFLAN_DMA_RX2_BUF_ADDR, SampleCnt, RflanStreamChannel_Rx2, Cyclic );
  }
  else
  {
    Cli_Printf(CliInstance,"Invalid Parameter\r\n");
    return;
  }

  Cli_Printf(CliInstance,"Stream Start %s\r\n",StatusString(status));
}

static void RflanCli_StreamBufLoad(cli_t *CliInstance, const char *cmd, rflan_stream_t *Stream)
{
  uint16_t len;
  uint32_t BufAddr;

  const char *str = Cli_FindParameter( cmd, 1, &len );

  if(strncmp(str, "Tx1", 3) == 0 )
  {
    BufAddr = RFLAN_DMA_TX1_BUF_ADDR;
  }
  else if(strncmp(str, "Tx2", 3) == 0 )
  {
    BufAddr = RFLAN_DMA_TX2_BUF_ADDR;
  }
  else if(strncmp(str, "Rx1", 3) == 0 )
  {
    BufAddr = RFLAN_DMA_RX1_BUF_ADDR;
  }
  else if(strncmp(str, "Rx2", 3) == 0 )
  {
    BufAddr = RFLAN_DMA_RX2_BUF_ADDR;
  }
  else
  {
    Cli_Printf(CliInstance,"Invalid Parameter\r\n");
    return;
  }

  char *filename = calloc(1, 256);
  strcpy(filename, FF_LOGICAL_DRIVE_PATH);
  Cli_GetParameter(cmd, 2, CliParamTypeStr, &filename[strlen(filename)]);

  RflanCli_IqFileRead( filename, BufAddr );

  free(filename);

  Cli_Printf(CliInstance,"Success\r\n");
}

static void RflanCli_StreamBufSave(cli_t *CliInstance, const char *cmd, rflan_stream_t *Stream)
{
  uint16_t len;
  uint32_t BufAddr;
  uint32_t SampleCnt;

  const char *str = Cli_FindParameter( cmd, 1, &len );

  if(strncmp(str, "Tx1", 3) == 0 )
  {
    BufAddr = RFLAN_DMA_TX1_BUF_ADDR;
  }
  else if(strncmp(str, "Tx2", 3) == 0 )
  {
    BufAddr = RFLAN_DMA_TX2_BUF_ADDR;
  }
  else if(strncmp(str, "Rx1", 3) == 0 )
  {
    BufAddr = RFLAN_DMA_RX1_BUF_ADDR;
  }
  else if(strncmp(str, "Rx2", 3) == 0 )
  {
    BufAddr = RFLAN_DMA_RX2_BUF_ADDR;
  }
  else
  {
    Cli_Printf(CliInstance,"Invalid Parameter\r\n");
    return;
  }

  Cli_GetParameter(cmd, 2, CliParamTypeU32, &SampleCnt);

  char *filename = calloc(1, 256);
  strcpy(filename, FF_LOGICAL_DRIVE_PATH);
  Cli_GetParameter(cmd, 3, CliParamTypeStr, &filename[strlen(filename)]);

  RflanCli_IqFileWrite( filename, BufAddr, SampleCnt );

  free(filename);

  Cli_Printf(CliInstance,"Success\r\n");
}

static void RflanCli_StreamStop(cli_t *CliInstance, const char *cmd, rflan_stream_t *Stream)
{
  uint16_t len;
  int32_t status;

  const char *str = Cli_FindParameter( cmd, 1, &len );

  if(strncmp(str, "Tx1", 3) == 0 )
  {
    status = RflanStream_StopTransfer( Stream, RflanStreamChannel_Tx1 );
  }
  else if(strncmp(str, "Tx2", 3) == 0 )
  {
    status = RflanStream_StopTransfer( Stream, RflanStreamChannel_Tx2 );
  }
  else if(strncmp(str, "Rx1", 3) == 0 )
  {
    status = RflanStream_StopTransfer( Stream, RflanStreamChannel_Rx1 );
  }
  else if(strncmp(str, "Rx2", 3) == 0 )
  {
    status = RflanStream_StopTransfer( Stream, RflanStreamChannel_Rx2 );
  }
  else
  {
    Cli_Printf(CliInstance,"Invalid Parameter\r\n");
    return;
  }

  Cli_Printf(CliInstance,"Stream Stop %s\r\n",StatusString(status));
}

static void RflanCli_ExecuteScript(cli_t *CliInstance, const char *cmd, void *ref)
{
  int32_t status;

  char *filename = calloc(1, 256);
  strcpy(filename, FF_LOGICAL_DRIVE_PATH);
  Cli_GetParameter(cmd, 1, CliParamTypeStr, &filename[strlen(filename)]);
  
  Cli_Printf(CliInstance,"Executing CLI Script %s\r\n", filename);
  
  FIL fil;
  UINT len = 1;
  char c;

  do
  {
    /* Open File */
    if((status = f_open(&fil, filename, FA_OPEN_EXISTING | FA_READ)) != 0)
      break;

    if((status = f_lseek(&fil, 0)) != 0)
      break;

    while( len > 0 )
    {
      if((status = f_read(&fil, (void*)&c, 1, &len)) != 0)
        break;

      if( len > 0 )
        Cli_ProcessRxChar( CliInstance, c );
    }

  }while(0);

  f_close(&fil);

  Cli_Printf(CliInstance,"CLI Script %s\r\n", StatusString(status));
}

static void RflanCli_DelayMs(cli_t *CliInstance, const char *cmd, void *ref)
{
  uint32_t DelayMs;

  Cli_GetParameter(cmd, 1, CliParamTypeU32, &DelayMs);
  
  DelayMs = DelayMs / portTICK_PERIOD_MS;

  vTaskDelay( DelayMs );
}

static cli_cmd_t RflanCliDelayMsDef =
{
  "DelayMs",
  "DelayMs: Delay in milliseconds \r\n"
  "DelayMs < delay >\r\n\n",
  (CliCmdFn_t)RflanCli_DelayMs,
  1,
  NULL
};

static cli_cmd_t RflanCliExecuteScriptDef =
{
  "ExecuteScript",
  "ExecuteScript: Execute CLI script from file system \r\n"
  "ExecuteScript < filename >\r\n\n",
  (CliCmdFn_t)RflanCli_ExecuteScript,
  1,
  NULL
};

static cli_cmd_t RflanCliStreamStopDef =
{
  "RflanStreamStop",
  "RflanStreamStop: Disable Stream \r\n"
  "RflanStreamStop < Channel (Tx1, Tx2, Rx1, Rx2) >\r\n\n",
  (CliCmdFn_t)RflanCli_StreamStop,
  1,
  NULL
};

static cli_cmd_t RflanCliStreamBufSaveDef =
{
  "RflanStreamBufSave",
  "RflanStreamBufSave: Write stream buffer to file \r\n"
  "RflanStreamBufSave < Channel (Tx1, Tx2, Rx1, Rx2), SampleCnt, Filename >\r\n\n",
  (CliCmdFn_t)RflanCli_StreamBufSave,
  3,
  NULL
};

static cli_cmd_t RflanCliStreamBufLoadDef =
{
  "RflanStreamBufLoad",
  "RflanStreamBufLoad: Load stream buffer from file \r\n"
  "RflanStreamBufLoad < Channel (Tx1, Tx2, Rx1, Rx2), Filename >\r\n\n",
  (CliCmdFn_t)RflanCli_StreamBufLoad,
  2,
  NULL
};

static cli_cmd_t RflanCliStreamBufGetDef =
{
  "RflanStreamBufGet",
  "RflanStreamBufGet: Get data from stream buffer \r\n"
  "RflanStreamBufGet < Channel (Tx1, Tx2, Rx1, Rx2), SampleOffset, SampleCnt >\r\n\n",
  (CliCmdFn_t)RflanCli_StreamBufGet,
  3,
  NULL
};

static cli_cmd_t RflanCliStreamBufPutDef =
{
  "RflanStreamBufPut",
  "RflanStreamBufPut: Put data into stream buffer \r\n"
  "RflanStreamBufPut < Channel (Tx1, Tx2, Rx1, Rx2), SampleOffset, Data( I0, Q0, I1, Q1, .... ) >\r\n\n",
  (CliCmdFn_t)RflanCli_StreamBufPut,
  3,
  NULL
};

static cli_cmd_t RflanCliStreamStartDef =
{
  "RflanStreamStart",
  "RflanStreamStart: Start streaming data from programmable logic to memory \r\n"
  "RflanStreamStart < Channel (Tx1, Tx2, Rx1, Rx2), Cyclic (0 or 1), SampleCnt >\r\n\n",
  (CliCmdFn_t)RflanCli_StreamStart,
  3,
  NULL
};

static cli_cmd_t RflanCliListParamNamesDef =
{
  "RflanListParams",
  "RflanListParams: List RFLAN Parameter Names \r\n"
  "RflanListParams <  >\r\n\n",
  (CliCmdFn_t)RflanCli_ListParamNames,
  0,
  NULL
};

static cli_cmd_t RflanCliSetParamDef =
{
  "RflanSetParam",
  "RflanSetParam: Set RFLAN Parameter \r\n"
  "RflanSetParam < name (see ListRflanParamNames), value >\r\n\n",
  (CliCmdFn_t)RflanCli_SetRflanParam,
  2,
  NULL
};

static cli_cmd_t RflanCliGetPibDef =
{
  "RflanGetParam",
  "RflanGetParam: Get RFLAN Parameter\r\n"
  "RflanGetParam < name (see ListRflanParamNames) >\r\n\n",
  (CliCmdFn_t)RflanCli_GetRflanParam,
  1,
  NULL
};

static cli_cmd_t RflanCliRebootDef =
{
  "Reboot",
  "Reboot: Reboot processor with optional .bin file from file system. \r\n"
  "Reboot < filename (optional) >\r\n\n",
  (CliCmdFn_t)RflanCli_Reboot,
  -1,
  NULL
};

static cli_cmd_t RflanCliLsDef =
{
  "ls",
  "ls: List files \r\n"
  "ls < path (optional) >\r\n\n",
  (CliCmdFn_t)RflanCli_CliLs,
  -1,
  NULL
};

static cli_cmd_t RflanCliTaskInfoDef =
{
  "TaskInfo",
  "TaskInfo: Get Task Info \r\n"
  "TaskInfo < >\r\n\n",
  (CliCmdFn_t)RflanCli_TaskInfo,
  0,
  NULL
};

cli_status_t RflanCli_Initialize( cli_t *Cli, rflan_pib_t *Pib, XGpioPs *Gpio, rflan_stream_t *Stream )
{
  RflanCliListParamNamesDef.userData = Pib;
  RflanCliSetParamDef.userData = Pib;
  RflanCliGetPibDef.userData = Pib;
  RflanCliStreamBufSaveDef.userData = Stream;
  RflanCliStreamBufLoadDef.userData = Stream;
  RflanCliStreamBufGetDef.userData = Stream;
  RflanCliStreamBufPutDef.userData = Stream;
  RflanCliStreamStartDef.userData = Stream;
  RflanCliStreamStopDef.userData = Stream;

  Cli_RegisterCommand(Cli, &RflanCliLsDef);
  Cli_RegisterCommand(Cli, &RflanCliTaskInfoDef);
  Cli_RegisterCommand(Cli, &RflanCliRebootDef);
  Cli_RegisterCommand(Cli, &RflanCliListParamNamesDef);
  Cli_RegisterCommand(Cli, &RflanCliSetParamDef);
  Cli_RegisterCommand(Cli, &RflanCliGetPibDef);
  Cli_RegisterCommand(Cli, &RflanCliStreamBufLoadDef);
  Cli_RegisterCommand(Cli, &RflanCliStreamBufGetDef);
  Cli_RegisterCommand(Cli, &RflanCliStreamBufPutDef);
  Cli_RegisterCommand(Cli, &RflanCliStreamStartDef);
  Cli_RegisterCommand(Cli, &RflanCliStreamStopDef);
  Cli_RegisterCommand(Cli, &RflanCliExecuteScriptDef);
  Cli_RegisterCommand(Cli, &RflanCliDelayMsDef);

  return XST_SUCCESS;
}
