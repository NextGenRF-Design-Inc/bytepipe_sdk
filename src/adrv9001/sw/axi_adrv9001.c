/***************************************************************************//**
*  \addtogroup AXI_ADRV9001
*   @{
*******************************************************************************/
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



/* Register Address */
#define ADRV9001_TX1_MODE_ADDR            ((0 ) << 2)    ///< TX1 Mode register address offset
#define ADRV9001_TX2_MODE_ADDR            ((1 ) << 2)    ///< TX2 Mode register address offset
#define ADRV9001_RX1_MODE_ADDR            ((2 ) << 2)    ///< RX1 Mode register address offset
#define ADRV9001_RX2_MODE_ADDR            ((3 ) << 2)    ///< RX2 Mode register address offset
#define ADRV9001_RSTN_ADDR                ((4 ) << 2)    ///< Reset pin register address offset

#define ADRV9001_TX1_ENABLE_DLY_ADDR      ((5 ) << 2)    ///< TX1 enable delay register address offset
#define ADRV9001_TX2_ENABLE_DLY_ADDR      ((6 ) << 2)    ///< TX2 enable delay register address offset
#define ADRV9001_RX1_ENABLE_DLY_ADDR      ((7 ) << 2)    ///< RX1 enable delay register address offset
#define ADRV9001_RX2_ENABLE_DLY_ADDR      ((8 ) << 2)    ///< RX2 enable delay register address offset

#define ADRV9001_TX1_DISABLE_DLY_ADDR     ((9 ) << 2)    ///< TX1 disable delay register address offset
#define ADRV9001_TX2_DISABLE_DLY_ADDR     ((10) << 2)    ///< TX2 disable delay register address offset
#define ADRV9001_RX1_DISABLE_DLY_ADDR     ((11) << 2)    ///< RX1 disable delay register address offset
#define ADRV9001_RX2_DISABLE_DLY_ADDR     ((12) << 2)    ///< RX2 disable delay register address offset

#define ADRV9001_DGPIO_DIR_ADDR           ((13) << 2)    ///< DGPIO direction register address offset
#define ADRV9001_DGPIO_IO_ADDR            ((14) << 2)    ///< DGPIO value register address offset

#define ADRV9001_CH1_DATA_ADDR            ((15) << 2)    ///< TX1/RX1 IQ data register offset
#define ADRV9001_CH2_DATA_ADDR            ((16) << 2)    ///< TX2/RX2 IQ data register offset

#define ADRV9001_TX1_DATA_PATH_ADDR       ((17) << 2)    ///< TX1 data path register offset
#define ADRV9001_TX2_DATA_PATH_ADDR       ((18) << 2)    ///< TX2 data path register offset

#define ADRV9001_CAPTURE_CONTROL_CNT_ADDR ((19) << 2)    ///< Capture control count register offset

#define ADRV9001_MSPI_ADDR                ((21) << 2)    ///< SPI data register offset

#define ADRV9001_ID_ADDR                  ((31) << 2)    ///< Driver id register offset



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

void AxiAdrv9001_SetDisableDelay(axi_adrv9001_t *Instance, adi_common_Port_e Port, adi_common_ChannelNumber_e Channel, uint16_t SampleCnt )
{
  uint32_t tmp = (uint32_t)SampleCnt;
  
  if( (Port == ADI_TX) && (Channel == ADI_CHANNEL_1) )
  {
    Xil_Out32(Instance->Base + ADRV9001_TX1_DISABLE_DLY_ADDR, tmp);
  }
  else if( (Port == ADI_TX) && (Channel == ADI_CHANNEL_2) )
  {
    Xil_Out32(Instance->Base + ADRV9001_TX2_DISABLE_DLY_ADDR, tmp);
  }
  else if( (Port == ADI_RX) && (Channel == ADI_CHANNEL_1) )
  {
    Xil_Out32(Instance->Base + ADRV9001_RX1_DISABLE_DLY_ADDR, tmp);
  }
  else if( (Port == ADI_RX) && (Channel == ADI_CHANNEL_2) )
  {
    Xil_Out32(Instance->Base + ADRV9001_RX2_DISABLE_DLY_ADDR, tmp);
  }
}

void AxiAdrv9001_SetEnableDelay(axi_adrv9001_t *Instance, adi_common_Port_e Port, adi_common_ChannelNumber_e Channel, uint16_t SampleCnt )
{
  uint32_t tmp = (uint32_t)SampleCnt;
    
  if( (Port == ADI_TX) && (Channel == ADI_CHANNEL_1) )
  {
    Xil_Out32(Instance->Base + ADRV9001_TX1_ENABLE_DLY_ADDR, tmp);
  }
  else if( (Port == ADI_TX) && (Channel == ADI_CHANNEL_2) )
  {
    Xil_Out32(Instance->Base + ADRV9001_TX2_ENABLE_DLY_ADDR, tmp);
  }
  else if( (Port == ADI_RX) && (Channel == ADI_CHANNEL_1) )
  {
    Xil_Out32(Instance->Base + ADRV9001_RX1_ENABLE_DLY_ADDR, tmp);
  }
  else if( (Port == ADI_RX) && (Channel == ADI_CHANNEL_2) )
  {
    Xil_Out32(Instance->Base + ADRV9001_RX2_ENABLE_DLY_ADDR, tmp);
  }
}

void AxiAdrv9001_GetDisableDelay(axi_adrv9001_t *Instance, adi_common_Port_e Port, adi_common_ChannelNumber_e Channel, uint16_t *SampleCnt )
{
  uint32_t tmp;
  
  if( (Port == ADI_TX) && (Channel == ADI_CHANNEL_1) )
  {
    tmp = Xil_In32(Instance->Base + ADRV9001_TX1_DISABLE_DLY_ADDR);
  }
  else if( (Port == ADI_TX) && (Channel == ADI_CHANNEL_2) )
  {
    tmp = Xil_In32(Instance->Base + ADRV9001_TX2_DISABLE_DLY_ADDR);
  }
  else if( (Port == ADI_RX) && (Channel == ADI_CHANNEL_1) )
  {
    tmp = Xil_In32(Instance->Base + ADRV9001_RX1_DISABLE_DLY_ADDR);
  }
  else if( (Port == ADI_RX) && (Channel == ADI_CHANNEL_2) )
  {
    tmp = Xil_In32(Instance->Base + ADRV9001_RX2_DISABLE_DLY_ADDR);
  }
  
  *SampleCnt = (uint16_t)tmp;
}

void AxiAdrv9001_GetEnableDelay(axi_adrv9001_t *Instance, adi_common_Port_e Port, adi_common_ChannelNumber_e Channel, uint16_t *SampleCnt )
{
  uint32_t tmp;
  
  if( (Port == ADI_TX) && (Channel == ADI_CHANNEL_1) )
  {
    tmp = Xil_In32(Instance->Base + ADRV9001_TX1_ENABLE_DLY_ADDR);
  }
  else if( (Port == ADI_TX) && (Channel == ADI_CHANNEL_2) )
  {
    tmp = Xil_In32(Instance->Base + ADRV9001_TX2_ENABLE_DLY_ADDR);
  }
  else if( (Port == ADI_RX) && (Channel == ADI_CHANNEL_1) )
  {
    tmp = Xil_In32(Instance->Base + ADRV9001_RX1_ENABLE_DLY_ADDR);
  }
  else if( (Port == ADI_RX) && (Channel == ADI_CHANNEL_2) )
  {
    tmp = Xil_In32(Instance->Base + ADRV9001_RX2_ENABLE_DLY_ADDR);
  }
  
  *SampleCnt = (uint16_t)tmp;  
}

void AxiAdrv9001_SetEnableMode(axi_adrv9001_t *Instance, adi_common_Port_e Port, adi_common_ChannelNumber_e Channel, adi_adrv9001_ChannelEnableMode_e mode )
{
  if( (Port == ADI_TX) && (Channel == ADI_CHANNEL_1) )
    Xil_Out32(Instance->Base + ADRV9001_TX1_MODE_ADDR, (uint32_t)mode);
  else if( (Port == ADI_TX) && (Channel == ADI_CHANNEL_2) )
    Xil_Out32(Instance->Base + ADRV9001_TX2_MODE_ADDR, (uint32_t)mode);
  else if( (Port == ADI_RX) && (Channel == ADI_CHANNEL_1) )
    Xil_Out32(Instance->Base + ADRV9001_RX1_MODE_ADDR, (uint32_t)mode);
  else if( (Port == ADI_RX) && (Channel == ADI_CHANNEL_2) )
    Xil_Out32(Instance->Base + ADRV9001_RX2_MODE_ADDR, (uint32_t)mode);
}

void AxiAdrv9001_SetCaptureControlCnt(axi_adrv9001_t *Instance, uint32_t Value )
{
  Xil_Out32(Instance->Base + ADRV9001_CAPTURE_CONTROL_CNT_ADDR, Value);
}

void AxiAdrv9001_GetCaptureControlCnt(axi_adrv9001_t *Instance, uint32_t *Value )
{
  *Value = Xil_In32(Instance->Base + ADRV9001_CAPTURE_CONTROL_CNT_ADDR);
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

void AxiAdrv9001_SetTxDataPath(axi_adrv9001_t *Instance, adi_common_ChannelNumber_e Channel, axi_adrv9001_data_path_t Value )
{
  if( Channel == ADI_CHANNEL_1 )
    Xil_Out32(Instance->Base + ADRV9001_TX1_DATA_PATH_ADDR, (uint32_t)Value);
  else
    Xil_Out32(Instance->Base + ADRV9001_TX2_DATA_PATH_ADDR, (uint32_t)Value);
}

void AxiAdrv9001_GetTxDataPath(axi_adrv9001_t *Instance, adi_common_ChannelNumber_e Channel, axi_adrv9001_data_path_t *Value )
{
  if( Channel == ADI_CHANNEL_1 )
    *Value = Xil_In32(Instance->Base + ADRV9001_TX1_DATA_PATH_ADDR);
  else
    *Value = Xil_In32(Instance->Base + ADRV9001_TX2_DATA_PATH_ADDR);
}

void AxiAdrv9001_SetTxData(axi_adrv9001_t *Instance, adi_common_ChannelNumber_e Channel, uint32_t Value )
{
  if( Channel == ADI_CHANNEL_1 )
    Xil_Out32(Instance->Base + ADRV9001_CH1_DATA_ADDR, Value);
  else
    Xil_Out32(Instance->Base + ADRV9001_CH2_DATA_ADDR, Value);
}

void AxiAdrv9001_GetRxData(axi_adrv9001_t *Instance, adi_common_ChannelNumber_e Channel, uint32_t *Value )
{
  if( Channel == ADI_CHANNEL_1 )
    *Value = Xil_In32(Instance->Base + ADRV9001_CH1_DATA_ADDR);
  else
    *Value = Xil_In32(Instance->Base + ADRV9001_CH2_DATA_ADDR);
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

  uint32_t regVal = Xil_In32(Instance->Base + ADRV9001_ID_ADDR);

  if(XScuGic_Connect(Instance->IrqInstance, Instance->IrqId, (XInterruptHandler)AxiAdrv9001_SpiIrq, Instance ) != 0)
    return Adrv9001Status_IrqErr;

  if(regVal == 0x12345678)
    return 0;
  else
    return -1;
}

