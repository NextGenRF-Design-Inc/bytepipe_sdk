/***************************************************************************//**
 *   @file   axi_dmac.h
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
#ifndef AXI_DMAC_H_
#define AXI_DMAC_H_

/******************************************************************************/
/***************************** Include Files **********************************/
/******************************************************************************/
#include <stdint.h>
#include <stdbool.h>

/******************************************************************************/
/********************** Macros and Constants Definitions **********************/
/******************************************************************************/
#define AXI_DMAC_REG_IRQ_MASK   0x80
#define AXI_DMAC_REG_IRQ_PENDING  0x84
#define AXI_DMAC_IRQ_SOT      BIT(0)
#define AXI_DMAC_IRQ_EOT      BIT(1)

#define AXI_DMAC_REG_CTRL     0x400
#define AXI_DMAC_CTRL_ENABLE    BIT(0)
#define AXI_DMAC_CTRL_PAUSE     BIT(1)

#define AXI_DMAC_REG_TRANSFER_ID  0x404
#define AXI_DMAC_REG_START_TRANSFER 0x408
#define AXI_DMAC_REG_FLAGS      0x40c
#define AXI_DMAC_REG_DEST_ADDRESS 0x410
#define AXI_DMAC_REG_SRC_ADDRESS  0x414
#define AXI_DMAC_REG_X_LENGTH   0x418
#define AXI_DMAC_REG_Y_LENGTH   0x41c
#define AXI_DMAC_REG_DEST_STRIDE  0x420
#define AXI_DMAC_REG_SRC_STRIDE   0x424
#define AXI_DMAC_REG_TRANSFER_DONE  0x428

/******************************************************************************/
/*************************** Types Declarations *******************************/
/******************************************************************************/
typedef enum {
  DMA_DEV_TO_MEM,
  DMA_MEM_TO_DEV
}axi_dmac_direction_t;

typedef enum  {
  DMA_CYCLIC = 1,
  DMA_LAST = 2
}axi_dmac_flags_t;

typedef struct {
  uint32_t size;
  uint32_t address;
  uint32_t size_done;
  volatile bool transfer_done;
}axi_dmac_transfer_t;

typedef void (*axi_dmac_callback_t)( axi_dmac_transfer_t *transfer, void *param );

typedef struct {
  axi_dmac_callback_t           Callback;
  void                         *CallbackRef;
  uint32_t                      base;
  axi_dmac_direction_t          direction;
  uint32_t                      flags;
  uint32_t                      transfer_max_size;
  volatile axi_dmac_transfer_t  big_transfer;
}axi_dmac_t;

typedef struct {
  axi_dmac_callback_t           Callback;
  void                         *CallbackRef;
  uint32_t                      base;
  uint32_t                      irqId;
  void                         *irqInstance;
  axi_dmac_direction_t          direction;
  uint32_t                      flags;
}axi_dmac_init_t;

/******************************************************************************/
/************************ Functions Declarations ******************************/
/******************************************************************************/
void axi_dmac_default_isr(void *instance);
int32_t axi_dmac_read(axi_dmac_t *dmac, uint32_t reg_addr, uint32_t *reg_data);
int32_t axi_dmac_write(axi_dmac_t *dmac, uint32_t reg_addr, uint32_t reg_data);
int32_t axi_dmac_transfer_nonblocking(axi_dmac_t *dmac,  uint32_t address, uint32_t size);
int32_t axi_dmac_is_transfer_ready(axi_dmac_t *dmac, bool *rdy);
int32_t axi_dmac_transfer(axi_dmac_t *dmac, uint32_t address, uint32_t size);
int32_t axi_dmac_init(axi_dmac_t **adc_core, axi_dmac_init_t *init);
int32_t axi_dmac_remove(axi_dmac_t *dmac);

#endif
