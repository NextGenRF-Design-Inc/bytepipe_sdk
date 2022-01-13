/***************************************************************************//**
*  \addtogroup PHY
*   @{
*******************************************************************************/
/***************************************************************************//**
*  \file       phy.c
*
*  \details    This file contains the RFLAN PHY layer implementation.
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
#include <string.h>
#include <stdio.h>
#include "app.h"
#include "phy.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "phy_cli.h"
#include "parameters.h"
#include "xscugic.h"
#include "adrv9001.h"
#include "adi_adrv9001_types.h"

/**
**  PHY Queue
*/
typedef struct
{
  enum
  {
    PhyQEvt_StreamStart     = 0,
    PhyQEvt_StreamStop      = 1,
    PhyQEvt_StreamRemove    = 2,
    PhyQEvt_StreamDone      = 3,
  }Evt;
  union
  {
    phy_stream_t       *Stream;
    struct
    {
      adrv9001_port_t     Port;
      adrv9001_status_t   Status;
    };
  }Data;
} phy_queue_t;

static QueueHandle_t            PhyQueue;                       ///< PHY Queue
static phy_stream_t            *PhyStream[Adrv9001Port_Num];    ///< Stream Data
static adi_adrv9001_Device_t   *Adrv9001;
extern XScuGic                  xInterruptController;
static phy_callback_t           PhyCallback;
static void                    *PhyCallbackRef;

static void Phy_IqStreamRemove( adrv9001_port_t Port )
{
  if( Port < Adrv9001Port_Num )
  {
    /* Free Stream Data */
    free( PhyStream[ Port ] );

    /* Clear Stream Data */
    PhyStream[ Port ] = NULL;
  }
}

static void Phy_IqStreamStop( adrv9001_port_t Port )
{
  phy_stream_t *Stream = PhyStream[ Port ];

  /* Check if Valid */
  if( (Stream != NULL) && (Port < Adrv9001Port_Num) )
  {
    /* Disable RF */
    if( Adrv9001_ToRfCalibrated( Port ) != Adrv9001Status_Success )
      Stream->Status = PhyStatus_RadioStateError;

    /* Disable Streaming */
    if( Adrv9001Dma_Transfer( Stream->Port, Stream->Cyclic, NULL, 0 ) != Adrv9001Status_Success)
      Stream->Status = PhyStatus_Adrv9001Error;
  }
}

static void Phy_IqStreamDone( adrv9001_port_t Port, adrv9001_status_t Status )
{
  phy_stream_t *Stream = PhyStream[ Port ];

  if(( Stream == NULL) || (Port >= Adrv9001Port_Num))
    return;

  /* Send Stream Error */
  phy_evt_data_t PhyEvtData = {
      .Stream.Port = Port,
      .Stream.SampleBuf = Stream->SampleBuf,
      .Stream.SampleCnt = Stream->SampleCnt,
      .Stream.Status = Status,
      .Stream.CallbackRef = Stream->CallbackRef
  };

  if(Stream->Callback != NULL)
    Stream->Callback( PhyEvtType_StreamDone, PhyEvtData, Stream->CallbackRef );
}

static void Phy_IqStreamStart( phy_stream_t *Stream )
{
  if( Stream == NULL )
    return;

  /* Copy Stream Reference */
  PhyStream[ Stream->Port ] = Stream;

  /* Enable RF */
  if( Adrv9001_ToRfEnabled( Stream->Port ) != Adrv9001Status_Success )
  {
    /* Abort Stream */
    Phy_IqStreamDone( Stream->Port, PhyStatus_RadioStateError );

    /* Remove Stream */
    Phy_IqStreamRemove( Stream->Port );
  }
  /* Enable Streaming */
  else if(Adrv9001Dma_Transfer( Stream->Port, Stream->Cyclic, Stream->SampleBuf, Stream->SampleCnt ) != Adrv9001Status_Success)
  {
    /* Attempt Stop Current Stream */
    Phy_IqStreamStop( Stream->Port );

    /* Abort Stream */
    Phy_IqStreamDone( Stream->Port, PhyStatus_Adrv9001Error );

    /* Remove Stream */
    Phy_IqStreamRemove( Stream->Port );
  }
  else
  {
    /* Send Stream Error */
    phy_evt_data_t PhyEvtData = {
        .Stream.Port = Stream->Port ,
        .Stream.SampleBuf = Stream->SampleBuf,
        .Stream.SampleCnt = Stream->SampleCnt,
        .Stream.Status = PhyStatus_Success,
        .Stream.CallbackRef = Stream->CallbackRef
    };

    if(Stream->Callback != NULL)
      Stream->Callback( PhyEvtType_StreamStart, PhyEvtData, Stream->CallbackRef );
  }
}

static void Phy_Task( void *pvParameters )
{
  phy_queue_t qItem;

  for( ;; )
  {
    /* Wait for Message */
    xQueueReceive( PhyQueue, (void *)&qItem, portMAX_DELAY);

    /* Process Message */
    switch( qItem.Evt )
    {
      case PhyQEvt_StreamStart:     Phy_IqStreamStart( qItem.Data.Stream );                   break;
      case PhyQEvt_StreamStop:      Phy_IqStreamStop( qItem.Data.Port );                      break;
      case PhyQEvt_StreamRemove:    Phy_IqStreamRemove( qItem.Data.Port );                    break;
      case PhyQEvt_StreamDone:      Phy_IqStreamDone( qItem.Data.Port, qItem.Data.Status );   break;
    }
  }
}

static void Phy_Adrv9001Callback( adrv9001_evt_type_t EvtType, adrv9001_evt_data_t EvtData, void *param )
{
  if(EvtType == Adrv9001EvtType_DmaDone)
  {
    PhyStream[EvtData.dma.Port]->Status = EvtData.dma.Status;

    phy_queue_t qItem = {.Data.Port = EvtData.dma.Port, .Data.Status = EvtData.dma.Status};
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    /* Disable Stream */
    qItem.Evt = PhyQEvt_StreamStop;
    xQueueSendFromISR( PhyQueue, &qItem, &xHigherPriorityTaskWoken );
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

    /* Process Stream Done */
    qItem.Evt = PhyQEvt_StreamDone;
    xQueueSendFromISR( PhyQueue, &qItem, &xHigherPriorityTaskWoken );
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

    /* Remove Stream */
    qItem.Evt = PhyQEvt_StreamRemove;
    xQueueSendFromISR( PhyQueue, &qItem, &xHigherPriorityTaskWoken );
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
  }
  else if( EvtType == Adrv9001EvtType_LogWrite )
  {

    if( PhyCallback != NULL )
    {
      phy_evt_data_t PhyEvtData = {.Message = EvtData.Message };

      PhyCallback( PhyEvtType_LogWrite, PhyEvtData, PhyCallbackRef );
    }
  }
}

phy_status_t Phy_IqStreamDisable( adrv9001_port_t Port )
{
  /* Create Queue Item */
  phy_queue_t qItem = {.Data.Port = Port, .Data.Status = PhyStatus_IqStreamAbort};

  /* Stop Stream */
  qItem.Evt = PhyQEvt_StreamStop;
  if( xQueueSend( PhyQueue, &qItem, 1) != pdPASS )
    return PhyStatus_Busy;

  /* Abort Stream */
  qItem.Evt = PhyQEvt_StreamDone;
  if( xQueueSend( PhyQueue, &qItem, 1) != pdPASS )
    return PhyStatus_Busy;

  /* Remove Stream */
  qItem.Evt = PhyQEvt_StreamRemove;
  if( xQueueSend( PhyQueue, &qItem, 1) != pdPASS )
    return PhyStatus_Busy;

  return PhyStatus_Success;
}

phy_status_t Phy_IqStreamEnable( phy_stream_t *Stream )
{
  if( Stream == NULL )
    return PhyStatus_InvalidParameter;

  if( (Stream->SampleBuf == NULL) || (Stream->SampleCnt == 0) )
    return PhyStatus_InvalidParameter;

  if( Stream->Callback == NULL )
    return PhyStatus_InvalidParameter;

  /* Disable Current Streams on Same Port */
  Phy_IqStreamDisable( Stream->Port );

  /* Create Queue Item */
  phy_queue_t qItem = { .Evt = PhyQEvt_StreamStart };

  /* Create New Stream */
  if((qItem.Data.Stream = calloc(1, sizeof(phy_stream_t))) == NULL)
    return PhyStatus_MemoryError;

  /* Initialize Status */
  Stream->Status = PhyStatus_Success;

  /* Copy Stream */
  memcpy((uint8_t*)qItem.Data.Stream, (uint8_t*)Stream, sizeof(phy_stream_t));

  /* Send to PHY Task */
  if( xQueueSend( PhyQueue, &qItem, 1) != pdPASS )
  {
    free(qItem.Data.Stream);
    return PhyStatus_Busy;
  }

  return PhyStatus_Success;
}

phy_status_t Phy_UpdateProfile( profile_t *Profile )
{
  int32_t status = PhyStatus_NotSupported;



//  /* Load ADRV9001 Profile */
//  if((status = Adrv9001_LoadProfile( )) != Adrv9001Status_Success)
//    return status;
//
//  /* Get Version Info */
//  adrv9001_ver_t VerInfo;
//  if((status = Adrv9001_GetVersionInfo( &VerInfo )) != Adrv9001Status_Success)
//    return status;
//
//  printf("%s Version Information:\r\n","ADRV9002");
//  printf("  -Silicon Version: %X%X\r\n",VerInfo.Silicon.major, VerInfo.Silicon.minor);
//  printf("  -Firmware Version: %u.%u.%u.%u\r\n",VerInfo.Arm.major, VerInfo.Arm.minor, VerInfo.Arm.maint, VerInfo.Arm.rcVer);
//  printf("  -API Version: %u.%u.%u\r\n\r\n", VerInfo.Api.major,  VerInfo.Api.minor, VerInfo.Api.patch);

  return status;
}

phy_status_t Phy_Initialize( phy_cfg_t *Cfg )
{
  int32_t status;

  /* Copy Configuration Items */
  PhyCallback = Cfg->Callback;
  PhyCallbackRef = Cfg->CallbackRef;

  /* Clear Stream Data */
  for(adrv9001_port_t i = 0; i < Adrv9001Port_Num; i++)
    PhyStream[i] = NULL;

  /* Create Queue */
  PhyQueue = xQueueCreate(PHY_QUEUE_SIZE, sizeof(phy_queue_t));

  /* Create Task */
  if(xTaskCreate(Phy_Task, PHY_TASK_NAME, PHY_TASK_STACK_SIZE, NULL, PHY_TASK_PRIORITY, NULL) != pdPASS)
    return PhyStatus_OsError;

  /* Initialize PHY CLI */
  if((status = PhyCli_Initialize()) != PhyStatus_Success)
    return status;

  adrv9001_dma_cfg_t DmaCfg;

  DmaCfg.BaseAddr[Adrv9001Port_Tx1] = XPAR_TX1_DMA_BASEADDR;
  DmaCfg.BaseAddr[Adrv9001Port_Tx2] = XPAR_TX2_DMA_BASEADDR;
  DmaCfg.BaseAddr[Adrv9001Port_Rx1] = XPAR_RX1_DMA_BASEADDR;
  DmaCfg.BaseAddr[Adrv9001Port_Rx2] = XPAR_RX2_DMA_BASEADDR;

  DmaCfg.IrqId[Adrv9001Port_Tx1] = XPAR_FABRIC_TX1_DMA_IRQ_INTR;
  DmaCfg.IrqId[Adrv9001Port_Tx2] = XPAR_FABRIC_TX2_DMA_IRQ_INTR;
  DmaCfg.IrqId[Adrv9001Port_Rx1] = XPAR_FABRIC_RX1_DMA_IRQ_INTR;
  DmaCfg.IrqId[Adrv9001Port_Rx2] = XPAR_FABRIC_RX2_DMA_IRQ_INTR;


  axi_adrv9001_cfg_t AxiAdrv9001Cfg = {
      .BaseAddr = XPAR_ADRV9002_0_BASEADDR,
      .IrqId = 0,
  };

  adrv9001_spi_cfg_t SpiCfg = {
      .DeviceId = ADRV9001_SPI_DEVICE_ID,
      .CsPinId = ADRV9001_SPI_CS
  };

  adrv9001_cfg_t Adrv9001Cfg = {
     .Callback        = Phy_Adrv9001Callback,
     .CallbackRef     = NULL,
     .DmaCfg          = &DmaCfg,
     .AxiAdrv9001Cfg  = &AxiAdrv9001Cfg,
     .SpiCfg          = &SpiCfg,
     .IrqInstance     = &xInterruptController
  };

  /* Initialize ADRV9001 */
  if((status = Adrv9001_Initialize( (void**)&Adrv9001, &Adrv9001Cfg )) != Adrv9001Status_Success)
    return status;

  /* Load ADRV9001 Profile */
  if((status = Adrv9001_LoadProfile( )) != Adrv9001Status_Success)
    Adrv9001_ClearError( );

  /* Get Version Info */
  adrv9001_ver_t VerInfo;
  if( Adrv9001_GetVersionInfo( &VerInfo ) == Adrv9001Status_Success)
  {
    char *Buf = calloc(1, 1024);

    snprintf( &Buf[strlen(Buf)], 1023, "%s Version Information:\r\n","ADRV9002" );
    snprintf( &Buf[strlen(Buf)], 1023, "  -Silicon Version: %X%X\r\n",VerInfo.Silicon.major, VerInfo.Silicon.minor);
    snprintf( &Buf[strlen(Buf)], 1023, "  -Firmware Version: %u.%u.%u.%u\r\n",VerInfo.Arm.major, VerInfo.Arm.minor, VerInfo.Arm.maint, VerInfo.Arm.rcVer);
    snprintf( &Buf[strlen(Buf)], 1023, "  -API Version: %lu.%lu.%lu\r\n\r\n", VerInfo.Api.major,  VerInfo.Api.minor, VerInfo.Api.patch);

    print(Buf);

    if( PhyCallback != NULL )
    {
      phy_evt_data_t EvtData = {.Message = Buf };
      PhyCallback( PhyEvtType_LogWrite, EvtData, PhyCallbackRef );
    }

    free(Buf);
  }
  else
  {
    printf("Unable to Get ADRV9002 Version Information\r\n");
  }

  return status;
}
