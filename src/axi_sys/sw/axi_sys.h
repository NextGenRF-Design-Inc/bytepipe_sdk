#ifndef AXI_SYS_H_
#define AXI_SYS_H_
/***************************************************************************//**
*  \file       axi_sys.h
*
*  \details
*
*  This file contains the axi_sys driver.
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


typedef struct
{
  uint32_t         BaseAddr;
} axi_sys_t;

typedef struct
{
  uint32_t         BaseAddr;
} axi_sys_init_t;


bool AxiSys_GetLed( axi_sys_t *Instance );
void AxiSys_SetLed( axi_sys_t *Instance, bool Value );
uint32_t AxiSys_GetBuildTime( axi_sys_t *Instance );
float AxiSys_TicksToSeconds( axi_sys_t *Instance, uint64_t ticks );
uint64_t AxiSys_TicksToMicroSeconds( axi_sys_t *Instance, uint64_t ticks );
uint64_t AxiSys_TicksToMilliSeconds( axi_sys_t *Instance, uint64_t ticks );
uint64_t AxiSys_GetTicks( axi_sys_t *Instance );
uint32_t AxiSys_GetTickFreq( axi_sys_t *Instance );
int32_t AxiSys_Initialize( axi_sys_t *Instance, axi_sys_init_t *Init );


#endif


