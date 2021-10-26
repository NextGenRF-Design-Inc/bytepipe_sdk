#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "iq_file.h"
#include "ff.h"
#include "xstatus.h"

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
  char line[256] = {0};

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
  uint32_t *SampleBuf;
  uint32_t SampleCnt = 0;

  /* Open File */
  if(f_open(&fil, filename, FA_OPEN_EXISTING | FA_READ) != FR_OK)
    return XST_FAILURE;

  /* Get Number of Samples */
  if(IqFile_GetSampleCnt( &fil, &SampleCnt ) != XST_SUCCESS)
    return XST_FAILURE;

  /* Allocate Buffer */
  if((SampleBuf = malloc(SampleCnt * sizeof(uint32_t))) == NULL)
    return XST_FAILURE;

  /* Pointer to beginning of file */
  if(f_lseek(&fil, 0) != FR_OK)
    return XST_FAILURE;

  while(cnt < SampleCnt)
  {
    if(( status = IqFile_ReadSample( &fil, &SampleBuf[cnt] )) != XST_SUCCESS )
    {
      free(SampleBuf);
      break;
    }

    cnt++;
  }

  if(f_close(&fil) != FR_OK)
    status = XST_FAILURE;

  *Length = SampleCnt;
  *Buf = SampleBuf;

  return status;
}



