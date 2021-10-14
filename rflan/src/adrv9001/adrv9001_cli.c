#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "adrv9001.h"
#include "app_cli.h"

static const char* Adrv9001Cli_ParsePort(const char *cmd, uint16_t pNum, adrv9001_port_t *port)
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

static void Adrv9001Cli_SetRadioState(Cli_t *CliInstance, const char *cmd, void *userData)
{
  adrv9001_port_t     port;
  uint32_t            state;

  if(Adrv9001Cli_ParsePort(cmd, 1, &port) == NULL)
  {
    printf("Invalid Parameter\r\n");
    return;
  }

  Cli_GetParameter(cmd, 2, CliParamTypeU32, &state);

	Adrv9001_ClearError( );

	if(Adrv9001_SetRadioState(port, state) == Adrv9001Status_Success)
  {
    printf("Success\r\n");
  }
  else
  {
    printf("Failed\r\n");
  }
}

/**
*  Set State
*/
static const CliCmd_t Adrv9001CliSetRadioStateDef =
{
  "SetRadioState",
  "SetRadioState: Set radio state \r\n"
  "SetRadioState < port ( Rx1,Rx2,Tx1,Tx2), state (0=standby, 1=calibrated, 2=primed, 3=enabled) >\r\n\r\n",
  (CliCmdFn_t)Adrv9001Cli_SetRadioState,
  2,
  NULL
};

static void Adrv9001Cli_GetRadioState(Cli_t *CliInstance, const char *cmd, void *userData)
{
  adrv9001_port_t         port;
  adrv9001_radio_state_t  state;
  const char         *s = NULL;

  if((s = Adrv9001Cli_ParsePort(cmd, 1, &port)) == NULL)
  {
    printf("Invalid Parameter\r\n");
    return;
  }

	Adrv9001_ClearError( );

  if(Adrv9001_GetRadioState(port, &state) == Adrv9001Status_Success)
  {
    printf("%s = %s\r\n",s, state == Adrv9001RadioState_Standby? "Standby" :
                            state == Adrv9001RadioState_Calibrated? "Calibrated" :
                            state == Adrv9001RadioState_Primed? "Primed" :
                            state == Adrv9001RadioState_Enabled? "Enabled" : "Unknown" );
  }
  else
  {
    printf("Failed\r\n");
  }
}

/**
*  Get Radio State
*/
static const CliCmd_t Adrv9001CliGetRadioStateDef =
{
  "GetRadioState",
  "GetRadioState: Get radio state \r\n"
  "GetRadioState < port ( Rx1,Rx2,Tx1,Tx2) >\r\n\r\n",
  (CliCmdFn_t)Adrv9001Cli_GetRadioState,
  1,
  NULL
};


/*******************************************************************************

  PURPOSE:  Initialize APP CLI

  COMMENT:

*******************************************************************************/
int Adrv9001Cli_Initialize( void )
{
  Cli_t *Instance = AppCli_GetInstance( );

  Cli_RegisterCommand(Instance, &Adrv9001CliSetRadioStateDef);
  Cli_RegisterCommand(Instance, &Adrv9001CliGetRadioStateDef);

	return Adrv9001Status_Success;
}
