/***************************************************************************//**
*  \addtogroup ADRV9001_CLI
*   @{
*******************************************************************************/
/***************************************************************************//**
*  \file       adrv9001_cli.c
*
*  \details    This file contains the ADRV9001 layer CLI commands.
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
#include "xstatus.h"
#include "adrv9001_cli.h"
#include "adrv9001_pib.h"
#include "math.h"
#include "status.h"


#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

static void Adrv9001Cli_GetPortChannelParameter(cli_t *CliInstance, const char *cmd, uint8_t pNum, adi_common_ChannelNumber_e *Channel, adi_common_Port_e *Port)
{
  char pName[64] = {0};

  Cli_GetParameter(cmd, pNum, CliParamTypeStr, pName);

  if(strncmp(pName, "Tx1", 3) == 0 )
  {
    *Port = ADI_TX;
    *Channel = ADI_CHANNEL_1;
  }
  else if(strncmp(pName, "Tx2", 3) == 0 )
  {
    *Port = ADI_TX;
    *Channel = ADI_CHANNEL_2;
  }
  else if(strncmp(pName, "Rx1", 3) == 0 )
  {
    *Port = ADI_RX;
    *Channel = ADI_CHANNEL_1;
  }
  else if(strncmp(pName, "Rx2", 3) == 0 )
  {
    *Port = ADI_RX;
    *Channel = ADI_CHANNEL_2;
  }
}

static void Adrv9001Cli_GetParam(cli_t *CliInstance, const char *cmd, adrv9001_t *Adrv9001)
{
  char *name;
  char *value;
  int32_t status;

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
  if((status = Adrv9001Pib_GetStringByName( Adrv9001, name, value )) != 0)
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

static void Adrv9001Cli_SetParam(cli_t *CliInstance, const char *cmd, adrv9001_t *Adrv9001)
{
  char *name;
  char *str;

  if((name = calloc(1, PIB_NAME_SIZE )) == NULL)
  {
    Cli_Printf(CliInstance,"Memory Error\r\n");
    return;
  }

  if((str = calloc(1, 1024 )) == NULL)
  {
    free(name);
    Cli_Printf(CliInstance,"Memory Error\r\n");
    return;
  }

  /* Get Name */
  Cli_GetParameter(cmd, 1, CliParamTypeStr, name);

  /* Get Value */
  Cli_GetParameter(cmd, 2, CliParamTypeStr, str);

  /* Set parameter */
  int32_t status = Adrv9001Pib_SetByNameByString( Adrv9001, name, str );

  Cli_Printf(CliInstance,"SetParam %s\r\n",StatusString(status));

  free(name);
  free(str);
}

static void Adrv9001Cli_ListParams(cli_t *CliInstance, const char *cmd, adrv9001_t *Adrv9001)
{
  Cli_Printf(CliInstance,"ADRV9001 Parameter Names:\r\n");

  if( Cli_GetParameterCount( cmd ) > 0 )
  {
    char *key;

    if((key = calloc(1, PIB_NAME_SIZE )) == NULL)
    {
      Cli_Printf(CliInstance,"Memory Error\r\n");
      return;
    }

    Cli_GetParameter(cmd, 1, CliParamTypeStr, key);

    for(int i = 0; i < Adrv9001->Pib.PibLen; i++)
    {
      if(strstr( Adrv9001->Pib.Def[i].name, key ))
        Cli_Printf(CliInstance,"  - %s\r\n", Adrv9001->Pib.Def[i].name);
    }
  }
  else
  {
    for(int i = 0; i < Adrv9001->Pib.PibLen; i++)
      Cli_Printf(CliInstance,"  - %s\r\n", Adrv9001->Pib.Def[i].name);

    Cli_Printf(CliInstance,"Note: Add keyword to display relevant parameters\r\n");
  }
}

static void Adrv9001Cli_ToRfCalibrated(cli_t *CliInstance, const char *cmd, adrv9001_t *Adrv9001)
{
  int32_t status = 0;
  adi_common_ChannelNumber_e Channel;
  adi_common_Port_e Port;

  Adrv9001_ClearError( Adrv9001 );

  Adrv9001Cli_GetPortChannelParameter(CliInstance, cmd, 1, &Channel, &Port);

  status = Adrv9001_ToCalibrated( Adrv9001, Port, Channel );

  Cli_Printf(CliInstance,"%s\r\n",StatusString( status ));
}

static void Adrv9001Cli_ToRfPrimed(cli_t *CliInstance, const char *cmd, adrv9001_t *Adrv9001)
{
  int32_t status = 0;
  adi_common_ChannelNumber_e Channel;
  adi_common_Port_e Port;

  Adrv9001_ClearError( Adrv9001 );

  Adrv9001Cli_GetPortChannelParameter(CliInstance, cmd, 1, &Channel, &Port);

  status = Adrv9001_ToPrimed( Adrv9001, Port, Channel );

  Cli_Printf(CliInstance,"%s\r\n",StatusString( status ));
}

static void Adrv9001Cli_ToRfEnabled(cli_t *CliInstance, const char *cmd, adrv9001_t *Adrv9001)
{
  int32_t status = 0;
  adi_common_ChannelNumber_e Channel;
  adi_common_Port_e Port;

  Adrv9001_ClearError( Adrv9001 );

  Adrv9001Cli_GetPortChannelParameter(CliInstance, cmd, 1, &Channel, &Port);

  status = Adrv9001_ToRfEnabled( Adrv9001, Port, Channel, ADRV9001_TDD_ENABLE_DUR_FOREVER );

  Cli_Printf(CliInstance,"%s\r\n",StatusString( status ));
}

static void Adrv9001Cli_CalibrateSsi(cli_t *CliInstance, const char *cmd, adrv9001_t *Adrv9001)
{
  int32_t status = 0;
  adi_common_ChannelNumber_e Channel;
  adi_common_Port_e Port;

  Adrv9001_ClearError( Adrv9001 );

  Adrv9001Cli_GetPortChannelParameter(CliInstance, cmd, 1, &Channel, &Port);

  status = Adrv9001_CalibrateSsiDelay( Adrv9001, Port, Channel );

  Cli_Printf(CliInstance,"%s\r\n",StatusString( status ));
}

static void Adrv9001Cli_SweepSsi(cli_t *CliInstance, const char *cmd, adrv9001_t *Adrv9001)
{
  int32_t status = 0;
  adi_common_ChannelNumber_e Channel;
  adi_common_Port_e Port;

  Adrv9001_ClearError( Adrv9001 );

  Adrv9001Cli_GetPortChannelParameter(CliInstance, cmd, 1, &Channel, &Port);

  uint16_t results[8][8];
  if((status = Adrv9001_PerformSsiSweep( Adrv9001, Port, Channel, results )) == 0)
  {
    printf("SSI Sweep Test Results:\r\n");

    for(uint8_t clkDelay = 0; clkDelay < 8; clkDelay++)
    {
      printf("  ClkDelay %u: ",clkDelay);

      for(uint8_t dataDelay = 0; dataDelay < 8; dataDelay++)
      {
        printf("  %u ",results[clkDelay][dataDelay]);
      }
      printf("%s","\r\n");
    }
  }
  else
  {
    Cli_Printf(CliInstance,"%s\r\n",StatusString( status ));
  }
}

cli_cmd_t Adrv9001CliSweepSsiDef =
{
    "Adrv9001SweepSsi",
    "Adrv9001SweepSsi:  Sweep ADRV9001 SSI Delay\r\n"
    "Adrv9001SweepSsi < Port (Tx1, Tx2, Rx1, Rx2) >\r\n\r\n",
    (CliCmdFn_t)Adrv9001Cli_SweepSsi,
    1,
    NULL
};

cli_cmd_t Adrv9001CliCalibrateSsiDef =
{
    "Adrv9001CalibrateSsi",
    "Adrv9001CalibrateSsi:  Calibrate ADRV9001 SSI Delay\r\n"
    "Adrv9001CalibrateSsi < Port (Tx1, Tx2, Rx1, Rx2) >\r\n\r\n",
    (CliCmdFn_t)Adrv9001Cli_CalibrateSsi,
    1,
    NULL
};

cli_cmd_t Adrv9001CliToRfCalibratedDef =
{
    "Adrv9001ToRfCalibrated",
    "Adrv9001ToRfCalibrated:  Changes ADRV9001 state to calibrated\r\n"
    "Adrv9001ToRfCalibrated < Port (Tx1, Tx2, Rx1, Rx2) >\r\n\r\n",
    (CliCmdFn_t)Adrv9001Cli_ToRfCalibrated,
    1,
    NULL
};

cli_cmd_t Adrv9001CliToRfPrimedDef =
{
    "Adrv9001ToRfPrimed",
    "Adrv9001ToRfPrimed:  Changes ADRV9001 state to primed\r\n"
    "Adrv9001ToRfPrimed < Port (Tx1, Tx2, Rx1, Rx2) >\r\n\r\n",
    (CliCmdFn_t)Adrv9001Cli_ToRfPrimed,
    1,
    NULL
};

cli_cmd_t Adrv9001CliToRfEnabledDef =
{
    "Adrv9001ToRfEnabled",
    "Adrv9001ToRfEnabled:  Changes ADRV9001 state to enabled\r\n"
    "Adrv9001ToRfEnabled < Port (Tx1, Tx2, Rx1, Rx2) >\r\n\r\n",
    (CliCmdFn_t)Adrv9001Cli_ToRfEnabled,
    1,
    NULL
};

cli_cmd_t Adrv9001CliListParamsDef =
{
    "Adrv9001ListParams",
    "Adrv9001ListParams:  List PHY parameter names for use with Adrv9001SetParms and Adrv9001GetParams.\r\n"
    "Adrv9001ListParams < keyword (optional) >\r\n\r\n",
    (CliCmdFn_t)Adrv9001Cli_ListParams,
    -1,
    NULL
};

cli_cmd_t Adrv9001CliSetParamDef =
{
    "Adrv9001SetParam",
    "Adrv9001SetParam:  Set parameter value \r\n"
    "Adrv9001SetParam < ParameterName, Value >\r\n\r\n",
    (CliCmdFn_t)Adrv9001Cli_SetParam,
    2,
    NULL
};

cli_cmd_t Adrv9001CliGetParamDef =
{
    "Adrv9001GetParam",
    "Adrv9001GetParam:  Get parameter value \r\n"
    "Adrv9001GetParam < ParameterName, >\r\n\r\n",
    (CliCmdFn_t)Adrv9001Cli_GetParam,
    1,
    NULL
};

int32_t Adrv9001Cli_Initialize( cli_t *Cli, adrv9001_t *Adrv9001 )
{
  Cli->CallbackRef = Cli;

  Adrv9001CliGetParamDef.userData = Adrv9001;
  Adrv9001CliSetParamDef.userData = Adrv9001;
  Adrv9001CliListParamsDef.userData = Adrv9001;
  Adrv9001CliToRfCalibratedDef.userData = Adrv9001;
  Adrv9001CliToRfPrimedDef.userData = Adrv9001;
  Adrv9001CliToRfEnabledDef.userData = Adrv9001;
  Adrv9001CliCalibrateSsiDef.userData = Adrv9001;
  Adrv9001CliSweepSsiDef.userData = Adrv9001;

  Cli_RegisterCommand(Cli, &Adrv9001CliGetParamDef);
  Cli_RegisterCommand(Cli, &Adrv9001CliSetParamDef);
  Cli_RegisterCommand(Cli, &Adrv9001CliListParamsDef);
  Cli_RegisterCommand(Cli, &Adrv9001CliToRfCalibratedDef);
  Cli_RegisterCommand(Cli, &Adrv9001CliToRfPrimedDef);
  Cli_RegisterCommand(Cli, &Adrv9001CliToRfEnabledDef);
  Cli_RegisterCommand(Cli, &Adrv9001CliCalibrateSsiDef);
  Cli_RegisterCommand(Cli, &Adrv9001CliSweepSsiDef);

  return Adrv9001Status_Success;
}
