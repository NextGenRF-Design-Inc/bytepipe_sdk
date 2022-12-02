/***************************************************************************//**
*  \addtogroup RFLAN_UART
*   @{
*******************************************************************************/
/***************************************************************************//**
*  \file       rflan_uart.c
*
*  \details    This file contains the RFLAN uart.
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
#include "xstatus.h"
#include "rflan_uart.h"


static void RflanUart_Handler(void *CallBackRef, u32 Event, unsigned int EventData)
{
  rflan_uart_t *Instance = (rflan_uart_t*)CallBackRef;

  if((Event == XUARTPS_EVENT_RECV_DATA) && (Instance->ParentCallback != NULL))
  {
    Instance->ParentCallback( Instance->RxChar, Instance->ParentCallbackRef );
  }

  /* Enable Receive */
  XUartPs_Recv(&Instance->RflanUart, (uint8_t*)&Instance->RxChar, 1);
}

int32_t RflanUart_Send( rflan_uart_t *Instance, char c )
{
  if( XUartPs_Send(&Instance->RflanUart, (uint8_t*)&c, 1) == 0)
    return XST_FAILURE;

  return XST_SUCCESS;
}

int32_t RflanUart_Initialize( rflan_uart_t *Instance, rflan_uart_init_t *Init )
{
  XUartPs_Config *Config;

  Instance->ParentCallback = Init->ParentCallback;
  Instance->ParentCallbackRef = Init->ParentCallbackRef;
  Instance->IrqInstance = Init->IrqInstance;

  /* Lookup Configuration */
  if((Config = XUartPs_LookupConfig(Init->DeviceId)) == NULL) return XST_FAILURE;

  /* Initialize Driver */
  if(XUartPs_CfgInitialize(&Instance->RflanUart, Config, Config->BaseAddress) != XST_SUCCESS)
	  return XST_FAILURE;

  /* Self Test */
  if(XUartPs_SelfTest(&Instance->RflanUart) != XST_SUCCESS) return XST_FAILURE;

  /* Connect UART handler */
  XScuGic_Connect(Instance->IrqInstance, Init->IntrId,
      (Xil_ExceptionHandler) XUartPs_InterruptHandler, (void *) &Instance->RflanUart);

  /* Set Local Handler */
  XUartPs_SetHandler(&Instance->RflanUart, (XUartPs_Handler)RflanUart_Handler, Instance);

  if( XUartPs_SetBaudRate(&Instance->RflanUart, Init->BaudRate) != 0)
    return XST_FAILURE;

  /* Setup Interrupt Mask */
  XUartPs_SetInterruptMask(&Instance->RflanUart, XUARTPS_IXR_RXFULL | XUARTPS_IXR_RXOVR );

  XUartPs_SetRecvTimeout(&Instance->RflanUart, 0);

  XUartPs_SetFifoThreshold(&Instance->RflanUart, 1);

  /* Enable the Interrupt */
  XScuGic_Enable(Instance->IrqInstance, Init->IntrId);

  XUartPs_Recv(&Instance->RflanUart, (u8*)&Instance->RxChar, 1);

  /* Set Priority */
  XScuGic_SetPriorityTriggerType(Instance->IrqInstance, Init->IntrId, 0xA0, 0x3);

  return XST_SUCCESS;
}

