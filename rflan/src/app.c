/*
 * main.c
 *
 *  Created on: Dec 17, 2019
 *      Author: enelson
 */
#include <stdlib.h>
#include "xparameters.h"
#include "xil_printf.h"
#include "FreeRTOS.h"
#include "task.h"
#include "app.h"
#include "app_cli.h"
#include "adrv9001.h"
#include "sd.h"


static TaskHandle_t 			AppTask;

static void APP_Task( void *pvParameters )
{
	int status;

  /* Initialize SD */
  SD_Init();

	/* Initialize CLI */
	if((status = AppCli_Initialize()) != 0)
	  xil_printf("CLI Initialize Error %d\r\n",status);

	xil_printf("\r\n\r\n\r\n");
	xil_printf("************************************************\r\n");
	xil_printf("    BytePipe_x900x RFLAN - Version %d.%d.%d\r\n", APP_FW_VER_MAJOR, APP_FW_VER_MINOR, APP_FW_VER_REV );
	xil_printf("************************************************\r\n");

	xil_printf("\r\nType help for a list of commands\r\n\r\n");


	/* Initialize ADRV9001 */
	if((status = Adrv9001_Initialize( NULL )) != 0)
    printf("Adrv9001 Initialize Error %d\r\n",status);

  /* Load ADRV9001 Profile */
  if((status = Adrv9001_LoadProfile( )) != 0)
    printf("Adrv9001 Load Profile Error %d\r\n",status);

  /* Begin Receiving */
  if((status = Adrv9001_BeginReceiving( )) != 0)
    printf("Adrv9001 Begin Receiving Error %d\r\n",status);

  /* Begin Transmitting */
  if((status = Adrv9001_BeginTransmitting( )) != 0)
    printf("Adrv9001 Begin Transmitting Error %d\r\n",status);



	for( ;; )
	{
		vTaskDelay(1000);
	}
}


int main()
{
	/* Create App Task */
	xTaskCreate( APP_Task,
			( const char * ) APP_TASK_NAME,
			APP_TASK_STACK_SIZE,
			NULL,
			APP_TASK_PRIORITY,
			&AppTask );

	/* Start RTOS */
	vTaskStartScheduler( );

	for( ;; );
}
