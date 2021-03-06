/***************************************************************************//**
*  \addtogroup APP_CLI
*   @{
*******************************************************************************/
/***************************************************************************//**
*  \file       app_cli.c
*
*  \details    This file contains the RFLAN application CLI implementation.
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
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "cli.h"
#include "xparameters.h"
#include "xil_types.h"
#include "xstatus.h"
#include "xscugic.h"
#include "string.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "portmacro.h"
#include "ff.h"
#include "xuartps.h"
#include "adrv9001.h"
#include "xsysmonpsu.h"
#include "parameters.h"
#include "app_cli.h"
#include "app.h"


static QueueHandle_t    AppCliRxCharQueue;
static QueueHandle_t  	AppCliTxCharQueue;
static volatile u8 		  cliRxChar;
extern XScuGic 			    xInterruptController;
static char             AppCliHistoryBuf[ APP_CLI_HISTORY_BUF_SIZE ];
static char             AppCliCmdBuf[ APP_CLI_CMD_BUF_SIZE ];
static CliCmd_t const  *AppCliCmdList[ APP_CLI_CMD_LIST_SIZE ];
static XUartPs          AppCliUart;
static FIL             *AppCliLogFil;


void outbyte(char c)
{
	if(AppCliTxCharQueue != NULL)
	{
		BaseType_t xHigherPriorityTaskWoken = pdFALSE;

		/* Send Byte to Tx Queue */
		xQueueSendFromISR( AppCliTxCharQueue, &c, &xHigherPriorityTaskWoken );

		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	}
	else
	{
		XUartPs_SendByte(STDOUT_BASEADDRESS, c);
	}
}

static void CLI_UartHandler(void *CallBackRef, u32 Event, unsigned int EventData)
{
	if(Event == XUARTPS_EVENT_RECV_DATA)
	{
		BaseType_t xHigherPriorityTaskWoken = pdFALSE;

		if( AppCliRxCharQueue != NULL )
		{
			xQueueSendFromISR( AppCliRxCharQueue, (u8*)&cliRxChar, &xHigherPriorityTaskWoken );

			portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
		}
	}

	/* Enable Receive */
	XUartPs_Recv(&AppCliUart, (u8*)&cliRxChar, 1);
}

static void AppCli_TxTask( void *pvParameters )
{
	u8 c;

	for( ;; )
	{
		xQueueReceive( AppCliTxCharQueue, (void *)&c, portMAX_DELAY);

		while(XUartPs_IsSending(&AppCliUart)) vTaskDelay(1);

		XUartPs_Send(&AppCliUart, &c, 1);
	}
}

static void AppCli_RxTask( void *param )
{
  uint8_t c;

	for( ;; )
	{
		xQueueReceive( AppCliRxCharQueue, (void *)&c, portMAX_DELAY );

  	Cli_ProcessChar((Cli_t*)param, c);
	}
}

static void AppCli_Callback( const char *s, void *param )
{
  if(s == NULL)
    return;

  uint16_t len = strlen(s);

  if( APP_CLI_TX_QUEUE_SIZE > len )
  {
    while(len > 0)
    {
      xQueueSend( AppCliTxCharQueue, s++, 1);
      len--;
    }
  }
}

static void AppCli_CliLs(Cli_t *CliInstance, const char *cmd, void *userData)
{
  int status;
  DIR dp;
  const TCHAR path[10] = FF_LOGICAL_DRIVE_PATH;
  FILINFO fno;

  if((status = f_opendir(&dp, path)) != XST_SUCCESS)
    printf("Failure\r\n");

  for (;;)
  {
    status = f_readdir(&dp, &fno);

    if ((status != FR_OK ) || (fno.fname[0] == 0))
      break;

    xil_printf("%s (%ld bytes)\r\n", fno.fname, fno.fsize);
  }

  xil_printf("\r\n\r\n");

  f_closedir(&dp);
}

static const CliCmd_t AppCliLsDef =
{
  "ls",
  "ls: List files \r\n"
  "ls < >\r\n\n",
  (CliCmdFn_t)AppCli_CliLs,
  0,
  NULL
};

static void AppCli_CliCls(Cli_t *CliInstance, const char *cmd, void *userData)
{
  CliInstance->Callback( "\e[1;1H\e[2J", CliInstance->CallbackRef );
}

static const CliCmd_t AppCliClsDef =
{
  "cls",
  "cls: Clear screen \r\n"
  "cls < >\r\n\n",
  (CliCmdFn_t)AppCli_CliCls,
  0,
  NULL
};

static void AppCli_TaskInfo(Cli_t *CliInstance, const char *cmd, void *userData)
{
  printf( "Name            Stack   Usage\r\n");
  printf( "-----------------------------\r\n");


  uint32_t TotalTime;
  uint32_t Percentage;

  uint32_t TaskCnt = uxTaskGetNumberOfTasks();
  TaskStatus_t *TaskStatus = pvPortMalloc( TaskCnt * sizeof( TaskStatus_t ) );

  if( TaskStatus != NULL )
  {
    /* Generate the (binary) data. */
    TaskCnt = uxTaskGetSystemState( TaskStatus, TaskCnt, &TotalTime );

    /* For percentage calculations. */
    TotalTime /= 100UL;

    /* Avoid divide by zero errors. */

    /* Create a human readable table from the binary data. */
    for( int x = 0; x < TaskCnt; x++ )
    {
    	if( TotalTime > 0UL )
    	{
    		Percentage = TaskStatus[x].ulRunTimeCounter / TotalTime;
    	}
    	else
    	{
    		Percentage = 0;
    	}

    	printf("%s", TaskStatus[x].pcTaskName);

    	/* Pad the end of the string with spaces to ensure columns line up when printed out. */
    	for(int i = strlen( TaskStatus[x].pcTaskName ); i < ( configMAX_TASK_NAME_LEN - 1 ); i++ )
    	{
    		printf("%s"," ");
    	}

    	printf("\t%lu\t%u%%\r\n", TaskStatus[x].usStackHighWaterMark*4, (uint16_t) Percentage );
    }

    vPortFree( TaskStatus );
  }

  size_t remainingHeap = xPortGetFreeHeapSize( );

  size_t minHeap = xPortGetMinimumEverFreeHeapSize( );

  printf("Remaining Heap = %d bytes\r\n",remainingHeap);
  printf("Minimum Heap = %d bytes\r\n",minHeap);

}

static const CliCmd_t AppCliTaskInfoDef =
{
  "TaskInfo",
  "TaskInfo: Get Task Info \r\n"
  "TaskInfo < >\r\n\n",
  (CliCmdFn_t)AppCli_TaskInfo,
  0,
  NULL
};

static void AppCli_fdelete(Cli_t *CliInstance, const char *cmd, void *userData)
{
  char *filename;

  if((filename = calloc(1, FF_FILENAME_MAX_LEN )) == NULL)
  {
    printf("Memory Error\r\n");
    return;
  }

  strcpy(filename,FF_LOGICAL_DRIVE_PATH);

  Cli_GetParameter(cmd, 1, CliParamTypeStr, &filename[strlen(filename)]);

  if( f_unlink(filename) != FR_OK)
  {
    printf("Failed\r\n");
  }
  else
  {
    printf("Success\r\n");
  }

  free(filename);
}

static const CliCmd_t AppCliDeleteFileDef =
{
  "fdelete",
  "fdelete: Delete file \r\n"
  "fdelete < filename >\r\n\n",
  (CliCmdFn_t)AppCli_fdelete,
  1,
  NULL
};

static void AppCli_fread(Cli_t *CliInstance, const char *cmd, void *userData)
{
  int32_t offset;
  int32_t length;
  UINT len;
  FIL fil;
  int32_t status = -1;
  uint32_t size;
  char c;

  char *filename;

  if((filename = calloc(1, FF_FILENAME_MAX_LEN )) == NULL)
  {
    printf("Memory Error\r\n");
    return;
  }

  strcpy(filename,FF_LOGICAL_DRIVE_PATH);

  Cli_GetParameter(cmd, 1, CliParamTypeStr, &filename[strlen(filename)]);
  Cli_GetParameter(cmd, 2, CliParamTypeS32, &offset);
  Cli_GetParameter(cmd, 3, CliParamTypeS32, &length);

  do
  {
    /* Open File */
    if(f_open(&fil, filename, FA_OPEN_EXISTING | FA_READ) != FR_OK) break;

    /* Get Size of file */
    size = f_size(&fil);

    if(length == -1) length = size;

    size = size - offset;

    if(size < length)
      length = size;

    /* Check length */
    if(length > APP_CLI_PRINT_BUF_SIZE ) break;

    /* Pointer to beginning of file */
    if(f_lseek(&fil, offset) != FR_OK) break;

    for(int i = 0; i < length; i++)
    {
      /* Read data from file */
      if(f_read(&fil, (void*)&c, 1, (UINT*)&len) != FR_OK) break;

      printf("%c",c);
    }

  }while(0);

  if(status == 0)
  {
    printf("Failed\r\n");
  }

  free(filename);
  f_close(&fil);
}

static const CliCmd_t AppCliReadFileDef =
{
  "fread",
  "fread: Read contents of a file \r\n"
  "fread < filename, offset, length >\r\n\n",
  (CliCmdFn_t)AppCli_fread,
  3,
  NULL
};

static void AppCli_GetTemp(Cli_t *CliInstance, const char *cmd, void *userData)
{
  int16_t Temp_C;
  float cpuTemp;

//  if(PhyAdrv9001_GetTemperature( &Temp_C ) == Adrv9001Status_Success)
//  {
//    App_GetCpuTemp( &cpuTemp );
//    printf("Adrv9001 = %dC; CPU = %dC\r\n", Temp_C,(int)cpuTemp);
//  }
//  else
//  {
//    printf("Failed\r\n");
//  }
}

static const CliCmd_t AppCliGetTempDef =
{
  "GetTemp",
  "GetTemp: Read temperature \r\n"
  "GetTemp < >\r\n\n",
  (CliCmdFn_t)AppCli_GetTemp,
  0,
  NULL
};

static void AppCli_Reboot(Cli_t *CliInstance, const char *cmd, void *userData)
{
  App_Reboot();
}

static const CliCmd_t AppCliRebootDef =
{
  "Reboot",
  "Reboot: Reboot processor \r\n"
  "Reboot <  >\r\n\n",
  (CliCmdFn_t)AppCli_Reboot,
  0,
  NULL
};

static void AppCli_ResetPl(Cli_t *CliInstance, const char *cmd, void *userData)
{
  App_ResetPl();
}

static const CliCmd_t AppCliResetPlDef =
{
  "ResetPl",
  "ResetPl: Reset programmable logic \r\n"
  "ResetPl <  >\r\n\n",
  (CliCmdFn_t)AppCli_ResetPl,
  0,
  NULL
};

static void AppCli_ClearLog(Cli_t *CliInstance, const char *cmd, void *userData)
{
  int status;

  f_sync(AppCliLogFil);

  f_close(AppCliLogFil);

  /* Delete PHY Log file */
  f_unlink(APP_LOG_FILENAME);

  do
  {
    /* Open File */
    if( (status = f_open(AppCliLogFil, APP_LOG_FILENAME, FA_CREATE_NEW | FA_WRITE)) != FR_OK)
      break;

    /* Pointer to beginning of file */
    if((status = f_lseek(AppCliLogFil, 0)) != FR_OK)
      break;

    f_sync(AppCliLogFil);

  }while(0);

  if( status == 0)
    printf("Success\r\n");
  else
    printf("Failed\r\n");

}

static const CliCmd_t AppCliClearLogDef =
{
  "ClearLog",
  "ClearLog: Clear log file \r\n"
  "ClearLog <  >\r\n\n",
  (CliCmdFn_t)AppCli_ClearLog,
  0,
  NULL
};

static void AppCli_ReadLog(Cli_t *CliInstance, const char *cmd, void *userData)
{
  int32_t status;
  UINT len;
  char c;

  do
  {
    f_sync(AppCliLogFil);

    /* Get Size of file */
    int32_t length = f_size(AppCliLogFil);

    /* Pointer to beginning of file */
    if((status = f_lseek(AppCliLogFil, 0)) != FR_OK) break;

    for(int i = 0; i < length; i++)
    {
      /* Read data from file */
      if((status = f_read(AppCliLogFil, (void*)&c, 1, (UINT*)&len)) != FR_OK) break;

      printf("%c",c);
    }

  }while(0);

  if( status == 0 )
    printf("Success\r\n");
  else
    printf("Failed\r\n");

}

static const CliCmd_t AppCliReadLogDef =
{
  "ReadLog",
  "ReadLog: Read log file \r\n"
  "ReadLog <  >\r\n\n",
  (CliCmdFn_t)AppCli_ReadLog,
  0,
  NULL
};

int32_t AppCli_Initialize( FIL *LogFil, Cli_t *Cli )
{
	XUartPs_Config *Config;

	/* Copy Configuration */
	AppCliLogFil = LogFil;

	/* Lookup Configuration */
	if((Config = XUartPs_LookupConfig(APP_CLI_UART_DEVICE_ID)) == NULL) return XST_FAILURE;

	/* Initialize Driver */
	if(XUartPs_CfgInitialize(&AppCliUart, Config, Config->BaseAddress) != XST_SUCCESS) return XST_FAILURE;

	/* Self Test */
	if(XUartPs_SelfTest(&AppCliUart) != XST_SUCCESS) return XST_FAILURE;

	/* Connect UART handler */
	XScuGic_Connect(&xInterruptController, APP_CLI_UART_INTR_ID,
	    (Xil_ExceptionHandler) XUartPs_InterruptHandler, (void *) &AppCliUart);

	/* Set Local Handler */
	XUartPs_SetHandler(&AppCliUart, (XUartPs_Handler)CLI_UartHandler, &AppCliUart);

	/* Setup Interrupt Mask */
	XUartPs_SetInterruptMask(&AppCliUart, XUARTPS_IXR_RXFULL | XUARTPS_IXR_RXOVR );

	XUartPs_SetRecvTimeout(&AppCliUart, 0);

	XUartPs_SetFifoThreshold(&AppCliUart, 1);

	/* Enable the Interrupt */
	XScuGic_Enable(&xInterruptController, APP_CLI_UART_INTR_ID);

	XUartPs_Recv(&AppCliUart, (u8*)&cliRxChar, 1);

	/* Set Priority */
	XScuGic_SetPriorityTriggerType(&xInterruptController, APP_CLI_UART_INTR_ID, 0xA0, 0x3);

	/* Create CLI Configuration */
	CliCfg_t CliCfg = {
	    .CmdList          = AppCliCmdList,
	    .CmdListSize      = APP_CLI_CMD_LIST_SIZE,
	    .CmdBuf           = AppCliCmdBuf,
	    .CmdBufSize       = APP_CLI_CMD_BUF_SIZE,
	    .HistoryBuf       = AppCliHistoryBuf,
	    .HistoryBufSize   = APP_CLI_HISTORY_BUF_SIZE,
	    .Callback         = AppCli_Callback,
	    .CallbackRef      = NULL
	};

	/* Initialize CLI */
	CLi_Init( Cli, &CliCfg );

	/* Create Rx Queue */
	AppCliRxCharQueue = xQueueCreate(APP_CLI_RX_QUEUE_SIZE, sizeof(uint8_t));

	/* Create Tx Queue */
	AppCliTxCharQueue = xQueueCreate(APP_CLI_TX_QUEUE_SIZE, sizeof(uint8_t));

	/* Create CLI Tx Task */
	if(xTaskCreate(AppCli_TxTask, APP_CLI_TX_TASK_NAME, APP_CLI_TX_STACK_SIZE, Cli, APP_CLI_TX_TASK_PRIORITY, NULL) != pdPASS)
	  return XST_FAILURE;

	/* Create CLI Rx Task */
	if(xTaskCreate(AppCli_RxTask, APP_CLI_RX_TASK_NAME, APP_CLI_RX_STACK_SIZE, Cli, APP_CLI_RX_TASK_PRIORITY, NULL) != pdPASS)
	  return XST_FAILURE;

	/* Register APP Specific */
	Cli_RegisterCommand(Cli, &AppCliClsDef);
	Cli_RegisterCommand(Cli, &AppCliLsDef);
	Cli_RegisterCommand(Cli, &AppCliTaskInfoDef);
	Cli_RegisterCommand(Cli, &AppCliReadFileDef);
	Cli_RegisterCommand(Cli, &AppCliDeleteFileDef);
	Cli_RegisterCommand(Cli, &AppCliGetTempDef);
	Cli_RegisterCommand(Cli, &AppCliRebootDef);
	Cli_RegisterCommand(Cli, &AppCliResetPlDef);
	Cli_RegisterCommand(Cli, &AppCliClearLogDef);
	Cli_RegisterCommand(Cli, &AppCliReadLogDef);

	return XST_SUCCESS;
}
