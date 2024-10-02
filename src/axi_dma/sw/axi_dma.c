/***************************************************************************//**
 *   @file   axi_dma.c
 *   @brief  Driver for the Analog Devices AXI-DMAC core.
 *   @author DBogdan (dragos.bogdan@analog.com)
 ********************************************************************************
 * Copyright 2018(c) Analog Devices, Inc.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *  - Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  - Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *  - Neither the name of Analog Devices, Inc. nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *  - The use of this software may or may not infringe the patent rights
 *    of one or more patent holders.  This license does not release you
 *    from the requirement that you obtain separate licenses from these
 *    patent holders to use this software.
 *  - Use of the software either in source or binary form, must be run
 *    on or directly connected to an Analog Devices Inc. component.
 *
 * THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-INFRINGEMENT,
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL ANALOG DEVICES BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, INTELLECTUAL PROPERTY RIGHTS, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <xil_io.h>
#include "axi_dma.h"
#include "xil_cache.h"




static void AxiDma_Read(axi_dma_t *Instance, uint32_t Addr, uint32_t *Value)
{
	*Value = Xil_In32(Instance->Base + Addr);
}

static void AxiDma_Write(axi_dma_t *Instance, uint32_t Addr, uint32_t Value)
{
	Xil_Out32(Instance->Base + Addr, Value);
}

static void AxiDma_Handler(axi_dma_t *Instance)
{
	uint32_t reg_val;

	/* Get interrupt sources and clear interrupts. */
	AxiDma_Read(Instance, AXI_DMAC_REG_IRQ_PENDING, &reg_val);
	AxiDma_Write(Instance, AXI_DMAC_REG_IRQ_PENDING, reg_val);

	axi_dma_evt_type_t evt;

	if (reg_val & AXI_DMAC_IRQ_EOT)
	{
		evt = EvtType_EndofTransfer;
	}
	else if(reg_val & AXI_DMAC_IRQ_SOT)
	{
		evt = EvtType_StartofTransfer;
	}

	if( Instance->Callback != NULL )
	{
		Instance->Callback(Instance->CallbackRef, evt);
	}
}

int32_t AxiDma_GetTransferCnt( axi_dma_t *Instance, uint32_t *Value )
{
  if( Instance == NULL )
    return AxiDmaStatus_InvalidParameter;

  uint32_t flags;
  uint32_t xfrDone;
  uint32_t length = 0;

  AxiDma_Read( Instance, AXI_DMAC_REG_FLAGS, &flags);
  AxiDma_Read( Instance, AXI_DMAC_REG_TRANSFER_DONE, &xfrDone);

  if( (flags & AXI_DMAC_REG_FLAGS_PARTIAL) == AXI_DMAC_REG_FLAGS_PARTIAL)
  {
	  if( (xfrDone & AXI_DMAC_REG_TRANSFER_DONE_PARTIAL) == AXI_DMAC_REG_TRANSFER_DONE_PARTIAL )
	  {
		  AxiDma_Read( Instance, AXI_DMAC_REG_PARTIAL_TRANSFER_LEN, &length);

		  length = length / Instance->SampleSize;

		  uint32_t tmp32;
		  AxiDma_Read( Instance, AXI_DMAC_REG_PARTIAL_TRANSFER_ID, &tmp32);

		  *Value = length;

		  return AxiDmaStatus_Success;
	  }
  }

  if( xfrDone > 0 )
  {
	  AxiDma_Read( Instance, AXI_DMAC_REG_X_LENGTH, &length);

	  length = (length + 1) / Instance->SampleSize;
  }

  *Value = length;

  return AxiDmaStatus_Success;
}

int32_t AxiDma_Stop( axi_dma_t *Instance )
{
  if( Instance == NULL )
    return AxiDmaStatus_InvalidParameter;

	AxiDma_Write(Instance, AXI_DMAC_REG_CTRL, 0x0);

  return AxiDmaStatus_Success;
}

int32_t AxiDma_SetCyclic(axi_dma_t *Instance, bool Value )
{
  if( Instance == NULL )
    return AxiDmaStatus_InvalidParameter;

  AxiDma_Stop( Instance );

  if( Value )
  {
    Instance->Flags |= AXI_DMAC_REG_FLAGS_CYCLIC;
  }
  else
  {
    Instance->Flags &= ~AXI_DMAC_REG_FLAGS_CYCLIC;
  }

  return AxiDmaStatus_Success;
}

int32_t AxiDma_SetAddr(axi_dma_t *Instance, uint32_t Value )
{
  if( Instance == NULL )
    return AxiDmaStatus_InvalidParameter;

  AxiDma_Stop( Instance );

  Instance->Addr = Value;

  return AxiDmaStatus_Success;
}

int32_t AxiDma_SetSampleCnt(axi_dma_t *Instance, uint32_t Value )
{
  if( Instance == NULL )
    return AxiDmaStatus_InvalidParameter;

  AxiDma_Stop( Instance );

  Instance->SampleCnt = Value;

  return AxiDmaStatus_Success;
}

int32_t AxiDma_GetCyclic(axi_dma_t *Instance, bool *Value )
{
  if( Instance == NULL )
    return AxiDmaStatus_InvalidParameter;

  *Value = ( (Instance->Flags & AXI_DMAC_REG_FLAGS_CYCLIC) == AXI_DMAC_REG_FLAGS_CYCLIC );

  return AxiDmaStatus_Success;
}

int32_t AxiDma_GetDirection(axi_dma_t *Instance, axi_dma_direction_t *Value )
{
  if( Instance == NULL )
    return AxiDmaStatus_InvalidParameter;

  *Value = Instance->Direction;

  return AxiDmaStatus_Success;
}

int32_t AxiDma_GetAddr(axi_dma_t *Instance, uint32_t *Value )
{
  if( Instance == NULL )
    return AxiDmaStatus_InvalidParameter;

  *Value = Instance->Addr;

  return AxiDmaStatus_Success;
}

int32_t AxiDma_GetSampleCnt(axi_dma_t *Instance, uint32_t *Value )
{
  if( Instance == NULL )
    return AxiDmaStatus_InvalidParameter;

  *Value = Instance->SampleCnt;

  return AxiDmaStatus_Success;
}

int32_t AxiDma_GetSampleRate(axi_dma_t *Instance, uint32_t *Value )
{
  if( Instance == NULL )
    return AxiDmaStatus_InvalidParameter;

  *Value = Instance->SampleRate;

  return AxiDmaStatus_Success;
}

int32_t AxiDma_SetSampleRate(axi_dma_t *Instance, uint32_t Value )
{
  if( Instance == NULL )
    return AxiDmaStatus_InvalidParameter;

  memcpy((uint8_t*)&Instance->SampleRate, (uint8_t*)&Value, sizeof(Value));

  return AxiDmaStatus_Success;
}

int32_t AxiDma_GetSampleSize(axi_dma_t *Instance, uint8_t *Value )
{
  if( Instance == NULL )
    return AxiDmaStatus_InvalidParameter;

  *Value = Instance->SampleSize;

  return AxiDmaStatus_Success;
}

int32_t AxiDma_GetEnabled( axi_dma_t *Instance, bool *Value )
{
  uint32_t reg_val;
  AxiDma_Read(Instance, AXI_DMAC_REG_CTRL, &reg_val);
  if (reg_val & AXI_DMAC_CTRL_ENABLE)
  {
    *Value = true;
  }
  else
  {
    *Value = false;
  }

  return AxiDmaStatus_Success;
}
int32_t AxiDma_StartTransfer( axi_dma_t *Instance )
{
  if( Instance == NULL )
    return AxiDmaStatus_InvalidParameter;

  AxiDma_Stop( Instance );

	uint32_t size = Instance->SampleCnt * (uint32_t)Instance->SampleSize;

	/* Flush Cache */
	Xil_DCacheFlush();

  uint32_t reg_val;
	AxiDma_Read(Instance, AXI_DMAC_REG_CTRL, &reg_val);
	if (!(reg_val & AXI_DMAC_CTRL_ENABLE))
	{
		AxiDma_Write(Instance, AXI_DMAC_REG_CTRL, 0x0);
		AxiDma_Write(Instance, AXI_DMAC_REG_CTRL, AXI_DMAC_CTRL_ENABLE);
		AxiDma_Write(Instance, AXI_DMAC_REG_IRQ_MASK, 0x0);
	}

	AxiDma_Read(Instance, AXI_DMAC_REG_START_TRANSFER, &reg_val);
	if (!(reg_val & 1))
	{
		switch (Instance->Direction)
		{
		case AxiDmaDir_DevToMem:
			AxiDma_Write(Instance, AXI_DMAC_REG_DEST_ADDRESS, Instance->Addr);
			AxiDma_Write(Instance, AXI_DMAC_REG_DEST_STRIDE, 0x0);
			break;

		case AxiDmaDir_MemToDev:
			AxiDma_Write(Instance, AXI_DMAC_REG_SRC_ADDRESS, Instance->Addr);
			AxiDma_Write(Instance, AXI_DMAC_REG_SRC_STRIDE, 0x0);
			break;

		default:
			return AxiDmaStatus_InvalidParameter;
		}

		AxiDma_Write(Instance, AXI_DMAC_REG_X_LENGTH, size - 1);
		AxiDma_Write(Instance, AXI_DMAC_REG_Y_LENGTH, 0x0);
		AxiDma_Write(Instance, AXI_DMAC_REG_FLAGS, Instance->Flags);
		AxiDma_Write(Instance, AXI_DMAC_REG_START_TRANSFER, 0x1);
	}
	else
	{
		return AxiDmaStatus_RegErr;
	}

	return AxiDmaStatus_Success;
}

int32_t AxiDma_Initialize(axi_dma_t *Instance, axi_dma_init_t *Init)
{
	int32_t status;

	memset(Instance, 0, sizeof(axi_dma_t));

	Instance->IrqInstance           = Init->IrqInstance;
	Instance->Base                  = Init->Base;
	Instance->Callback              = Init->Callback;
	Instance->CallbackRef           = Init->CallbackRef;
	Instance->Direction             = Init->Direction;
	Instance->Flags                 = Init->Flags;
	Instance->SampleSize            = Init->SampleSize;
  Instance->SampleCnt             = Init->SampleCnt;
	Instance->Addr                  = Init->Addr;
	Instance->SampleRate            = Init->SampleRate;

	if((status = XScuGic_Connect(Instance->IrqInstance, Init->IrqId, (XInterruptHandler)AxiDma_Handler, Instance )) != 0)
		return status;

	XScuGic_Enable(Instance->IrqInstance, Init->IrqId);

	return XST_SUCCESS;
}

