/***************************************************************************//**
*  \addtogroup CLI
*   @{
*******************************************************************************/
/***************************************************************************//**
*  \file       cli.c
*
*  \details    This file contains the CLI library implementation.
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
#include "stdint.h"
#include "stdio.h"
#include "stdarg.h"
#include "stdbool.h"
#include "stdlib.h"
#include "string.h"
#include "ctype.h"
#include "cli.h"
#include "mem.h"
#include "xil_printf.h"
#include "portmacro.h"
#include "xparameters.h"
#include "xil_types.h"
#include "xstatus.h"

#define CR                       (0x0D)
#define LF                       (0x0A)
#define BS                       (0x08)
#define ESC                      (0x1B)
#define SPACE					 					 (0x20)
#define DEL                      (0x7F)
#define UP                       (27)

extern u32 xInsideISR;
static QueueHandle_t   OutByteQueue = NULL;

void outbyte(char c)
{
  if( xInsideISR == pdTRUE )
  {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xQueueSendFromISR( OutByteQueue, &c, &xHigherPriorityTaskWoken );
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
  }
  else
  {
    xQueueSend(OutByteQueue, &c, 1);
  }
}

int32_t Cli_Printf( cli_t *Instance, const char * msg,...)
{
  va_list args;
  va_start( args, msg );
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;

  Instance->PrintfBuf[0] = 0;
  vsnprintf( Instance->PrintfBuf, Instance->PrintfBufSize-1 , msg, args );

  for( int i = 0; i < strlen(Instance->PrintfBuf); i++ )
  {
    if( xInsideISR == pdTRUE )
    {
      xQueueSendFromISR( Instance->TxCharQueue, &Instance->PrintfBuf[i], &xHigherPriorityTaskWoken );
      portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
    else
    {
      xQueueSend(Instance->TxCharQueue, &Instance->PrintfBuf[i], 1);
    }
  }

  va_end( args );

  return 0;
}

static void CliHelpCmd(cli_t *Instance, const char *cmd, void *param )
{
  uint8_t len = strlen(cmd);

  if( len >= 5 )
    len = len - 5;
  else
    len = 0;

  for( uint16_t i = 0; i < Instance->CmdListLen; i++ )
  {
    if(Instance->CmdList[i] == NULL) break;

    if(len > 0)
    {
      if(strstr(Instance->CmdList[i]->helpString,cmd + 5) != NULL)
        Cli_Printf(Instance, Instance->CmdList[i]->helpString);
    }
    else
    {
      Cli_Printf(Instance, Instance->CmdList[i]->helpString );
    }
  }

  Cli_Printf(Instance, "\r\n" );
}

cli_cmd_t CliHelpDef =
{
    "help",
    "\r\nhelp Lists all the registered commands.\r\n"
    "help <optional sorting string with first letters of command(s) of interest>\r\n\r\n",
    (CliCmdFn_t)CliHelpCmd,
    -1,
    NULL
};

static cli_status_t Cli_StrToNum(char *p, CliParamType_t type, void *param )
{
	short       tmpResult = -1;
	int64_t 		*temIPtr;
	double 			doubleFloat;
	uint64_t    utemp;

	if( (p == NULL) || (param == NULL)) return -1;

	switch( type )
	{
	case CliParamTypeU8:
	case CliParamTypeH8:

    if( (p[1] == 'x') || (p[1] == 'X') )
      tmpResult = sscanf( p, "%hhx", (uint8_t*)&utemp );
    else
      tmpResult = sscanf( p, "%hhu", (uint8_t*)&utemp );

    memcpy((uint8_t*)param, (uint8_t*)&utemp, sizeof(uint8_t));
		break;

	case CliParamTypeU16:
	case CliParamTypeH16:
    if( (p[1] == 'x') || (p[1] == 'X') )
      tmpResult = sscanf( p, "%hx", (uint16_t*)&utemp );
    else
      tmpResult = sscanf( p, "%hu", (uint16_t*)&utemp );

    memcpy((uint8_t*)param, (uint8_t*)&utemp, sizeof(uint16_t));
    break;

	case CliParamTypeU32:
	case CliParamTypeH32:

		if( (p[1] == 'x') || (p[1] == 'X') )
			tmpResult = sscanf( p, "%lx", (uint32_t*)&utemp );
		else
			tmpResult = sscanf( p, "%lu", (uint32_t*)&utemp );

		memcpy((uint8_t*)param, (uint8_t*)&utemp, sizeof(uint32_t));
		break;

	case CliParamTypeU64:
		if( (p[1] == 'x') || (p[1] == 'X') )
			tmpResult = sscanf( p, "%llx", (uint64_t*)param );
		else
			tmpResult = sscanf( p, "%llu", (uint64_t*)param );
		break;

	case CliParamTypeS8:
		if( (p[1] == 'x') || (p[1] == 'X') )
		{
			int16_t itemp;
			tmpResult = sscanf( p, "%hx", &itemp );
			*(int8_t*)param = (int8_t)itemp;
		}
		else
			tmpResult = sscanf( p, "%hhi", (char*)param );
		break;

	case CliParamTypeS16:
		if( (p[1] == 'x') || (p[1] == 'X') )
			tmpResult = sscanf( p, "%hx", (int16_t*)param );
		else
			tmpResult = sscanf( p, "%hi", (int16_t*)param );
		break;

	case CliParamTypeS32:
		if( (p[1] == 'x') || (p[1] == 'X') )
			tmpResult = sscanf( p, "%lx", (int32_t*)param );
		else
			tmpResult = sscanf( p, "%d", (int*)param );
		break;

	case CliParamTypeS64:
		temIPtr = (int64_t*)param;
		tmpResult = sscanf( p, "%lf", (double*)&doubleFloat );
		*temIPtr = (int64_t)doubleFloat;
		break;

	case CliParamTypeFloat:
		tmpResult = sscanf( p, "%f", (float *)param );
		break;

	case CliParamTypeDouble:
		tmpResult = sscanf( p, "%lf", (double*)param );
		break;

	case CliParamTypeStr:
		tmpResult = sscanf( p, "%s", (uint8_t *)param );
		break;

	case CliParamTypeChar:
	  *(char *)param = *p;
		tmpResult = 1;
		break;

	default:
		tmpResult = 0;
		break;
	}

	if( tmpResult == 0 ) return CliStatus_InvalidParameter;

	return CliStatus_Success;
}

static int Cli_ProcessCommand( cli_t *Instance )
{
  const char   *cmdString;
  const char   *cmd = Instance->CmdBuf;
  int           cmdStringLen;
  cli_status_t 	status = 0;
  uint16_t i;

  do
  {
	  // Make sure variables are initialized
	  if ( (Instance->CmdList == NULL) || (Instance->CmdListLen == 0) )
		{
			status = CliStatus_InvalidParameter;
			break;
		}

	  /* Check for commented text */
	  if( strncmp( cmd, "//", 2 ) == 0 )
	    return CliStatus_Success;

	  for(i = 0; i < Instance->CmdListLen; i++)
	  {
		  cmdString = Instance->CmdList[i]->cmd;
		  cmdStringLen = strlen( cmdString );

		  /*
		   ** To ensure the string lengths match exactly, so as not to pick up
		   ** a sub-string of a longer command, check the byte after the expected
		   ** end of the string is either the end of the string or a space before
		   ** a parameter.
		   */
		  if( ( cmd[ cmdStringLen ] == ' ' ) || ( cmd[ cmdStringLen ] == 0x00 ) )
		  {
			  if( strncmp( cmd, cmdString, cmdStringLen ) == 0 )
			  {
				  /*
				   ** The command has been found.  Check if it has the expected
				   ** number of parameters.  If cmdList[i]->pcntMin is -1, then
				   ** the number of parameters is not checked.
				   */
			  	if(Instance->CmdList[i]->pcount >= 0)
				  {
					  // is there a valid number of parameters
					  if(Cli_GetParameterCount( cmd ) != Instance->CmdList[i]->pcount)
					  {
							  status = CliStatus_InvalidParameter;
							  Cli_Printf(Instance, "Incorrect command parameter(s).  Enter \"help\" to view a list of available commands.\r\n\r\n" );
					  }
				  }
				  break;
			  }
		  }
	  }

	  if(i == Instance->CmdListLen)
	  {
		  /*
		   ** No command match was found.
		   */
		  status = CliStatus_InvalidParameter;
		  Cli_Printf(Instance, "Command not recognized.\r\n\n");
	  }
	  else if(status == 0)
	  {
			Instance->CmdList[i]->CmdFn(Instance, cmd, Instance->CmdList[i]->userData );
	  }
  } while (0);

  return status;
}

static void Cli_TxTask( cli_t *Instance )
{
  cli_evt_t Evt = {
      .Type = CliEvtType_TxChar,
      .CallbackRef = Instance->CallbackRef
  };

  for( ;; )
  {
    xQueueReceive( Instance->TxCharQueue, (void *)&Evt.c, portMAX_DELAY);

    if( Instance->Callback != NULL )
      Instance->Callback( &Evt );
  }
}

static void Cli_RxTask( cli_t *Instance )
{
  uint8_t c;

  for( ;; )
  {
    xQueueReceive( Instance->RxCharQueue, (void *)&c, portMAX_DELAY );

    // Check to see if this is the end of the command string
     if( (c == CR) || (c == LF) )
     {
       /* Echo New line */
       Cli_Printf(Instance, "\r\n");

       if( Instance->CmdBufLen != 0 )
       {
         /* Add NULL to end of command */
         Instance->CmdBuf[Instance->CmdBufLen] = 0;

         /* Shift history with new command */
         for(int i = Instance->HistoryBufSize-1; i > Instance->CmdBufLen; i--)
         {
           Instance->HistoryBuf[i] = Instance->HistoryBuf[i - 1 - Instance->CmdBufLen];
         }

         /* Copy new Command */
         memcpy(Instance->HistoryBuf, Instance->CmdBuf, Instance->CmdBufLen+1);

         /* Reset History Reference */
         Instance->HistoryHead = NULL;

         /* Process Command */
         Cli_ProcessCommand( Instance );

         /* Clear Command */
         Instance->CmdBufLen = 0;
         memset(Instance->CmdBuf, 0x00, Instance->CmdBufSize);
       }
     }
     else if ( c == '\t' )
     {
       if( Instance->CmdBufLen > 1 )
       {
         if (Instance->CmdBuf[Instance->CmdBufLen-1] == '\t')
         {
           /* Clear Terminal */
           Cli_Printf(Instance, "\033[2K");
           Cli_Printf(Instance, "\r" );

           for(uint16_t i = 0; i < Instance->CmdListLen; i++)
           {

             /* Check if current text is at beginning of command in list */
             if(strstr(Instance->CmdList[i]->cmd, Instance->CmdBuf) == Instance->CmdList[i]->cmd)
             {
               /* Display on screen */
               Cli_Printf(Instance, Instance->CmdList[i]->cmd, Instance->CallbackRef );
               Cli_Printf(Instance, "\r\n", Instance->CallbackRef );
             }
           }

           Instance->CmdBufLen = 0;

           /* Add NULL to end of command */
           Instance->CmdBuf[Instance->CmdBufLen] = 0;
         }
       }

       if(Instance->CmdBufLen > 0)
       {
         const char *Match = NULL;
         uint8_t  MatchLen = 0xff;

         for(uint16_t i = 0; i < Instance->CmdListLen; i++)
         {
           if(Instance->CmdList[i] == NULL) break;

           /* Check if current text is at beginning of command in list */
           if(strstr(Instance->CmdList[i]->cmd, Instance->CmdBuf) == Instance->CmdList[i]->cmd)
           {
             /* Find the shortest match that doesn't match any previous match */
             for(int j = 0; j < MatchLen; j++)
             {
               if( Match == NULL)
                 Match = Instance->CmdList[i]->cmd;

               if((Instance->CmdList[i]->cmd[j] == '\0') || (Instance->CmdList[i]->cmd[j] == ' ') || (Instance->CmdList[i]->cmd[j] != Match[j]))
               {
                 /* Compare to previous match */
                 if( Match != NULL )
                 {
                   MatchLen = strspn(Match, Instance->CmdList[i]->cmd);
                   Match = Instance->CmdList[i]->cmd;
                 }
                 else
                 {
                   MatchLen = j;
                 }
                 break;
               }
             }
           }
         }

         if( Match != NULL )
         {
           /* Clear Terminal */
           Cli_Printf(Instance, "\033[2K", Instance->CallbackRef );
           Cli_Printf(Instance, "\r", Instance->CallbackRef );

           Instance->CmdBufLen = MatchLen;

           /* Copy reference to current command */
           strncpy(Instance->CmdBuf, Match, Instance->CmdBufLen);

           /* Add NULL to end of command */
           Instance->CmdBuf[Instance->CmdBufLen] = 0;

           /* Display on screen */
           Cli_Printf(Instance, Instance->CmdBuf, Instance->CallbackRef );
         }
       }
     }
     else if ( c == BS ) // Check for Backspace and Delete characters
     {
       if(Instance->CmdBufLen > 0)
       {
         const char str[4] = {BS,SPACE,BS,0};
         Instance->CmdBuf[--Instance->CmdBufLen] = 0;
         Cli_Printf(Instance, str, Instance->CallbackRef );
       }
     }
     else if (strncmp(&Instance->CmdBuf[Instance->CmdBufLen-2],"\e[",2) == 0 )
     {
       if( Instance->HistoryHead == NULL)
       {
         /* Clear Terminal */
         Cli_Printf(Instance, "\033[2K", Instance->CallbackRef );
         Cli_Printf(Instance, "\r", Instance->CallbackRef );

         /* Set Reference at beginning of Buf */
         Instance->HistoryHead = Instance->HistoryBuf;

         /* Get length */
         Instance->CmdBufLen = strlen(Instance->HistoryHead);

         /* Copy reference to current command */
         strncpy(Instance->CmdBuf, Instance->HistoryHead, Instance->CmdBufLen);

         /* Add NULL to end of command */
         Instance->CmdBuf[Instance->CmdBufLen] = 0;

         /* Display on screen */
         Cli_Printf(Instance, Instance->CmdBuf, Instance->CallbackRef );

       }
       else if( c == 'A')
       {
         /* Clear Terminal */
         Cli_Printf(Instance, "\033[2K", Instance->CallbackRef );
         Cli_Printf(Instance, "\r", Instance->CallbackRef );

         /* Move Reference by length of previous reference */
         if((Instance->HistoryHead + strlen(Instance->HistoryHead)) < (Instance->HistoryBuf + Instance->HistoryBufSize))
         {
           Instance->HistoryHead += strlen(Instance->HistoryHead) + 1;
         }

         /* Get New command Length from history */
         if((Instance->HistoryHead + strlen(Instance->HistoryHead)) <= (Instance->HistoryBuf + Instance->HistoryBufSize))
           Instance->CmdBufLen = strlen(Instance->HistoryHead);
         else
           Instance->CmdBufLen = Instance->HistoryHead - Instance->HistoryBuf;

         /* Copy reference to current command */
         strncpy(Instance->CmdBuf, Instance->HistoryHead, Instance->CmdBufLen);

         /* Add NULL to end of command */
         Instance->CmdBuf[Instance->CmdBufLen] = 0;

         /* Display on screen */
         Cli_Printf(Instance, Instance->CmdBuf, Instance->CallbackRef );
       }
       else if( c == 'B')
       {
         /* Clear Terminal */
         Cli_Printf(Instance, "\033[2K", Instance->CallbackRef );
         Cli_Printf(Instance, "\r", Instance->CallbackRef );

         /* Move Reference by length of previous reference */
         while(Instance->HistoryHead > Instance->HistoryBuf)
         {
           Instance->HistoryHead--;

           if(*(Instance->HistoryHead-1) == 0)
             break;
         }

         /* Get New command Length from history */
         Instance->CmdBufLen = strlen(Instance->HistoryHead);

         /* Copy reference to current command */
         strncpy(Instance->CmdBuf, Instance->HistoryHead, Instance->CmdBufLen);

         /* Add NULL to end of command */
         Instance->CmdBuf[Instance->CmdBufLen] = 0;

         /* Display on screen */
         Cli_Printf(Instance, Instance->CmdBuf, Instance->CallbackRef );
       }
     }
     else
     {
       /* Save character */
   //    if ( isprint((uint8_t)c) && (Instance->CmdBufLen < Instance->CmdBufSize) )
       if ( Instance->CmdBufLen < Instance->CmdBufSize )
       {
         /* Echo */
         const char str[2] = {c,0};

         Cli_Printf(Instance, str, Instance->CallbackRef );
         Instance->CmdBuf[Instance->CmdBufLen++] = c;
         Instance->CmdBuf[Instance->CmdBufLen + 1] = 0;

       }
     }

    Instance->pChar = c;

  }

  /* Terminate this task */
  vTaskDelete(NULL);
}

const char *Cli_FindParameter( const char *s, uint16_t pNum, uint16_t *len )
{
  uint16_t cnt = 0;
  uint16_t length = 0;
  const char *sReturn = NULL;

  while( cnt < pNum )
  {
    /* Find first space */
    while((( *s ) != 0x00 ) && ( ( *s ) != ' ' )) s++;

    /* Find the start of the next string */
    while((( *s ) != 0x00 ) && ( ( *s ) == ' ' )) s++;

    /* Check if a string found */
    if( *s != 0x00 )
    {
      cnt++;

      if( cnt == pNum )
      {
        sReturn = s;
        while((( *s ) != 0x00 ) && ( ( *s ) != ' ' ))
        {
          length++;
          s++;
        }

        if( length == 0 ) sReturn = NULL;

        break;
      }
    }
    else
    {
      break;
    }
  }

  if( len != NULL)
    *len = length;

  return sReturn;
}

cli_status_t Cli_GetParameter(const char *s, uint8_t pNum, CliParamType_t type, void *param )
{
  const char   *p = NULL;
  uint16_t      len = 0;

  if( (s == NULL) || (param == NULL)) return CliStatus_InvalidParameter;

  /* Get the parameter */
  p = Cli_FindParameter( s, pNum, &len );

  if( p == NULL ) return CliStatus_InvalidParameter;

  return Cli_StrToNum((char*)p, type, param );
}

cli_status_t Cli_ProcessRxString(cli_t *Instance, char *str )
{
  strncpy( Instance->CmdBuf, str, Instance->CmdBufSize );

  /* Process Command */
  Cli_ProcessCommand( Instance );

  /* Clear Command */
  Instance->CmdBuf[0] = 0;

  return 0;
}

cli_status_t Cli_ProcessRxChar(cli_t *Instance, char c )
{
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;

  int32_t status = xQueueSendFromISR( Instance->RxCharQueue, &c, &xHigherPriorityTaskWoken );

  portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

  return status == pdPASS? CliStatus_Success : CliStatus_OsError;
}

int32_t Cli_GetParameterCount( const char *s )
{
  uint32_t cnt = 0;
  uint8_t wasSpace = 0;

  while( *s != 0x00 )
  {
    if( ( *s ) == ' ' )
    {
      if( wasSpace != 1 )
      {
        cnt++;
        wasSpace = 1;
      }
    }
    else
    {
      wasSpace = 0;
    }

    s++;
  }

  /* Check if ends with Space */
  if( wasSpace == 1 ) cnt--;

  return cnt;
}

cli_status_t Cli_RegisterCommand(cli_t *Instance, cli_cmd_t *cmd )
{
	/*  Make sure variables are initialized */
  if ( (Instance->CmdList == NULL) || (Instance->CmdListSize == 0) ) return -1;

	/* Check the command to register is not NULL */
	if (cmd == NULL) return CliStatus_InvalidParameter;

	/* Find an empty slot to populate */
	for (uint16_t i = 0; i < Instance->CmdListSize; i++)
	{
		if (Instance->CmdList[i] == NULL)
		{
			Instance->CmdList[i] = cmd;
			Instance->CmdListLen++;
			break;
		}
	}

	return CliStatus_Success;
}

cli_status_t Cli_Initialize(cli_t *Instance, cli_init_t *Init )
{
  if ((Instance == NULL) || (Init == NULL))
    return CliStatus_InvalidParameter;

  /* Copy Callback Information */
  Instance->Callback = Init->Callback;
  Instance->CallbackRef = Init->CallbackRef;

  /* Create Command List */
  Instance->CmdList = calloc(CLI_CMD_LIST_SIZE, sizeof( cli_cmd_t*));
  Instance->CmdListLen = 0;
  Instance->CmdListSize = CLI_CMD_LIST_SIZE;

  /* Create Working Command Buffer */
  Instance->CmdBuf = calloc(1, CLI_CMD_BUF_SIZE);
  Instance->CmdBufSize = CLI_CMD_BUF_SIZE;

  /* Create History Buffer */
  Instance->HistoryBuf = calloc(1, CLI_HISTORY_BUF_SIZE);
  Instance->HistoryBufSize = CLI_HISTORY_BUF_SIZE;

  /* Create Printf Buffer */
  Instance->PrintfBuf = calloc(1, CLI_PRINT_BUF_SIZE);
  Instance->PrintfBufSize = CLI_PRINT_BUF_SIZE;

  /* Create Rx Queue */
  Instance->RxCharQueue = xQueueCreate(CLI_RX_QUEUE_SIZE, sizeof(uint8_t));

  /* Create CLI Rx Task */
  if(xTaskCreate((TaskFunction_t)Cli_RxTask, "CliRx", CLI_RX_STACK_SIZE, Instance, CLI_RX_TASK_PRIORITY, NULL) != pdPASS)
    return CliStatus_OsError;

  /* Create Tx Queue */
  Instance->TxCharQueue = xQueueCreate(CLI_TX_QUEUE_SIZE, sizeof(uint8_t));

  /* Create CLI Tx Task */
  if(xTaskCreate((TaskFunction_t)Cli_TxTask, "CliTx", CLI_TX_STACK_SIZE, Instance, CLI_TX_TASK_PRIORITY, NULL) != pdPASS)
    return CliStatus_OsError;

  /* Register Base Commands */
  Cli_RegisterCommand(Instance, &CliHelpDef);

  OutByteQueue = Instance->TxCharQueue;

  return CliStatus_Success;
}



/** @} */
