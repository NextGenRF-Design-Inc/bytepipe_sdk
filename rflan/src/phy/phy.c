
#include <stdlib.h>
#include <string.h>
#include "app.h"
#include "phy.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"


static QueueHandle_t      PhyQueue;     ///< PHY Queue
static phy_callback_t     PhyCallback;     ///< Callback
static void              *PhyCallbackRef;  ///< Callback reference data

static void Phy_Adrv9001Callback( adrv9001_evt_t evt, void *param )
{

  if(evt.Type == Adrv9001EvtType_Stream)
  {

  }
}

static void Phy_Task( void *pvParameters )
{
  phy_queue_t qItem;

  for( ;; )
  {
    xQueueReceive( PhyQueue, (void *)&qItem, portMAX_DELAY);

    if( qItem.Evt.Type == PhyEvtType_Stream )
    {

    }
    else if( qItem.Evt.Type )
    {

    }
  }
}

int32_t Phy_IqStream( adrv9001_port_t Port, const char *filename )
{

  return XST_SUCCESS;
}

int32_t Phy_LoadProfile( uint8_t *Buf )
{


  return XST_SUCCESS;
}

int32_t Phy_Initialize( phy_cfg_t *Cfg )
{
  int32_t status;

  /* Copy Configuration Items */
  PhyCallback = Cfg->Callback;
  PhyCallbackRef = Cfg->CallbackRef;

  /* Create Queue */
  PhyQueue = xQueueCreate(PHY_QUEUE_SIZE, sizeof(phy_queue_t));

  /* Create Task */
  if(xTaskCreate(Phy_Task, PHY_TASK_NAME, PHY_TASK_STACK_SIZE, NULL, PHY_TASK_PRIORITY, NULL) != pdPASS)
    return XST_FAILURE;

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
