/***************************************************************************//**
*  \addtogroup VERSA_CLOCK5
*   @{
*******************************************************************************/
/***************************************************************************//**
*  \file       versa_clock5.c
*
*  \details    This file contains the VersaClock 5 driver.
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
#include "versa_clock5.h"
#include "xparameters.h"
#include "xiicps.h"
#include "math.h"
#include "sleep.h"

XIicPs VersaClock5Iic;   /**< Instance of the IIC Device */

#define VERSA_CLOCK5_IIC_ADDR                     (0x6A)
#define VERSA_CLOCK5_VCO_FREQ_HZ                  (2700000000)
#define VC5_OUT_DIV_FRAC(idx, n)  (0x22 + ((idx) * 0x10) + (n))
#define VC5_GLOBAL_REGISTER     0x76
#define VC5_GLOBAL_REGISTER_GLOBAL_RESET  BIT(5)
#define VC5_OUT_DIV_INT(idx, n)   (0x2d + ((idx) * 0x10) + (n))

int32_t VersaClock5_GlobalReset( void )
{
  int32_t status;
  uint8_t regVal = 0xC3;

  if(( status = VersaClock5_WriteRegister( VC5_GLOBAL_REGISTER, regVal )) != 0)
    return status;

  regVal = 0xE3;
  if(( status = VersaClock5_WriteRegister( VC5_GLOBAL_REGISTER, regVal )) != 0)
    return status;

  usleep( 100000 );

  return XST_SUCCESS;
}

int32_t VersaClock5_SetClockFreq( uint8_t Port, uint32_t FreqHz )
{
  int32_t status;
  uint32_t div_frc = 0;
  uint32_t div_int = 0;


  double fdiv = (double)(VERSA_CLOCK5_VCO_FREQ_HZ / 2) / (double)FreqHz;
  double fdiv_int = floor(fdiv);
  div_int = (uint32_t)fdiv_int;

  double fdiv_frc = fdiv - fdiv_int;
  div_frc = (uint32_t)(fdiv_frc * (double)(1 << 24));

  uint8_t Buf[15] = {
      VC5_OUT_DIV_FRAC( Port, 0 ),
      div_frc >> 22,
      div_frc >> 14,
      div_frc >> 6,
      div_frc << 2,
      0, 0, 0, 0, 0, 0, 0,
      div_int >> 4,
      div_int << 4, 0
  };

  if(( status = VersaClock5_Write( Buf, 15)) != 0)
    return status;

  return status;
}

int32_t VersaClock5_GetClockFreq( uint8_t Port, uint32_t *FreqHz )
{
  uint8_t tmpReg;
  int32_t status;
  uint32_t odx_frac = 0;
  uint32_t odx_int = 0;

  *FreqHz = 0;

  if((status = VersaClock5_ReadRegister( VC5_OUT_DIV_FRAC(Port,0), &tmpReg )) != 0)
    return status;

  odx_frac = tmpReg << 24;

  if((status = VersaClock5_ReadRegister( VC5_OUT_DIV_FRAC(Port,1), &tmpReg )) != 0)
    return status;

  odx_frac |= tmpReg << 16;

  if((status = VersaClock5_ReadRegister( VC5_OUT_DIV_FRAC(Port,2), &tmpReg )) != 0)
    return status;

  odx_frac |= tmpReg << 8;

  if((status = VersaClock5_ReadRegister( VC5_OUT_DIV_FRAC(Port,3), &tmpReg )) != 0)
    return status;

  odx_frac |= tmpReg;

  odx_frac = odx_frac >> 2;

  double div = (double)odx_frac / (double)(1 << 24);

  if((status = VersaClock5_ReadRegister( VC5_OUT_DIV_INT(Port,0), &tmpReg )) != 0)
    return status;

  odx_int = tmpReg << 8;

  if((status = VersaClock5_ReadRegister( VC5_OUT_DIV_INT(Port,1), &tmpReg )) != 0)
    return status;

  odx_int |= tmpReg;

  odx_int = odx_int >> 4;

  div += (double)odx_int;

  double tmpFreq = (double)VERSA_CLOCK5_VCO_FREQ_HZ / div / 2;

  *FreqHz = (uint32_t)tmpFreq;

  return XST_SUCCESS;
}

int32_t VersaClock5_Read( uint8_t *Buf, uint16_t Length)
{
  int32_t status;

  if((status = XIicPs_MasterRecvPolled(&VersaClock5Iic, Buf, Length, VERSA_CLOCK5_IIC_ADDR)) != XST_SUCCESS)
    return status;

  while (XIicPs_BusIsBusy(&VersaClock5Iic));

  return XST_SUCCESS;
}

int32_t VersaClock5_Write( uint8_t *Buf, uint16_t Length)
{
  int32_t status;

  if((status = XIicPs_MasterSendPolled(&VersaClock5Iic, Buf, Length, VERSA_CLOCK5_IIC_ADDR)) != XST_SUCCESS)
    return status;

  while (XIicPs_BusIsBusy(&VersaClock5Iic));

  return XST_SUCCESS;
}

int32_t VersaClock5_WriteRegister( uint16_t Address, uint8_t Value)
{
//  int32_t status;
//
//  uint8_t Buf[2];
//
//  Buf[0] = (uint8_t)Address;
//  Buf[1] = Value;
//
//  XIicPs_ClearOptions(&VersaClock5Iic, XIICPS_REP_START_OPTION);
//
//  if((status = XIicPs_MasterSendPolled(&VersaClock5Iic, Buf, 2, VERSA_CLOCK5_IIC_ADDR)) != XST_SUCCESS)
//    return status;
//
//  while (XIicPs_BusIsBusy(&VersaClock5Iic));


  int32_t status;

  uint8_t Buf[1];

  Buf[0] = (uint8_t)Address;

  XIicPs_SetOptions(&VersaClock5Iic, XIICPS_REP_START_OPTION);

  if((status = XIicPs_MasterSendPolled(&VersaClock5Iic, Buf, 1, VERSA_CLOCK5_IIC_ADDR)) != XST_SUCCESS)
    return status;

  XIicPs_ClearOptions(&VersaClock5Iic, XIICPS_REP_START_OPTION);

  Buf[0] = (uint8_t)Value;
  if((status = XIicPs_MasterSendPolled(&VersaClock5Iic, Buf, 1, VERSA_CLOCK5_IIC_ADDR)) != XST_SUCCESS)
    return status;

  while (XIicPs_BusIsBusy(&VersaClock5Iic));

  return XST_SUCCESS;
}

int32_t VersaClock5_ReadRegister( uint16_t Address, uint8_t *Value)
{
  int32_t status;

  uint8_t Buf[1];

  Buf[0] = (uint8_t)Address;

  XIicPs_SetOptions(&VersaClock5Iic, XIICPS_REP_START_OPTION);

  if((status = XIicPs_MasterSendPolled(&VersaClock5Iic, Buf, 1, VERSA_CLOCK5_IIC_ADDR)) != XST_SUCCESS)
    return status;

  XIicPs_ClearOptions(&VersaClock5Iic, XIICPS_REP_START_OPTION);

  if((status = XIicPs_MasterRecvPolled(&VersaClock5Iic, Buf, 1, VERSA_CLOCK5_IIC_ADDR)) != XST_SUCCESS)
    return status;

  *Value = Buf[0];

  while (XIicPs_BusIsBusy(&VersaClock5Iic));

  return XST_SUCCESS;
}

static uint8_t VersaClock5Cfg[] = {0x00,0x01,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x01,0x00,0x00,0xB6,0xB4,0x92,0x80,0x2E,0xFD,0xFC,0x00,0x03,0x84,0x06,0xC0,0x00,0x00,0x00,0x9F,0xFD,0xE0,0x80,0x00,0x81,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x00,0x00,0x00,0x40,0x00,0x00,0x81,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x00,0x00,0x03,0x20,0x00,0x00,0x81,0x03,0xD8,0x9D,0x8C,0x00,0x00,0x00,0x00,0x04,0x00,0x00,0x01,0x90,0x00,0x00,0x81,0x03,0x33,0x33,0x30,0x00,0x00,0x00,0x00,0x04,0x00,0x00,0x00,0xA0,0x00,0x63,0x01,0x63,0x01,0x63,0x01,0x63,0x01,0xFF,0x7C};

int32_t VersaClock5_Initialize( void )
{
  int Status;
  XIicPs_Config *Config;

  Config = XIicPs_LookupConfig(XPAR_PSU_I2C_1_DEVICE_ID);
  if (NULL == Config) {
    return XST_FAILURE;
  }

  if((Status = XIicPs_CfgInitialize(&VersaClock5Iic, Config, Config->BaseAddress)) != XST_SUCCESS)
    return XST_FAILURE;

  /* Perform a self-test to ensure that the hardware was built correctly */
  if((Status = XIicPs_SelfTest(&VersaClock5Iic)) != XST_SUCCESS)
    return XST_FAILURE;

  /* Set the IIC serial clock rate */
  XIicPs_SetSClk(&VersaClock5Iic, 100e3);

  /* Write Registers */
  VersaClock5_Write( VersaClock5Cfg, sizeof(VersaClock5Cfg));

  /* Initialize CLI */
  VersaClock5Cli_Initialize();

	return 0;
}
