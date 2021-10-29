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


