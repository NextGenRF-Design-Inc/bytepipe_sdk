#ifndef __CLI_H__
#define __CLI_H__
/***************************************************************************//**
*  \ingroup   LIB
*  \defgroup  CLI Command Line Interface (CLI)
*  @{
*******************************************************************************/
/***************************************************************************//**
*  \file       cli.h
*
*  \details    Command Line Interface
*
*  \copyright  NextGen RF Design makes no warranty for the use of this code or
*              design. This code is provided "As Is". NextGen RF Design assumes no
*              responsibility for any errors, which may appear in this code, nor
*              does it make a commitment to update the information contained
*              herein. NextGen RF Design specifically disclaims any implied
*              warranties of fitness for a particular purpose.
*              Copyright(c) NextGen RF Design All rights reserved.
*
*
*******************************************************************************/
#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <string.h>


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
typedef void (*CliCallback_t)( const char *s, void *param );

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
  const char * const 	cmd;
  const char * const 	helpString;
  const CliCmdFn_t 		CmdFn;
  int8_t		   				pcount;
  void               *userData;
} CliCmd_t;

/**
 * Command History
 */
typedef struct {
  uint16_t            Size;
  char               *Buf;
  char               *Ref;
} CliHistory_t;

/**
**  CLI Instance Definition
*/
typedef struct {
	CliCmd_t const 	  **CmdList;
	uint16_t 						CmdListLen;
	uint16_t						CmdListSize;
  CliCallback_t				Callback;
  void               *CallbackRef;
  char							 *Cmd;
  uint16_t						CmdLen;
  uint16_t						CmdSize;
  CliHistory_t        History;
} Cli_t;

/**
** Type Definition
*/
typedef enum{
  CliParamTypeChar   		= 0,	///< char
  CliParamTypeU8     		= 1, 	///< uint8_t
  CliParamTypeS8     		= 2, 	///< int8_t
  CliParamTypeU16    		= 3, 	///< uint16_t
  CliParamTypeS16    		= 4, 	///< int16_t
  CliParamTypeU32    		= 5, 	///< uint32_t
  CliParamTypeS32    		= 6, 	///< int32_t
  CliParamTypeU64    		= 7,	///< uint64_t
  CliParamTypeS64    		= 8, 	///< int64_t
  CliParamTypeFloat  		= 9, 	///< float
  CliParamTypeDouble  	= 10,	///< double
  CliParamTypeStr    		= 11,	///< string
  CliParamTypeH8      	= 12,	///< hex uint8_t
  CliParamTypeH16     	= 13,	///< hex uint16_t
  CliParamTypeH32     	= 14,	///< hex uint32_t
} CliParamType_t;

/**
**  CLI Configuration Definition
*/
typedef struct {
  CliCmd_t const    **CmdList;
  uint16_t            CmdListSize;
  char               *CmdBuf;
  uint16_t            CmdBufSize;
  char               *HistoryBuf;
  uint16_t            HistoryBufSize;
  CliCallback_t       Callback;
  void               *CallbackRef;
} CliCfg_t;


/******************************************************************************/
/**
*  \details   Processes a single Received character
*
*  \param	  	Instance is reference to the CLI instance
*
*  \param	  	c is the character to be processed
*
*  \return    none
*******************************************************************************/
void Cli_ProcessChar(Cli_t *Instance, char c );

/******************************************************************************/
/**
*  \details   Retrieve parameter value based on requested parameter number/id
*
*  \param	  	s is the command string to search
*
*  \param	  	pNum is the parameter number
*
*  \param			type is the parameter type to be converted to from a string
*
*  \param			param is the converted parameter
*
*  \return    Status Code
*******************************************************************************/
int32_t Cli_GetParameter(const char *s, uint8_t pNum, CliParamType_t type, void *param );

/******************************************************************************/
/**
*  \details   Find parameter string based on requested parameter number/id
*
*  \param	  	s is the command string to search
*
*  \param	  	pNum is the parameter number
*
*  \param			len is the returned length of the parameter if found
*
*  \return    Status Code
*******************************************************************************/
const char *Cli_FindParameter( const char *s, uint16_t pNum, uint16_t *len );

/******************************************************************************/
/**
*  \details   Adds a command to the list
*
*  \param	  	Instance is reference to the CLI instance
*
*  \param	  	cmd to be added to list
*
*  \return    Status Code
*******************************************************************************/
int32_t Cli_RegisterCommand(Cli_t *Instance, CliCmd_t const * cmd );

/******************************************************************************/
/**
*  \details   Register CLI Callback
*
*  \param	  	Instance is reference to the CLI instance
*
*  \param	  	FuncPtr is the callback function to be called
*
*  \param 	  CallbackRef is the user supplied referenced passed during callback
*
*  \return    Status Code
*******************************************************************************/
void Cli_SetCallback( Cli_t *Instance, CliCallback_t FuncPtr, void *CallBackRef );

/******************************************************************************/
/**
*  \details   Initialize Command Line Interface Library
*
*  \param	  	Instance is reference to the CLI instance
*
*  \param	  	CmdList is a list to store defined commands
*
*  \param 	  CmdListSize is the max number of commands the list can store
*
*  \return    Status Code
*******************************************************************************/
int32_t CLi_Init(Cli_t *Instance, CliCfg_t *Cfg);

#ifdef __cplusplus
}
#endif

#endif // __CLI_H_
