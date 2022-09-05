/***************************************************************************//**
*  \addtogroup RFLAN
*   @{
*******************************************************************************/
/***************************************************************************//**
*  \file       rflan_gpio.c
*
*  \details    This file contains the RFLAN gpio.
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
#include "rflan_gpio.h"



/**
* Write GPIO Pin
*/
int32_t RflanGpio_WritePin( XGpioPs *Instance, u32 pin, u32 value )
{
  XGpioPs_WritePin(Instance, pin, value);

  return XST_SUCCESS;
}

/**
* Read GPIO Pin
*/
int32_t RflanGpio_ReadPin( XGpioPs *Instance, u32 pin )
{
  return XGpioPs_ReadPin(Instance, pin);
}

/**
* Toggle GPIO Pin
*/
int32_t RflanGpio_TogglePin( XGpioPs *Instance, u32 pin )
{
  if(XGpioPs_ReadPin(Instance, pin))
  {
    XGpioPs_WritePin(Instance, pin, 0);
  }
  else
  {
    XGpioPs_WritePin(Instance, pin, 1);
  }

  return XST_SUCCESS;
}

int32_t RflanGpio_Initialize( XGpioPs *Instance, uint32_t DeviceId )
{
	XGpioPs_Config *Config;

  /* Lookup Configuration */
  if((Config = XGpioPs_LookupConfig(DeviceId)) == NULL)
    return XST_FAILURE;

  /* Initialize Driver */
  if(XGpioPs_CfgInitialize(Instance, Config, Config->BaseAddr) != XST_SUCCESS)
	  return XST_FAILURE;

  /* Perform Self test */
  if(XGpioPs_SelfTest(Instance) != XST_SUCCESS) return XST_FAILURE;

  /* Set Default Direction */
  XGpioPs_SetDirectionPin(Instance, GPIO_LED_PIN, 1);
  XGpioPs_SetDirectionPin(Instance, GPIO_HWV2_PIN, 0);
  XGpioPs_SetDirectionPin(Instance, GPIO_HWV1_PIN, 0);
  XGpioPs_SetDirectionPin(Instance, GPIO_HWV0_PIN, 0);

  /* Enable Outputs */
  XGpioPs_SetOutputEnablePin(Instance, GPIO_LED_PIN, 1);

  /* Write Default Values */
  XGpioPs_WritePin(Instance, GPIO_LED_PIN, 1);


  return XST_SUCCESS;
}

