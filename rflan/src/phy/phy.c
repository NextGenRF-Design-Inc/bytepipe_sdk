
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "app.h"
#include "phy.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "phy_cli.h"

#define PHY_TX_STREAM_MAX_CNT         (0x20000)
#define PHY_TX_STREAM_MAX_BUF_SIZE    (PHY_TX_STREAM_MAX_CNT * sizeof(adrv9001_iqdata_t))
#define PHY_RX_STREAM_CNT             (1024)
#define PHY_RX_STREAM_BUF_SIZE        (PHY_RX_STREAM_CNT * sizeof(adrv9001_iqdata_t))
#define PHY_NUM_RX_CHANNELS           (2)
#define PHY_NUM_TX_CHANNELS           (2)
#define PHY_NUM_CHANNELS              (PHY_NUM_RX_CHANNELS + PHY_NUM_TX_CHANNELS)
#define PHY_IS_PORT_TX(p)             ((( p == Adrv9001Port_Tx1 ) || ( p == Adrv9001Port_Tx2 )) ? true : false)
#define PHY_IS_PORT_RX(p)             ((( p == Adrv9001Port_Rx1 ) || ( p == Adrv9001Port_Rx2 )) ? true : false)
#define PHY_CHANNEL(p)                ((p == Adrv9001Port_Rx1)? 0 : (p == Adrv9001Port_Rx2)? 1 : (p == Adrv9001Port_Tx1)? 2 : 3)


/**
**  PHY Queue
*/
typedef struct
{
  enum
  {
    PhyQEvt_StreamEnable     = 0,
    PhyQEvt_StreamDisable    = 1,
  }Evt;
  union
  {
    phy_stream_t       *Stream;
    adrv9001_port_t     Port;
  }Data;
} phy_queue_t;

static QueueHandle_t      PhyQueue;                       ///< PHY Queue
static phy_stream_t      *PhyStream[PHY_NUM_CHANNELS];    ///< Stream Data


static void Phy_Adrv9001Callback( adrv9001_evt_t evt, void *param )
{

  if(evt.Type == Adrv9001EvtType_Stream)
  {

  }
}

static void Phy_IqStreamStop( adrv9001_port_t Port )
{
  phy_stream_t *Stream = PhyStream[PHY_CHANNEL(Port)];

  /* Check if Valid */
  if( Stream != NULL )
  {
    /* Disable RF */
    if( Adrv9001_ToRfCalibrated( Port ) != Adrv9001Status_Success )
    {
      /* Execute Callback */
      if(Stream->Callback != NULL)
        Stream->Callback( PhyEvtType_StreamError, Stream, Stream->CallbackRef );
    }

    /* Disable Streaming */
    Adrv9001_IQStream( Stream->Port, NULL, 0 );

    /* Execute Callback */
    if(Stream->Callback != NULL)
      Stream->Callback( PhyEvtType_StreamStop, Stream, Stream->CallbackRef );

    /* Free Sample Buffer */
    free( Stream->SampleBuf );

    /* Free Stream Data */
    free( PhyStream[PHY_CHANNEL(Port)] );

    /* Clear Stream Data */
    PhyStream[PHY_CHANNEL(Port)] = NULL;
  }
}

static void Phy_IqStreamStart( phy_stream_t *Stream )
{
  if( Stream == NULL )
    return;

  /* Copy Stream Reference */
  PhyStream[PHY_CHANNEL(Stream->Port)] = Stream;

  /* Enable RF */
  if( Adrv9001_ToRfEnabled( Stream->Port ) != Adrv9001Status_Success )
  {
    /* Execute Callback */
    if(Stream->Callback != NULL)
      Stream->Callback( PhyEvtType_StreamError, Stream, Stream->CallbackRef );
  }

  /* Enable Streaming */
  if(Adrv9001_IQStream( Stream->Port, (adrv9001_iqdata_t*)Stream->SampleBuf, Stream->SampleCnt ) != Adrv9001Status_Success)
  {
    /* Execute Callback */
    if(Stream->Callback != NULL)
      Stream->Callback( PhyEvtType_StreamError, Stream, Stream->CallbackRef );

    /* Attempt Stop Current Stream */
    Phy_IqStreamStop( Stream->Port );
  }
  else
  {
    /* Execute Callback */
    if(Stream->Callback != NULL)
      Stream->Callback( PhyEvtType_StreamStart, Stream, Stream->CallbackRef );
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
      case PhyQEvt_StreamEnable:      Phy_IqStreamStart( qItem.Data.Stream );       break;
      case PhyQEvt_StreamDisable:     Phy_IqStreamStop( qItem.Data.Port );          break;
    }
  }
}

int32_t Phy_IqStreamDisable( adrv9001_port_t Port )
{
  /* Create Queue Item */
  phy_queue_t qItem = {.Evt = PhyQEvt_StreamDisable, .Data.Port = Port };

  /* Send to PHY Task */
  if( xQueueSend( PhyQueue, &qItem, 1) != pdPASS )
    return PhyStatus_Busy;

  return PhyStatus_Success;
}

phy_status_t Phy_IqStreamEnable( phy_stream_t *Stream )
{
  if( Stream == NULL )
    return PhyStatus_InvalidParameter;

  if( PHY_IS_PORT_TX(Stream->Port) && ((Stream->SampleBuf == NULL) || (Stream->SampleCnt == 0)) )
    return PhyStatus_InvalidParameter;

  if( Stream->Callback == NULL )
    return PhyStatus_InvalidParameter;

  /* Disable Current Streams on Same Port */
  Phy_IqStreamDisable( Stream->Port );

  /* Create Queue Item */
  phy_queue_t qItem = {.Evt = PhyQEvt_StreamEnable};

  /* Create New Stream */
  if((qItem.Data.Stream = calloc(1, sizeof(phy_stream_t))) == NULL)
    return PhyStatus_MemoryError;

  /* Copy Stream */
  memcpy((uint8_t*)qItem.Data.Stream, (uint8_t*)Stream, sizeof(phy_stream_t));

  /* Send to PHY Task */
  if( xQueueSend( PhyQueue, &qItem, 1) != pdPASS )
    return PhyStatus_Busy;

  return PhyStatus_Success;
}

phy_status_t Phy_LoadProfile( profile_t *Profile )
{
  int32_t status = PhyStatus_NotSupported;


  return status;
}

phy_status_t Phy_Initialize( void )
{
  int32_t status;

  /* Clear Stream Data */
  for(int i = 0; i < PHY_NUM_CHANNELS; i++)
    PhyStream[i] = NULL;

  /* Create Queue */
  PhyQueue = xQueueCreate(PHY_QUEUE_SIZE, sizeof(phy_queue_t));

  /* Create Task */
  if(xTaskCreate(Phy_Task, PHY_TASK_NAME, PHY_TASK_STACK_SIZE, NULL, PHY_TASK_PRIORITY, NULL) != pdPASS)
    return PhyStatus_OsError;

  /* Initialize PHY CLI */
  if((status = PhyCli_Initialize()) != PhyStatus_Success)
    return status;

  adrv9001_cfg_t Adrv9001Cfg = {
     .Callback = Phy_Adrv9001Callback,
     .CallbackRef = NULL
  };

  /* Initialize ADRV9001 */
  if((status = Adrv9001_Initialize( &Adrv9001Cfg )) != Adrv9001Status_Success)
    return status;

  /* Load ADRV9001 Profile */
  if((status = Adrv9001_LoadProfile( )) != Adrv9001Status_Success)
    return status;

  /* Begin Receiving */
  if((status = Adrv9001_BeginReceiving( )) != Adrv9001Status_Success)
    return status;

  /* Begin Transmitting */
  if((status = Adrv9001_BeginTransmitting( )) != Adrv9001Status_Success)
    return status;

  return XST_SUCCESS;
}
