/***************************************************************************//**
*  \addtogroup APP
*   @{
*******************************************************************************/
/***************************************************************************//**
*  \file       app.c
*
*  \details    This file contains the RFLAN application.
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

#include <stdlib.h>
#include <stdio.h>
#include "parameters.h"
#include "xil_printf.h"
#include "FreeRTOS.h"
#include "task.h"
#include "app.h"
#include "app_cli.h"
#include "adrv9001_cli.h"
#include "phy.h"
#include "ff.h"
#include "zmodem.h"
#include "xdppsu.h"
#include "versa_clock5.h"

static TaskHandle_t 			AppTask;
FATFS sdfs;


static void App_Task( void *pvParameters )
{
	int status;

  /* Mount File System */
  if(f_mount(&sdfs, FF_LOGICAL_DRIVE_PATH, 1) != FR_OK)
    xil_printf("Failed to initialize file system\r\n");

	/* Initialize CLI */
	if((status = AppCli_Initialize()) != 0)
	  xil_printf("CLI Initialize Error %d\r\n",status);

	/* Initialize ZMODEM */
	if((status = ZModem_Initialize(FF_LOGICAL_DRIVE_PATH, outbyte)) != 0)
		xil_printf("ZMODEM Initialize Error %d\r\n",status);
  
  /* Initialize Clocks */
  if(VersaClock5_Initialize() != 0)
    xil_printf("Failed to initialize external clock driver\r\n");

  /* Initialize ADRV9001 CLI */
  if((status = Adrv9001Cli_Initialize()) != 0)
    xil_printf("Adrv9001 CLI Initialize Error %d\r\n",status);

	xil_printf("\r\n\r\n\r\n");
	xil_printf("************************************************\r\n");
	xil_printf("        BytePipe_x900x RFLAN - v%d.%d.%d\r\n", APP_FW_VER_MINOR, APP_FW_VER_REV, APP_FW_VER_MAJOR );
	xil_printf("************************************************\r\n");

	xil_printf("\r\nType help for a list of commands\r\n\r\n");

  /* Initialize PHY */
  if((status = Phy_Initialize( )) != 0)
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



