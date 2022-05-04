/***************************************************************************//**
*  \addtogroup PHY
*   @{
*******************************************************************************/
/***************************************************************************//**
*  \file       phy_task.c
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
#include "phy.h"
#include "phy_task.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"


static void PhyTask( phy_t *Instance )
{
  phy_msg_t msg;
  int32_t status;

  for( ;; )
  {
    /* Wait for Message */
    xQueueReceive( Instance->Queue, (void *)&msg, portMAX_DELAY);

    /* Process Message */
    if( msg.task != NULL )
    {
      if((status =  msg.task( Instance, msg.req )) != 0)
      {
        printf("PHY Task Failure: %d\r\n", (int)status);
      }
    }
  }
}

phy_status_t PhyTask_Send( phy_t *Instance, phy_task_t task, phy_task_req_t req )
{
  phy_msg_t msg = {
      .task = task,
      .req = req
  };

  /* Send to PHY Task */
  if( xQueueSend( Instance->Queue, &msg, 1) != pdPASS )
    return PhyStatus_Busy;

  return PhyStatus_Success;
}

phy_status_t PhyTask_Initialize( phy_t *Instance )
{
  /* Create Queue */
  Instance->Queue = xQueueCreate(PHY_QUEUE_SIZE, sizeof(phy_msg_t));

  /* Create Task */
  if(xTaskCreate((TaskFunction_t)PhyTask, PHY_TASK_NAME, PHY_TASK_STACK_SIZE, Instance, PHY_TASK_PRIORITY, NULL) != pdPASS)
    return PhyStatus_OsError;

  return PhyStatus_Success;
}
