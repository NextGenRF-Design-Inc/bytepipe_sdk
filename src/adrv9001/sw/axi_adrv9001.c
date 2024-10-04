/***************************************************************************//**
*  \file       axi_adrv9001.c
*
*  \details    This file contains the ADRV9001 AXI interface implementation.
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
#include <stdio.h>
#include <string.h>
#include <xil_io.h>
#include "axi_adrv9001.h"
#include "adrv9001.h"
#include "sleep.h"


/***************************************************************************//**
*
* \details  SPI Interrupt routine
*
* \param    Instance [in]  Driver Instance
*
* \return   none
*
*******************************************************************************/
static void AxiAdrv9001_SpiIrq( axi_adrv9001_t *Instance )
{
  Instance->SpiDone = 1;

  XScuGic_Disable(Instance->IrqInstance, Instance->IrqId);
}

/***************************************************************************//**
*
* \details  SPI Write single character to FIFO
*
* \param    Instance [in]  Driver Instance
* \param    Value    [in]  Value to write
* \param    Start    [in]  Start sending immediately or wait
*
* \return   none
*
*******************************************************************************/
static void AxiAdrv9001_MspiWrite(axi_adrv9001_t *Instance, uint8_t Value, uint8_t Start )
{
  uint32_t regValue = (uint32_t)Value + ((uint32_t)Start << 8);

  Xil_Out32(Instance->Base + ADRV9001_MSPI_ADDR, regValue);
}

/***************************************************************************//**
*
* \details  SPI Read single character from FIFO
*
* \param    Instance [in]  Driver Instance
* \param    Value    [in]  Value to write
*
* \return   status
*
*******************************************************************************/
static bool AxiAdrv9001_MspiRead(axi_adrv9001_t *Instance, uint8_t *Value )
{
  uint32_t regValue = Xil_In32(Instance->Base + ADRV9001_MSPI_ADDR);

  if( (regValue & 0x100) == 0x100)
  {
    *Value = (uint8_t)regValue;
    return true;
  }
  else
  {
    return false;
  }
}

int32_t AxiAdrv9001_MspiTransfer(axi_adrv9001_t *Instance, uint8_t *TxBuf, uint8_t *RxBuf, uint32_t Length)
{
  Instance->SpiDone = 0;

  XScuGic_Enable(Instance->IrqInstance, Instance->IrqId);

  for( int i = 0; i < Length; i++ )
  {
    if( i == (Length - 1))
      AxiAdrv9001_MspiWrite(Instance, TxBuf[i], 1);
    else
      AxiAdrv9001_MspiWrite(Instance, TxBuf[i], 0);
  }

  while( Instance->SpiDone == 0 );

  uint8_t regValue;
  for( int i = 0; i < Length; i++ )
  {
    if( AxiAdrv9001_MspiRead( Instance, &regValue) == false)
      return Adrv9001Status_SpiErr;

    if( RxBuf != NULL )
      RxBuf[i] = regValue;
  }

  return Adrv9001Status_Success;
}

void AxiAdrv9001_SetNextHopEnableMask(axi_adrv9001_t *Instance, uint8_t mask )
{
  Xil_Out32(Instance->Base + ADRV9001_NEXT_HOP_ENABLE_MASK_ADDR, (uint32_t)mask);
}

uint8_t AxiAdrv9001_GetNextHopEnableMask(axi_adrv9001_t *Instance )
{
  return (uint8_t)Xil_In32(Instance->Base + ADRV9001_NEXT_HOP_ENABLE_MASK_ADDR);
}

bool AxiAdrv9001_GetNextHopEnable(axi_adrv9001_t *Instance, adi_common_Port_e Port, adi_common_ChannelNumber_e Channel )
{
  uint8_t mask = AxiAdrv9001_GetNextHopEnableMask( Instance);

  if( (Port == ADI_TX) && (Channel == ADI_CHANNEL_1) )
  {
    return ((mask & ADRV9001_TX1_HOP_ENABLE_MASK) == ADRV9001_TX1_HOP_ENABLE_MASK);
  }
  else if( (Port == ADI_TX) && (Channel == ADI_CHANNEL_2) )
  {
    return ((mask & ADRV9001_TX2_HOP_ENABLE_MASK) == ADRV9001_TX2_HOP_ENABLE_MASK);
  }
  else if( (Port == ADI_RX) && (Channel == ADI_CHANNEL_1) )
  {
    return ((mask & ADRV9001_RX1_HOP_ENABLE_MASK) == ADRV9001_RX1_HOP_ENABLE_MASK);
  }
  else if( (Port == ADI_RX) && (Channel == ADI_CHANNEL_2) )
  {
    return ((mask & ADRV9001_RX2_HOP_ENABLE_MASK) == ADRV9001_RX2_HOP_ENABLE_MASK);
  }

  return false;
}

void AxiAdrv9001_SetNextHopEnable(axi_adrv9001_t *Instance, adi_common_Port_e Port, adi_common_ChannelNumber_e Channel, bool Enable )
{
  uint8_t mask = AxiAdrv9001_GetNextHopEnableMask( Instance);

  if( (Port == ADI_TX) && (Channel == ADI_CHANNEL_1) )
  {
    if( Enable )
    {
      mask |= ADRV9001_TX1_HOP_ENABLE_MASK;
    }
    else
    {
      mask &= ~ADRV9001_TX1_HOP_ENABLE_MASK;
    }
  }
  else if( (Port == ADI_TX) && (Channel == ADI_CHANNEL_2) )
  {
    if( Enable )
    {
      mask |= ADRV9001_TX2_HOP_ENABLE_MASK;
    }
    else
    {
      mask &= ~ADRV9001_TX2_HOP_ENABLE_MASK;
    }
  }
  else if( (Port == ADI_RX) && (Channel == ADI_CHANNEL_1) )
  {
    if( Enable )
    {
      mask |= ADRV9001_RX1_HOP_ENABLE_MASK;
    }
    else
    {
      mask &= ~ADRV9001_RX1_HOP_ENABLE_MASK;
    }
  }
  else if( (Port == ADI_RX) && (Channel == ADI_CHANNEL_2) )
  {
    if( Enable )
    {
      mask |= ADRV9001_RX2_HOP_ENABLE_MASK;
    }
    else
    {
      mask &= ~ADRV9001_RX2_HOP_ENABLE_MASK;
    }
  }

  AxiAdrv9001_SetNextHopEnableMask( Instance, mask );
}

void AxiAdrv9001_SetHoppingMode(axi_adrv9001_t *Instance, bool Enable )
{
  uint32_t tmp = (uint32_t)Enable;

  Xil_Out32(Instance->Base + ADRV9001_HOPPING_MODE_ENABLE_ADDR, tmp);
}

bool AxiAdrv9001_GetHoppingMode(axi_adrv9001_t *Instance )
{
  uint32_t tmp = Xil_In32(Instance->Base + ADRV9001_HOPPING_MODE_ENABLE_ADDR);

  return tmp > 0;
}

void AxiAdrv9001_SetHopDelay(axi_adrv9001_t *Instance, float Value )
{
  uint32_t tmp = (uint32_t)(Value * (float)Instance->ClockFreqHz);

  Xil_Out32(Instance->Base + ADRV9001_HOP_DGPIO_SETUP_CNT_ADDR, tmp);
}

void AxiAdrv9001_GetHopDelay(axi_adrv9001_t *Instance, float *Value )
{
  uint32_t tmp = Xil_In32(Instance->Base + ADRV9001_HOP_DGPIO_SETUP_CNT_ADDR);
  
  *Value = (float)tmp / (float)Instance->ClockFreqHz;
}

void AxiAdrv9001_SetSetupPin(axi_adrv9001_t *Instance, adi_common_Port_e Port, adi_common_ChannelNumber_e Channel )
{
  if( (Port == ADI_TX) && (Channel == ADI_CHANNEL_1) )
  {
    Xil_Out32(Instance->Base + ADRV9001_PS_SETUP_PIN_ADDR, ADRV9001_TX1_SETUP_PIN_MASK);
  }
  else if( (Port == ADI_TX) && (Channel == ADI_CHANNEL_2) )
  {
    Xil_Out32(Instance->Base + ADRV9001_PS_SETUP_PIN_ADDR, ADRV9001_TX2_SETUP_PIN_MASK);
  }
  else if( (Port == ADI_RX) && (Channel == ADI_CHANNEL_1) )
  {
    Xil_Out32(Instance->Base + ADRV9001_PS_SETUP_PIN_ADDR, ADRV9001_RX1_SETUP_PIN_MASK);
  }
  else if( (Port == ADI_RX) && (Channel == ADI_CHANNEL_2) )
  {
    Xil_Out32(Instance->Base + ADRV9001_PS_SETUP_PIN_ADDR, ADRV9001_RX2_SETUP_PIN_MASK);
  }
}

void AxiAdrv9001_SetHopSetupDelay(axi_adrv9001_t *Instance,adi_common_Port_e Port, adi_common_ChannelNumber_e Channel, float Value )
{
  uint32_t tmp = (uint32_t)(Value * (float)Instance->ClockFreqHz);

  if( (Port == ADI_TX) && (Channel == ADI_CHANNEL_1) )
  {
	  Xil_Out32(Instance->Base + ADRV9001_TX1_HOP_SETUP_CNT_ADDR, tmp);
  }
  else if( (Port == ADI_TX) && (Channel == ADI_CHANNEL_2) )
  {
	  Xil_Out32(Instance->Base + ADRV9001_TX2_HOP_SETUP_CNT_ADDR, tmp);
  }
  else if( (Port == ADI_RX) && (Channel == ADI_CHANNEL_1) )
  {
	  Xil_Out32(Instance->Base + ADRV9001_RX1_HOP_SETUP_CNT_ADDR, tmp);
  }
  else if( (Port == ADI_RX) && (Channel == ADI_CHANNEL_2) )
  {
	  Xil_Out32(Instance->Base + ADRV9001_RX2_HOP_SETUP_CNT_ADDR, tmp);
  }
}

void AxiAdrv9001_GetHopSetupDelay(axi_adrv9001_t *Instance,adi_common_Port_e Port, adi_common_ChannelNumber_e Channel, float *Value )
{
  uint32_t tmp = 0;

  if( (Port == ADI_TX) && (Channel == ADI_CHANNEL_1) )
  {
	  tmp = Xil_In32(Instance->Base + ADRV9001_TX1_HOP_SETUP_CNT_ADDR);
  }
  else if( (Port == ADI_TX) && (Channel == ADI_CHANNEL_2) )
  {
	  tmp = Xil_In32(Instance->Base + ADRV9001_TX2_HOP_SETUP_CNT_ADDR);
  }
  else if( (Port == ADI_RX) && (Channel == ADI_CHANNEL_1) )
  {
	  tmp = Xil_In32(Instance->Base + ADRV9001_RX1_HOP_SETUP_CNT_ADDR);
  }
  else if( (Port == ADI_RX) && (Channel == ADI_CHANNEL_2) )
  {
	  tmp = Xil_In32(Instance->Base + ADRV9001_RX2_HOP_SETUP_CNT_ADDR);
  }

  *Value = (float)tmp / (float)Instance->ClockFreqHz;
}

void AxiAdrv9001_TriggerSingleHop(axi_adrv9001_t *Instance )
{
  uint32_t tmp = 0x01;

  Xil_Out32(Instance->Base + ADRV9001_PS_HOP_TRIG_ADDR, tmp);
}

void AxiAdrv9001_SetEnablePlHopTrig(axi_adrv9001_t *Instance, bool Enable )
{
  uint32_t tmp = (uint32_t)Enable;

  Xil_Out32(Instance->Base + ADRV9001_ENABLE_PL_HOP_TRIG_ADDR, tmp);
}

uint8_t AxiAdrv9001_GetEnablePlHopTrig(axi_adrv9001_t *Instance )
{
  uint32_t tmp = Xil_In32(Instance->Base + ADRV9001_ENABLE_PL_HOP_TRIG_ADDR);

  return tmp > 0;
}

void AxiAdrv9001_ClearHopIrq( axi_adrv9001_t *Instance )
{
  uint32_t tmp = Xil_In32(Instance->Base + ADRV9001_HOP_IRQ_ADDR);

  tmp |= ADRV9001_HOP_TRIG_CLEAR_MASK;

  Xil_Out32(Instance->Base + ADRV9001_HOP_IRQ_ADDR, tmp);
}

void AxiAdrv9001_SetHopIrqEnable( axi_adrv9001_t *Instance, bool Enable )
{
  uint32_t tmp = Xil_In32(Instance->Base + ADRV9001_HOP_IRQ_ADDR);

  if( Enable )
    tmp |= ADRV9001_HOP_TRIG_ENABLE_MASK;
  else
    tmp &= ~ADRV9001_HOP_TRIG_ENABLE_MASK;

  Xil_Out32(Instance->Base + ADRV9001_HOP_IRQ_ADDR, tmp);
}

bool AxiAdrv9001_GetHopIrqEnable( axi_adrv9001_t *Instance )
{
  uint32_t tmp = Xil_In32(Instance->Base + ADRV9001_HOP_IRQ_ADDR);

  return ((tmp & ADRV9001_HOP_TRIG_ENABLE_MASK ) == ADRV9001_HOP_TRIG_ENABLE_MASK);
}

bool AxiAdrv9001_GetHopIrqStatus( axi_adrv9001_t *Instance )
{
  uint32_t tmp = Xil_In32(Instance->Base + ADRV9001_HOP_IRQ_ADDR);

  return ((tmp & ADRV9001_HOP_TRIG_STATUS_MASK ) == ADRV9001_HOP_TRIG_STATUS_MASK);
}

void AxiAdrv9001_SetPsEnable(axi_adrv9001_t *Instance, adi_common_Port_e Port, adi_common_ChannelNumber_e Channel, uint8_t Enable )
{
  uint32_t tmp = (uint32_t)Enable;

  if( (Port == ADI_TX) && (Channel == ADI_CHANNEL_1) )
    Xil_Out32(Instance->Base + ADRV9001_TX1_PS_ENABLE_ADDR, tmp);
  else if( (Port == ADI_TX) && (Channel == ADI_CHANNEL_2) )
    Xil_Out32(Instance->Base + ADRV9001_TX2_PS_ENABLE_ADDR, tmp);
  else if( (Port == ADI_RX) && (Channel == ADI_CHANNEL_1) )
    Xil_Out32(Instance->Base + ADRV9001_RX1_PS_ENABLE_ADDR, tmp);
  else if( (Port == ADI_RX) && (Channel == ADI_CHANNEL_2) )
    Xil_Out32(Instance->Base + ADRV9001_RX2_PS_ENABLE_ADDR, tmp);
}

bool AxiAdrv9001_GetPsEnable(axi_adrv9001_t *Instance, adi_common_Port_e Port, adi_common_ChannelNumber_e Channel)
{
  uint32_t tmp;

  if( (Port == ADI_TX) && (Channel == ADI_CHANNEL_1) )
    tmp = Xil_In32(Instance->Base + ADRV9001_TX1_PS_ENABLE_ADDR);
  else if( (Port == ADI_TX) && (Channel == ADI_CHANNEL_2) )
    tmp = Xil_In32(Instance->Base + ADRV9001_TX2_PS_ENABLE_ADDR);
  else if( (Port == ADI_RX) && (Channel == ADI_CHANNEL_1) )
    tmp = Xil_In32(Instance->Base + ADRV9001_RX1_PS_ENABLE_ADDR);
  else if( (Port == ADI_RX) && (Channel == ADI_CHANNEL_2) )
    tmp = Xil_In32(Instance->Base + ADRV9001_RX2_PS_ENABLE_ADDR);

  return tmp > 0;
}

void AxiAdrv9001_SetSwapIq(axi_adrv9001_t *Instance, adi_common_Port_e Port, adi_common_ChannelNumber_e Channel, bool Enable )
{
  uint32_t mask = Xil_In32(Instance->Base + ADRV9001_SWAP_IQ_ADDR);

  if( (Port == ADI_TX) && (Channel == ADI_CHANNEL_1) )
  {
    if( Enable )
    {
      mask |= ADRV9001_TX1_SWAP_IQ_MASK;
    }
    else
    {
      mask &= ~ADRV9001_TX1_SWAP_IQ_MASK;
    }
  }
  else if( (Port == ADI_TX) && (Channel == ADI_CHANNEL_2) )
  {
    if( Enable )
    {
      mask |= ADRV9001_TX2_SWAP_IQ_MASK;
    }
    else
    {
      mask &= ~ADRV9001_TX2_SWAP_IQ_MASK;
    }
  }
  else if( (Port == ADI_RX) && (Channel == ADI_CHANNEL_1) )
  {
    if( Enable )
    {
      mask |= ADRV9001_RX1_SWAP_IQ_MASK;
    }
    else
    {
      mask &= ~ADRV9001_RX1_SWAP_IQ_MASK;
    }
  }
  else if( (Port == ADI_RX) && (Channel == ADI_CHANNEL_2) )
  {
    if( Enable )
    {
      mask |= ADRV9001_RX2_SWAP_IQ_MASK;
    }
    else
    {
      mask &= ~ADRV9001_RX2_SWAP_IQ_MASK;
    }
  }

  Xil_Out32(Instance->Base + ADRV9001_SWAP_IQ_ADDR, mask );
}

bool AxiAdrv9001_GetSwapIq(axi_adrv9001_t *Instance, adi_common_Port_e Port, adi_common_ChannelNumber_e Channel)
{
  uint32_t mask = Xil_In32(Instance->Base + ADRV9001_SWAP_IQ_ADDR);

  if( (Port == ADI_TX) && (Channel == ADI_CHANNEL_1) )
  {
    return ((mask & ADRV9001_TX1_SWAP_IQ_MASK) == ADRV9001_TX1_SWAP_IQ_MASK);
  }
  else if( (Port == ADI_TX) && (Channel == ADI_CHANNEL_2) )
  {
    return ((mask & ADRV9001_TX2_SWAP_IQ_MASK) == ADRV9001_TX2_SWAP_IQ_MASK);
  }
  else if( (Port == ADI_RX) && (Channel == ADI_CHANNEL_1) )
  {
    return ((mask & ADRV9001_RX1_SWAP_IQ_MASK) == ADRV9001_RX1_SWAP_IQ_MASK);
  }
  else if( (Port == ADI_RX) && (Channel == ADI_CHANNEL_2) )
  {
    return ((mask & ADRV9001_RX2_SWAP_IQ_MASK) == ADRV9001_RX2_SWAP_IQ_MASK);
  }

  return false;
}

axi_adrv9001_data_src_t AxiAdrv9001_GetTxDataSrc(axi_adrv9001_t *Instance, adi_common_ChannelNumber_e Channel)
{
  uint32_t tmp;

  if( Channel == ADI_CHANNEL_1 )
    tmp = Xil_In32(Instance->Base + ADRV9001_TX1_DATA_SRC_ADDR);
  else if( Channel == ADI_CHANNEL_2 )
    tmp = Xil_In32(Instance->Base + ADRV9001_TX2_DATA_SRC_ADDR);

  return (axi_adrv9001_data_src_t)tmp;
}

void AxiAdrv9001_SetTxDataSrc(axi_adrv9001_t *Instance, axi_adrv9001_data_src_t DataSrc, adi_common_ChannelNumber_e Channel )
{
  uint32_t tmp = (uint32_t)DataSrc;

  if( Channel == ADI_CHANNEL_1 )
    Xil_Out32(Instance->Base + ADRV9001_TX1_DATA_SRC_ADDR, tmp);
  else if( Channel == ADI_CHANNEL_2 )
    Xil_Out32(Instance->Base + ADRV9001_TX2_DATA_SRC_ADDR, tmp);
}

bool AxiAdrv9001_GetRampMonDet(axi_adrv9001_t *Instance, adi_common_ChannelNumber_e Channel)
{
  uint32_t tmp = Xil_In32(Instance->Base + ADRV9001_RX_RAMP_DET_ADDR);

  uint32_t mask = (Channel == ADI_CHANNEL_1)? 0x01 : 0x02;

  return ((tmp & mask) == mask);
}

bool AxiAdrv9001_GetPn15MonDet(axi_adrv9001_t *Instance, adi_common_ChannelNumber_e Channel)
{
  uint32_t tmp = Xil_In32(Instance->Base + ADRV9001_RX_PN15_DET_ADDR);

  uint32_t mask = (Channel == ADI_CHANNEL_1)? 0x01 : 0x02;

  return ((tmp & mask) == mask);
}

bool AxiAdrv9001_GetFixedPatternMonDet(axi_adrv9001_t *Instance, adi_common_ChannelNumber_e Channel)
{
  uint32_t tmp = Xil_In32(Instance->Base + ADRV9001_RX_FIXED_PATTERN_DET_ADDR);

  uint32_t mask = (Channel == ADI_CHANNEL_1)? 0x01 : 0x02;

  return ((tmp & mask) == mask);
}

uint32_t AxiAdrv9001_GetFixedPattern(axi_adrv9001_t *Instance, adi_common_Port_e Port, adi_common_ChannelNumber_e Channel)
{
  uint32_t tmp;

  if( (Port == ADI_TX) && (Channel == ADI_CHANNEL_1) )
    tmp = Xil_In32(Instance->Base + ADRV9001_TX1_FIXED_PATTERN_ADDR);
  else if( (Port == ADI_TX) && (Channel == ADI_CHANNEL_2) )
    tmp = Xil_In32(Instance->Base + ADRV9001_TX2_FIXED_PATTERN_ADDR);
  else if( (Port == ADI_RX) && (Channel == ADI_CHANNEL_1) )
    tmp = Xil_In32(Instance->Base + ADRV9001_RX1_FIXED_PATTERN_ADDR);
  else if( (Port == ADI_RX) && (Channel == ADI_CHANNEL_2) )
    tmp = Xil_In32(Instance->Base + ADRV9001_RX2_FIXED_PATTERN_ADDR);

  return tmp;
}

void AxiAdrv9001_SetFixedPattern(axi_adrv9001_t *Instance, adi_common_Port_e Port, adi_common_ChannelNumber_e Channel, uint32_t Value)
{

  if( (Port == ADI_TX) && (Channel == ADI_CHANNEL_1) )
    Xil_Out32(Instance->Base + ADRV9001_TX1_FIXED_PATTERN_ADDR, Value);
  else if( (Port == ADI_TX) && (Channel == ADI_CHANNEL_2) )
    Xil_Out32(Instance->Base + ADRV9001_TX2_FIXED_PATTERN_ADDR, Value);
  else if( (Port == ADI_RX) && (Channel == ADI_CHANNEL_1) )
    Xil_Out32(Instance->Base + ADRV9001_RX1_FIXED_PATTERN_ADDR, Value);
  else if( (Port == ADI_RX) && (Channel == ADI_CHANNEL_2) )
    Xil_Out32(Instance->Base + ADRV9001_RX2_FIXED_PATTERN_ADDR, Value);

}

void AxiAdrv9001_SetDgpioPin(axi_adrv9001_t *Instance, uint8_t Pin, uint8_t Level )
{
  uint32_t value;

  AxiAdrv9001_GetDgpio( Instance, &value );

  if( Level > 0 )
    value |= ( 1UL << Pin );
  else
    value &= ~( 1UL << Pin );

  Xil_Out32(Instance->Base + ADRV9001_DGPIO_IO_ADDR, value);

}

void AxiAdrv9001_ToggleDgpioPin(axi_adrv9001_t *Instance, uint8_t Pin )
{
  uint32_t value;
  uint32_t mask = (uint32_t)( 1UL << Pin );

  AxiAdrv9001_GetDgpio( Instance, &value );

  if( (value & mask) == mask )
    value &= ~( 1UL << Pin );
  else
    value |= ( 1UL << Pin );

  Xil_Out32(Instance->Base + ADRV9001_DGPIO_IO_ADDR, value);
}

void AxiAdrv9001_SetDgpio(axi_adrv9001_t *Instance, uint32_t Value )
{
  Xil_Out32(Instance->Base + ADRV9001_DGPIO_IO_ADDR, Value);
}

void AxiAdrv9001_GetDgpio(axi_adrv9001_t *Instance, uint32_t *Value )
{
  *Value = Xil_In32(Instance->Base + ADRV9001_DGPIO_IO_ADDR);
}

void AxiAdrv9001_SetDgpioDir(axi_adrv9001_t *Instance, uint32_t Value )
{
  Xil_Out32(Instance->Base + ADRV9001_DGPIO_DIR_ADDR, Value);
}

void AxiAdrv9001_GetDgpioDir(axi_adrv9001_t *Instance, uint32_t *Value )
{
  *Value = Xil_In32(Instance->Base + ADRV9001_DGPIO_DIR_ADDR);
}

void AxiAdrv9001_ResetbPinSet(axi_adrv9001_t *Instance, uint8_t Level )
{
  Xil_Out32(Instance->Base + ADRV9001_RSTN_ADDR, Level);
}

int32_t AxiAdrv9001_Initialize(axi_adrv9001_t *Instance, axi_adrv9001_init_t *Init )
{
  Instance->Base = Init->Base;
  Instance->IrqId = Init->IrqId;
  Instance->IrqInstance = Init->IrqInstance;
  Instance->ClockFreqHz = Init->ClockFreqHz;

  if( Instance->ClockFreqHz == 0 )
    return Adrv9001Status_InvalidParameter;

  uint32_t regVal = Xil_In32(Instance->Base + ADRV9001_ID_ADDR);

  if(XScuGic_Connect(Instance->IrqInstance, Instance->IrqId, (XInterruptHandler)AxiAdrv9001_SpiIrq, Instance ) != 0)
    return Adrv9001Status_IrqErr;

  if(regVal == 0x12345678)
    return 0;
  else
    return -1;
}

