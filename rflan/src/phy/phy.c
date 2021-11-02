
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


extern XScuGic                  xInterruptController;
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
    if( Adrv9001_IQStream( Stream->Port, NULL, 0 ) != Adrv9001Status_Success)
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
  else if(Adrv9001_IQStream( Stream->Port, (adrv9001_iqdata_t*)Stream->SampleBuf, Stream->SampleCnt ) != Adrv9001Status_Success)
  {
    /* Attempt Stop Current Stream */
    Phy_IqStreamStop( Stream->Port );

    /* Abort Stream */
    Phy_IqStreamDone( Stream->Port, PhyStatus_Adrv9001Error );

    /* Remove Stream */
    Phy_IqStreamRemove( Stream->Port );
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
  if(EvtType == Adrv9001EvtType_StreamDone)
  {
    PhyStream[EvtData.Stream.Port]->Status = EvtData.Stream.Status;

    phy_queue_t qItem = {.Data.Port = EvtData.Stream.Port, .Data.Status = EvtData.Stream.Status};
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
}

int32_t Phy_IqStreamDisable( adrv9001_port_t Port )
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
//  printf("%s Successfully Initialized\r\n","ADRV9002");
//  printf("  -Silicon Version: %X%X\r\n",VerInfo.Silicon.major, VerInfo.Silicon.minor);
//  printf("  -Firmware Version: %u.%u.%u.%u\r\n",VerInfo.Arm.major, VerInfo.Arm.minor, VerInfo.Arm.maint, VerInfo.Arm.rcVer);
//  printf("  -API Version: %u.%u.%u\r\n\r\n", VerInfo.Api.major,  VerInfo.Api.minor, VerInfo.Api.patch);

  return status;
}

phy_status_t Phy_Initialize( void )
{
  int32_t status;

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

  DmaCfg.BaseAddr[Adrv9001Port_Tx1] = XPAR_ADRV9001_TX1_DMA_BASEADDR;
  DmaCfg.BaseAddr[Adrv9001Port_Tx2] = XPAR_ADRV9001_TX2_DMA_BASEADDR;
  DmaCfg.BaseAddr[Adrv9001Port_Rx1] = XPAR_ADRV9001_RX1_DMA_BASEADDR;
  DmaCfg.BaseAddr[Adrv9001Port_Rx2] = XPAR_ADRV9001_RX2_DMA_BASEADDR;

  DmaCfg.IrqId[Adrv9001Port_Tx1] = XPAR_FABRIC_ADRV9001_TX1_DMA_IRQ_INTR;
  DmaCfg.IrqId[Adrv9001Port_Tx2] = XPAR_FABRIC_ADRV9001_TX2_DMA_IRQ_INTR;
  DmaCfg.IrqId[Adrv9001Port_Rx1] = XPAR_FABRIC_ADRV9001_RX1_DMA_IRQ_INTR;
  DmaCfg.IrqId[Adrv9001Port_Rx2] = XPAR_FABRIC_ADRV9001_RX2_DMA_IRQ_INTR;


  adrv9001_gpio_cfg_t GpioCfg = {
      .DeviceId = GPIO_DEVICE_ID,
      .IrqPin = 0,
      .RstnPin = ADRV9001_GPIO_RSTN,
      .Rx1EnPin = ADRV9001_GPIO_RX1_EN,
      .Rx2EnPin = ADRV9001_GPIO_RX2_EN,
      .Tx1EnPin = ADRV9001_GPIO_TX1_EN,
      .Tx2EnPin = ADRV9001_GPIO_TX2_EN
  };

  adrv9001_spi_cfg_t SpiCfg = {
      .DeviceId = ADRV9001_SPI_DEVICE_ID,
      .CsPinId = ADRV9001_SPI_CS
  };

  adrv9001_cfg_t Adrv9001Cfg = {
     .Callback      = Phy_Adrv9001Callback,
     .CallbackRef   = NULL,
     .DmaCfg        = &DmaCfg,
     .GpioCfg       = &GpioCfg,
     .SpiCfg        = &SpiCfg,
     .IrqInstance   = &xInterruptController
  };

  /* Initialize ADRV9001 */
  if((status = Adrv9001_Initialize( (void**)&Adrv9001, &Adrv9001Cfg )) != Adrv9001Status_Success)
    return status;

  /* Load ADRV9001 Profile */
  if((status = Adrv9001_LoadProfile( )) != Adrv9001Status_Success)
    return status;

  /* Get Version Info */
  adrv9001_ver_t VerInfo;
  if((status = Adrv9001_GetVersionInfo( &VerInfo )) != Adrv9001Status_Success)
    return status;

  printf("%s Successfully Initialized\r\n","ADRV9002");
  printf("  -Silicon Version: %X%X\r\n",VerInfo.Silicon.major, VerInfo.Silicon.minor);
  printf("  -Firmware Version: %u.%u.%u.%u\r\n",VerInfo.Arm.major, VerInfo.Arm.minor, VerInfo.Arm.maint, VerInfo.Arm.rcVer);
  printf("  -API Version: %u.%u.%u\r\n\r\n", VerInfo.Api.major,  VerInfo.Api.minor, VerInfo.Api.patch);


  return XST_SUCCESS;
}
