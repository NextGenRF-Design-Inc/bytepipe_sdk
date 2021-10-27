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

/*******************************************************************************
*
* \details Get Transmitter Boost
*
*******************************************************************************/
static void Adrv9001Cli_GetTxBoost(Cli_t *CliInstance, const char *cmd, void *userData)
{
  adrv9001_port_t         port;
  const char         *s = NULL;
  bool                Enable;

  if((s = Adrv9001Cli_ParsePort(cmd, 1, &port)) == NULL)
  {
    printf("Invalid Parameter\r\n");
    return;
  }

  Adrv9001_ClearError( );

  if(Adrv9001_GetTxBoost(port, &Enable) == Adrv9001Status_Success)
  {
    printf("%s Boost %s\r\n", s, Enable? "Enabled" : "Disabled");
  }
  else
  {
    printf("Failed\r\n");
  }
}

/**
*  Get Transmitter Boost
*/
static const CliCmd_t Adrv9001CliGetTxBoostDef =
{
  "GetTxBoost",
  "GetTxBoost: Get Transmit boost \r\n"
  "GetTxBoost < port ( Tx1,Tx2 )>\r\n\r\n",
  (CliCmdFn_t)Adrv9001Cli_GetTxBoost,
  1,
  NULL
};

/*******************************************************************************
*
* \details Set Transmitter Boost
*
*******************************************************************************/
static void Adrv9001Cli_SetTxBoost(Cli_t *CliInstance, const char *cmd, void *userData)
{
  adrv9001_port_t         port;
  uint32_t                Enable;

  if(Adrv9001Cli_ParsePort(cmd, 1, &port) == NULL)
  {
    printf("Invalid Parameter\r\n");
    return;
  }

  Cli_GetParameter(cmd, 2, CliParamTypeU32,  &Enable);

  Adrv9001_ClearError( );

  if(Adrv9001_SetTxBoost(port, Enable) == Adrv9001Status_Success)
  {
    printf("Success\r\n");
  }
  else
  {
    printf("Failed\r\n");
  }
}

/**
*  Set Transmitter Boost
*/
static const CliCmd_t Adrv9001CliSetTxBoostDef =
{
  "SetTxBoost",
  "SetTxBoost: Set Transmit boost \r\n"
  "SetTxBoost < port ( Tx1,Tx2 ), Enable (ie. 0 or 1) >\r\n\r\n",
  (CliCmdFn_t)Adrv9001Cli_SetTxBoost,
  2,
  NULL
};

/*******************************************************************************
*
* \details Set Transmitter Attenuation
*
*******************************************************************************/
static void Adrv9001Cli_SetTxAttn(Cli_t *CliInstance, const char *cmd, void *userData)
{
  adrv9001_port_t         port;
  uint16_t                attn_mdB;
  float                   attn;

  if(Adrv9001Cli_ParsePort(cmd, 1, &port) == NULL)
  {
    printf("Invalid Parameter\r\n");
    return;
  }

  Cli_GetParameter(cmd, 2, CliParamTypeFloat,  &attn);

  attn_mdB = (uint16_t)(attn * 1000);

  Adrv9001_ClearError( );

  if(Adrv9001_SetTxAttenuation(port, attn_mdB) == Adrv9001Status_Success)
  {
    printf("Success\r\n");
  }
  else
  {
    printf("Failed\r\n");
  }
}

/**
*  Set Transmitter Attenuation
*/
static const CliCmd_t Adrv9001CliSetTxAttnDef =
{
  "SetTxAttn",
  "SetTxAttn: Set Transmit attenuation in dB \r\n"
  "SetTxAttn < port ( Tx1,Tx2 ), attn ( ie 10.5 ) >\r\n\r\n",
  (CliCmdFn_t)Adrv9001Cli_SetTxAttn,
  2,
  NULL
};

/*******************************************************************************
*
* \details Get Transmitter Attenuation
*
*******************************************************************************/
static void Adrv9001Cli_GetTxAttn(Cli_t *CliInstance, const char *cmd, void *userData)
{
  adrv9001_port_t         port;
  const char         *s = NULL;
  uint16_t                attn_mdB;
  float                   attn;

  if((s = Adrv9001Cli_ParsePort(cmd, 1, &port)) == NULL)
  {
    printf("Invalid Parameter\r\n");
    return;
  }

  Adrv9001_ClearError( );

  if(Adrv9001_GetTxAttenuation(port, &attn_mdB) == Adrv9001Status_Success)
  {
    attn = (float)attn_mdB;
    attn = attn / 1000;
    printf("%s Attenuation = %f dB\r\n",s, attn);
  }
  else
  {
    printf("Failed\r\n");
  }
}

/**
*  Get Transmitter Attenuation
*/
static const CliCmd_t Adrv9001CliGetTxAttnDef =
{
  "GetTxAttn",
  "GetTxAttn: Get Transmit attenuation in dB \r\n"
  "GetTxAttn < port ( Tx1,Tx2 ) >\r\n\r\n",
  (CliCmdFn_t)Adrv9001Cli_GetTxAttn,
  1,
  NULL
};

/*******************************************************************************
*
* \details Get Sample Rate Frequency
*
*******************************************************************************/
static void Adrv9001Cli_GetSampleRate(Cli_t *CliInstance, const char *cmd, void *userData)
{
  adrv9001_port_t         port;
  const char         *s = NULL;
  uint32_t                freq;

  if((s = Adrv9001Cli_ParsePort(cmd, 1, &port)) == NULL)
  {
    printf("Invalid Parameter\r\n");
    return;
  }

  Adrv9001_ClearError( );

  if(Adrv9001_GetSampleRate(port, &freq) == Adrv9001Status_Success)
  {
    printf("%s Sample Rate = %d Hz\r\n",s,freq);
  }
  else
  {
    printf("Failed\r\n");
  }
}

/**
*  Get Sample Rate
*/
static const CliCmd_t Adrv9001CliGetSampleRateDef =
{
  "GetSampleRate",
  "GetSampleRate: Get SSI IQ Sample Rate \r\n"
  "GetSampleRate < port ( Rx1,Rx2,Tx1,Tx2) >\r\n\r\n",
  (CliCmdFn_t)Adrv9001Cli_GetSampleRate,
  1,
  NULL
};

/*******************************************************************************
*
* \details Get Carrier Frequency
*
*******************************************************************************/
static void Adrv9001Cli_GetCarrierFrequency(Cli_t *CliInstance, const char *cmd, void *userData)
{
  adrv9001_port_t         port;
  const char         *s = NULL;
  uint64_t                freq;

  if((s = Adrv9001Cli_ParsePort(cmd, 1, &port)) == NULL)
  {
    printf("Invalid Parameter\r\n");
    return;
  }

  Adrv9001_ClearError( );

  if(Adrv9001_GetCarrierFrequency(port, &freq) == Adrv9001Status_Success)
  {
    printf("%s Carrier Frequency = %ld Hz\r\n",s,freq);
  }
  else
  {
    printf("Failed\r\n");
  }
}

/**
*  Get Carrier Frequency
*/
static const CliCmd_t Adrv9001CliGetCarrierFrequencyDef =
{
  "GetCarrierFrequency",
  "GetCarrierFrequency: Get Carrier Frequency \r\n"
  "GetCarrierFrequency < port ( Rx1,Rx2,Tx1,Tx2) >\r\n\r\n",
  (CliCmdFn_t)Adrv9001Cli_GetCarrierFrequency,
  1,
  NULL
};

/*******************************************************************************
*
* \details Set Radio State
*
*******************************************************************************/
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

static const CliCmd_t Adrv9001CliSetRadioStateDef =
{
  "SetRadioState",
  "SetRadioState: Set radio state \r\n"
  "SetRadioState < port ( Rx1,Rx2,Tx1,Tx2), state (0=standby, 1=calibrated, 2=primed, 3=enabled) >\r\n\r\n",
  (CliCmdFn_t)Adrv9001Cli_SetRadioState,
  2,
  NULL
};

/*******************************************************************************
*
* \details Get Radio State
*
*******************************************************************************/
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
*
* \details To RF Enabled
*
*******************************************************************************/
static void Adrv9001Cli_ToRfEnabled(Cli_t *CliInstance, const char *cmd, void *userData)
{
  adrv9001_port_t         port;

  if(Adrv9001Cli_ParsePort(cmd, 1, &port) == NULL)
  {
    printf("Invalid Parameter\r\n");
    return;
  }

  Adrv9001_ClearError( );

  if(Adrv9001_ToRfEnabled(port) == Adrv9001Status_Success)
  {
    printf("Success\r\n");
  }
  else
  {
    printf("Failed\r\n");
  }
}

static const CliCmd_t Adrv9001CliToRfEnabledDef =
{
  "ToRfEnabled",
  "ToRfEnabled: Transition to RF enabled \r\n"
  "ToRfEnabled < port ( Rx1,Rx2,Tx1,Tx2) >\r\n\r\n",
  (CliCmdFn_t)Adrv9001Cli_ToRfEnabled,
  1,
  NULL
};

/*******************************************************************************
*
* \details To RF Calibrated
*
*******************************************************************************/
static void Adrv9001Cli_ToRfCalibrated(Cli_t *CliInstance, const char *cmd, void *userData)
{
  adrv9001_port_t         port;

  if(Adrv9001Cli_ParsePort(cmd, 1, &port) == NULL)
  {
    printf("Invalid Parameter\r\n");
    return;
  }

  Adrv9001_ClearError( );

  if(Adrv9001_ToRfCalibrated(port) == Adrv9001Status_Success)
  {
    printf("Success\r\n");
  }
  else
  {
    printf("Failed\r\n");
  }
}

static const CliCmd_t Adrv9001CliToRfCalibratedDef =
{
  "ToRfCalibrated",
  "ToRfCalibrated: Transition to RF calibrated \r\n"
  "ToRfCalibrated < port ( Rx1,Rx2,Tx1,Tx2) >\r\n\r\n",
  (CliCmdFn_t)Adrv9001Cli_ToRfCalibrated,
  1,
  NULL
};

/*******************************************************************************
*
* \details To RF Primed
*
*******************************************************************************/
static void Adrv9001Cli_ToRfPrimed(Cli_t *CliInstance, const char *cmd, void *userData)
{
  adrv9001_port_t         port;

  if(Adrv9001Cli_ParsePort(cmd, 1, &port) == NULL)
  {
    printf("Invalid Parameter\r\n");
    return;
  }

  Adrv9001_ClearError( );

  if(Adrv9001_ToRfPrimed(port) == Adrv9001Status_Success)
  {
    printf("Success\r\n");
  }
  else
  {
    printf("Failed\r\n");
  }
}

static const CliCmd_t Adrv9001CliToRfPrimedDef =
{
  "ToRfPrimed",
  "ToRfPrimed: Transition to RF primed \r\n"
  "ToRfPrimed < port ( Rx1,Rx2,Tx1,Tx2) >\r\n\r\n",
  (CliCmdFn_t)Adrv9001Cli_ToRfPrimed,
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
  Cli_RegisterCommand(Instance, &Adrv9001CliGetCarrierFrequencyDef);
  Cli_RegisterCommand(Instance, &Adrv9001CliGetSampleRateDef);
  Cli_RegisterCommand(Instance, &Adrv9001CliGetTxAttnDef);
  Cli_RegisterCommand(Instance, &Adrv9001CliSetTxAttnDef);
  Cli_RegisterCommand(Instance, &Adrv9001CliGetTxBoostDef);
  Cli_RegisterCommand(Instance, &Adrv9001CliSetTxBoostDef);
  Cli_RegisterCommand(Instance, &Adrv9001CliToRfPrimedDef);
  Cli_RegisterCommand(Instance, &Adrv9001CliToRfCalibratedDef);
  Cli_RegisterCommand(Instance, &Adrv9001CliToRfEnabledDef);


	return Adrv9001Status_Success;
}
