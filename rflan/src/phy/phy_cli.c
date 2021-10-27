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


/*******************************************************************************
*
* \details LoadProfile
*
*******************************************************************************/
static void PhyCli_LoadProfile(Cli_t *CliInstance, const char *cmd, void *userData)
{
  char *filename = calloc(1, FF_FILENAME_MAX_LEN );
  strcpy(filename,FF_LOGICAL_DRIVE_PATH);
  Cli_GetParameter(cmd, 1, CliParamTypeStr, &filename[strlen(filename)]);


  FIL fil;
  profile_t *Buf = NULL;

  /* Open File */
  if(f_open(&fil, filename, FA_OPEN_EXISTING | FA_READ) != FR_OK)
  {
    printf("Invalid Filename\r\n");
    return;
  }

  /* Get Size */
  uint32_t length = f_size(&fil);

  /* Check Size */
  if( length > ADRV9001_PROFILE_SIZE )
  {
    printf("Invalid File Size\r\n");
    f_close( &fil );
    return;
  }

  /* Allocate Memory */
  if((Buf = malloc( ADRV9001_PROFILE_SIZE )) == NULL)
  {
    printf("Insufficient Memory\r\n");
    f_close( &fil );
    return;
  }

  /* Set Pointer to beginning of file */
  f_lseek(&fil, 0);

  /* Read data from file */
  if(f_read(&fil, Buf, length, &length) != FR_OK)
  {
    printf("Read File Error\r\n");
    free( Buf );
    f_close( &fil );
    return;
  }

  /* Close File */
  f_close( &fil );

  /* Load Profile */
  if( Phy_LoadProfile( Buf ) == PhyStatus_Success)
  {
    printf("Success\r\n");
  }
  else
  {
    printf("Failed\r\n");
  }

  /* Free Memory */
  free( Buf );

}

static const CliCmd_t PhyCliLoadProfileDef =
{
  "LoadProfile",
  "LoadProfile: Load ADRV9001 Profile \r\n"
  "LoadProfile < profile ( name of profile on file system) >\r\n\r\n",
  (CliCmdFn_t)PhyCli_LoadProfile,
  1,
  NULL
};

/*******************************************************************************
*
* \details IQ Stream
*
*******************************************************************************/
#define PHY_STREAM_RX_SAMPLE_CNT          (16384)

static void PhyCli_PhyCallback( phy_evt_type_t EvtType, void *EvtData, void *param )
{
  if( EvtType == PhyEvtType_StreamStart )
  {
    phy_stream_t *Stream = (phy_stream_t*)EvtData;

    printf("%s stream has started\r\n", (Stream->Port == Adrv9001Port_Tx1)? "Tx1" :
                                        (Stream->Port == Adrv9001Port_Tx2)? "Tx2" :
                                        (Stream->Port == Adrv9001Port_Rx1)? "Rx1" :
                                        (Stream->Port == Adrv9001Port_Rx2)? "Rx2" : "Unknown");
  }
  else if( EvtType == PhyEvtType_StreamStop )
  {
    phy_stream_t *Stream = (phy_stream_t*)EvtData;

    printf("%s stream has stopped\r\n", (Stream->Port == Adrv9001Port_Tx1)? "Tx1" :
                                        (Stream->Port == Adrv9001Port_Tx2)? "Tx2" :
                                        (Stream->Port == Adrv9001Port_Rx1)? "Rx1" :
                                        (Stream->Port == Adrv9001Port_Rx2)? "Rx2" : "Unknown");
  }
  else if( EvtType == PhyEvtType_StreamError )
  {
    phy_stream_t *Stream = (phy_stream_t*)EvtData;

    printf("%s stream error\r\n", (Stream->Port == Adrv9001Port_Tx1)? "Tx1" :
                                  (Stream->Port == Adrv9001Port_Tx2)? "Tx2" :
                                  (Stream->Port == Adrv9001Port_Rx1)? "Rx1" :
                                  (Stream->Port == Adrv9001Port_Rx2)? "Rx2" : "Unknown");
  }
  else if( EvtType == PhyEvtType_StreamData )
  {
//    phy_stream_t *Stream = (phy_stream_t*)EvtData;


  }
  else if( EvtType == PhyEvtType_ProfileUpdated )
  {
    printf("Profile Update Success\r\n");
  }
  else
  {
    printf("Unknown PHY Event Callback\r\n");
  }
}

static void PhyCli_IqStreamEnable(Cli_t *CliInstance, const char *cmd, void *userData)
{
  phy_stream_t Stream = {.Callback = PhyCli_PhyCallback, .Cyclic = true};

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

  Adrv9001_ClearError( );

  if( (Stream.Port == Adrv9001Port_Tx1) || (Stream.Port == Adrv9001Port_Tx2) )
  {
    if(IqFile_Read( filename, &Stream.SampleBuf, &Stream.SampleCnt ) != XST_SUCCESS)
    {
      printf("Invalid Parameter\r\n");
      return;
    }

    /* Enable Streaming */
    if(Phy_IqStreamEnable( &Stream ) != PhyStatus_Success)
    {
      printf("Failed\r\n");
      return;
    }
  }
  else if( (Stream.Port == Adrv9001Port_Rx1) || (Stream.Port == Adrv9001Port_Rx2) )
  {
    printf("Invalid Parameter\r\n");
  }

  printf("Success\r\n");
}

static void PhyCli_IqStreamDisable(Cli_t *CliInstance, const char *cmd, void *userData)
{
  adrv9001_port_t     port;

  /* Parse Port */
  if(PhyCli_ParsePort(cmd, 1, &port) == NULL)
  {
    printf("Invalid Parameter\r\n");
    return;
  }

  /* Disable Stream */
  if( Phy_IqStreamDisable( port ) != PhyStatus_Success )
  {
    printf("Failed\r\n");
  }
  else
  {
    printf("Success\r\n");
  }
}

static const CliCmd_t PhyCliIqStreamEnableDef =
{
  "PhyIqStreamEnable",
  "PhyIqStreamEnable:  Enable IQ stream or from a file. \r\n"
  "PhyIqStreamEnable < port ( Rx1,Rx2,Tx1,Tx2 ), filename >\r\n\r\n",
  (CliCmdFn_t)PhyCli_IqStreamEnable,
  2,
  NULL
};

static const CliCmd_t PhyCliIqStreamDisableDef =
{
  "PhyIqStreamDisable",
  "PhyIqStreamDisable:  Disable IQ stream. \r\n"
  "PhyIqStreamDisable < port ( Rx1,Rx2,Tx1,Tx2 ) >\r\n\r\n",
  (CliCmdFn_t)PhyCli_IqStreamDisable,
  1,
  NULL
};


/*******************************************************************************

  PURPOSE:  Initialize APP CLI

  COMMENT:

*******************************************************************************/
int PhyCli_Initialize( void )
{
  Cli_t *Instance = AppCli_GetInstance( );

  Cli_RegisterCommand(Instance, &PhyCliIqStreamEnableDef);
  Cli_RegisterCommand(Instance, &PhyCliIqStreamDisableDef);
  Cli_RegisterCommand(Instance, &PhyCliLoadProfileDef);


	return PhyStatus_Success;
}
