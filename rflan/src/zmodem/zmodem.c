//******************************************************************************
//
//  NEXTGEN RF DESIGN RESTRICTED MATERIAL
//  Copyright 2010-2021 NextGen RF Design Inc.
//  All rights reserved worldwide
//  Unauthorized use, distribution or duplication is
//  strictly prohibited without written authorization.
//
//******************************************************************************

//******************************************************************************
//
//  File Name      : zmodem.c
//  Summary        : Z Modem Parser
//
//******************************************************************************

// =============================================================================
// INCLUDE FILES
// =============================================================================

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "xil_types.h"
#include "xil_printf.h"
#include "zmodem_pub.h"
#include "app_cli.h"

typedef struct
{   
  int ready;                                                // The module is ready
  int run;                                                  // User requested to disable it
  ZMODEM_INSTANCE instance;                                 // The ZMODEM control block (internal)
                                              
} ZMODEM_CTRL; 

static ZMODEM_CTRL zmodem_ctrl;                             // The ZMODEM control block (external)

extern void outbyte(char c);

//--------------------------------------------------------------------------
// Function:    ZModem_Write
//
// Parameters:  The byte to send out to the remote device.
//
// Return:      0 on success.
//
// Description: This function is called by zmodem to send out bytes.
//--------------------------------------------------------------------------

static SHORT ZModem_Write(UBYTE data)
{
  if (zmodem_ctrl.ready && zmodem_ctrl.run)
    outbyte(data);

  return (0);
}

//--------------------------------------------------------------------------
// Function:    ZModem_Parse 
//
// Parameters:  The caracter comming from the remove device.
//
// Return:      0: drop char (it is used by zmodem), otherwise process char
//
// Description: The ZMODEM parsing algorithm
//--------------------------------------------------------------------------

int ZModem_Parse(UBYTE c)
{
  UBYTE ZModemResult;

  if ((zmodem_ctrl.ready == 0) || (zmodem_ctrl.run == 0))
    return 1;

  ZModemResult = zModemParse(&zmodem_ctrl.instance, c); 
  if ((ZModemResult & ZM_BYTE_USED) == 0)
    return 1;

  return 0;
}

//--------------------------------------------------------------------------
// Function:    ZModem_CliZmodem
//
// Parameters:  Command line parameters 
//
// Return:      Nothing
//
// Description: ZMODEM command line controls.
//--------------------------------------------------------------------------
static void ZModem_CliZmodem(Cli_t *CliInstance, const char *cmd, void *userData)
{
  char operation[32];

  Cli_GetParameter(cmd, 1, CliParamTypeStr, &operation[0]);

  if (strcmp(operation, "enable") == 0)
  {
    zmodem_ctrl.run = 1;
    zModemInit(&zmodem_ctrl.instance, ZModem_Write, NULL);
  }
  else if (strcmp(operation, "disable") == 0)
  {
    zmodem_ctrl.run = 0;
    zModemInit(&zmodem_ctrl.instance, ZModem_Write, NULL);
  }
  else if (strcmp(operation, "status") == 0)
  {
    xil_printf("-------------------------------\r\n");
    xil_printf("            ZMODEM             \r\n");
    xil_printf("-------------------------------\r\n");
    xil_printf("Module state         : %s\r\n", (zmodem_ctrl.run ? "enabled(1)" : "disabled(0)"));
    xil_printf("Last operation \r\n");
    xil_printf("    file name        : %s\r\n",  zmodem_ctrl.instance.Stats.FileName);
    xil_printf("    file size        : %ld\r\n", zmodem_ctrl.instance.Stats.FileSize);
    xil_printf("    file read count  : %ld\r\n", zmodem_ctrl.instance.Stats.FileRead);
    xil_printf("    file write count : %ld\r\n", zmodem_ctrl.instance.Stats.FileWrite);
    xil_printf("\r\n");
  }
  else
  {
    xil_printf("\r\n");
    xil_printf("unknown command\r\n");
    xil_printf("\r\n");
  }
}

static const CliCmd_t ZModemCliDef =
{
  "zmodem",
  "zmodem: control and info of the zmodem module\r\n"
  "zmodem < enable | disable | status >\r\n\n",
  (CliCmdFn_t)ZModem_CliZmodem,
  1,
  NULL
};

//--------------------------------------------------------------------------
// Function:    ZModem_Initialize
//
// Parameters:  The drive where the filesystem resides.
//
// Return:      0 on success
//
// Description: Launch the ZMODEM module.
//--------------------------------------------------------------------------

int ZModem_Initialize(char *drive)
{
  int RetVal = -1;
  Cli_t *Instance = AppCli_GetInstance( );

  if (Instance != NULL)
  {
    zModemInit(&zmodem_ctrl.instance, ZModem_Write, drive);
    if (Cli_RegisterCommand(Instance, &ZModemCliDef) == 0)
    {
      RetVal = 0; // Success;
      zmodem_ctrl.run = 0;
      zmodem_ctrl.ready = 1;
    }
  }

  return RetVal;
}

//EOF
