/***************************************************************************//**
*  \file       axi_dma_trig.c
*
*  \details    
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
#include <xil_io.h>
#include "axi_dma_trig.h"
#include "math.h"


int32_t AxiDmaTrig_GetLvl( axi_dma_trig_t *Instance, float *Value )
{
	if( Instance == NULL )
		return AxiDmaTrigStatus_InvalidParameter;

  uint32_t amplitude_squared_fix = Xil_In32(Instance->BaseAddr + AXI_DMA_TRIG_LVL_ADDR);  //ufix32_30

  double amplitude_squared = (double)amplitude_squared_fix / 1073741824.0;

  double amplitude = sqrt(amplitude_squared);

  double amplitude_log = 20 * log10(amplitude);

  *Value = (float)amplitude_log;

  return AxiDmaTrigStatus_Success;
}

int32_t AxiDmaTrig_SetLvl( axi_dma_trig_t *Instance, float Value )
{
  if( Instance == NULL )
    return AxiDmaTrigStatus_InvalidParameter;

  double amplitude_log = (double)Value;

  double amplitude = pow(10, amplitude_log / 20);

  double amplitude_squared = pow(amplitude, 2);

  uint32_t amplitude_squared_fix = (uint32_t)(amplitude_squared * 1073741824.0);  //ufix32_30

  Xil_Out32(Instance->BaseAddr + AXI_DMA_TRIG_LVL_ADDR, amplitude_squared_fix);

  return AxiDmaTrigStatus_Success;
}

int32_t AxiDmaTrig_GetType( axi_dma_trig_t *Instance, axi_dma_trig_type_t *Value )
{
  if( Instance == NULL )
    return AxiDmaTrigStatus_InvalidParameter;

  uint32_t tmp = Xil_In32(Instance->BaseAddr + AXI_DMA_TRIG_TYPE_ADDR);

  *Value = (axi_dma_trig_type_t)tmp;

  return AxiDmaTrigStatus_Success;
}

int32_t AxiDmaTrig_SetType( axi_dma_trig_t *Instance, axi_dma_trig_type_t Value )
{
  if( Instance == NULL )
    return AxiDmaTrigStatus_InvalidParameter;

  uint32_t tmp = (uint32_t)(Value);

  Xil_Out32(Instance->BaseAddr + AXI_DMA_TRIG_TYPE_ADDR, (uint32_t)tmp);

  return AxiDmaTrigStatus_Success;
}

int32_t AxiDmaTrig_GetEdge( axi_dma_trig_t *Instance, axi_dma_trig_edge_t *Value )
{
  if( Instance == NULL )
    return AxiDmaTrigStatus_InvalidParameter;

  uint32_t tmp = Xil_In32(Instance->BaseAddr + AXI_DMA_TRIG_EDGE_ADDR);

  *Value = (axi_dma_trig_edge_t)tmp;

  return AxiDmaTrigStatus_Success;
}

int32_t AxiDmaTrig_SetEdge( axi_dma_trig_t *Instance, axi_dma_trig_edge_t Value )
{
  if( Instance == NULL )
    return AxiDmaTrigStatus_InvalidParameter;

  uint32_t tmp = (uint32_t)(Value);

  Xil_Out32(Instance->BaseAddr + AXI_DMA_TRIG_EDGE_ADDR, (uint32_t)tmp);

  return AxiDmaTrigStatus_Success;
}

int32_t AxiDmaTrig_Initialize( axi_dma_trig_t *Instance, axi_dma_trig_init_t *Init )
{
  Instance->BaseAddr = Init->BaseAddr;

  if( Instance->BaseAddr == 0 )
	  return AxiDmaTrigStatus_InvalidParameter;

  uint32_t regVal = Xil_In32(Instance->BaseAddr + AXI_DMA_TRIG_ID_ADDR);

  if(regVal != 0x12345678)
    return AxiDmaTrigStatus_InvalidId;

  AxiDmaTrig_SetType(Instance, AxiDmaTrigType_Immediate);

  return AxiDmaTrigStatus_Success;
}
