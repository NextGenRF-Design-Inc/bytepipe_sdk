/***************************************************************************//**
*  \addtogroup ADRV9001_CLI
*   @{
*******************************************************************************/
/***************************************************************************//**
*  \file       adrv9001_cli.c
*
*  \details
*
*  This file contains the implementation of the adrv9001 interface driver
*  command line interface.
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
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
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
  else
    *port = Adrv9001Port_Num;

  return s;
}

static void Adrv9001Cli_GetTxBoost(Cli_t *CliInstance, const char *cmd, void *userData)
{
  adrv9001_port_t         port;
  const char         *s = NULL;
  bool                Enable;
  int status;

  if((s = Adrv9001Cli_ParsePort(cmd, 1, &port)) == NULL)
  {
    printf("Invalid Parameter\r\n");
    return;
  }

  Adrv9001_ClearError( );

  if((status = Adrv9001_GetTxBoost(port, &Enable)) == Adrv9001Status_Success)
  {
    printf("%s Boost %s\r\n", s, Enable? "Enabled" : "Disabled");
  }
  else
  {
    printf("%s\r\n",ADRV9001_STATUS_2_STR(status));
  }
}

static const CliCmd_t Adrv9001CliGetTxBoostDef =
{
  "Adrv9001GetTxBoost",
  "Adrv9001GetTxBoost: Get Transmit boost \r\n"
  "Adrv9001GetTxBoost < port ( Tx1,Tx2 )>\r\n\r\n",
  (CliCmdFn_t)Adrv9001Cli_GetTxBoost,
  1,
  NULL
};

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

  int status = Adrv9001_SetTxBoost(port, Enable);
  printf("%s\r\n",ADRV9001_STATUS_2_STR(status));
}

static const CliCmd_t Adrv9001CliSetTxBoostDef =
{
  "Adrv9001SetTxBoost",
  "Adrv9001SetTxBoost: Set Transmit boost \r\n"
  "Adrv9001SetTxBoost < port ( Tx1,Tx2 ), Enable (ie. 0 or 1) >\r\n\r\n",
  (CliCmdFn_t)Adrv9001Cli_SetTxBoost,
  2,
  NULL
};

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

  int status = Adrv9001_SetTxAttenuation(port, attn_mdB);
  printf("%s\r\n",ADRV9001_STATUS_2_STR(status));
}

static const CliCmd_t Adrv9001CliSetTxAttnDef =
{
  "Adrv9001SetTxAttn",
  "Adrv9001SetTxAttn: Set Transmit attenuation in dB \r\n"
  "Adrv9001SetTxAttn < port ( Tx1,Tx2 ), attn ( ie 10.5 ) >\r\n\r\n",
  (CliCmdFn_t)Adrv9001Cli_SetTxAttn,
  2,
  NULL
};

static void Adrv9001Cli_GetTxAttn(Cli_t *CliInstance, const char *cmd, void *userData)
{
  adrv9001_port_t         port;
  const char         *s = NULL;
  uint16_t                attn_mdB;
  float                   attn;
  int status;

  if((s = Adrv9001Cli_ParsePort(cmd, 1, &port)) == NULL)
  {
    printf("Invalid Parameter\r\n");
    return;
  }

  Adrv9001_ClearError( );

  if((status = Adrv9001_GetTxAttenuation(port, &attn_mdB)) == Adrv9001Status_Success)
  {
    attn = (float)attn_mdB;
    attn = attn / 1000;
    printf("%s Attenuation = %f dB\r\n",s, attn);
  }
  else
  {
    printf("%s\r\n",ADRV9001_STATUS_2_STR(status));
  }
}

static const CliCmd_t Adrv9001CliGetTxAttnDef =
{
  "Adrv9001GetTxAttn",
  "Adrv9001GetTxAttn: Get Transmit attenuation in dB \r\n"
  "Adrv9001GetTxAttn < port ( Tx1,Tx2 ) >\r\n\r\n",
  (CliCmdFn_t)Adrv9001Cli_GetTxAttn,
  1,
  NULL
};

static void Adrv9001Cli_GetSampleRate(Cli_t *CliInstance, const char *cmd, void *userData)
{
  adrv9001_port_t         port;
  const char         *s = NULL;
  uint32_t                freq;
  int status;

  if((s = Adrv9001Cli_ParsePort(cmd, 1, &port)) == NULL)
  {
    printf("Invalid Parameter\r\n");
    return;
  }

  Adrv9001_ClearError( );

  if((status = Adrv9001_GetSampleRate(port, &freq)) == Adrv9001Status_Success)
  {
    printf("%s Sample Rate = %lu Hz\r\n",s,freq);
  }
  else
  {
    printf("%s\r\n",ADRV9001_STATUS_2_STR(status));
  }
}

static const CliCmd_t Adrv9001CliGetSampleRateDef =
{
  "Adrv9001GetSampleRate",
  "Adrv9001GetSampleRate: Get SSI IQ Sample Rate \r\n"
  "Adrv9001GetSampleRate < port ( Rx1,Rx2,Tx1,Tx2) >\r\n\r\n",
  (CliCmdFn_t)Adrv9001Cli_GetSampleRate,
  1,
  NULL
};

static void Adrv9001Cli_GetCarrierFrequency(Cli_t *CliInstance, const char *cmd, void *userData)
{
  adrv9001_port_t         port;
  const char         *s = NULL;
  uint64_t                freq;
  int status;

  if((s = Adrv9001Cli_ParsePort(cmd, 1, &port)) == NULL)
  {
    printf("Invalid Parameter\r\n");
    return;
  }

  Adrv9001_ClearError( );

  if((status = Adrv9001_GetCarrierFrequency(port, &freq)) == Adrv9001Status_Success)
  {
    printf("%s Carrier Frequency = %lld Hz\r\n",s,freq);
  }
  else
  {
    printf("%s\r\n",ADRV9001_STATUS_2_STR(status));
  }
}

static const CliCmd_t Adrv9001CliGetCarrierFrequencyDef =
{
  "Adrv9001GetCarrierFrequency",
  "Adrv9001GetCarrierFrequency: Get Carrier Frequency \r\n"
  "Adrv9001GetCarrierFrequency < port ( Rx1,Rx2,Tx1,Tx2) >\r\n\r\n",
  (CliCmdFn_t)Adrv9001Cli_GetCarrierFrequency,
  1,
  NULL
};

static void Adrv9001Cli_SetCarrierFrequency(Cli_t *CliInstance, const char *cmd, void *userData)
{
  adrv9001_port_t         port;
  const char         *s = NULL;
  uint64_t                freq;

  if((s = Adrv9001Cli_ParsePort(cmd, 1, &port)) == NULL)
  {
    printf("Invalid Parameter\r\n");
    return;
  }

  Cli_GetParameter(cmd, 2, CliParamTypeU64,  &freq);
  {
    printf("Invalid Parameter\r\n");
    return;
  }

  Adrv9001_ClearError( );

  int status = Adrv9001_SetCarrierFrequency(port, freq);
  printf("%s\r\n",ADRV9001_STATUS_2_STR(status));
}

static const CliCmd_t Adrv9001CliSetCarrierFrequencyDef =
{
  "Adrv9001SetCarrierFrequency",
  "Adrv9001SetCarrierFrequency: Set Carrier Frequency \r\n"
  "Adrv9001SetCarrierFrequency < port ( Rx1,Rx2,Tx1,Tx2) >\r\n\r\n",
  (CliCmdFn_t)Adrv9001Cli_SetCarrierFrequency,
  2,
  NULL
};

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

	int status = Adrv9001_SetRadioState(port, state);
	printf("%s\r\n",ADRV9001_STATUS_2_STR(status));
}

static const CliCmd_t Adrv9001CliSetRadioStateDef =
{
  "Adrv9001SetRadioState",
  "Adrv9001SetRadioState: Set radio state \r\n"
  "Adrv9001SetRadioState < port ( Rx1,Rx2,Tx1,Tx2), state (0=standby, 1=calibrated, 2=primed, 3=enabled) >\r\n\r\n",
  (CliCmdFn_t)Adrv9001Cli_SetRadioState,
  2,
  NULL
};

static void Adrv9001Cli_GetRadioState(Cli_t *CliInstance, const char *cmd, void *userData)
{
  adrv9001_port_t         port;
  adrv9001_radio_state_t  state;
  const char         *s = NULL;
  int status;

  if((s = Adrv9001Cli_ParsePort(cmd, 1, &port)) == NULL)
  {
    printf("Invalid Parameter\r\n");
    return;
  }

	Adrv9001_ClearError( );

  if((status = Adrv9001_GetRadioState(port, &state)) == Adrv9001Status_Success)
  {
    printf("%s = %s\r\n",s, state == Adrv9001RadioState_Standby? "Standby" :
                            state == Adrv9001RadioState_Calibrated? "Calibrated" :
                            state == Adrv9001RadioState_Primed? "Primed" :
                            state == Adrv9001RadioState_Enabled? "Enabled" : "Unknown" );
  }
  else
  {
    printf("%s\r\n",ADRV9001_STATUS_2_STR(status));
  }
}

static const CliCmd_t Adrv9001CliGetRadioStateDef =
{
  "Adrv9001GetRadioState",
  "Adrv9001GetRadioState: Get radio state \r\n"
  "Adrv9001GetRadioState < port ( Rx1,Rx2,Tx1,Tx2) >\r\n\r\n",
  (CliCmdFn_t)Adrv9001Cli_GetRadioState,
  1,
  NULL
};

static void Adrv9001Cli_ToRfEnabled(Cli_t *CliInstance, const char *cmd, void *userData)
{
  adrv9001_port_t         port;

  if(Adrv9001Cli_ParsePort(cmd, 1, &port) == NULL)
  {
    printf("Invalid Parameter\r\n");
    return;
  }

  Adrv9001_ClearError( );

  int status = Adrv9001_ToRfEnabled(port);

  printf("%s\r\n",ADRV9001_STATUS_2_STR(status));
}

static const CliCmd_t Adrv9001CliToRfEnabledDef =
{
  "Adrv9001ToRfEnabled",
  "Adrv9001ToRfEnabled: Transition to RF enabled \r\n"
  "Adrv9001ToRfEnabled < port ( Rx1,Rx2,Tx1,Tx2) >\r\n\r\n",
  (CliCmdFn_t)Adrv9001Cli_ToRfEnabled,
  1,
  NULL
};

static void Adrv9001Cli_ToRfCalibrated(Cli_t *CliInstance, const char *cmd, void *userData)
{
  adrv9001_port_t         port;

  if(Adrv9001Cli_ParsePort(cmd, 1, &port) == NULL)
  {
    printf("Invalid Parameter\r\n");
    return;
  }

  Adrv9001_ClearError( );

  int status = Adrv9001_ToRfCalibrated(port);

  printf("%s\r\n",ADRV9001_STATUS_2_STR(status));
}

static const CliCmd_t Adrv9001CliToRfCalibratedDef =
{
  "Adrv9001ToRfCalibrated",
  "Adrv9001ToRfCalibrated: Transition to RF calibrated \r\n"
  "Adrv9001ToRfCalibrated < port ( Rx1,Rx2,Tx1,Tx2) >\r\n\r\n",
  (CliCmdFn_t)Adrv9001Cli_ToRfCalibrated,
  1,
  NULL
};

static void Adrv9001Cli_ToRfPrimed(Cli_t *CliInstance, const char *cmd, void *userData)
{
  adrv9001_port_t         port;

  if(Adrv9001Cli_ParsePort(cmd, 1, &port) == NULL)
  {
    printf("Invalid Parameter\r\n");
    return;
  }

  Adrv9001_ClearError( );

  int status = Adrv9001_ToRfPrimed(port);

  printf("%s\r\n",ADRV9001_STATUS_2_STR(status));

}

static const CliCmd_t Adrv9001CliToRfPrimedDef =
{
  "Adrv9001ToRfPrimed",
  "Adrv9001ToRfPrimed: Transition to RF primed \r\n"
  "Adrv9001ToRfPrimed < port ( Rx1,Rx2,Tx1,Tx2) >\r\n\r\n",
  (CliCmdFn_t)Adrv9001Cli_ToRfPrimed,
  1,
  NULL
};

static void Adrv9001Cli_GetTemp(Cli_t *CliInstance, const char *cmd, void *userData)
{
  int16_t Temp_C;
  int status;

  Adrv9001_ClearError( );

  if((status = Adrv9001_GetTemperature( &Temp_C )) == Adrv9001Status_Success)
  {
    printf("%dC\r\n", Temp_C);
  }
  else
  {
    printf("%s\r\n",ADRV9001_STATUS_2_STR(status));
  }
}

static const CliCmd_t Adrv9001CliGetTempDef =
{
  "Adrv9001GetTemp",
  "Adrv9001GetTemp: Get Temperature in degC \r\n"
  "Adrv9001GetTemp <  >\r\n\r\n",
  (CliCmdFn_t)Adrv9001Cli_GetTemp,
  0,
  NULL
};

static void Adrv9001Cli_GetVerInfo(Cli_t *CliInstance, const char *cmd, void *userData)
{
  adrv9001_ver_t VerInfo;
  int status;
  Adrv9001_ClearError( );

  if((status = Adrv9001_GetVersionInfo( &VerInfo )) == Adrv9001Status_Success)
  {
    printf("%s Version Information:\r\n","ADRV9002");
    printf("  -Silicon Version: %X%X\r\n",VerInfo.Silicon.major, VerInfo.Silicon.minor);
    printf("  -Firmware Version: %u.%u.%u.%u\r\n",VerInfo.Arm.major, VerInfo.Arm.minor, VerInfo.Arm.maint, VerInfo.Arm.rcVer);
    printf("  -API Version: %lu.%lu.%lu\r\n\r\n", VerInfo.Api.major,  VerInfo.Api.minor, VerInfo.Api.patch);
  }
  else
  {
    printf("%s\r\n",ADRV9001_STATUS_2_STR(status));
  }
}

static const CliCmd_t Adrv9001CliGetVerInfoDef =
{
  "Adrv9001GetVerInfo",
  "Adrv9001GetVerInfo: Get version information \r\n"
  "Adrv9001GetVerInfo <  >\r\n\r\n",
  (CliCmdFn_t)Adrv9001Cli_GetVerInfo,
  0,
  NULL
};

static void Adrv9001Cli_GetRssi(Cli_t *CliInstance, const char *cmd, void *userData)
{
  adrv9001_port_t         port;
  const char         *s = NULL;
  uint32_t                rssi;
  int status;

  if((s = Adrv9001Cli_ParsePort(cmd, 1, &port)) == NULL)
  {
    printf("Invalid Parameter\r\n");
    return;
  }

  Adrv9001_ClearError( );

  if((status = Adrv9001_GetRssi(port, &rssi)) == Adrv9001Status_Success)
  {
    printf("RSSI = %2.1fdB\r\n",((float)rssi)/1000);
  }
  else
  {
    printf("%s\r\n",ADRV9001_STATUS_2_STR(status));
  }
}

static const CliCmd_t Adrv9001CliGetRssiDef =
{
  "Adrv9001GetRssi",
  "Adrv9001GetRssi: Get Received Signal Strength \r\n"
  "Adrv9001GetRssi < port ( Rx1,Rx2) >\r\n\r\n",
  (CliCmdFn_t)Adrv9001Cli_GetRssi,
  1,
  NULL
};

static void Adrv9001Cli_LoadProfile(Cli_t *CliInstance, const char *cmd, void *userData)
{
  Adrv9001_ClearError( );

  int status = Adrv9001_LoadProfile( );

  printf("%s\r\n",ADRV9001_STATUS_2_STR(status));
}

static const CliCmd_t Adrv9001CliLoadProfileDef =
{
  "Adrv9001LoadProfile",
  "Adrv9001LoadProfile: Load profile settings into ADRV9002 \r\n"
  "Adrv9001LoadProfile < >\r\n\r\n",
  (CliCmdFn_t)Adrv9001Cli_LoadProfile,
  0,
  NULL
};

static void Adrv9001Cli_SetLoopBack(Cli_t *CliInstance, const char *cmd, void *userData)
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

  int status = Adrv9001_SetInternalLoopBack(port, state);

  printf("%s\r\n",ADRV9001_STATUS_2_STR(status));
}

static const CliCmd_t Adrv9001CliSetLoopBackDef =
{
  "Adrv9001SetLoopBack",
  "Adrv9001SetLoopBack: Set loopback \r\n"
  "Adrv9001SetLoopBack < port ( Rx1,Rx2,Tx1,Tx2), enabled >\r\n\r\n",
  (CliCmdFn_t)Adrv9001Cli_SetLoopBack,
  2,
  NULL
};

static void Adrv9001Cli_ReadDma(Cli_t *CliInstance, const char *cmd, void *userData)
{
  adrv9001_port_t     port;
  const char         *s = NULL;
  uint32_t            Length;
  int status;

  if((s = Adrv9001Cli_ParsePort(cmd, 1, &port)) == NULL)
  {
    printf("Invalid Parameter\r\n");
    return;
  }

  Cli_GetParameter(cmd, 2, CliParamTypeU32,  &Length);

  uint32_t *Buf = malloc(Length * sizeof(uint32_t));

  if((status = Adrv9001_DmaTransferBlocking(port, Buf, Length)) == Adrv9001Status_Success)
  {
    printf("%s DMA = \r\n",s);

    uint16_t idata;
    uint16_t qdata;
    for(int i = 0; i < Length; i++)
    {
      idata = (uint16_t)(Buf[i] >> 16);
      qdata = (uint16_t)Buf[i];
      printf(" %04x %04x\r\n", idata, qdata );
    }
  }
  else
  {
    printf("%s\r\n",ADRV9001_STATUS_2_STR(status));
  }
}

static const CliCmd_t Adrv9001CliReadDmaDef =
{
  "Adrv9001ReadDma",
  "Adrv9001ReadDma: Read DMA Channel \r\n"
  "Adrv9001ReadDma < port ( Rx1,Rx2 ), SampleCnt >\r\n\r\n",
  (CliCmdFn_t)Adrv9001Cli_ReadDma,
  2,
  NULL
};

int Adrv9001Cli_Initialize( void )
{
  Cli_t *Instance = AppCli_GetInstance( );

  Cli_RegisterCommand(Instance, &Adrv9001CliSetRadioStateDef);
  Cli_RegisterCommand(Instance, &Adrv9001CliGetRadioStateDef);
  Cli_RegisterCommand(Instance, &Adrv9001CliGetCarrierFrequencyDef);
  Cli_RegisterCommand(Instance, &Adrv9001CliSetCarrierFrequencyDef);
  Cli_RegisterCommand(Instance, &Adrv9001CliGetSampleRateDef);
  Cli_RegisterCommand(Instance, &Adrv9001CliGetTxAttnDef);
  Cli_RegisterCommand(Instance, &Adrv9001CliSetTxAttnDef);
  Cli_RegisterCommand(Instance, &Adrv9001CliGetTxBoostDef);
  Cli_RegisterCommand(Instance, &Adrv9001CliSetTxBoostDef);
  Cli_RegisterCommand(Instance, &Adrv9001CliToRfPrimedDef);
  Cli_RegisterCommand(Instance, &Adrv9001CliToRfCalibratedDef);
  Cli_RegisterCommand(Instance, &Adrv9001CliToRfEnabledDef);
  Cli_RegisterCommand(Instance, &Adrv9001CliGetRssiDef);
  Cli_RegisterCommand(Instance, &Adrv9001CliGetTempDef);
  Cli_RegisterCommand(Instance, &Adrv9001CliGetVerInfoDef);
  Cli_RegisterCommand(Instance, &Adrv9001CliLoadProfileDef);
  Cli_RegisterCommand(Instance, &Adrv9001CliSetLoopBackDef);
  Cli_RegisterCommand(Instance, &Adrv9001CliReadDmaDef);

	return Adrv9001Status_Success;
}
