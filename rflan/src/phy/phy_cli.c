#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "adrv9001.h"
#include "phy_cli.h"
#include "app_cli.h"
#include "sd.h"
#include "phy.h"

static uint8_t ProfileBuf[ADRV9001_PROFILE_SIZE];

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
  const char *filename = Cli_FindParameter( cmd, 1, NULL );

  if( SD_ReadBinary( filename, ProfileBuf, 0, ADRV9001_PROFILE_SIZE ) == FR_OK)
  {
    Phy_LoadProfile( ProfileBuf );
  }
  else
  {
    printf("Failed\r\n");
  }
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
static void PhyCli_IqStream(Cli_t *CliInstance, const char *cmd, void *userData)
{
  adrv9001_port_t         port;

  if(PhyCli_ParsePort(cmd, 1, &port) == NULL)
  {
    printf("Invalid Parameter\r\n");
    return;
  }

  const char *filename = Cli_FindParameter( cmd, 2, NULL );

  Adrv9001_ClearError( );

  if(Phy_IqStream(port, filename) == XST_SUCCESS)
  {
    printf("Success\r\n");
  }
  else
  {
    printf("Failed\r\n");
  }

}

static const CliCmd_t PhyCliIqStreamDef =
{
  "IqStream",
  "IqStream:  Stream IQ data to or from a file. \r\n"
  "IqStream < port ( Rx1,Rx2,Tx1,Tx2 ), filename >\r\n\r\n",
  (CliCmdFn_t)PhyCli_IqStream,
  2,
  NULL
};


/*******************************************************************************

  PURPOSE:  Initialize APP CLI

  COMMENT:

*******************************************************************************/
int PhyCli_Initialize( void )
{
  Cli_t *Instance = AppCli_GetInstance( );

  Cli_RegisterCommand(Instance, &PhyCliIqStreamDef);
  Cli_RegisterCommand(Instance, &PhyCliLoadProfileDef);


	return Adrv9001Status_Success;
}
