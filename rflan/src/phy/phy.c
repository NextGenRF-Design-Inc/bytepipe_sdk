
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


static QueueHandle_t            PhyQueue;             ///< PHY Queue
static phy_callback_t           PhyCallback;          ///< Callback
static void                    *PhyCallbackRef;       ///< Callback reference data
static phy_stream_t            *PhyTxStream[PHY_NUM_TX_CHANNELS] = {NULL, NULL};
static phy_stream_t            *PhyRxStream[PHY_NUM_TX_CHANNELS] = {NULL, NULL};

static void Phy_Adrv9001Callback( adrv9001_evt_t evt, void *param )
{

  if(evt.Type == Adrv9001EvtType_Stream)
  {

  }
}

static int32_t Phy_IqStreamStop( phy_stream_t *Stream )
{
  if( Stream == NULL)
    return PhyStatus_Success;

  /* Disable Streaming */
  Adrv9001_IQStream( Stream->Port, NULL, 0 );

  if( PHY_IS_PORT_TX( Stream->Port ))
  {
    /* Clear Global Stream Variable */
    PhyTxStream[PHY_CHANNEL(Stream->Port)] = NULL;
  }
  else if( PHY_IS_PORT_RX( Stream->Port ) )
  {
    /* Clear Global Stream Variable */
    PhyRxStream[PHY_CHANNEL(Stream->Port)] = NULL;
  }

  /* Free Memory */
  free( Stream->Buf );
  free( Stream );

  return PhyStatus_Success;
}

static int32_t Phy_IqStreamStart( phy_stream_t *Stream )
{
  int32_t status = PhyStatus_FileError;

  if( PHY_IS_PORT_TX( Stream->Port ))
  {
    /* Update Global Stream Variable */
    PhyTxStream[PHY_CHANNEL(Stream->Port)] = Stream;
  }
  else if( PHY_IS_PORT_RX( Stream->Port ) )
  {
    /* Update Global Stream Variable */
    PhyRxStream[PHY_CHANNEL(Stream->Port)] = Stream;
  }

  /* Enable Streaming */
  if((status = Adrv9001_IQStream( Stream->Port, (adrv9001_iqdata_t*)Stream->Buf, Stream->Length )) != Adrv9001Status_Success)
  {
    free( Stream->Buf );
    free( Stream );
  }

  return status;
}

static void Phy_Task( void *pvParameters )
{
  phy_queue_t qItem;
  int32_t status;

  for( ;; )
  {
    xQueueReceive( PhyQueue, (void *)&qItem, portMAX_DELAY);

    if( qItem.Evt.Type == PhyEvtType_StreamStop )
    {
      if((status = Phy_IqStreamStop( qItem.Evt.Data.Stream )) != PhyStatus_Success)
        printf("Phy_IqStreamStop Error %d\r\n",status);
    }
    else if( qItem.Evt.Type == PhyEvtType_StreamStart )
    {
      if((status = Phy_IqStreamStart( qItem.Evt.Data.Stream  )) != PhyStatus_Success)
        printf("Phy_IqStreamStart Error %d\r\n",status);
    }
  }
}

phy_status_t Phy_IqStream( adrv9001_port_t Port, adrv9001_iqdata_t *Buf, uint32_t Length)
{
  phy_evt_t Evt = {.Type = PhyEvtType_StreamStop};

  if( PHY_IS_PORT_TX( Port ))
  {
    /* Abstract Stream */
    Evt.Data.Stream = PhyTxStream[PHY_CHANNEL(Port)];
  }
  else if( PHY_IS_PORT_RX( Port ) )
  {
    /* Abstract Stream */
    Evt.Data.Stream = PhyRxStream[PHY_CHANNEL(Port)];
  }
  else
  {
    return PhyStatus_InvalidParameter;
  }

  /* Send Stream Stop to PHY Task */
  if( xQueueSend( PhyQueue, &Evt, 1) != pdPASS )
    return PhyStatus_OsError;

  /* Create New Stream */
  if((Evt.Data.Stream = calloc(1, sizeof(phy_stream_t))) == NULL)
    return PhyStatus_MemoryError;

  /* Initialize Stream */
  Evt.Data.Stream->Buf = (uint32_t*)Buf;
  Evt.Data.Stream->Length = Length;
  Evt.Data.Stream->Port = Port;

  /* Update Event Type to Stream Start */
  Evt.Type = PhyEvtType_StreamStart;

  /* Send Stream Stop to PHY Task */
  if( xQueueSend( PhyQueue, &Evt, 1) != pdPASS )
  {
    free( Evt.Data.Stream );
    return PhyStatus_OsError;
  }

  return PhyStatus_Success;
}

phy_status_t Phy_LoadProfile( const char *filename )
{
  FIL fil;
  int32_t status = PhyStatus_FileError;
  uint32_t *ProfileBuf = NULL;;

  do
  {
    /* Open File */
    if(f_open(&fil, filename, FA_OPEN_EXISTING | FA_READ) != FR_OK) break;

    /* Get Size */
    uint32_t size = f_size(&fil);

    /* Check Size */
    if( size > ADRV9001_PROFILE_SIZE ) break;

    /* Allocate Memory */
    ProfileBuf = malloc(size);

    /* Set Pointer to beginning of file */
    if(f_lseek(&fil, 0) != FR_OK) break;

    /* Read data from file */
    if(f_read(&fil, ProfileBuf, size, &size) != FR_OK) break;

    /* Copy to memory */
    status = 0;

  }while(0);

  /* Free Memory */
  free( ProfileBuf );

  /* Close File */
  if(f_close( &fil ) != FR_OK)
    return PhyStatus_FileError;

  return status;
}

phy_status_t Phy_Initialize( phy_cfg_t *Cfg )
{
  int32_t status;

  /* Copy Configuration Items */
  PhyCallback = Cfg->Callback;
  PhyCallbackRef = Cfg->CallbackRef;

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
