/***************************************************************************//**
 *   @file   axi_dma.h
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
#ifndef AXI_DMA_H_
#define AXI_DMA_H_

#include <stdint.h>
#include <stdbool.h>
#include "xscugic.h"

#define BIT(n)          (1 << (n))

#define AXI_DMAC_REG_SIZE_MASK              0x10

#define AXI_DMAC_REG_IRQ_MASK               0x80
#define AXI_DMAC_REG_IRQ_PENDING            0x84
#define AXI_DMAC_IRQ_SOT                    BIT(0)
#define AXI_DMAC_IRQ_EOT                    BIT(1)

#define AXI_DMAC_REG_CTRL                   0x400
#define AXI_DMAC_CTRL_ENABLE                BIT(0)
#define AXI_DMAC_CTRL_PAUSE                 BIT(1)

#define AXI_DMAC_REG_TRANSFER_ID            0x404
#define AXI_DMAC_REG_START_TRANSFER         0x408

#define AXI_DMAC_REG_FLAGS                  0x40c
#define AXI_DMAC_REG_FLAGS_CYCLIC           BIT(0)
#define AXI_DMAC_REG_FLAGS_TLAST            BIT(1)
#define AXI_DMAC_REG_FLAGS_PARTIAL          BIT(2)



#define AXI_DMAC_REG_DEST_ADDRESS           0x410
#define AXI_DMAC_REG_SRC_ADDRESS            0x414
#define AXI_DMAC_REG_X_LENGTH               0x418
#define AXI_DMAC_REG_Y_LENGTH               0x41c
#define AXI_DMAC_REG_DEST_STRIDE            0x420
#define AXI_DMAC_REG_SRC_STRIDE             0x424
#define AXI_DMAC_REG_TRANSFER_DONE          0x428
#define AXI_DMAC_REG_TRANSFER_DONE_PARTIAL  BIT(31)

#define AXI_DMAC_REG_PARTIAL_TRANSFER_LEN   0x44C
#define AXI_DMAC_REG_PARTIAL_TRANSFER_ID    0x450

#define AXI_DMA_STATUS_OFFSET              (-600)

typedef enum
{
  AxiDmaStatus_Success             = (0),
  AxiDmaStatus_InvalidParameter    = (AXI_DMA_STATUS_OFFSET - 1),
  AxiDmaStatus_RegErr              = (AXI_DMA_STATUS_OFFSET - 2),
} axi_dma_status_t;


typedef enum {
  EvtType_StartofTransfer     = 0,
  EvtType_EndofTransfer       = 1,
}axi_dma_evt_type_t;

typedef enum {
  AxiDmaDir_DevToMem          = 0,
  AxiDmaDir_MemToDev          = 1
}axi_dma_direction_t;

typedef enum  {
  AxiDmaFlags_Cyclic          = 1,
  AxiDmaFlags_Last            = 2
}axi_dma_flags_t;

typedef void (*axi_dma_callback_t)( void *Instance, axi_dma_evt_type_t evt );

typedef struct {
  axi_dma_callback_t            Callback;
  void                         *CallbackRef;
  uint32_t                      Base;
  axi_dma_direction_t           Direction;
  uint8_t                       SampleSize;
  uint32_t                      SampleCnt;
  uint32_t                      SampleRate;
  uint32_t                      Flags;
  uint32_t                      Addr;
  XScuGic                      *IrqInstance;
}axi_dma_t;

typedef struct {
  axi_dma_callback_t            Callback;
  void                         *CallbackRef;
  uint32_t                      Base;
  uint32_t                      IrqId;
  axi_dma_direction_t           Direction;
  uint8_t                       SampleSize;
  uint32_t                      SampleCnt;
  uint32_t                      SampleRate;
  uint32_t                      Flags;
  uint32_t                      Addr;
  XScuGic                      *IrqInstance;
}axi_dma_init_t;

int32_t AxiDma_GetEnabled( axi_dma_t *Instance, bool *Value );
int32_t AxiDma_GetTransferCnt( axi_dma_t *Instance, uint32_t *Value );
int32_t AxiDma_Stop( axi_dma_t *Instance );
int32_t AxiDma_SetCyclic(axi_dma_t *Instance, bool Value );
int32_t AxiDma_SetAddr(axi_dma_t *Instance, uint32_t Value );
int32_t AxiDma_SetSampleCnt(axi_dma_t *Instance, uint32_t Value );
int32_t AxiDma_GetCyclic(axi_dma_t *Instance, bool *Value );
int32_t AxiDma_GetAddr(axi_dma_t *Instance, uint32_t *Value );
int32_t AxiDma_GetSampleCnt(axi_dma_t *Instance, uint32_t *Value );
int32_t AxiDma_SetSampleRate(axi_dma_t *Instance, uint32_t Value );
int32_t AxiDma_GetSampleRate(axi_dma_t *Instance, uint32_t *Value );
int32_t AxiDma_GetSampleSize(axi_dma_t *Instance, uint8_t *Value );
int32_t AxiDma_StartTransfer( axi_dma_t *Instance );
int32_t AxiDma_Initialize(axi_dma_t *Instance, axi_dma_init_t *Init);
int32_t AxiDma_GetDirection(axi_dma_t *Instance, axi_dma_direction_t *Value );

#endif
