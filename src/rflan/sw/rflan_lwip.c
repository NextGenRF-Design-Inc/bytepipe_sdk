/***************************************************************************//**
*  \addtogroup RFLAN
*   @{
*******************************************************************************/
/***************************************************************************//**
*  \file       rflan_server.c
*
*  \details    This file contains the RFLAN server application.
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
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "portmacro.h"
#include "xstatus.h"
#include "rflan_lwip.h"
#include "status.h"
#include "lwip/init.h"
#include "lwip/inet.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "netif/xadapter.h"

static void RflanLwip_PrintIp(char *msg, ip_addr_t *ip)
{
  printf("%s %d.%d.%d.%d\n\r", msg, ip4_addr1(ip), ip4_addr2(ip), ip4_addr3(ip), ip4_addr4(ip));
}

static rflan_lwip_status_t RflanLwip_OpenSocket( rflan_lwip_t *Instance, uint32_t port, struct sockaddr_in *Remote)
{
  /* Create a TCP socket */
  if( ( Instance->Socket = lwip_socket( AF_INET, SOCK_STREAM, 0 ) ) < 0)
    return RflanLwipStatus_SocketError;

  int opt = 1;
  setsockopt(Instance->Socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  /* Bind to port at any interface */
  struct sockaddr_in Address = {
      .sin_family        = AF_INET,
      .sin_port          = htons( port ),
      .sin_addr.s_addr   = INADDR_ANY };

  if( lwip_bind( Instance->Socket, (struct sockaddr*)&Address, sizeof(Address)) < 0)
    return RflanLwipStatus_SocketError;

  /* Set Socket to Listen mode */
  if( lwip_listen( Instance->Socket, 0 ))
    return RflanLwipStatus_SocketError;

  socklen_t Size = sizeof(struct sockaddr_in);

  /* Accept connection and process packets */
  Instance->Socket = lwip_accept( Instance->Socket, (struct sockaddr *)Remote, &Size);

  return RflanLwipStatus_Success;
}

static void RflanLwip_Task( rflan_lwip_t *Instance )
{
  struct sockaddr_in Remote;
  int Length = 0;
  int32_t status;

  if(( status = RflanLwip_OpenSocket( Instance, 2233, &Remote )) != 0 )
    printf("Lwip CLI %s\r\n",StatusString(status));

  /* Indicate Connection */
  RflanLwip_PrintIp("Cli Socket Connected to ", (ip_addr_t*)&Remote.sin_addr);

  for( ;; )
  {
    /* Wait for read data */
    Length = lwip_read( Instance->Socket, Instance->RxBuf, RFLAN_LWIP_RECV_BUF_SIZE);

    if( Length <= 0 )
    {
      lwip_close( Instance->Socket );
      printf("Lwip CLI Socket Disconnected, Reboot device to reset socket server\r\n");
      vTaskDelay(10000);
    }
    else
    {
      Instance->RxBuf[Length] = 0;
      Instance->Callback( Instance->RxBuf, Length, Instance->CallbackRef);
    }
  }
}

rflan_lwip_status_t RflanLwip_Send( rflan_lwip_t *Instance, char *Buf, uint16_t Length )
{
  if( lwip_write( Instance->Socket, Buf, Length ) == Length )
    return RflanLwipStatus_Success;
  else
    return RflanLwipStatus_Busy;
}

rflan_lwip_status_t RflanLwip_Initialize( rflan_lwip_t *Instance, rflan_lwip_init_t *Init )
{
  /* Check Inputs */
  if( (Instance == NULL) || (Init->Callback == NULL))
    return RflanLwipStatus_InvalidParameter;

  /* Copy Init */
  Instance->Callback = Init->Callback;
  Instance->CallbackRef = Init->CallbackRef;

  /* Allocate receive buffer */
  Instance->RxBuf = calloc(1, Init->RxBufSize );

  /* Initialize lwIP */
  lwip_init();

  /* Add network interface to the netif_list, and set it as default */
  if( !xemac_add( &Instance->NetIf, NULL, NULL, NULL, Init->MacAddr, Init->BaseAddr ))
    return XST_FAILURE;

  /* Set network interface */
  netif_set_default( &Instance->NetIf );

  /* Setup network interface */
  netif_set_up( &Instance->NetIf );

  /* Start receive thread */
  sys_thread_new( "lwip_rx",(void(*)(void*))xemacif_input_thread, &Instance->NetIf, TCPIP_THREAD_STACKSIZE, DEFAULT_THREAD_PRIO);

  /* Add IP address */
  inet_aton(Init->IpAddr, &Instance->NetIf.ip_addr );
  inet_aton(Init->IpMask, &Instance->NetIf.netmask );
  inet_aton(Init->IpGw, &Instance->NetIf.gw );

  /* Create CLI Socket Thread */
  sys_thread_new("lwip", (lwip_thread_fn)RflanLwip_Task, Instance, TCPIP_THREAD_STACKSIZE, TCPIP_THREAD_PRIO);

  printf("Lwip Enabled\r\n");
  printf("Ip Address: %s\r\n", Init->IpAddr);
  printf("Opening CLI on Socket %lu\r\n", Init->CliPort);

  return XST_SUCCESS;
}


