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
static void PhyCli_UpdateProfile(Cli_t *CliInstance, const char *cmd, void *userData)
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
  if( Phy_UpdateProfile( Buf ) == PhyStatus_Success)
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

static const CliCmd_t PhyCliUpdateProfileDef =
{
  "PhyUpdateProfile",
  "PhyUpdateProfile: Update ADRV9001 Profile \r\n"
  "PhyUpdateProfile < profile ( name of profile on file system) >\r\n\r\n",
  (CliCmdFn_t)PhyCli_UpdateProfile,
  1,
  NULL
};

/*******************************************************************************
*
* \details IQ Stream
*
*******************************************************************************/
#define PHY_STREAM_RX_SAMPLE_CNT          (16384)

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


  if( (Stream.Port == Adrv9001Port_Tx1) || (Stream.Port == Adrv9001Port_Tx2) )
  {
    if( SampleCnt > 0 )
    {
      printf("Invalid Parameter. Sample Count must be -1(indefinite) or 0(file size)\r\n");
      return;
    }

    if(IqFile_Read( filename, &Stream.SampleBuf, &Stream.SampleCnt ) != XST_SUCCESS)
    {
      printf("Invalid Parameter\r\n");
      return;
    }
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
  if(Phy_IqStreamEnable( &Stream ) != PhyStatus_Success)
  {
    printf("Failed\r\n");
  }
  else
  {
    printf("Success\r\n");
  }
}

static void PhyCli_IqFileStreamDisable(Cli_t *CliInstance, const char *cmd, void *userData)
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

static const CliCmd_t PhyCliIqFileStreamEnableDef =
{
  "PhyIqFileStreamEnable",
  "PhyIqFileStreamEnable:  Enable IQ stream to or from a file. \r\n"
  "PhyIqFileStreamEnable < port ( Rx1,Rx2,Tx1,Tx2 ), filename, SampleCnt (-1 = indefinite, 0 = file size, >0 = number of samples ) >\r\n\r\n",
  (CliCmdFn_t)PhyCli_IqFileStreamEnable,
  3,
  NULL
};

static const CliCmd_t PhyCliIqFileStreamDisableDef =
{
  "PhyIqFileStreamDisable",
  "PhyIqFileStreamDisable:  Disable IQ stream. \r\n"
  "PhyIqFileStreamDisable < port ( Rx1,Rx2,Tx1,Tx2 ) >\r\n\r\n",
  (CliCmdFn_t)PhyCli_IqFileStreamDisable,
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

  Cli_RegisterCommand(Instance, &PhyCliIqFileStreamEnableDef);
  Cli_RegisterCommand(Instance, &PhyCliIqFileStreamDisableDef);
  Cli_RegisterCommand(Instance, &PhyCliUpdateProfileDef);


	return PhyStatus_Success;
}
