#ifndef __CLI_H__
#define __CLI_H__
/***************************************************************************//**
*  \ingroup    RFLAN
*  \defgroup   CLI Command Line Interface Library
*  @{
*******************************************************************************/
/***************************************************************************//**
*  \file       cli.h
*
*  \details    This file contains the CLI library definitions.
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
 extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#define CLI_STATUS_OFFSET			   (-3000)                  ///< Status code offset
#define CLI_RX_STACK_SIZE               16384                   ///< Rx stack size in words
#define CLI_TX_STACK_SIZE               16384                   ///< Tx stack size in words
#define CLI_RX_TASK_PRIORITY            tskIDLE_PRIORITY + 2    ///< Rx task priority
#define CLI_TX_TASK_PRIORITY            tskIDLE_PRIORITY + 3    ///< Tx task priority
#define CLI_RX_QUEUE_SIZE               32768                   ///< Number of characters in rx queue
#define CLI_TX_QUEUE_SIZE               32768                   ///< Number of characters in tx queue
#define CLI_PRINT_BUF_SIZE              2048                    ///< Size of dedicated print buffer
#define CLI_CMD_BUF_SIZE                1024                    ///< Size of command buffer
#define CLI_CMD_LIST_SIZE               100                     ///< Number of commands
#define CLI_HISTORY_BUF_SIZE            256                     ///< Size of dedicated history buffer

 /**
 ** Status Definition
 */
 typedef enum
 {
   CliStatus_Success               = (0),                       ///< Success
   CliStatus_MemoryError           = (CLI_STATUS_OFFSET - 1),   ///< Memory error
   CliStatus_NotSupported          = (CLI_STATUS_OFFSET - 2),   ///< Not supported function or command
   CliStatus_OsError               = (CLI_STATUS_OFFSET - 3),   ///< Operating system error
   CliStatus_Busy                  = (CLI_STATUS_OFFSET - 4),   ///< Operating system busy
   CliStatus_InvalidParameter      = (CLI_STATUS_OFFSET - 5),   ///< Invalid parameter
   CliStatus_FileError             = (CLI_STATUS_OFFSET - 6),   ///< File error
 } cli_status_t;

 /**
 ** Event type Definition
 */
 typedef enum
 {
   CliEvtType_TxChar    = 0,    ///< Transmit Character Event
   CliEvtType_TxMsg     = 1,    ///< Transmit Message Event
 } cli_evt_type_t;

 /**
 ** Event Definition
 */
 typedef struct
 {
   cli_evt_type_t  Type;         ///< Event Type
   union{
     char          c;            ///< Character
     struct{
     char         *Buf;          ///< Reference to buffer
     uint32_t      Length;       ///< Length of buffer
     }msg;
   };
   void           *CallbackRef;  ///< Parent data provided relative to each request
 } cli_evt_t;

 /***************************************************************************//**
 *
 * \details  Callback function prototype.
 *
 * \param    Evt   [in]  Reference to event, see #cli_evt_t
 *
 * \return   none
 *
 *******************************************************************************/
typedef void (*cli_callback_t)( void *Evt );

/******************************************************************************/
/**
*  \details   This is the CLI callback definition called when the corresponding
*  						command is called
*
*	 \param     ref is a generic reference to be passed between NHL and CLI
*
*  \param	    s is the received string
*
*  \return    none
*******************************************************************************/
typedef void (*CliCmdFn_t)( void *ref, const char *s, void *userData );

/**
 * CLI Command Definition
 */
typedef struct
{
  const char         *cmd;
  const char         *helpString;
  const CliCmdFn_t 		CmdFn;
  int8_t		   				pcount;
  void               *userData;
} cli_cmd_t;

/**
**  Instance structure
**
**  This structure holds the variables associated with this module.  This
**  structure must be allocated and maintained by the application.  The application
**  should not access this structure directly.  The application must pass this
**  variable when calling all APIs.
*/
typedef struct {
  cli_callback_t          Callback;
  void                   *CallbackRef;
  cli_cmd_t      	      **CmdList;
  uint16_t                CmdListLen;
  uint16_t                CmdListSize;
  char                   *CmdBuf;
  uint16_t                CmdBufLen;
  uint16_t                CmdBufSize;
  char                   *HistoryHead;
  char                   *HistoryBuf;
  uint16_t                HistoryBufSize;
  char                   *PrintfBuf;
  uint16_t                PrintfBufSize;
  QueueHandle_t           TxCharQueue;
  QueueHandle_t           RxCharQueue;
  char                    pChar;
} cli_t;

/**
** Type Definition
*/
typedef enum{
  CliParamTypeChar   		= 0,  ///< char
  CliParamTypeU8     		= 1,  ///< uint8_t
  CliParamTypeS8     		= 2,  ///< int8_t
  CliParamTypeU16    		= 3,  ///< uint16_t
  CliParamTypeS16    		= 4,  ///< int16_t
  CliParamTypeU32    		= 5,  ///< uint32_t
  CliParamTypeS32    		= 6,  ///< int32_t
  CliParamTypeU64    		= 7,  ///< uint64_t
  CliParamTypeS64    		= 8,  ///< int64_t
  CliParamTypeFloat  		= 9,  ///< float
  CliParamTypeDouble        = 10, ///< double
  CliParamTypeStr           = 11, ///< string
  CliParamTypeH8            = 12, ///< hex uint8_t
  CliParamTypeH16           = 13, ///< hex uint16_t
  CliParamTypeH32           = 14, ///< hex uint32_t
  CliParamTypeNum           = 15, ///< Number
} CliParamType_t;

/**
**  Initialization Structure
**
**  This structure is used to initialization the module.  The application can
**  destroy the corresponding parameter after calling initializing the module.
*/
typedef struct {
  cli_callback_t        Callback;
  void                 *CallbackRef;
} cli_init_t;

const char *Cli_FindParameter( const char *s, uint16_t pNum, uint16_t *len );
int32_t Cli_Printf( cli_t *Instance, const char * msg,...);
int32_t Cli_GetParameterCount( const char *s );
cli_status_t Cli_ProcessRxChar(cli_t *Instance, char c );
cli_status_t Cli_ProcessRxString(cli_t *Instance, char *str );
cli_status_t Cli_GetParameter(const char *s, uint8_t pNum, CliParamType_t type, void *param );
cli_status_t Cli_RegisterCommand(cli_t *Instance, cli_cmd_t *cmd );

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
cli_status_t Cli_Initialize(cli_t *Instance, cli_init_t *Init );

#ifdef __cplusplus
}
#endif

#endif // __CLI_H_
