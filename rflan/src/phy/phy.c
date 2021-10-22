
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "app.h"
#include "phy.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "phy_cli.h"
#include "ff.h"

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

static int32_t Phy_IqReadLine( FIL *fil, char *line, int size )
{
  uint len = 1;
  char c;

  while((len > 0) && (size > 0))
  {
    f_read(fil, (void*)&c, 1, &len);

    if(len == 0)
    {
      return XST_FAILURE;
    }
    else
    {
      *line = c;
      line++;
      size--;

      if( c == '\n' )
      {
        *line = 0;
        return XST_SUCCESS;
      }
    }
  }

  return XST_FAILURE;
}

static int32_t Phy_IqStreamStart( phy_stream_t *Stream )
{
  FIL fil;
  int32_t status = PhyStatus_FileError;

  do
  {
    if( PHY_IS_PORT_TX( Stream->Port ))
    {
      /* Open File */
      if(f_open(&fil, Stream->filename, FA_OPEN_EXISTING | FA_READ) != FR_OK) break;

      /* Pointer to beginning of file */
      if(f_lseek(&fil, 0) != FR_OK) break;

      /* Allocate Buffer */
      if((Stream->Buf = malloc(PHY_TX_STREAM_MAX_BUF_SIZE)) == NULL)
      {
         status = PhyStatus_MemoryError;
         break;
      }

      char line[256] = {0};
      char *iq;
      int32_t idata;
      int32_t qdata;
      uint32_t cnt = 0;

      while(Phy_IqReadLine( &fil, line, sizeof(line) ) == XST_SUCCESS)
      {
        iq = strtok(line,",");
        sscanf( iq, "%i", (int32_t*)&idata );
        iq = strtok(NULL,",");
        sscanf( iq, "%i", (int32_t*)&qdata );
        Stream->Buf[cnt] = (((uint32_t)idata & 0xffff) << 16) | ((uint32_t)qdata & 0xffff);
        cnt++;
      }

      /* Enable Streaming */
      if((status = Adrv9001_IQStream( Stream->Port, (adrv9001_iqdata_t*)Stream->Buf, cnt )) != Adrv9001Status_Success) break;

      /* Update Global Stream Variable */
      PhyTxStream[PHY_CHANNEL(Stream->Port)] = Stream;
    }
    else if( PHY_IS_PORT_RX( Stream->Port ) )
    {
      /* Update Global Stream Variable */
      PhyRxStream[PHY_CHANNEL(Stream->Port)] = Stream;

      /* Open File */
      if(f_open(&fil, Stream->filename, FA_CREATE_ALWAYS | FA_WRITE | FA_READ) != FR_OK) break;

      /* Pointer to beginning of file */
      if(f_lseek(&fil, 0) != FR_OK) break;

      /* Enable Streaming */
      if((status = Adrv9001_IQStream( Stream->Port, (adrv9001_iqdata_t*)Stream->Buf, PHY_RX_STREAM_CNT )) != Adrv9001Status_Success) break;

      /* Update Global Stream Variable */
      PhyRxStream[PHY_CHANNEL(Stream->Port)] = Stream;
    }

  }while(0);

  if( status != PhyStatus_Success )
  {
    free( Stream->Buf );
    free( Stream );
  }

  if(f_close(&fil) != FR_OK)
    status = PhyStatus_FileError;

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

int32_t Phy_IqStream( adrv9001_port_t Port, const char *filename )
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
  strcpy(Evt.Data.Stream->filename, filename);
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

int32_t Phy_LoadProfile( const char *filename )
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
