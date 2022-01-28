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
#include "xsysmonpsu.h"
#include "xresetps.h"


static TaskHandle_t 			AppTask;
static XSysMonPsu         AppSysMon;
static XResetPs           AppReset;
static FATFS              AppFatFs;
static FIL                AppLogFil;

static int32_t App_ResetInitialize( void )
{
  XResetPs_Config *Cfg = XResetPs_LookupConfig(XPAR_XRESETPS_DEVICE_ID);

  return XResetPs_CfgInitialize(&AppReset, Cfg);
}

static int32_t App_SysMonInitialize( void )
{
  int Status;
  XSysMonPsu_Config *ConfigPtr;
  u64 IntrStatus;
  XSysMonPsu *SysMonInstPtr = &AppSysMon;

  /* Initialize the SysMon driver. */
  if((ConfigPtr = XSysMonPsu_LookupConfig(SYSMON_DEVICE_ID)) == NULL )
    return XST_FAILURE;

  XSysMonPsu_CfgInitialize(SysMonInstPtr, ConfigPtr, ConfigPtr->BaseAddress);

  /* Self Test the System Monitor device */
  if((Status = XSysMonPsu_SelfTest(SysMonInstPtr)) != XST_SUCCESS)
    return Status;

  /* Disable the Channel Sequencer */
  XSysMonPsu_SetSequencerMode(SysMonInstPtr, XSM_SEQ_MODE_SAFE, XSYSMON_PS);

  /* Disable all the alarms in the Configuration Register 1. */
  XSysMonPsu_SetAlarmEnables(SysMonInstPtr, 0x0, XSYSMON_PS);

  /* Setup Averaging */
  XSysMonPsu_SetAvg(SysMonInstPtr, XSM_AVG_16_SAMPLES, XSYSMON_PS);

  /* Setup the Sequencer */
  if((Status = XSysMonPsu_SetSeqInputMode(SysMonInstPtr, XSYSMONPSU_SEQ_CH1_VAUX00_MASK << 16, XSYSMON_PS)) != XST_SUCCESS)
    return Status;

  if((Status = XSysMonPsu_SetSeqAcqTime(SysMonInstPtr, (XSYSMONPSU_SEQ_CH1_VAUX0F_MASK | XSYSMONPSU_SEQ_CH1_VAUX00_MASK) << 16, XSYSMON_PS)) != XST_SUCCESS)
    return Status;

  /* Enable the averaging */
  Status =  XSysMonPsu_SetSeqAvgEnables(SysMonInstPtr, XSYSMONPSU_SEQ_CH0_TEMP_MASK |
      XSYSMONPSU_SEQ_CH0_SUP1_MASK |
      XSYSMONPSU_SEQ_CH0_SUP3_MASK |
      ((XSYSMONPSU_SEQ_CH1_VAUX00_MASK |
      XSYSMONPSU_SEQ_CH1_VAUX0F_MASK) << 16) |
      XSYSMONPSU_SEQ_CH0_CALIBRTN_MASK, XSYSMON_PS);

  if (Status != XST_SUCCESS)
    return XST_FAILURE;

  /* Enable channels */
  Status =  XSysMonPsu_SetSeqChEnables(SysMonInstPtr, XSYSMONPSU_SEQ_CH0_TEMP_MASK |
      XSYSMONPSU_SEQ_CH0_SUP1_MASK |
      XSYSMONPSU_SEQ_CH0_SUP3_MASK |
      ((XSYSMONPSU_SEQ_CH1_VAUX00_MASK |
      XSYSMONPSU_SEQ_CH1_VAUX0F_MASK) << 16) |
      XSYSMONPSU_SEQ_CH0_CALIBRTN_MASK, XSYSMON_PS);

  if (Status != XST_SUCCESS)
    return XST_FAILURE;

  /* Clear any bits set in the Interrupt Status Register. */
  IntrStatus = XSysMonPsu_IntrGetStatus(SysMonInstPtr);
  XSysMonPsu_IntrClear(SysMonInstPtr, IntrStatus);

  /* Enable the Channel Sequencer in continuous sequencer cycling mode. */
  XSysMonPsu_SetSequencerMode(SysMonInstPtr, XSM_SEQ_MODE_CONTINPASS, XSYSMON_PS);

  /* Wait till the End of Sequence occurs */
  while ((XSysMonPsu_IntrGetStatus(SysMonInstPtr) & ((u64)XSYSMONPSU_ISR_1_EOS_MASK<< 32)) !=
      ((u64)XSYSMONPSU_ISR_1_EOS_MASK<< 32));

  return Status;
}

static int32_t App_WriteLogFile( const char *str )
{
  UINT len = 0;
  int status;

  if((status = f_write(&AppLogFil, (const void*)str, strlen(str), (UINT*)&len)) != FR_OK)
    return status;

  f_sync(&AppLogFil);

  return FR_OK;
}

static int32_t App_CloseLogFile( void )
{
  f_sync(&AppLogFil);

  return f_close(&AppLogFil);
}

static int32_t App_OpenLogFile( void )
{
  int status;

  /* Delete PHY Log file */
  f_unlink(APP_LOG_FILENAME);

  /* Open File */
  if( (status = f_open(&AppLogFil, APP_LOG_FILENAME, FA_CREATE_NEW | FA_WRITE | FA_READ)) != FR_OK)
    return status;

  /* Pointer to beginning of file */
  if((status = f_lseek(&AppLogFil, 0)) != FR_OK)
    return status;

  f_sync(&AppLogFil);

  return FR_OK;
}

static void App_PhyCallback( phy_evt_type_t EvtType, phy_evt_data_t EvtData, void *param )
{
  if( EvtType == PhyEvtType_LogWrite )
  {
    App_WriteLogFile( EvtData.Message );
  }
}

static int32_t App_Initialize( void )
{
  int status;

  /* Mount File System */
  if(f_mount(&AppFatFs, FF_LOGICAL_DRIVE_PATH, 1) != FR_OK)
    printf("Failed to initialize file system\r\n");

  /* Open Log File */
  if(App_OpenLogFile( ) != FR_OK)
    printf("Failed to open log file\r\n");

  /* Initialize CLI */
  if((status = AppCli_Initialize( &AppLogFil )) != 0)
    printf("CLI Initialize Error %d\r\n",status);

  /* Initialize System Monitor */
  if((status = App_SysMonInitialize()) != 0)
    printf("System Monitor Initialize Error %d\r\n",status);

  /* Initialize System Reset */
  if((status = App_ResetInitialize()) != 0)
    printf("System Reset Initialize Error %d\r\n",status);

  /* Initialize ZMODEM */
  if((status = ZModem_Initialize(FF_LOGICAL_DRIVE_PATH, outbyte)) != 0)
    printf("ZMODEM Initialize Error %d\r\n",status);

  /* Initialize Clocks */
  if(VersaClock5_Initialize() != 0)
    printf("Failed to initialize external clock driver\r\n");

  /* Initialize ADRV9001 CLI */
  if((status = Adrv9001Cli_Initialize()) != 0)
    printf("Adrv9001 CLI Initialize Error %d\r\n",status);

  char *Buf = calloc(1, 1024);

  snprintf( &Buf[strlen(Buf)], 1023, "\r\n\r\n\r\n" );
  snprintf( &Buf[strlen(Buf)], 1023, "************************************************\r\n");
  snprintf( &Buf[strlen(Buf)], 1023, "        BytePipe_x900x RFLAN - v%d.%d.%d\r\n", APP_FW_VER_MINOR, APP_FW_VER_REV, APP_FW_VER_MAJOR );
  snprintf( &Buf[strlen(Buf)], 1023, "************************************************\r\n");

  App_WriteLogFile(Buf);
  print(Buf);
  free(Buf);

  printf("\r\nType help for a list of commands\r\n\r\n");

  phy_cfg_t Cfg = {
    .Callback = App_PhyCallback,
    .CallbackRef = NULL,
  };

  /* Initialize PHY */
  if((status = Phy_Initialize( &Cfg )) != 0)
    printf("Phy Initialize Error %d\r\n",status);

  return status;
}

static void App_Task( void *pvParameters )
{
  /* Initialize Application Routines */
  App_Initialize( );

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

	App_CloseLogFile( );
}

int32_t App_ResetPl( void )
{
  return XResetPs_ResetPulse(&AppReset, XRESETPS_RSTID_PL);
}

void App_Reboot( void )
{
  XResetPs_ResetPulse(&AppReset, XRESETPS_RSTID_SOFT);
}

int32_t App_GetCpuTemp( float *Temp )
{
  u32 TempRawData;

  TempRawData = XSysMonPsu_GetAdcData(&AppSysMon, XSM_CH_TEMP, XSYSMON_PS);
  *Temp = XSysMonPsu_RawToTemperature_OnChip(TempRawData);

  return XST_SUCCESS;
}

