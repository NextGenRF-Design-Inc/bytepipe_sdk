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
    PhyQEvt_LoadProfile     = 4,
  }Evt;
  union
  {
    phy_stream_t       *Stream;
    struct
    {
      adrv9001_port_t     Port;
      adrv9001_status_t   Status;
    };
    struct
    {
      char *StreamImageName;
      char *ProfileName;
    };
  }Data;
} phy_queue_t;

static QueueHandle_t            PhyQueue;                       ///< PHY Queue
static phy_stream_t            *PhyStream[Adrv9001Port_Num];    ///< Stream Data
adi_adrv9001_Device_t          *Adrv9001;
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

static phy_status_t Phy_Adrv9001LoadProfile( const char *ProfileFilename, const char *StreamImageFilename )
{
  FIL fil;
  int32_t status;
  UINT ProfileBufLength;
  UINT StreamBufLength;
  char *ProfileBuf = NULL;
  uint8_t *StreamImageBuf = NULL;

  char *path;

  if((path = calloc(1, FF_FILENAME_MAX_LEN )) == NULL)
  {
    return PhyStatus_MemoryError;
  }

  strncpy(path, FF_LOGICAL_DRIVE_PATH, FF_FILENAME_MAX_LEN);
  strncpy(&path[strlen(path)], ProfileFilename, FF_FILENAME_MAX_LEN);

  do
  {
    /* Open File */
    if((status = f_open(&fil, path, FA_OPEN_EXISTING | FA_READ)) != FR_OK) break;

    /* Allocate Buffer */
    if((ProfileBuf = malloc(f_size(&fil))) == NULL)
    {
      status = PhyStatus_MemoryError;
      break;
    }

    /* Pointer to beginning of file */
    if((status = f_lseek(&fil, 0)) != FR_OK)
      break;

    if((status = f_read(&fil, ProfileBuf, f_size(&fil), &ProfileBufLength)) != FR_OK)
      break;

    f_close(&fil);

    strncpy(path, FF_LOGICAL_DRIVE_PATH, FF_FILENAME_MAX_LEN);
    strncpy(&path[strlen(path)], StreamImageFilename, FF_FILENAME_MAX_LEN);

    /* Open File */
    if((status = f_open(&fil, path, FA_OPEN_EXISTING | FA_READ)) != FR_OK) break;

    /* Allocate Buffer */
    if((StreamImageBuf = malloc(f_size(&fil))) == NULL)
    {
      status = PhyStatus_MemoryError;
      break;
    }

    /* Pointer to beginning of file */
    if((status = f_lseek(&fil, 0)) != FR_OK)
      break;

    if((status = f_read(&fil, StreamImageBuf, f_size(&fil), &StreamBufLength)) != FR_OK)
      break;

  }while(0);

  f_close(&fil);
  free(path);

  if(status == 0)
  {
    /* Initialize ADRV9001 */
    if((status = Phy_Adrv9001Initialize( )) == Adrv9001Status_Success)
    {
      /* Load Profile */
      status = Adrv9001_LoadProfile( ProfileBuf, ProfileBufLength, StreamImageBuf, StreamBufLength );
    }
  }

  if( ProfileBuf != NULL )
    free(ProfileBuf);

  if( StreamImageBuf != NULL )
    free(StreamImageBuf);

  return status;
}

static void Phy_Task( void *pvParameters )
{
  phy_queue_t qItem;
  int32_t status;

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
      case PhyQEvt_LoadProfile:
        printf("Updating ADRV9001 Profile...\r\n");
        vTaskDelay(10);
        status = Phy_Adrv9001LoadProfile( qItem.Data.ProfileName, qItem.Data.StreamImageName);
        printf("%s\r\n",PHY_STATUS_2_STR(status));
        free(qItem.Data.ProfileName);
        free(qItem.Data.StreamImageName);
        break;
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

phy_status_t Phy_Adrv9001LoadProfileReq( const char *ProfileFilename, const char *StreamImageFilename )
{
  /* Create Queue Item */
  phy_queue_t qItem = { .Evt = PhyQEvt_LoadProfile };

  /* Create Memory for ProfileName */
  if((qItem.Data.ProfileName = calloc(1, FF_FILENAME_MAX_LEN)) == NULL)
    return PhyStatus_MemoryError;

  /* Create Memory for StreamImageName */
  if((qItem.Data.StreamImageName = calloc(1, FF_FILENAME_MAX_LEN)) == NULL)
  {
    free(qItem.Data.ProfileName);
    return PhyStatus_MemoryError;
  }

  /* Copy Profile Name */
  memcpy((uint8_t*)qItem.Data.ProfileName, (uint8_t*)ProfileFilename, strlen(ProfileFilename));

  /* Copy Stream Image Name */
  memcpy((uint8_t*)qItem.Data.StreamImageName, (uint8_t*)StreamImageFilename, strlen(StreamImageFilename));

  /* Send to PHY Task */
  if( xQueueSend( PhyQueue, &qItem, 1) != pdPASS )
  {
    free(qItem.Data.ProfileName);
    free(qItem.Data.StreamImageName);
    return PhyStatus_Busy;
  }

  return PhyStatus_Success;
}

phy_status_t Phy_Adrv9001Initialize( void )
{
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
  return Adrv9001_Initialize( (void**)&Adrv9001, &Adrv9001Cfg );
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

  /* Initialize ADRV9001 */
  if((status = Phy_Adrv9001Initialize( )) != Adrv9001Status_Success)
    return status;

  /* Load ADRV9001 Profile */
  if((status = Phy_Adrv9001LoadProfile( "default.json", "default.bin" )) != Adrv9001Status_Success)
    Adrv9001_ClearError( );

  printf("%s Version Information:\r\n","ADRV9002" );

  adi_adrv9001_SiliconVersion_t SiliconVer;
  if( adi_adrv9001_SiliconVersion_Get( Adrv9001, &SiliconVer ) != Adrv9001Status_Success)
    return status;

  printf("  -Silicon Version: %X%X\r\n", SiliconVer.major, SiliconVer.minor);

  adi_adrv9001_ArmVersion_t ArmVer;

  if( adi_adrv9001_arm_Version( Adrv9001, &ArmVer ) != Adrv9001Status_Success)
    return status;

  printf("  -Firmware Version: %u.%u.%u.%u\r\n",ArmVer.majorVer, ArmVer.minorVer, ArmVer.maintVer, ArmVer.rcVer);

  /* Get Version Info */
  adi_common_ApiVersion_t ApiVer;

  if(adi_adrv9001_ApiVersion_Get( Adrv9001, &ApiVer) != Adrv9001Status_Success)
    return status;

  printf("  -API Version: %lu.%lu.%lu\r\n\r\n", ApiVer.major,  ApiVer.minor, ApiVer.patch);

  return status;
}
