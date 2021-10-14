#ifndef APP_CLI_H_
#define APP_CLI_H_

/***************************************************************************//**
*  \ingroup    APP
*  \defgroup   APP_CLI APP Command Line Interface
*  @{
*******************************************************************************/
/***************************************************************************//**
*  \file       app_cli.h
*
*  \details    APP Command Line Interface Definitions
*
*  \copyright  NextGen RF Design makes no warranty for the use of this code or
*              design. This code is provided "As Is". NextGen RF Design assumes no
*              responsibility for any errors, which may appear in this code, nor
*              does it make a commitment to update the information contained
*              herein. NextGen RF Design specifically disclaims any implied
*              warranties of fitness for a particular purpose.
*              Copyright(c) NextGen RF Design All rights reserved.
*
*******************************************************************************/
#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include "cli.h"

/******************************************************************************/
/**
*  \details   Initialize APP CLI
*
*  \param	    param is the callback reference used when processing commands.
*  						This should reference the APP PIB
*
*  \return    status
*******************************************************************************/
int32_t AppCli_Initialize( void );
Cli_t *AppCli_GetInstance( void );

#endif /* APP_CLI_H_ */
