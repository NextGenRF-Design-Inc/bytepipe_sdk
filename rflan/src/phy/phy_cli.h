#ifndef PHY_CLI_H_
#define PHY_CLI_H_

/***************************************************************************//**
*  \ingroup    PHY
*  \defgroup   PHY_CLI PHY Command Line Interface
*  @{
*******************************************************************************/
/***************************************************************************//**
*  \file       phy_cli.h
*
*  \details    PHY Command Line Interface Definitions
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


/******************************************************************************/
/**
*  \details   Initialize PHY CLI
*
*  \return    status
*******************************************************************************/
int PhyCli_Initialize( void );

#endif /* PHY_CLI_H_ */
