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
#include "adrv9001_cli.h"
#include "sd.h"
#include "phy.h"


static TaskHandle_t 			AppTask;

static void App_PhyCallback( phy_evt_t Evt, void *param )
{

}

static void App_Task( void *pvParameters )
{
	int status;

  /* Initialize SD */
  SD_Init();

	/* Initialize CLI */
	if((status = AppCli_Initialize()) != 0)
	  xil_printf("CLI Initialize Error %d\r\n",status);

  /* Initialize ADRV9001 CLI */
  if((status = Adrv9001Cli_Initialize()) != 0)
    xil_printf("Adrv9001 CLI Initialize Error %d\r\n",status);

	xil_printf("\r\n\r\n\r\n");
	xil_printf("************************************************\r\n");
	xil_printf("        BytePipe_x900x RFLAN - v%d.%d.%d\r\n", APP_FW_VER_MINOR, APP_FW_VER_REV, APP_FW_VER_MAJOR );
	xil_printf("************************************************\r\n");

	xil_printf("\r\nType help for a list of commands\r\n\r\n");

	phy_cfg_t PhyCfg = {
	    .Callback     = App_PhyCallback,
	    .CallbackRef  = NULL
	};

  /* Initialize PHY */
  if((status = Phy_Initialize( &PhyCfg )) != 0)
    printf("Phy Initialize Error %d\r\n",status);





	for( ;; )
	{
		vTaskDelay(1000);
	}
}


int main()
{
	/* Create App Task */
	xTaskCreate( App_Task,
			( const char * ) APP_TASK_NAME,
			APP_TASK_STACK_SIZE,
			NULL,
			APP_TASK_PRIORITY,
			&AppTask );

	/* Start RTOS */
	vTaskStartScheduler( );

	for( ;; );
}
