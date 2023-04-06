/***************************************************************************//**
*  \addtogroup EEPROM
*   @{
*******************************************************************************/
/***************************************************************************//**
*  \file       eeprom.c
*
*  \details    This file contains the eeprom driver.
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
#include "eeprom.h"
#include "rflan.h"


#define EEPROM_EUI64_START_ADDR     (0xf8)



int32_t Eeprom_Read(eeprom_t *Instance, uint8_t Address, uint8_t *Buf, uint16_t Length)
{
  int32_t status;

  XIicPs_SetOptions(Instance->Iic, XIICPS_REP_START_OPTION);

  if((status = XIicPs_MasterSendPolled(Instance->Iic, &Address, 1, Instance->Addr)) != 0)
    return RflanStatus_IicError;

  XIicPs_ClearOptions(Instance->Iic, XIICPS_REP_START_OPTION);

  if((status = XIicPs_MasterRecvPolled(Instance->Iic, Buf, Length, Instance->Addr)) != 0)
    return RflanStatus_IicError;

  while (XIicPs_BusIsBusy(Instance->Iic));

  return RflanStatus_Success;
}

int32_t Eeprom_Write(eeprom_t *Instance, uint8_t Address, uint8_t *Buf, uint16_t Length)
{
  int32_t status;

  XIicPs_SetOptions(Instance->Iic, XIICPS_REP_START_OPTION);

  if((status = XIicPs_MasterSendPolled(Instance->Iic, &Address, 1, Instance->Addr)) != 0)
    return RflanStatus_IicError;

  XIicPs_ClearOptions(Instance->Iic, XIICPS_REP_START_OPTION);

  if((status = XIicPs_MasterSendPolled(Instance->Iic, Buf, Length, Instance->Addr)) != 0)
    return RflanStatus_IicError;

  while (XIicPs_BusIsBusy(Instance->Iic));

  return RflanStatus_Success;
}

int32_t Eeprom_GetEUI64(eeprom_t *Instance, uint64_t *Value)
{
  int32_t status;

  if((status = Eeprom_Read(Instance, EEPROM_EUI64_START_ADDR, (uint8_t*)Value, sizeof( uint64_t))) != 0)
    return RflanStatus_IicError;

  return RflanStatus_Success;
}

int32_t Eeprom_Initialize( eeprom_t *Instance, eeprom_init_t *Init )
{
  Instance->Iic = Init->Iic;
  Instance->Addr = Init->Addr;

  return RflanStatus_Success;
}


