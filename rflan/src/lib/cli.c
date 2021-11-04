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

#define CR                       (0x0D)
#define LF                       (0x0A)
#define BS                       (0x08)
#define ESC                      (0x1B)
#define SPACE					 					 (0x20)
#define DEL                      (0x7F)
#define UP                       (27)

/******************************************************************************/
/**
*  \details   CLI Command for Help
*
*  \param     Instance should be a reference to the CLI instance
*
*  \param     cmd is the command string
*
*  \return    none
*******************************************************************************/
static void CliHelpCmd(Cli_t *CliInstance, const char *cmd, void *param )
{
  uint8_t len = strlen(cmd);

  if( len >= 5 )
    len = len - 5;
  else
    len = 0;

  for(uint16_t i = 0; i < CliInstance->CmdListLen; i++)
  {
    if(CliInstance->CmdList[i] == NULL) break;

    if(len > 0)
    {
      if(strstr(CliInstance->CmdList[i]->helpString,cmd + 5) != NULL)
        CliInstance->Callback(CliInstance->CmdList[i]->helpString, CliInstance->CallbackRef );
    }
    else
    {
      CliInstance->Callback(CliInstance->CmdList[i]->helpString, CliInstance->CallbackRef );
    }
  }

  CliInstance->Callback("\r\n", CliInstance->CallbackRef );
}

/**
* Help Command Definition
*/
const CliCmd_t CliHelpDef =
{
    "help",
    "\r\nhelp Lists all the registered commands.\r\n"
    "help <optional sorting string with first letters of command(s) of interest>\r\n\r\n",
    (CliCmdFn_t)CliHelpCmd,
    -1,
    NULL
};


/******************************************************************************/
/**
*  \details   Convert String to number
*
*  \param	  	p is a pointer to the string
*
*  \param	 	 	type is the data type
*
*  \param 	  param is a pointer to the number
*
*  \return    status
*******************************************************************************/
static int Cli_StrToNum(char *p, CliParamType_t type, void *param )
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

	case CliParamTypeU64:	// This is a work around for the Cortex M4
		if( (p[1] == 'x') || (p[1] == 'X') )
			tmpResult = sscanf( p, "%llx", (uint64_t*)param );
		else
			tmpResult = sscanf( p, "%llu", (uint64_t*)param );
		break;

	case CliParamTypeS8:
		if( (p[1] == 'x') || (p[1] == 'X') )
		{
			int16_t itemp;							// this is a work around for scanf
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

	case CliParamTypeS64:	// This is a work around for the Cortex M4
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

	if( tmpResult == 0 ) return -1;


	return 0;
}

static void CliStr2Num(Cli_t *CliInstance, const char *cmd, void *param )
{
  char *type = calloc(1, 32 );
  Cli_GetParameter(cmd, 1, CliParamTypeStr, type);

  char *str = calloc(1, 32 );
  Cli_GetParameter(cmd, 2, CliParamTypeStr, str);

  if(strcmp(type, "c") == 0)
  {
    char tmp;
    Cli_StrToNum(str, CliParamTypeChar, &tmp);
    printf("%c\r\n",tmp);
  }
  else if(strcmp(type, "u8") == 0)
  {
    uint8_t tmp;
    Cli_StrToNum(str, CliParamTypeU8, &tmp );
    printf("%hhu\r\n",tmp);
  }
  else if(strcmp(type, "s8") == 0)
  {
    int8_t tmp;
    Cli_StrToNum(str, CliParamTypeS8, &tmp );
    printf("%hhi\r\n",tmp);
  }
  else if(strcmp(type, "u16") == 0)
  {
    uint16_t tmp;
    Cli_StrToNum(str, CliParamTypeU16, &tmp );
    printf("%hu\r\n",tmp);
  }
  else if(strcmp(type, "s16") == 0)
  {
    int16_t tmp;
    Cli_StrToNum(str, CliParamTypeS16, &tmp );
    printf("%hi\r\n",tmp);
  }
  else if(strcmp(type, "u32") == 0)
  {
    uint32_t tmp;
    Cli_StrToNum(str, CliParamTypeU32, &tmp );
    printf("%lu\r\n",tmp);
  }
  else if(strcmp(type, "s32") == 0)
  {
    int32_t tmp;
    Cli_StrToNum(str, CliParamTypeS32, &tmp );
    printf("%li\r\n",tmp);
  }
  else if(strcmp(type, "u64") == 0)
  {
    uint64_t tmp;
    Cli_StrToNum(str, CliParamTypeU64, &tmp );
    printf("%llu\r\n",tmp);
  }
  else if(strcmp(type, "s64") == 0)
  {
    int64_t tmp;
    Cli_StrToNum(str, CliParamTypeS64, &tmp );
    printf("%lli\r\n",tmp);
  }
  else if(strcmp(type, "float") == 0)
  {
    float tmp;
    Cli_StrToNum(str, CliParamTypeFloat, &tmp );
    printf("%f\r\n",tmp);
  }
  else if(strcmp(type, "double") == 0)
  {
    double tmp;
    Cli_StrToNum(str, CliParamTypeDouble, &tmp );
    printf("%lf\r\n",tmp);
  }
  else
  {
    printf("Valid Formats include: c,u8,s8,u16,s16,u32,s32,u64,s64,float,double\r\n");
  }

}

/**
* Str2Num Command Definition
*/
const CliCmd_t CliStr2NumDef =
{
    "Str2Num",
    "Str2Num: Convert String to number\r\n"
    "Str2Num <format, value>\r\n\r\n",
    (CliCmdFn_t)CliStr2Num,
    2,
    NULL
};

/******************************************************************************/
/**
*  \details   Get Number of parameters in command string
*
*  \param	  	s is the command string
*
*  \return    number of parameters
*******************************************************************************/
static int16_t Cli_GetParameterCount( const char *s )
{
  uint8_t cnt = 0;
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

/******************************************************************************/
/**
*  \details   Process a received command string
*
*  \param	  	Instance is a reference to the CLI Instance
*
*  \return    status
*******************************************************************************/
static int Cli_ProcessCommand( Cli_t *Instance )
{
  const char   *cmdString;
  const char   *cmd = Instance->Cmd;
  int           cmdStringLen;
  int 					status = 0;
  uint16_t i;

  do
  {
	  // Make sure variables are initialized
	  if ( (Instance->CmdList == NULL) || (Instance->CmdListLen == 0) )
		{
			status = -1;
			break;
		}

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
							  status = -1;
							  Instance->Callback("Incorrect command parameter(s).  Enter \"help\" to view a list of available commands.\r\n\r\n", Instance->CallbackRef );
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
		  status = -1;
		  Instance->Callback("Command not recognized.\r\n\n", Instance->CallbackRef );
	  }
	  else if(status == 0)
	  {
			Instance->CmdList[i]->CmdFn(Instance, cmd, Instance->CmdList[i]->userData );
	  }
  } while (0);

  return status;
}

/*******************************************************************************

  PURPOSE:  Get the requested parameter by number.

  COMMENT:

*******************************************************************************/
int32_t Cli_GetParameter(const char *s, uint8_t pNum, CliParamType_t type, void *param )
{
	const char   *p = NULL;
	uint16_t  		len = 0;

	if( (s == NULL) || (param == NULL)) return -1;

	/* Get the parameter */
	p = Cli_FindParameter( s, pNum, &len );

	if( p == NULL ) return -1;

	return Cli_StrToNum((char*)p, type, param );
}

/*******************************************************************************

  PURPOSE:  Find Parameter string based on number

  COMMENT:

*******************************************************************************/
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

static char pChar = ' ';

/*******************************************************************************

  PURPOSE:  Process a received character

  COMMENT:

*******************************************************************************/
void Cli_ProcessChar(Cli_t *Instance, char c )
{
  // Check to see if this is the end of the command string
	if( (c == CR) || (c == LF) )
	{
    /* Echo New line */
    Instance->Callback("\r\n", Instance->CallbackRef );

		if( Instance->CmdLen != 0 )
		{
			/* Add NULL to end of command */
			Instance->Cmd[Instance->CmdLen] = 0;

			/* Shift history with new command */
			for(int i = Instance->History.Size-1; i > Instance->CmdLen; i--)
			{
			  Instance->History.Buf[i] = Instance->History.Buf[i - 1 - Instance->CmdLen];
			}

			/* Copy new Command */
      memcpy(Instance->History.Buf, Instance->Cmd, Instance->CmdLen+1);

      /* Reset History Reference */
      Instance->History.Ref = NULL;

      /* Process Command */
		  Cli_ProcessCommand( Instance );

		  /* Clear Command */
		  Instance->CmdLen = 0;
		  memset(Instance->Cmd, 0x00, Instance->CmdSize);
		}
	}
	else if ( c == '\t' )
	{
	  if( Instance->CmdLen > 1 )
	  {
	    if (Instance->Cmd[Instance->CmdLen-1] == '\t')
	    {
	      /* Clear Terminal */
	      Instance->Callback("\033[2K", Instance->CallbackRef );
	      Instance->Callback("\r", Instance->CallbackRef );

	      for(uint16_t i = 0; i < Instance->CmdListLen; i++)
	      {

	        /* Check if current text is at beginning of command in list */
	        if(strstr(Instance->CmdList[i]->cmd, Instance->Cmd) == Instance->CmdList[i]->cmd)
	        {
	          /* Display on screen */
	          Instance->Callback(Instance->CmdList[i]->cmd, Instance->CallbackRef );
	          Instance->Callback("\r\n", Instance->CallbackRef );
	        }
	      }

	      Instance->CmdLen = 0;

	      /* Add NULL to end of command */
	      Instance->Cmd[Instance->CmdLen] = 0;
	    }
	  }

	  if(Instance->CmdLen > 0)
	  {
	    const char *Match = NULL;
	    uint8_t  MatchLen = 0xff;

	    for(uint16_t i = 0; i < Instance->CmdListLen; i++)
	    {
	      if(Instance->CmdList[i] == NULL) break;

	      /* Check if current text is at beginning of command in list */
	      if(strstr(Instance->CmdList[i]->cmd, Instance->Cmd) == Instance->CmdList[i]->cmd)
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
        Instance->Callback("\033[2K", Instance->CallbackRef );
        Instance->Callback("\r", Instance->CallbackRef );

        Instance->CmdLen = MatchLen;

        /* Copy reference to current command */
        strncpy(Instance->Cmd, Match, Instance->CmdLen);

        /* Add NULL to end of command */
        Instance->Cmd[Instance->CmdLen] = 0;

        /* Display on screen */
        Instance->Callback(Instance->Cmd, Instance->CallbackRef );
      }
	  }
	}
	else if ( c == BS ) // Check for Backspace and Delete characters
	{
		if(Instance->CmdLen > 0)
		{
      const char str[4] = {BS,SPACE,BS,0};
			Instance->Cmd[--Instance->CmdLen] = 0;
			Instance->Callback(str, Instance->CallbackRef );
		}
	}
	else if (strncmp(&Instance->Cmd[Instance->CmdLen-2],"\e[",2) == 0 )
	{
	  if( Instance->History.Ref == NULL)
	  {
      /* Clear Terminal */
      Instance->Callback("\033[2K", Instance->CallbackRef );
      Instance->Callback("\r", Instance->CallbackRef );

      /* Set Reference at beginning of Buf */
      Instance->History.Ref = Instance->History.Buf;

      /* Get length */
      Instance->CmdLen = strlen(Instance->History.Ref);

      /* Copy reference to current command */
      strncpy(Instance->Cmd, Instance->History.Ref, Instance->CmdLen);

      /* Add NULL to end of command */
      Instance->Cmd[Instance->CmdLen] = 0;

      /* Display on screen */
      Instance->Callback(Instance->Cmd, Instance->CallbackRef );

	  }
	  else if( c == 'A')
	  {
	    /* Clear Terminal */
	    Instance->Callback("\033[2K", Instance->CallbackRef );
	    Instance->Callback("\r", Instance->CallbackRef );

	    /* Move Reference by length of previous reference */
	    if((Instance->History.Ref + strlen(Instance->History.Ref)) < (Instance->History.Buf + Instance->History.Size))
	    {
	      Instance->History.Ref += strlen(Instance->History.Ref) + 1;
	    }

	    /* Get New command Length from history */
	    if((Instance->History.Ref + strlen(Instance->History.Ref)) <= (Instance->History.Buf + Instance->History.Size))
	      Instance->CmdLen = strlen(Instance->History.Ref);
	    else
	      Instance->CmdLen = Instance->History.Ref - Instance->History.Buf;

	    /* Copy reference to current command */
	    strncpy(Instance->Cmd, Instance->History.Ref, Instance->CmdLen);

      /* Add NULL to end of command */
      Instance->Cmd[Instance->CmdLen] = 0;

	    /* Display on screen */
	    Instance->Callback(Instance->Cmd, Instance->CallbackRef );
	  }
	  else if( c == 'B')
	  {
	    /* Clear Terminal */
	    Instance->Callback("\033[2K", Instance->CallbackRef );
	    Instance->Callback("\r", Instance->CallbackRef );

	    /* Move Reference by length of previous reference */
	    while(Instance->History.Ref > Instance->History.Buf)
	    {
	      Instance->History.Ref--;

	      if(*(Instance->History.Ref-1) == 0)
	        break;
	    }

	    /* Get New command Length from history */
	    Instance->CmdLen = strlen(Instance->History.Ref);

	    /* Copy reference to current command */
	    strncpy(Instance->Cmd, Instance->History.Ref, Instance->CmdLen);

      /* Add NULL to end of command */
      Instance->Cmd[Instance->CmdLen] = 0;

	    /* Display on screen */
	    Instance->Callback(Instance->Cmd, Instance->CallbackRef );
	  }
	}
	else
	{
		/* Save character */
//		if ( isprint((uint8_t)c) && (Instance->CmdLen < Instance->CmdSize) )
	  if ( Instance->CmdLen < Instance->CmdSize )
		{
	    /* Echo */
	    const char str[2] = {c,0};

	    Instance->Callback(str, Instance->CallbackRef );
			Instance->Cmd[Instance->CmdLen++] = c;
			Instance->Cmd[Instance->CmdLen + 1] = 0;

		}
	}

	pChar = c;
}

/*******************************************************************************

  PURPOSE:  Register Command

  COMMENT:

*******************************************************************************/
int32_t Cli_RegisterCommand(Cli_t *Instance, CliCmd_t const * cmd )
{
	/*  Make sure variables are initialized */
  if ( (Instance->CmdList == NULL) || (Instance->CmdListSize == 0) ) return -1;

	/* Check the command to register is not NULL */
	if (NULL == cmd) return -1;

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

	return 0;
}

/*******************************************************************************

  PURPOSE:  CLI Set Callback Handler

  COMMENT:

*******************************************************************************/
void Cli_SetCallback( Cli_t *Instance, CliCallback_t FuncPtr, void *CallBackRef )
{
  Instance->Callback = FuncPtr;
  Instance->CallbackRef = CallBackRef;
}

/*******************************************************************************

  PURPOSE:  Initializes CLI Instance

  COMMENT:

*******************************************************************************/
int32_t CLi_Init(Cli_t *Instance, CliCfg_t *Cfg )
{
  if ((Instance == NULL) || (Cfg == NULL))
  {
		return -1;
  }
  else
	{
    Instance->Cmd           = Cfg->CmdBuf;
    Instance->CmdSize       = Cfg->CmdBufSize;
		Instance->CmdList 		  = Cfg->CmdList;
		Instance->CmdListSize	  = Cfg->CmdListSize;
		Instance->History.Buf   = Cfg->HistoryBuf;
		Instance->History.Size  = Cfg->HistoryBufSize;
		Instance->Callback      = Cfg->Callback;
		Instance->CallbackRef   = Cfg->CallbackRef;

		Instance->History.Ref = Instance->History.Buf;
    Instance->CmdLen        = 0;

		memset(Instance->History.Buf, 0, Instance->History.Size);

    /* Register Help Commands */
    Cli_RegisterCommand(Instance, &CliHelpDef);
//    Cli_RegisterCommand(Instance, &CliStr2NumDef);
		return 0;
  }
}



/** @} */
