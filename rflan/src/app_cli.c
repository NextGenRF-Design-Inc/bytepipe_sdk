#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "cli.h"
#include "xil_printf.h"
#include "xparameters.h"
#include "xil_types.h"
#include "xstatus.h"
#include "xscugic.h"
#include "string.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "portmacro.h"
#include "app.h"
#include "ff.h"


#if CLI_UART_DEVICE_ID == XPAR_PSU_UART_0_DEVICE_ID
#define CLI_UART_PS
#elif CLI_UART_DEVICE_ID == XPAR_PSU_UART_1_DEVICE_ID
#define CLI_UART_PS
#endif

#ifdef CLI_UART_PS
#include "xuartps.h"
#else
#include "xuartlite.h"
#endif

static Cli_t            AppCli;
static QueueHandle_t    AppCliRxCharQueue;
static QueueHandle_t  	AppCliTxCharQueue;
static volatile u8 		  cliRxChar;
extern XScuGic 			    xInterruptController;
static char             AppCliHistoryBuf[ APP_CLI_HISTORY_BUF_SIZE ];
static char             AppCliCmdBuf[ APP_CLI_CMD_BUF_SIZE ];
static CliCmd_t const  *AppCliCmdList[ APP_CLI_CMD_LIST_SIZE ];

#ifdef CLI_UART_PS
XUartPs cliUart;
#else
XUartLite cliUart;
#endif

/**
* 	Outbyte for printing to console
*/
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


/**
* 	UART Handler
*/
#ifdef CLI_UART_PS
static void CLI_UartHandler(void *CallBackRef, u32 Event, unsigned int EventData)
{
	if(Event == XUARTPS_EVENT_RECV_DATA)
	{
		BaseType_t xHigherPriorityTaskWoken = pdFALSE;

		if( AppCliRxCharQueue != NULL )
		{
			xQueueSendFromISR( AppCliRxCharQueue, &cliRxChar, &xHigherPriorityTaskWoken );

			portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
		}
	}

	/* Enable Receive */
	XUartPs_Recv(&cliUart, (u8*)&cliRxChar, 1);
}
#else
static void CLI_RxUartHandler(void *CallBackRef, unsigned int EventData)
{
	/* Send Rx Byte to Queue */
	xQueueSendFromISR( cliRxCharQueue, &cliRxChar, 1 );

	/* Echo */
	outbyte(cliRxChar);

	/* Enable Receive */
	XUartLite_Recv(&cliUart, (u8*)&cliRxChar, 1);
}
#endif


/**
*  CLI Task
*/
static void AppCli_TxTask( void *pvParameters )
{
	u8 c;

	for( ;; )
	{
		xQueueReceive( AppCliTxCharQueue, (void *)&c, portMAX_DELAY);

#ifdef CLI_UART_PS

		while(XUartPs_IsSending(&cliUart)) vTaskDelay(1);

		XUartPs_Send(&cliUart, &c, 1);

#else
		while(XUartLite_IsSending(&cliUart)) vTaskDelay(1);

		XUartLite_Send(&cliUart, &c, 1);

#endif
	}
}

/******************************************************************************/
/**
*  \details   CLI Rx Task receives one character at a time from queue and
*  						passes to CLI library for processing.
*
*  \param 	  param is a pointer to the CLI instance
*
*  \return    none
*******************************************************************************/
static void AppCli_RxTask( void *param )
{
  uint8_t c;

	for( ;; )
	{
		xQueueReceive( AppCliRxCharQueue, (void *)&c, portMAX_DELAY );

		Cli_ProcessChar((Cli_t*)param, c);
	}
}

/******************************************************************************/
/**
*  \details   CLI Command for Processing List Files Command
*
*  \param     CliInstance
*
*  \param     cmd is the command string
*
*  \param     NULL
*
*  \return    none
*******************************************************************************/
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

    xil_printf("%s\r\n", fno.fname);
  }

  f_closedir(&dp);
}

/******************************************************************************/
/**
*  \details   CLI Command for Processing Clear Screen Command
*
*  \param     CliInstance
*
*  \param     cmd is the command string
*
*  \param     NULL
*
*  \return    none
*******************************************************************************/
static void AppCli_CliCls(Cli_t *CliInstance, const char *cmd, void *userData)
{
  CliInstance->Callback( "\e[1;1H\e[2J", CliInstance->CallbackRef );
}

/******************************************************************************/
/**
*  \details   CLI Command for Processing Get Task Info
*
*  \param     CliInstance
*
*  \param     cmd is the command string
*
*  \param     NULL
*
*  \return    none
*******************************************************************************/
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

    	printf("\t%u\t%u%%\r\n", TaskStatus[x].usStackHighWaterMark*4,
    			(uint16_t) Percentage );
    }

    vPortFree( TaskStatus );
  }

}

/******************************************************************************/
/**
*  \details   CLI Command for Processing fread
*
*  \param     CliInstance
*
*  \param     cmd is the command string
*
*  \param     NULL
*
*  \return    none
*******************************************************************************/
static void AppCli_fread(Cli_t *CliInstance, const char *cmd, void *userData)
{
  int32_t offset;
  int32_t length;
  FIL fil;
  int32_t status = -1;
  uint32_t size;
  uint8_t *Buf = NULL;

  char *filename = calloc(1, FF_FILENAME_MAX_LEN );
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

    /* Allocate Buffer */
    if((Buf = malloc(length)) == NULL) break;

    /* Read data from file */
    if(f_read(&fil, Buf, length, (UINT*)&length) != FR_OK) break;

    printf("%s\r\n",Buf);

  }while(0);

  if(status == 0)
  {
    printf("Failed\r\n");
  }

  free(filename);
  f_close(&fil);
  free(Buf);
}

/******************************************************************************/
/**
*  \details   CLI Callback
*
*  \param 	  param is a pointer to the CLI instance
*
*  \return    none
*******************************************************************************/
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


/**
*  Clear Screen
*/
static const CliCmd_t AppCliClsDef =
{
  "cls",
  "cls: Clear screen \r\n"
  "cls < >\r\n\n",
  (CliCmdFn_t)AppCli_CliCls,
  0,
  NULL
};

/**
*  List Files
*/
static const CliCmd_t AppCliLsDef =
{
  "ls",
  "ls: List files \r\n"
  "ls < >\r\n\n",
  (CliCmdFn_t)AppCli_CliLs,
  0,
  NULL
};

/**
*  List Files
*/
static const CliCmd_t AppCliTaskInfoDef =
{
  "TaskInfo",
  "TaskInfo: Get Task Info \r\n"
  "TaskInfo < >\r\n\n",
  (CliCmdFn_t)AppCli_TaskInfo,
  0,
  NULL
};

/**
*  Read File
*/
static const CliCmd_t AppCliReadFileDef =
{
  "fread",
  "fread: Read contents of a file \r\n"
  "fread < filename, offset, length >\r\n\n",
  (CliCmdFn_t)AppCli_fread,
  3,
  NULL
};

Cli_t *AppCli_GetInstance( void )
{
	return &AppCli;
}

/*******************************************************************************

  PURPOSE:  Initialize APP CLI

  COMMENT:

*******************************************************************************/
int AppCli_Initialize( void )
{
#ifdef CLI_UART_PS

	XUartPs_Config *Config;

	/* Lookup Configuration */
	if((Config = XUartPs_LookupConfig(APP_CLI_UART_DEVICE_ID)) == NULL) return XST_FAILURE;

	/* Initialize Driver */
	if(XUartPs_CfgInitialize(&cliUart, Config, Config->BaseAddress) != XST_SUCCESS) return XST_FAILURE;

	/* Self Test */
	if(XUartPs_SelfTest(&cliUart) != XST_SUCCESS) return XST_FAILURE;

	/* Connect UART handler */
	XScuGic_Connect(&xInterruptController, APP_CLI_UART_INTR_ID,
			(Xil_ExceptionHandler) XUartPs_InterruptHandler, (void *) &cliUart);

	/* Set Local Handler */
	XUartPs_SetHandler(&cliUart, (XUartPs_Handler)CLI_UartHandler, &cliUart);

	/* Setup Interrupt Mask */
	XUartPs_SetInterruptMask(&cliUart, XUARTPS_IXR_RXFULL | XUARTPS_IXR_RXOVR );

	XUartPs_SetRecvTimeout(&cliUart, 0);

	XUartPs_SetFifoThreshold(&cliUart, 1);

	/* Enable the Interrupt */
	XScuGic_Enable(&xInterruptController, APP_CLI_UART_INTR_ID);

	XUartPs_Recv(&cliUart, (u8*)&cliRxChar, 1);

#else
	/* Initialize Controller */
	XUartLite_Initialize(&cliUart, CLI_UART_DEVICE_ID);

	/* Connect Interrupt Handler */
	XScuGic_Connect(&xInterruptController, CLI_UART_INTR_ID,
			   (XInterruptHandler)XUartLite_InterruptHandler, (void *)&cliUart);

	/* Connect Rx Handler */
	XUartLite_SetRecvHandler(&cliUart, CLI_RxUartHandler, &cliUart);

	/* Enable Interrupt */
	XUartLite_EnableInterrupt(&cliUart);

	/* Start Receiving */
	XUartLite_Recv(&cliUart, (u8*)&cliRxChar, 1);

#endif

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
	  CLi_Init( &AppCli, &CliCfg );

	  /* Create Rx Queue */
	  AppCliRxCharQueue = xQueueCreate(APP_CLI_RX_QUEUE_SIZE, sizeof(uint8_t));

	  /* Create Tx Queue */
	  AppCliTxCharQueue = xQueueCreate(APP_CLI_TX_QUEUE_SIZE, sizeof(uint8_t));

	  /* Create CLI Tx Task */
	  if(xTaskCreate(AppCli_TxTask, APP_CLI_TX_TASK_NAME, APP_CLI_TX_STACK_SIZE, &AppCli, APP_CLI_TX_TASK_PRIORITY, NULL) != pdPASS)
	    return XST_FAILURE;

	  /* Create CLI Rx Task */
	  if(xTaskCreate(AppCli_RxTask, APP_CLI_RX_TASK_NAME, APP_CLI_RX_STACK_SIZE, &AppCli, APP_CLI_RX_TASK_PRIORITY, NULL) != pdPASS)
	    return XST_FAILURE;

	  /* Register APP Specific */
	  Cli_RegisterCommand(&AppCli, &AppCliClsDef);
	  Cli_RegisterCommand(&AppCli, &AppCliLsDef);
	  Cli_RegisterCommand(&AppCli, &AppCliTaskInfoDef);
	  Cli_RegisterCommand(&AppCli, &AppCliReadFileDef);

	return XST_SUCCESS;
}
