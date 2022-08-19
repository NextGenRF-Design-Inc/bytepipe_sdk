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
#include "xscugic.h"
#include "xil_cache.h"

extern XScuGic          xInterruptController;
#define BIT(n)          (1 << (n))

static int32_t AxiDma_Read(axi_dma_t *Instance, uint32_t Addr, uint32_t *Value)
{
  *Value = Xil_In32(Instance->Base + Addr);

  return XST_SUCCESS;
}

static int32_t AxiDma_Write(axi_dma_t *Instance, uint32_t Addr, uint32_t Value)
{
  Xil_Out32(Instance->Base + Addr, Value);

  return XST_SUCCESS;
}

void AxiDma_Handler(axi_dma_t *Instance)
{
	uint32_t remaining_size, burst_size;
	uint32_t reg_val;

	/* Get interrupt sources and clear interrupts. */
	AxiDma_Read(Instance, AXI_DMAC_REG_IRQ_PENDING, &reg_val);
	AxiDma_Write(Instance, AXI_DMAC_REG_IRQ_PENDING, reg_val);

	if ((reg_val & AXI_DMAC_IRQ_SOT) && (Instance->BigTransfer.Size != 0))
	{
		remaining_size = Instance->BigTransfer.Size  - Instance->BigTransfer.SizeDone;

		burst_size = (remaining_size <= Instance->TransferMaxSize) ? remaining_size : Instance->TransferMaxSize;

		Instance->BigTransfer.SizeDone += burst_size;
		Instance->BigTransfer.Address += burst_size;

		switch (Instance->Direction)
		{
		case AxiDmaDir_DevToMem:
			AxiDma_Write(Instance, AXI_DMAC_REG_DEST_ADDRESS, Instance->BigTransfer.Address);
			AxiDma_Write(Instance, AXI_DMAC_REG_DEST_STRIDE, 0x0);
			break;

		case AxiDmaDir_MemToDev:
			AxiDma_Write(Instance, AXI_DMAC_REG_SRC_ADDRESS, Instance->BigTransfer.Address);
			AxiDma_Write(Instance, AXI_DMAC_REG_SRC_STRIDE, 0x0);
			break;

		default:
			return; // Other directions are not supported yet
		}

		/* The current transfer was started and a new one is queued. */
		AxiDma_Write(Instance, AXI_DMAC_REG_X_LENGTH, burst_size);
		AxiDma_Write(Instance, AXI_DMAC_REG_Y_LENGTH, 0x0);
		AxiDma_Write(Instance, AXI_DMAC_REG_START_TRANSFER, 0x1);
	}

  axi_dma_evt_type_t evt;

	if (reg_val & AXI_DMAC_IRQ_EOT)
	{
	  evt = EvtType_EndofTransfer;
	  Instance->BigTransfer.TransferDone = true;
	  Instance->BigTransfer.Address = 0;
	  Instance->BigTransfer.Size = 0;
	  Instance->BigTransfer.SizeDone = 0;
	}
	else if(reg_val & AXI_DMAC_IRQ_SOT)
	{
    evt = EvtType_StartofTransfer;
	}

  if( Instance->Callback != NULL )
  {
    Instance->Callback(Instance, evt);
  }
}

int32_t AxiDma_Stop(axi_dma_t *Instance )
{
  AxiDma_Write(Instance, AXI_DMAC_REG_CTRL, 0x0);
  return XST_SUCCESS;
}

int32_t AxiDma_StartTransfer(axi_dma_t *Instance, uint32_t address, uint32_t size, bool Cyclic)
{
	uint32_t reg_val;

  if (size == 0)
  {
    AxiDma_Write(Instance, AXI_DMAC_REG_CTRL, 0x0);
    return XST_SUCCESS;
  }

  /* Flush Cache */
  Xil_DCacheFlush();

  Instance->Flags = Cyclic;

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
			AxiDma_Write(Instance, AXI_DMAC_REG_DEST_ADDRESS, address);
			AxiDma_Write(Instance, AXI_DMAC_REG_DEST_STRIDE, 0x0);
			break;

		case AxiDmaDir_MemToDev:
			AxiDma_Write(Instance, AXI_DMAC_REG_SRC_ADDRESS, address);
			AxiDma_Write(Instance, AXI_DMAC_REG_SRC_STRIDE, 0x0);
			break;

		default:
			return XST_FAILURE; // Other directions are not supported yet
		}

		if ((size - 1) > Instance->TransferMaxSize)
		{
		  Instance->BigTransfer.Address = address;
		  Instance->BigTransfer.Size = size - 1;
		  Instance->BigTransfer.SizeDone = Instance->TransferMaxSize;

			AxiDma_Write(Instance, AXI_DMAC_REG_X_LENGTH, Instance->TransferMaxSize);
			AxiDma_Write(Instance, AXI_DMAC_REG_Y_LENGTH, 0x0);
			AxiDma_Write(Instance, AXI_DMAC_REG_FLAGS, Instance->Flags);
			AxiDma_Write(Instance, AXI_DMAC_REG_START_TRANSFER, 0x1);
		}
		else
		{
			AxiDma_Write(Instance, AXI_DMAC_REG_X_LENGTH, size - 1);
			AxiDma_Write(Instance, AXI_DMAC_REG_Y_LENGTH, 0x0);
			AxiDma_Write(Instance, AXI_DMAC_REG_FLAGS, Instance->Flags);
			AxiDma_Write(Instance, AXI_DMAC_REG_START_TRANSFER, 0x1);
		}
	}
	else
	{
		return XST_FAILURE;
	}
	return XST_SUCCESS;
}

int32_t AxiDma_IsTransferReady(axi_dma_t *Instance, bool *rdy)
{
	*rdy = Instance->BigTransfer.TransferDone;

	return XST_SUCCESS;
}

int32_t AxiDma_Transfer(axi_dma_t *Instance, uint32_t address, uint32_t size)
{
	uint32_t transfer_id;
	uint32_t reg_val;
	uint32_t timeout = 0;

	if (size == 0)
	{
	  AxiDma_Write(Instance, AXI_DMAC_REG_CTRL, 0x0);
		return XST_SUCCESS;
	}

  /* Flush Cache */
  Xil_DCacheFlush();

  Instance->Flags = 0;

	AxiDma_Write(Instance, AXI_DMAC_REG_CTRL, 0x0);
	AxiDma_Write(Instance, AXI_DMAC_REG_CTRL, AXI_DMAC_CTRL_ENABLE);

	AxiDma_Write(Instance, AXI_DMAC_REG_IRQ_MASK, 0x0);

	AxiDma_Read(Instance, AXI_DMAC_REG_TRANSFER_ID, &transfer_id);
	AxiDma_Read(Instance, AXI_DMAC_REG_IRQ_PENDING, &reg_val);
	AxiDma_Write(Instance, AXI_DMAC_REG_IRQ_PENDING, reg_val);

	switch (Instance->Direction)
	{
	case AxiDmaDir_DevToMem:
		AxiDma_Write(Instance, AXI_DMAC_REG_DEST_ADDRESS, address);
		AxiDma_Write(Instance, AXI_DMAC_REG_DEST_STRIDE, 0x0);
		break;

	case AxiDmaDir_MemToDev:
		AxiDma_Write(Instance, AXI_DMAC_REG_SRC_ADDRESS, address);
		AxiDma_Write(Instance, AXI_DMAC_REG_SRC_STRIDE, 0x0);
		break;

	default:
		return XST_FAILURE; // Other directions are not supported yet
	}

	Instance->BigTransfer.TransferDone = false;

	if ((size - 1) > Instance->TransferMaxSize)
	{
	  Instance->BigTransfer.Address = address;
	  Instance->BigTransfer.Size = size - 1;
	  Instance->BigTransfer.SizeDone = Instance->TransferMaxSize;

		AxiDma_Write(Instance, AXI_DMAC_REG_X_LENGTH, Instance->TransferMaxSize);
		AxiDma_Write(Instance, AXI_DMAC_REG_Y_LENGTH, 0x0);

		AxiDma_Write(Instance, AXI_DMAC_REG_FLAGS, Instance->Flags);

		AxiDma_Write(Instance, AXI_DMAC_REG_START_TRANSFER, 0x1);

		while(!Instance->BigTransfer.TransferDone)
		{
			timeout++;
			if (timeout == UINT32_MAX)
				return XST_FAILURE;
		}

		Instance->BigTransfer.Address = 0;
		Instance->BigTransfer.Size = 0;
		Instance->BigTransfer.SizeDone = 0;

		return XST_SUCCESS;
	}

	AxiDma_Write(Instance, AXI_DMAC_REG_X_LENGTH, size - 1);
	AxiDma_Write(Instance, AXI_DMAC_REG_Y_LENGTH, 0x0);
	AxiDma_Write(Instance, AXI_DMAC_REG_FLAGS, Instance->Flags);
	AxiDma_Write(Instance, AXI_DMAC_REG_START_TRANSFER, 0x1);

	if (Instance->Flags & AxiDmaFlags_Cyclic)
		return XST_SUCCESS;

	/* Wait until the new transfer is queued. */
	do
	{
		AxiDma_Read(Instance, AXI_DMAC_REG_START_TRANSFER, &reg_val);
	} while(reg_val == 1);

	/* Wait until the current transfer is completed. */
	do
	{
		AxiDma_Read(Instance, AXI_DMAC_REG_IRQ_PENDING, &reg_val);
		if (reg_val == (AXI_DMAC_IRQ_SOT | AXI_DMAC_IRQ_EOT))
			break;
	} while(Instance->BigTransfer.TransferDone == false);

	if (reg_val != (AXI_DMAC_IRQ_SOT | AXI_DMAC_IRQ_EOT))
		AxiDma_Write(Instance, AXI_DMAC_REG_IRQ_PENDING, reg_val);

	/* Wait until the transfer with the ID transfer_id is completed. */
	do
	{
		AxiDma_Read(Instance, AXI_DMAC_REG_TRANSFER_DONE, &reg_val);
	} while((reg_val & (1u << transfer_id)) != (1u << transfer_id));

	return XST_SUCCESS;
}

int32_t AxiDma_Initialize(axi_dma_t *Instance, axi_dma_init_t *Init)
{
  int32_t status;

	memset(Instance, 0, sizeof(axi_dma_t));

	Instance->Base                  = Init->Base;
	Instance->Callback              = Init->Callback;
	Instance->CallbackRef           = Init->CallbackRef;
	Instance->Direction             = Init->Direction;
	Instance->Flags                 = Init->Flags;
	Instance->TransferMaxSize       = -1;
	Instance->BigTransfer.Address   = 0;
	Instance->BigTransfer.Size      = 0;
	Instance->BigTransfer.SizeDone  = 0;

	AxiDma_Write(Instance, AXI_DMAC_REG_X_LENGTH, Instance->TransferMaxSize);
	AxiDma_Read(Instance, AXI_DMAC_REG_X_LENGTH, &Instance->TransferMaxSize);

  if((status = XScuGic_Connect(&xInterruptController, Init->IrqId, (XInterruptHandler)AxiDma_Handler, Instance )) != 0)
    return status;

	XScuGic_Enable(&xInterruptController, Init->IrqId);


	return XST_SUCCESS;
}

