
#include <stdio.h>
#include <string.h>
#include "rflan_stream.h"
#include "xil_cache.h"
#include <xil_io.h>
#include "mem.h"
#include "sleep.h"

#ifdef RFLAN_STREAM_ENABLE

static void RflanStream_Tx1DmaCallback( axi_dma_t *Instance, axi_dma_evt_type_t evt )
{
  rflan_stream_t *stream = (rflan_stream_t*)Instance->CallbackRef;

  if( evt == EvtType_EndofTransfer)
  {
    if( stream->Callback != NULL )
      stream->Callback( Instance->BigTransfer.Address, Instance->BigTransfer.Size, RflanStreamChannel_Tx1, stream->CallbackRef );
  }
}

static void RflanStream_Tx2DmaCallback( axi_dma_t *Instance, axi_dma_evt_type_t evt )
{
  rflan_stream_t *stream = (rflan_stream_t*)Instance->CallbackRef;

  if( evt == EvtType_EndofTransfer)
  {
    if( stream->Callback != NULL )
      stream->Callback( Instance->BigTransfer.Address, Instance->BigTransfer.Size, RflanStreamChannel_Tx2, stream->CallbackRef );
  }
}

static void RflanStream_Rx1DmaCallback( axi_dma_t *Instance, axi_dma_evt_type_t evt )
{
  rflan_stream_t *stream = (rflan_stream_t*)Instance->CallbackRef;

  if( evt == EvtType_EndofTransfer)
  {
    if( stream->Callback != NULL )
      stream->Callback( Instance->BigTransfer.Address, Instance->BigTransfer.Size, RflanStreamChannel_Rx1, stream->CallbackRef );
  }
}

static void RflanStream_Rx2DmaCallback( axi_dma_t *Instance, axi_dma_evt_type_t evt )
{
  rflan_stream_t *stream = (rflan_stream_t*)Instance->CallbackRef;

  if( evt == EvtType_EndofTransfer)
  {
    if( stream->Callback != NULL )
      stream->Callback( Instance->BigTransfer.Address, Instance->BigTransfer.Size, RflanStreamChannel_Rx2, stream->CallbackRef );
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

  if((status = AxiDma_Transfer( Dma, Addr, (WordCnt << 2) )) != 0)
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

  if((status = RflanStream_ChannelToAdrvPortChannel( Channel, &AdiPort, &AdiChannel )) != 0)
    return status;

  if((status = AxiDma_Stop( Dma )) != 0)
    return status;

  if((status = Adrv9001_ToPrimed( Instance->Adrv9001, AdiPort, AdiChannel )) != 0)
    return status;

  if((status = AxiDma_StartTransfer( Dma, Addr, (WordCnt << 2), Cyclic )) != 0)
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

  DmaInit.Base = XPAR_TX1_DMA_BASEADDR;
  DmaInit.Callback = (axi_dma_callback_t)RflanStream_Tx1DmaCallback;
  DmaInit.Direction = AxiDmaDir_MemToDev;
  DmaInit.IrqId = XPAR_FABRIC_TX1_DMA_IRQ_INTR;

  if((status = AxiDma_Initialize( &Instance->Tx1Dma, &DmaInit )) != 0)
    return status;

  DmaInit.Base = XPAR_TX2_DMA_BASEADDR;
  DmaInit.Callback = (axi_dma_callback_t)RflanStream_Tx2DmaCallback;
  DmaInit.Direction = AxiDmaDir_MemToDev;
  DmaInit.IrqId = XPAR_FABRIC_TX2_DMA_IRQ_INTR;

  if((status = AxiDma_Initialize( &Instance->Tx2Dma, &DmaInit )) != 0)
    return status;

  DmaInit.Base = XPAR_RX1_DMA_BASEADDR;
  DmaInit.Callback = (axi_dma_callback_t)RflanStream_Rx1DmaCallback;
  DmaInit.Direction = AxiDmaDir_DevToMem;
  DmaInit.IrqId = XPAR_FABRIC_RX1_DMA_IRQ_INTR;

  if((status = AxiDma_Initialize( &Instance->Rx1Dma, &DmaInit )) != 0)
    return status;

  DmaInit.Base = XPAR_RX2_DMA_BASEADDR;
  DmaInit.Callback = (axi_dma_callback_t)RflanStream_Rx2DmaCallback;
  DmaInit.Direction = AxiDmaDir_DevToMem;
  DmaInit.IrqId = XPAR_FABRIC_RX2_DMA_IRQ_INTR;

  if((status = AxiDma_Initialize( &Instance->Rx2Dma, &DmaInit )) != 0)
    return status;

  return RflanStreamStatus_Success;
}

#endif
