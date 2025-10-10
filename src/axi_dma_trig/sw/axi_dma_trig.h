#ifndef SRC_AXI_DMA_TRIG_H_
#define SRC_AXI_DMA_TRIG_H_
/***************************************************************************//**
*  \file       axi_dma_trig.h
*
*  \details
*
*  This file contains definitions for the adrv9001 interface driver.  The
*  adrv9001 interface driver implements algorithms that call one or several
*  adi_adrv9001 API functions.
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
#include <stdint.h>
#include <stdbool.h>
#include "xscugic.h"

/* Register Address */
#define AXI_DMA_TRIG_LVL_ADDR              ((0 ) << 2)    ///<
#define AXI_DMA_TRIG_TYPE_ADDR             ((1 ) << 2)    ///< 0 = level, 1 = external, 2 = immediate
#define AXI_DMA_TRIG_EDGE_ADDR             ((2 ) << 2)    ///< 0 = rising, 1 = falling

#define AXI_DMA_TRIG_ID_ADDR               ((31) << 2)    ///< Driver id register offset

#define AXI_DMA_TRIG_STATUS_OFFSET              (-400)

/**
 **  Status codes
 */
typedef enum
{
  AxiDmaTrigStatus_Success             = (0),
  AxiDmaTrigStatus_InvalidParameter    = (AXI_DMA_TRIG_STATUS_OFFSET - 1),
  AxiDmaTrigStatus_InvalidId           = (AXI_DMA_TRIG_STATUS_OFFSET - 2),
} axi_dma_trig_status_t;

typedef enum{
   AxiDmaTrigType_Lvl             = 0,
   AxiDmaTrigType_External,
   AxiDmaTrigType_Immediate
 }axi_dma_trig_type_t;

typedef enum{
  AxiDmaTrigEdge_Rising           = 0,
  AxiDmaTrigEdge_Falling,
}axi_dma_trig_edge_t;
  
typedef struct{
  uint32_t              BaseAddr;
}axi_dma_trig_t;

typedef struct{
  uint32_t              BaseAddr;
}axi_dma_trig_init_t;


int32_t AxiDmaTrig_GetLvl( axi_dma_trig_t *Instance, float *Value );
int32_t AxiDmaTrig_SetLvl( axi_dma_trig_t *Instance, float Value );
int32_t AxiDmaTrig_GetType( axi_dma_trig_t *Instance, axi_dma_trig_type_t *Value );
int32_t AxiDmaTrig_SetType( axi_dma_trig_t *Instance, axi_dma_trig_type_t Value );
int32_t AxiDmaTrig_GetEdge( axi_dma_trig_t *Instance, axi_dma_trig_edge_t *Value );
int32_t AxiDmaTrig_SetEdge( axi_dma_trig_t *Instance, axi_dma_trig_edge_t Value );
int32_t AxiDmaTrig_Initialize( axi_dma_trig_t *Instance, axi_dma_trig_init_t *Init );




#endif
