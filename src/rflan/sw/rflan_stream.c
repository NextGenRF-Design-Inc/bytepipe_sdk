/***************************************************************************//**
*  \addtogroup RFLAN_STREAM
*   @{
*******************************************************************************/
/***************************************************************************//**
*  \file       rflan_stream.c
*
*  \details    This file contains the RFLAN stream implementation.
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
#include "rflan_stream.h"
#include "rflan.h"
#include "xil_cache.h"
#include <xil_io.h>
#include "sleep.h"

#ifdef RFLAN_STREAM_ENABLE

extern XScuGic xInterruptController;       ///< Processor Interrupt Controller Instance

static void RflanStream_Tx1DmaCallback( axi_dma_t *Instance, axi_dma_evt_type_t evt )
{
  rflan_stream_t *stream = (rflan_stream_t*)Instance->CallbackRef;

  if( evt == EvtType_EndofTransfer)
  {
    if( stream->Callback != NULL )
      stream->Callback( Instance->Addr, Instance->SampleCnt << 2, RflanStreamChannel_Tx1, stream->CallbackRef );
  }
}

static void RflanStream_Tx2DmaCallback( axi_dma_t *Instance, axi_dma_evt_type_t evt )
{
  rflan_stream_t *stream = (rflan_stream_t*)Instance->CallbackRef;

  if( evt == EvtType_EndofTransfer)
  {
    if( stream->Callback != NULL )
      stream->Callback( Instance->Addr, Instance->SampleCnt << 2, RflanStreamChannel_Tx2, stream->CallbackRef );
  }
}

static void RflanStream_Rx1DmaCallback( axi_dma_t *Instance, axi_dma_evt_type_t evt )
{
  rflan_stream_t *stream = (rflan_stream_t*)Instance->CallbackRef;

  if( evt == EvtType_EndofTransfer)
  {
    if( stream->Callback != NULL )
      stream->Callback( Instance->Addr, Instance->SampleCnt << 2, RflanStreamChannel_Rx1, stream->CallbackRef );
  }
}

static void RflanStream_Rx2DmaCallback( axi_dma_t *Instance, axi_dma_evt_type_t evt )
{
  rflan_stream_t *stream = (rflan_stream_t*)Instance->CallbackRef;

  if( evt == EvtType_EndofTransfer)
  {
    if( stream->Callback != NULL )
      stream->Callback( Instance->Addr, Instance->SampleCnt << 2, RflanStreamChannel_Rx2, stream->CallbackRef );
  }
}

axi_dma_t *RflanStream_GetDma( rflan_stream_t *Instance, rflan_stream_channel_t Channel )
{
  switch( Channel )
  {
    case RflanStreamChannel_Tx1: return &Instance->Tx1Dma;
    case RflanStreamChannel_Tx2: return &Instance->Tx2Dma;
    case RflanStreamChannel_Rx1: return &Instance->Rx1Dma;
    case RflanStreamChannel_Rx2: return &Instance->Rx2Dma;
    default: return NULL;
  }
}

int32_t RflanStream_Disable( rflan_stream_t *Instance, rflan_stream_channel_t Channel )
{
  return AxiDma_Stop( RflanStream_GetDma(Instance, Channel) );
}

static int32_t RflanStream_ChannelToAdrvPortChannel( rflan_stream_channel_t StreamChannel, adi_common_Port_e *Port, adi_common_ChannelNumber_e *Channel)
{
  if( StreamChannel == RflanStreamChannel_Tx1 )
  {
    *Port = ADI_TX;
    *Channel = ADI_CHANNEL_1;
  }
  else if( StreamChannel == RflanStreamChannel_Tx2 )
  {
    *Port = ADI_TX;
    *Channel = ADI_CHANNEL_2;
  }
  else if( StreamChannel == RflanStreamChannel_Rx1 )
  {
    *Port = ADI_RX;
    *Channel = ADI_CHANNEL_1;
  }
  else if( StreamChannel == RflanStreamChannel_Rx2 )
  {
    *Port = ADI_RX;
    *Channel = ADI_CHANNEL_2;
  }
  else
    return RflanStreamStatus_InvalidParameter;

  return RflanStreamStatus_Success;
}

int32_t RflanStream_Transfer( rflan_stream_t *Instance, uint32_t Addr, uint32_t WordCnt, rflan_stream_channel_t Channel )
{
  int32_t status;
  adi_common_Port_e AdiPort;
  adi_common_ChannelNumber_e AdiChannel;

  axi_dma_t *Dma = RflanStream_GetDma(Instance, Channel);

  if((status = RflanStream_ChannelToAdrvPortChannel( Channel, &AdiPort, &AdiChannel )) != 0)
    return status;

  if((status = Adrv9001_ToRfEnabled( Instance->Adrv9001, AdiPort, AdiChannel )) != 0)
    return status;

  if((status = AxiDma_Stop( Dma )) != 0)
    return status;

  if((status = AxiDma_StartTransfer( Dma)) != 0)
    return status;

  if((status = Adrv9001_ToPrimed( Instance->Adrv9001, AdiPort, AdiChannel )) != 0)
    return status;

  return RflanStreamStatus_Success;
}

int32_t RflanStream_StopTransfer( rflan_stream_t *Instance, rflan_stream_channel_t Channel )
{

  int32_t status;
  adi_common_Port_e AdiPort;
  adi_common_ChannelNumber_e AdiChannel;

  axi_dma_t *Dma = RflanStream_GetDma(Instance, Channel);

  if((status = RflanStream_ChannelToAdrvPortChannel( Channel, &AdiPort, &AdiChannel )) != 0)
    return status;

  if((status = Adrv9001_ToPrimed( Instance->Adrv9001, AdiPort, AdiChannel )) != 0)
    return status;

  if((status = AxiDma_Stop( Dma )) != 0)
    return status;


  return RflanStreamStatus_Success;
}

int32_t RflanStream_StartTransfer( rflan_stream_t *Instance, uint32_t Addr, uint32_t WordCnt, rflan_stream_channel_t Channel, bool Cyclic )
{
  int32_t status;
  adi_common_Port_e AdiPort;
  adi_common_ChannelNumber_e AdiChannel;

  axi_dma_t *Dma = RflanStream_GetDma(Instance, Channel);

  AxiDma_SetCyclic(Dma, Cyclic);
  AxiDma_SetSampleCnt(Dma, WordCnt);

  if((status = RflanStream_ChannelToAdrvPortChannel( Channel, &AdiPort, &AdiChannel )) != 0)
    return status;

  if((status = AxiDma_Stop( Dma )) != 0)
    return status;

  if((status = AxiDma_StartTransfer(Dma)) != 0)
    return status;

  if((status = Adrv9001_ToRfEnabled( Instance->Adrv9001, AdiPort, AdiChannel )) != 0)
    return status;



  return RflanStreamStatus_Success;
}

int32_t RflanStream_Initialize(rflan_stream_t *Instance, rflan_stream_init_t *Init )
{
  Instance->Adrv9001 = Init->Adrv9001;
  Instance->Callback = Init->Callback;
  Instance->CallbackRef = Init->CallbackRef;

  int32_t status;

  axi_dma_init_t DmaInit = {
      .CallbackRef = Instance,
      .Flags = 0,
  };
  DmaInit.SampleSize = sizeof(uint32_t);
  DmaInit.SampleCnt = 4096;
  DmaInit.Addr = RFLAN_DMA_TX1_BUF_ADDR;
  DmaInit.IrqInstance = &xInterruptController;
  DmaInit.Base = XPAR_TX1_DMA_BASEADDR;
  DmaInit.Callback = (axi_dma_callback_t)RflanStream_Tx1DmaCallback;
  DmaInit.Direction = AxiDmaDir_MemToDev;
  DmaInit.IrqId = XPAR_FABRIC_TX1_DMA_IRQ_INTR;

  if((status = AxiDma_Initialize( &Instance->Tx1Dma, &DmaInit )) != 0)
    return status;

  DmaInit.SampleSize = sizeof(uint32_t);
  DmaInit.SampleCnt = 4096;
  DmaInit.Addr = RFLAN_DMA_TX2_BUF_ADDR;
  DmaInit.IrqInstance = &xInterruptController;
  DmaInit.Base = XPAR_TX2_DMA_BASEADDR;
  DmaInit.Callback = (axi_dma_callback_t)RflanStream_Tx2DmaCallback;
  DmaInit.Direction = AxiDmaDir_MemToDev;
  DmaInit.IrqId = XPAR_FABRIC_TX2_DMA_IRQ_INTR;

  if((status = AxiDma_Initialize( &Instance->Tx2Dma, &DmaInit )) != 0)
    return status;

  DmaInit.SampleSize = sizeof(uint32_t);
  DmaInit.SampleCnt = 4096;
  DmaInit.Addr = RFLAN_DMA_RX1_BUF_ADDR;
  DmaInit.IrqInstance = &xInterruptController;
  DmaInit.Base = XPAR_RX1_DMA_BASEADDR;
  DmaInit.Callback = (axi_dma_callback_t)RflanStream_Rx1DmaCallback;
  DmaInit.Direction = AxiDmaDir_DevToMem;
  DmaInit.IrqId = XPAR_FABRIC_RX1_DMA_IRQ_INTR;

  if((status = AxiDma_Initialize( &Instance->Rx1Dma, &DmaInit )) != 0)
    return status;

  DmaInit.SampleSize = sizeof(uint32_t);
  DmaInit.SampleCnt = 4096;
  DmaInit.Addr = RFLAN_DMA_RX2_BUF_ADDR;
  DmaInit.IrqInstance = &xInterruptController;
  DmaInit.Base = XPAR_RX2_DMA_BASEADDR;
  DmaInit.Callback = (axi_dma_callback_t)RflanStream_Rx2DmaCallback;
  DmaInit.Direction = AxiDmaDir_DevToMem;
  DmaInit.IrqId = XPAR_FABRIC_RX2_DMA_IRQ_INTR;

  if((status = AxiDma_Initialize( &Instance->Rx2Dma, &DmaInit )) != 0)
    return status;



  return RflanStreamStatus_Success;
}

#endif
