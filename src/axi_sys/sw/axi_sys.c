/***************************************************************************//**
*  \file       axi_sys.c
*
*  \details    This file contains the axi_sys driver.
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
#include <stdbool.h>
#include <stdio.h>
#include <xil_io.h>
#include "axi_sys.h"

#define AXI_SYS_TICKS_FREQ_HZ_ADDR    ((0 ) << 2)    ///< Frequency in hz
#define AXI_SYS_TICKS_LOW_ADDR        ((1 ) << 2)    ///< ticks[31:0]
#define AXI_SYS_TICKS_HIGH_ADDR       ((2 ) << 2)    ///< ticks[63:32]
#define AXI_SYS_TIMESTAMP_ADDR        ((3 ) << 2)    ///< contains the current date and time of the HDL build
#define AXI_SYS_RSTN_ADDR             ((4 ) << 2)    ///< General purpose reset

#define AXI_SYS_ID_ADDR               ((31) << 2)    ///< Driver id register offset

float AxiSys_TicksToSeconds( axi_sys_t *Instance, uint64_t ticks )
{
  float tmp = ticks;

  tmp = tmp / (float)AxiSys_GetTickFreq( Instance );

  return tmp;
}

uint64_t AxiSys_TicksToMicroSeconds( axi_sys_t *Instance, uint64_t ticks )
{
  uint64_t tmp = ticks * 1000000;

  tmp = tmp / AxiSys_GetTickFreq( Instance );

  return tmp;
}

uint64_t AxiSys_TicksToMilliSeconds( axi_sys_t *Instance, uint64_t ticks )
{
  uint64_t tmp = ticks * 1000;

  tmp = tmp / AxiSys_GetTickFreq( Instance );

  return tmp;
}

uint64_t AxiSys_GetTicks( axi_sys_t *Instance )
{
  uint32_t tmp0 = Xil_In32( Instance->BaseAddr + AXI_SYS_TICKS_HIGH_ADDR );
  uint32_t tmp1 = Xil_In32( Instance->BaseAddr + AXI_SYS_TICKS_LOW_ADDR );
  uint32_t tmp2 = Xil_In32( Instance->BaseAddr + AXI_SYS_TICKS_HIGH_ADDR );

  if( tmp2 > tmp0 )
	  tmp1 = Xil_In32( Instance->BaseAddr + AXI_SYS_TICKS_LOW_ADDR );

  return ((uint64_t)tmp2 << 32 | (uint64_t)tmp1);
}

uint32_t AxiSys_GetTickFreq( axi_sys_t *Instance )
{
  return Xil_In32( Instance->BaseAddr + AXI_SYS_TICKS_FREQ_HZ_ADDR );
}

uint32_t AxiSys_GetBuildTime( axi_sys_t *Instance )
{
  return Xil_In32( Instance->BaseAddr + AXI_SYS_TIMESTAMP_ADDR );
}

void AxiSys_SetRstn( axi_sys_t *Instance, bool Value )
{
  uint32_t tmp = (uint32_t)Value;
  Xil_Out32( Instance->BaseAddr + AXI_SYS_RSTN_ADDR, tmp );
}

bool AxiSys_GetRstn( axi_sys_t *Instance )
{
  uint32_t tmp = Xil_In32( Instance->BaseAddr + AXI_SYS_RSTN_ADDR );
  
  return (tmp > 0);
}

int32_t AxiSys_Initialize( axi_sys_t *Instance, axi_sys_init_t *Init )
{
  Instance->BaseAddr = Init->BaseAddr;

  AxiSys_SetRstn( Instance, true );

  uint32_t regVal = Xil_In32(Instance->BaseAddr + AXI_SYS_ID_ADDR);

  if(regVal == 0x12345678)
    return 0;
  else
    return -1;

  return XST_SUCCESS;
}


