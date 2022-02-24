
#include <stdio.h>
#include <string.h>
#include "phy.h"
#include "phy_stream.h"
#include "axi_dmac.h"
#include "xil_cache.h"
#include "axi_io.h"

static phy_status_t PhyStream_TransferStop( phy_t *Instance, phy_port_t Port )
{
  if(axi_dmac_stop(Instance->Stream[Port].Dma ) != 0)
    return PhyStatus_DmaError;

  return PhyStatus_Success;
}

static void PhyStream_DmaCallback( axi_dmac_evt_type_t evt, void *param )
{
  if( evt == EvtType_EndofTransfer)
  {
    phy_stream_t *Stream = (phy_stream_t*)param;

    if( PHY_IS_PORT_RX( Stream->Port ) )
    {
      Xil_DCacheFlush();

      uint32_t Data;

      for( int i = 0; i < Stream->SampleCnt; i++ )
      {
        axi_io_read( Stream->DmaBufAddr, i << 2, &Data);
        Stream->SampleBuf[i] = Data;
      }
    }

    Stream->Status = PhyStatus_StreamDone;

    phy_stream_ind_t ind = {
        .Port = Stream->Port,
        .SampleBuf = Stream->SampleBuf,
        .SampleCnt = Stream->SampleCnt,
        .Status = Stream->Status
    };

    if( Stream->Callback != NULL )
    {
      Stream->Callback( PhyEvtType_StreamInd, &ind, Stream->CallbackRef );
    }
  }
}

phy_status_t PhyStream_Disable(phy_t *Instance, phy_port_t Port )
{
  int32_t status;

  if( Port >= PhyPort_Num )
    return PhyStatus_InvalidPort;

  phy_stream_t *Stream = &Instance->Stream[Port];

  status = PhyStream_TransferStop( Instance, Port );

  if( Stream->Status != PhyStatus_StreamDone )
  {
    Stream->Status = (status == PhyStatus_Success)? PhyStatus_StreamDone : PhyStatus_DmaError;

    if( Stream->Callback != NULL )
    {
      phy_stream_ind_t ind = {
          .Port = Stream->Port,
          .SampleBuf = Stream->SampleBuf,
          .SampleCnt = Stream->SampleCnt,
          .Status = Stream->Status
      };

      Stream->Callback( PhyEvtType_StreamInd, &ind, Stream->CallbackRef );
    }
  }

  return status;
}

phy_status_t PhyStream_Enable(phy_t *Instance, phy_stream_req_t *Req )
{
  if( Req->Port >= PhyPort_Num )
    return PhyStatus_InvalidPort;

  if( (Req->SampleCnt > 0) && (Req->SampleBuf == NULL) )
    return PhyStatus_InvalidParameter;

  if( Instance->Stream[Req->Port].Status == PhyStatus_Busy )
    return PhyStatus_Busy;

  phy_stream_t *Stream = &Instance->Stream[Req->Port];

  /* Copy Request */
  Stream->Callback    = Req->Callback;
  Stream->CallbackRef = Req->CallbackRef;
  Stream->Cyclic      = Req->Cyclic;
  Stream->SampleBuf   = Req->SampleBuf;
  Stream->SampleCnt   = Req->SampleCnt;
  Stream->Status      = PhyStatus_Busy;

  /* If Tx write buffer to memory */
  if( PHY_IS_PORT_TX( Stream->Port ) )
  {
    for(int i = 0; i < Stream->SampleCnt; i++)
    {
      axi_io_write(Stream->DmaBufAddr, i << 2, Stream->SampleBuf[i]);
    }
  }

  /* Flush Cache */
  Xil_DCacheFlush();

  /* Update Flags */
  Stream->Dma->flags = (Stream->Cyclic && !Req->Blocking)? DMA_CYCLIC : 0;

  if( Req->Blocking )
  {
    if(axi_dmac_transfer(Stream->Dma, Stream->DmaBufAddr, Stream->SampleCnt * sizeof(uint32_t) ) != 0)
    {
      Stream->Status = PhyStatus_DmaError;
    }
    else
    {
      Stream->Status = PhyStatus_StreamDone;
    }
  }
  else
  {
    if(axi_dmac_transfer_nonblocking(Stream->Dma, Stream->DmaBufAddr, Stream->SampleCnt * sizeof(uint32_t) ) != 0)
    {
      Stream->Status = PhyStatus_DmaError;
    }
    else
    {
      Stream->Status = PhyStatus_StreamStart;
    }
  }

  if( Stream->Callback != NULL )
  {
    phy_stream_ind_t ind = {
        .Port = Req->Port,
        .SampleBuf = Req->SampleBuf,
        .SampleCnt = Req->SampleCnt,
        .Status = Stream->Status
    };

    Stream->Callback( PhyEvtType_StreamInd, &ind, Stream->CallbackRef );
  }

  return Stream->Status == PhyStatus_DmaError? PhyStatus_DmaError : PhyStatus_Success;
}

phy_status_t PhyStream_Initialize( phy_t *Instance, phy_stream_cfg_t *Cfg )
{

  Instance->Stream[0].DmaBufAddr = PHY_STREAM_DMA_RX1_BUF_ADDR;
  Instance->Stream[1].DmaBufAddr = PHY_STREAM_DMA_RX2_BUF_ADDR;
  Instance->Stream[2].DmaBufAddr = PHY_STREAM_DMA_TX1_BUF_ADDR;
  Instance->Stream[3].DmaBufAddr = PHY_STREAM_DMA_TX2_BUF_ADDR;

  uint32_t DmaBase[] = {
      PHY_STREAM_DMA_RX1_BASEADDR,
      PHY_STREAM_DMA_RX2_BASEADDR,
      PHY_STREAM_DMA_TX1_BASEADDR,
      PHY_STREAM_DMA_TX2_BASEADDR
  };

  uint32_t DmaIrq[] = {
      PHY_STREAM_DMA_RX1_IRQ,
      PHY_STREAM_DMA_RX2_IRQ,
      PHY_STREAM_DMA_TX1_IRQ,
      PHY_STREAM_DMA_TX2_IRQ
  };

  axi_dmac_init_t DmaInit = {
      .Callback = PhyStream_DmaCallback,
  };

  for(phy_port_t i = 0; i < PhyPort_Num; i++)
  {
    /* Initialize Stream */
    Instance->Stream[i].Cyclic = 0;
    Instance->Stream[i].SampleCnt = 0;
    Instance->Stream[i].SampleBuf = NULL;
    Instance->Stream[i].Callback = NULL;
    Instance->Stream[i].CallbackRef = 0;
    Instance->Stream[i].Port = i;
    Instance->Stream[i].Dma = NULL;

    /* Pass Stream instance from DMA callback */
    DmaInit.CallbackRef = &Instance->Stream[i];
    DmaInit.base = DmaBase[i];

    if(PHY_IS_PORT_TX(i))
    {
      DmaInit.direction = DMA_MEM_TO_DEV;
      DmaInit.flags = DMA_CYCLIC;
    }
    else
    {
      DmaInit.direction = DMA_DEV_TO_MEM;
      DmaInit.flags = 0;
    }

    DmaInit.irqId = DmaIrq[i];

    if( axi_dmac_init(&Instance->Stream[i].Dma, &DmaInit) != 0 )
      return PhyStatus_DmaError;
  }

  Xil_ICacheEnable();
  Xil_DCacheEnable();

  return PhyStatus_Success;
}
