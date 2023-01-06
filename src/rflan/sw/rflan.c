/***************************************************************************//**
*  \addtogroup RFLAN
*   @{
*******************************************************************************/
/***************************************************************************//**
*  \file       rflan.c
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
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "rflan.h"
#include "cli.h"
#include "ff.h"
#include "rflan_uart.h"
#include "rflan_cli.h"
#include "xresetps.h"
#include "adrv9001_cli.h"
#include "rflan_gpio.h"
#include "rflan_pib.h"
#include "status.h"
#include "xiicps.h"

extern XScuGic xInterruptController;       ///< Processor Interrupt Controller Instance

#if FILE_SYSTEM_LOGICAL_DRIVE == 0
#define FILE_SYSTEM_BASE_PATH "0:/"
#else
#define FILE_SYSTEM_BASE_PATH "1:/"
#endif

#ifdef VERSA_CLOCK5_ENABLE
#include "versa_clock5.h"
static versa_clock5_t RflanVersaClock5;
#endif

#ifdef RFLAN_LWIP_ENABLE
#include "rflan_lwip.h"
static rflan_lwip_t   RflanLwip;
#endif

#ifdef RFLAN_STREAM_ENABLE
#include "rflan_stream.h"
static rflan_stream_t RflanStream;
#endif

static XResetPs       RflanReset;
static FATFS          FatFs;
static cli_t          Cli;
static rflan_cli_t    RflanCli;
static rflan_uart_t   RflanUart;
static XGpioPs        RflanGpio;
static rflan_pib_t    RflanPib;
static adrv9001_t     RflanAdrv9001;
static adrv9001_pib_t Adrv9001Pib;
static QueueHandle_t  RflanEvtQueue;
static XIicPs         RflanIic0;

static int32_t RflanIic_Initialize( XIicPs *Instance )
{
  XIicPs_Config *Config;

  Config = XIicPs_LookupConfig(XPAR_PSU_I2C_1_DEVICE_ID);
  if (NULL == Config) {
    return RflanStatus_IicError;
  }

  if(XIicPs_CfgInitialize(Instance, Config, Config->BaseAddress) != 0)
    return RflanStatus_IicError;

  /* Perform a self-test to ensure that the hardware was built correctly */
  if(XIicPs_SelfTest(Instance) != 0)
    return RflanStatus_IicError;

  /* Set the IIC serial clock rate */
  XIicPs_SetSClk(Instance, 100e3);

  return RflanStatus_Success;
}

static int32_t RflanReset_Initialize( XResetPs *Instance )
{
  XResetPs_Config *Cfg = XResetPs_LookupConfig(XPAR_XRESETPS_DEVICE_ID);

  return XResetPs_CfgInitialize(Instance, Cfg);
}

#ifdef RFLAN_STREAM_ENABLE

static void Rflan_StreamCallback( uint32_t Buf, uint32_t Size, rflan_stream_channel_t Channel, void *CallbackRef )
{
  rflan_evt_type_t evt;
    
  if( Channel == RflanStreamChannel_Tx1)
    evt = RflanEvt_Tx1StreamDone;
  else if( Channel == RflanStreamChannel_Tx2)
    evt = RflanEvt_Tx2StreamDone;
  else if( Channel == RflanStreamChannel_Rx1)
    evt = RflanEvt_Rx1StreamDone;
  else
    evt = RflanEvt_Rx2StreamDone;

  BaseType_t xHigherPriorityTaskWoken = pdFALSE;

  xQueueSendFromISR( RflanEvtQueue, &evt, &xHigherPriorityTaskWoken );

  portYIELD_FROM_ISR(xHigherPriorityTaskWoken); 
}

#endif

static void Rflan_CliCallback( cli_evt_t *Evt )
{
  if( Evt->Type == CliEvtType_TxChar)
  {
    while( RflanUart_Send( &RflanUart, Evt->c ) != 0 )
      vTaskDelay(1);

#ifdef RFLAN_LWIP_ENABLE

    RflanLwip_Send( &RflanLwip, &Evt->c, 1);

#endif
  }
  else if( Evt->Type == CliEvtType_TxMsg)
  {
    for( int i = 0; i < Evt->msg.Length; i++ )
    {
      while( RflanUart_Send( &RflanUart, Evt->msg.Buf[i] ) != 0)
        vTaskDelay(1);
    }

#ifdef RFLAN_LWIP_ENABLE

    RflanLwip_Send( &RflanLwip, Evt->msg.Buf, Evt->msg.Length);

#endif
  }
}

static void Rflan_UartCallback( char c, void *CallbackRef )
{
  Cli_ProcessRxChar( CallbackRef, c );
}

#ifdef RFLAN_LWIP_ENABLE

static void Rflan_LwipCallback( char *Buf, uint16_t Length, void *CallbackRef )
{
  for( int i = 0; i < Length; i++ )
  {
    Cli_ProcessRxChar( CallbackRef, Buf[i] );
  }
}

#endif

static int32_t Rflan_SetupScript( cli_t *cli, char *Filename )
{
  FIL fil;
  int32_t status;
  UINT len = 1;
  char c;

  do
  {
    /* Open File */
    if((status = f_open(&fil, Filename, FA_OPEN_EXISTING | FA_READ)) != 0)
      break;

    if((status = f_lseek(&fil, 0)) != 0)
      break;

    while( len > 0 )
    {
      if((status = f_read(&fil, (void*)&c, 1, &len)) != 0)
        break;

      if( len > 0 )
        Cli_ProcessRxChar( cli, c );
    }

  }while(0);

  f_close(&fil);

  return status;
}

static int32_t Rflan_Initialize( void )
{
  int32_t status;

  /* Initialize GPIO */
  if((status = RflanGpio_Initialize( &RflanGpio, XPAR_PSU_GPIO_0_DEVICE_ID )) != 0)
    printf("%s\r\n",StatusString(status));

  cli_init_t CliInit = {
    .Callback         = (cli_callback_t)Rflan_CliCallback,
    .CallbackRef      = NULL
  };

  /* Initialize CLI */
  if((status = Cli_Initialize( &Cli, &CliInit )) != 0)
    printf("%s\r\n",StatusString(status));

  rflan_cli_init_t RflanCliInit = {
    .Cli           = &Cli,
    .Gpio          = &RflanGpio,
    .RflanPib      = &RflanPib,
#ifdef RFLAN_STREAM_ENABLE
    .RflanStream   = &RflanStream
#endif
  };

  /* Initialize RFLAN Commands */
  if((status = RflanCli_Initialize( &RflanCli, &RflanCliInit )) != 0)
    printf("Rflan Cli %s\r\n",StatusString(status));

  rflan_uart_init_t UartInit = {
      .BaudRate           = RFLAN_UART_BAUDRATE,
      .DeviceId           = XPAR_PSU_UART_0_DEVICE_ID,
      .IntrId             = XPAR_XUARTPS_0_INTR,
	  .IrqInstance        = &xInterruptController,
      .ParentCallback     = Rflan_UartCallback,
      .ParentCallbackRef  = &Cli };

  /* Initialize System Reset */
  if((status = RflanUart_Initialize( &RflanUart, &UartInit )) != 0)
    printf("Rflan Uart %s\r\n",StatusString(status));

  /* Initialize System Reset */
  if((status = RflanReset_Initialize( &RflanReset )) != 0)
    printf("Rflan Reset %s\r\n",StatusString(status));

  /* Initialize I2C*/
  if((status = RflanIic_Initialize( &RflanIic0 )) != 0)
    printf("I2C Initialize %s\r\n",StatusString(status));

#ifdef VERSA_CLOCK5_ENABLE

  versa_clock5_init_t VersaClock5Init = {
      .Addr = 0x6A,
      .Iic = &RflanIic0 };

  /* Initialize Versa Clock */
  if((status = VersaClock5_Initialize( &RflanVersaClock5, &VersaClock5Init )) != 0)
    printf("VersaClock5 Initialize %s\r\n",StatusString(status));

#endif

  /* Initialize File System*/
  if((status = f_mount(&FatFs, FILE_SYSTEM_BASE_PATH, 1)) != FR_OK)
    printf("FatFs %s\r\n",StatusString(status));

  rflan_pib_init_t PibInit = {
      .HwVer = Rflan_GetHwVer( ),
#ifdef VERSA_CLOCK5_ENABLE
      .VersaClock5 = &RflanVersaClock5,
#endif
  };

  /* Initialize PIB */
  if((status = RflanPib_Initialize( &RflanPib, &PibInit )) != 0)
    printf("Rflan Pib Init %s\r\n",StatusString(status));

  /* Execute Init Script */
  if((status = Rflan_SetupScript( &Cli, (FILE_SYSTEM_BASE_PATH "rflan_setup.txt"))) != 0 )
    printf("Rflan Setup Script %s\r\n",StatusString(status));


  printf("\r\n\r\n\r\n" );
  printf("************************************************\r\n");
  printf("        BytePipe_x900x RFLAN - v%d.%d.%d\r\n", RFLAN_FW_VER_MINOR, RFLAN_FW_VER_REV, RFLAN_FW_VER_MAJOR );
  printf("************************************************\r\n");

  printf("\r\nType help for a list of commands\r\n\r\n");

  adrv9001_init_t Adrv9001Init = {
		  .AxiBase = XPAR_ADRV9001_0_BASEADDR,
		  .AxiIrqId = XPAR_FABRIC_ADRV9002_0_SPI_IRQ_INTR,
		  .TcxoEnablePin = ADI_ADRV9001_GPIO_ANALOG_07,
		  .LogFilename = (FILE_SYSTEM_BASE_PATH "adi_adrv9001_log.txt"),
	      .IrqInstance = &xInterruptController
  };

  if( Rflan_GetHwVer( ) == 2 )
  {
    Adrv9001Init.Rx1FrontendEnablePin = ADI_ADRV9001_GPIO_ANALOG_06;
    Adrv9001Init.Rx2FrontendEnablePin = ADI_ADRV9001_GPIO_ANALOG_05;
    Adrv9001Init.Tx1FrontendEnablePin = ADI_ADRV9001_GPIO_UNASSIGNED;
    Adrv9001Init.Tx2FrontendEnablePin = ADI_ADRV9001_GPIO_UNASSIGNED;
    Adrv9001Init.TcxoDacChannel = ADI_ADRV9001_AUXDAC0;
  }
  else if( Rflan_GetHwVer( ) == 3 )
  {

    Adrv9001Init.Rx1FrontendEnablePin = ADI_ADRV9001_GPIO_ANALOG_01;
    Adrv9001Init.Rx2FrontendEnablePin = ADI_ADRV9001_GPIO_ANALOG_09;
    Adrv9001Init.Tx1FrontendEnablePin = ADI_ADRV9001_GPIO_ANALOG_00;
    Adrv9001Init.Tx2FrontendEnablePin = ADI_ADRV9001_GPIO_ANALOG_08;
    Adrv9001Init.TcxoDacChannel = ADI_ADRV9001_AUXDAC3;
  }

  /* Initialize ADRV9001 CLI */
  if((status = Adrv9001_Initialize( &RflanAdrv9001, &Adrv9001Init )) != 0)
    printf("%s\r\n",StatusString(status));

  /* Initialize ADRV9001 PIB */
  if((status = Adrv9001Pib_Initialize( &Adrv9001Pib, &RflanAdrv9001 )) != 0)
    printf("Adrv9001Cli %s\r\n",StatusString(status));

  /* Initialize ADRV9001 CLI */
  if((status = Adrv9001Cli_Initialize( &Cli, &RflanAdrv9001, &Adrv9001Pib )) != 0)
    printf("Adrv9001Cli %s\r\n",StatusString(status));

  /* Execute Init Script */
  if((status = Rflan_SetupScript( &Cli, (FILE_SYSTEM_BASE_PATH "adrv9001_setup.txt"))) != 0 )
    printf("Adrv9001 Setup Script %s\r\n",StatusString(status));

#ifdef RFLAN_STREAM_ENABLE

  rflan_stream_init_t StreamInit = {
      .Callback = Rflan_StreamCallback,
      .CallbackRef = &RflanStream,
      .Adrv9001 = &RflanAdrv9001,
      .Tx1DmaBase = XPAR_TX1_DMA_BASEADDR,
      .Tx2DmaBase = XPAR_TX2_DMA_BASEADDR,
      .Rx1DmaBase = XPAR_RX1_DMA_BASEADDR,
      .Rx2DmaBase = XPAR_RX2_DMA_BASEADDR,
      .Tx1DmaIrqId = XPAR_FABRIC_TX1_DMA_IRQ_INTR,
      .Tx2DmaIrqId = XPAR_FABRIC_TX2_DMA_IRQ_INTR,
      .Rx1DmaIrqId = XPAR_FABRIC_RX1_DMA_IRQ_INTR,
      .Rx2DmaIrqId = XPAR_FABRIC_RX2_DMA_IRQ_INTR,
	  .IrqInstance = &xInterruptController
  };

  /* Initialize Stream */
  if((status = RflanStream_Initialize( &RflanStream, &StreamInit )) != 0)
    printf("Stream %s\r\n",StatusString(status));

#endif

#ifdef RFLAN_LWIP_ENABLE

  if( RflanPib.Params.LwipEnable )
  {
    rflan_lwip_init_t LwipInit = {
        .Callback = Rflan_LwipCallback,
        .CallbackRef = &Cli,
        .RxBufSize = RFLAN_LWIP_RECV_BUF_SIZE,
        .CliPort = RFLAN_LWIP_CLI_PORT,
        .MacAddr = RFLAN_LWIP_MAC_ADDR,
        .BaseAddr = XPAR_XEMACPS_0_BASEADDR,
        .IpAddr = RflanPib.Params.IpAddr,
        .IpMask = RflanPib.Params.IpMask,
        .IpGw = RflanPib.Params.IpGwAddr
    };

    if((status = RflanLwip_Initialize( &RflanLwip, &LwipInit )) != 0)
      printf("Lwip %s\r\n",StatusString(status));
  }

#endif

  return status;
}

static void Rflan_Task( void *pvParameters )
{
  /* Initialize RFLAN Components */
  Rflan_Initialize( );

  rflan_evt_type_t evt;

  for( ;; )
  {
    if( xQueueReceive( RflanEvtQueue, (void *)&evt, 500 ) == pdTRUE)
    {
      switch( evt )
      {
#ifdef RFLAN_STREAM_ENABLE

        case RflanEvt_Rx1StreamDone:
          Adrv9001_ToPrimed( RflanStream.Adrv9001, ADI_RX, ADI_CHANNEL_1 );
          printf("%s Stream Done\r\n", "Rx1");
          break;

        case RflanEvt_Rx2StreamDone:
          Adrv9001_ToPrimed( RflanStream.Adrv9001, ADI_RX, ADI_CHANNEL_2 );
          printf("%s Stream Done\r\n", "Rx2");
          break;

        case RflanEvt_Tx1StreamDone:
          Adrv9001_ToPrimed( RflanStream.Adrv9001, ADI_TX, ADI_CHANNEL_1 );
          printf("%s Stream Done\r\n", "Tx1");
          break;      

        case RflanEvt_Tx2StreamDone:
          Adrv9001_ToPrimed( RflanStream.Adrv9001, ADI_TX, ADI_CHANNEL_2 );
          printf("%s Stream Done\r\n", "Tx2");
          break;      

#endif

        default:
          break;
      }
    }
        
    RflanGpio_TogglePin( &RflanGpio, GPIO_LED_PIN );
  }
}

int main()
{
  /* Create Queue */
  RflanEvtQueue = xQueueCreate(RFLAN_EVT_QUEUE_SIZE, sizeof(uint32_t));
  
  /* Create Rflan Task */
  xTaskCreate( Rflan_Task,
              "rflan",
              RFLAN_TASK_STACK_SIZE,
              NULL,
              RFLAN_TASK_PRIORITY,
              NULL );

  /* Start RTOS */
  vTaskStartScheduler( );

  for( ;; );
}

void Rflan_Reboot( void )
{
  XResetPs_ResetPulse(&RflanReset, XRESETPS_RSTID_SOFT);
}

uint32_t Rflan_GetHwVer( void )
{
  uint32_t hwv = 0;
  hwv += RflanGpio_ReadPin( &RflanGpio, GPIO_HWV2_PIN ) << 2;
  hwv += RflanGpio_ReadPin( &RflanGpio, GPIO_HWV1_PIN ) << 1;
  hwv += RflanGpio_ReadPin( &RflanGpio, GPIO_HWV0_PIN );

  if( hwv == 7 )
    hwv = 2;
  else if( hwv == 3 )
    hwv = 3;
  else
    hwv = 0;

  return hwv;
}


