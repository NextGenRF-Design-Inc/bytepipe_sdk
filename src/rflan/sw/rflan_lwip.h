#ifndef SRC_RFLAN_LWIP_H_
#define SRC_RFLAN_LWIP_H_
/***************************************************************************//**
*  \ingroup    RFLAN
*  \defgroup   RFLAN_LWIP RFLAN LWIP Application
*  @{
*******************************************************************************/
/***************************************************************************//**
*  \file       rflan_lwip.h
*
*  \details    This file contains the RFLAN lwip application definitions.
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
#ifdef __cplusplus
extern "C"
{
#endif


#include <stdint.h>

#ifdef RFLAN_LWIP_ENABLE

#include "rflan_cli.h"
#include "lwip/ip4_addr.h"
#include "lwip/tcp.h"
#include "lwip/tcpip.h"
#include "lwip/netif.h"

#define RFLAN_LWIP_STATUS_OFFSET        (-4000)
#define RFLAN_LWIP_STACK_SIZE           1024
#define RFLAN_LWIP_TASK_PRIORITY        tskIDLE_PRIORITY + 3
#define RFLAN_LWIP_MAC_ADDR             { 0x00, 0x00, 0x00, 0x0a, 0x35, 0x00, 0x01, 0x02 }
#define RFLAN_LWIP_CLI_PORT             (2233)
#define RFLAN_LWIP_RECV_BUF_SIZE        (1500)


/**
* \brief Code indicated status of request
*/
typedef enum
{
  RflanLwipStatus_Success               = (0),
  RflanLwipStatus_MemoryError           = (RFLAN_LWIP_STATUS_OFFSET - 1),
  RflanLwipStatus_OsError               = (RFLAN_LWIP_STATUS_OFFSET - 2),
  RflanLwipStatus_Busy                  = (RFLAN_LWIP_STATUS_OFFSET - 3),
  RflanLwipStatus_InvalidParameter      = (RFLAN_LWIP_STATUS_OFFSET - 4),
  RflanLwipStatus_Disconnected          = (RFLAN_LWIP_STATUS_OFFSET - 5),
  RflanLwipStatus_SocketError           = (RFLAN_LWIP_STATUS_OFFSET - 6),
} rflan_lwip_status_t;

/***************************************************************************//**
*
* \details  Callback function prototype.
*
* \param    Buf          [in]  Received buffer
* \param    Length       [in]  Received buffer length in characters
* \param    CallbackRef  [in]  Callback reference provided by the application
*
* \return   none
*
*******************************************************************************/
typedef void (*rflan_lwip_callback_t)( char *Buf, uint16_t Length, void *CallbackRef  );

/**
**  Instance structure
**
**  This structure holds the variables associated with this module.  This
**  structure must be allocated and maintained by the application.  The application
**  should not access this structure directly.  The application must pass this
**  variable when calling all APIs.
*/
typedef struct
{
  struct netif            NetIf;
  int                     Socket;
  char                   *RxBuf;
  rflan_lwip_callback_t   Callback;
  void                   *CallbackRef;
} rflan_lwip_t;

/**
**  Initialization Structure
**
**  This structure is used to initialization the module.  The application can
**  destroy the corresponding parameter after calling initializing the module.
*/
typedef struct
{
  uint32_t                RxBufSize;
  uint32_t                CliPort;
  rflan_lwip_callback_t   Callback;
  void                   *CallbackRef;
  uint32_t                BaseAddr;
  const char             *IpAddr;
  const char             *IpMask;
  const char             *IpGw;
  uint8_t                 MacAddr[8];
} rflan_lwip_init_t;

rflan_lwip_status_t RflanLwip_Send( rflan_lwip_t *Instance, char *Buf, uint16_t Length );

/***************************************************************************//**
*
* \details  Initialize Driver
*
* \param    Instance [in]  Driver Instance
* \param    Init     [in]  Initialization structure
*
* \return   status
*
*******************************************************************************/
rflan_lwip_status_t RflanLwip_Initialize( rflan_lwip_t *Instance, rflan_lwip_init_t *Init );

#endif

#endif /* SRC_RFLAN_LWIP_H_ */
