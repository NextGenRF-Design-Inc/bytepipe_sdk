#ifndef SRC_RFLAN_STREAM_H_
#define SRC_RFLAN_STREAM_H_
/***************************************************************************//**
*  \ingroup    RFLAN
*  \defgroup   RFLAN_STREAM RFLAN Stream
*  @{
*******************************************************************************/
/***************************************************************************//**
*  \file       rflan_stream.h
*
*  \details
*
*  This file contains the stream implementation.
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

#ifdef RFLAN_STREAM_ENABLE

#include "axi_dma.h"
#include "adrv9001.h"
#include "xscugic.h"

#define RFLAN_STREAM_STATUS_OFFSET 		(-2000)                                               ///< Status offset
#define RFLAN_DMA_BUF_ADDR              (0x40000000)                                          ///< Address of DMA buffer
#define RFLAN_DMA_TX1_BUF_ADDR          (RFLAN_DMA_BUF_ADDR)                                  ///< Address of Tx1 DMA buffer
#define RFLAN_DMA_TX1_BUF_SIZE          (0x400000)                                            ///< Length of Tx1 DMA Buffer in bytes
#define RFLAN_DMA_TX2_BUF_ADDR          (RFLAN_DMA_TX1_BUF_ADDR + RFLAN_DMA_TX1_BUF_SIZE)     ///< Address of Tx2 DMA buffer
#define RFLAN_DMA_TX2_BUF_SIZE          (0x400000)                                            ///< Length of Tx2 DMA Buffer in bytes
#define RFLAN_DMA_RX1_BUF_ADDR          (RFLAN_DMA_TX2_BUF_ADDR + RFLAN_DMA_TX2_BUF_SIZE)     ///< Address of Rx1 DMA buffer
#define RFLAN_DMA_RX1_BUF_SIZE          (0x400000)                                            ///< Length of Rx1 DMA Buffer in bytes
#define RFLAN_DMA_RX2_BUF_ADDR          (RFLAN_DMA_RX1_BUF_ADDR + RFLAN_DMA_RX1_BUF_SIZE)     ///< Address of Rx2 DMA buffer
#define RFLAN_DMA_RX2_BUF_SIZE          (0x400000)                                            ///< Length of Rx2 DMA Buffer in bytes


/**
** Status Definition
*/
typedef enum
{
  RflanStreamStatus_Success               = (0),                                 ///< Success
  RflanStreamStatus_MemoryError           = (RFLAN_STREAM_STATUS_OFFSET - 1),    ///< Memory Error
  RflanStreamStatus_InvalidParameter      = (RFLAN_STREAM_STATUS_OFFSET - 2),    ///< Invalid Parameter
  RflanStreamStatus_DmaError              = (RFLAN_STREAM_STATUS_OFFSET - 3),    ///< DMA Error
  RflanStreamStatus_Done                  = (RFLAN_STREAM_STATUS_OFFSET - 4),    ///< Stream Done
} rflan_stream_status_t;

/**
** Stream Channel Definition
*/
typedef enum
{
  RflanStreamChannel_Tx1      = (0),     ///< TX1 Channel
  RflanStreamChannel_Tx2      = (1),     ///< TX2 Channel
  RflanStreamChannel_Rx1      = (2),     ///< RX1 Channel
  RflanStreamChannel_Rx2      = (3)      ///< RX2 Channel
} rflan_stream_channel_t;


/***************************************************************************//**
*
* \details  Callback function prototype.
*
* \param    Buf          [in]  Address of stream buffer
* \param    Size         [in]  Size of buffer in words
* \param    Channel      [in]  Logical Channel of stream
* \param    CallbackRef  [in]  Callback reference provided by the application
*
* \return   none
*
*******************************************************************************/
typedef void (*stream_callback_t)( uint32_t Buf, uint32_t Size, rflan_stream_channel_t Channel, void *CallbackRef );


/**
**  Initialization Structure
**
**  This structure is used to initialization the module.  The application can
**  destroy the corresponding parameter after calling initializing the module.
*/
typedef struct{
  stream_callback_t   Callback;     ///< Parent Callback function
  void               *CallbackRef;  ///< Parent User Data
  adrv9001_t	       *Adrv9001;     ///< ADRV9001 Instance
  uint32_t            Tx1DmaBase;   ///< AXI Base address
  uint32_t            Tx2DmaBase;   ///< AXI Base address
  uint32_t            Rx1DmaBase;   ///< AXI Base address
  uint32_t            Rx2DmaBase;   ///< AXI Base address
  uint32_t            Tx1DmaIrqId;  ///< IRQ Id
  uint32_t            Tx2DmaIrqId;  ///< IRQ Id
  uint32_t            Rx1DmaIrqId;  ///< IRQ Id
  uint32_t            Rx2DmaIrqId;  ///< IRQ Id
  XScuGic                    *IrqInstance;           ///< Processor Interrupt Controller Instance
}rflan_stream_init_t;

/**
**  Instance structure
**
**  This structure holds the variables associated with this module.  This
**  structure must be allocated and maintained by the application.  The application
**  should not access this structure directly.  The application must pass this
**  variable when calling all APIs.
*/
typedef struct{
  stream_callback_t   Callback;     ///< Parent Callback function
  void               *CallbackRef;  ///< Parent User Data
  adrv9001_t         *Adrv9001;     ///< ADRV9001 Instance
  axi_dma_t           Tx1Dma;       ///< DMA Instance
  axi_dma_t           Tx2Dma;       ///< DMA Instance
  axi_dma_t           Rx1Dma;       ///< DMA Instance
  axi_dma_t           Rx2Dma;       ///< DMA Instance
}rflan_stream_t;


/***************************************************************************//**
*
* \details  Initialize Driver
*
* \param    Instance [in]  Driver Instance
* \param    Init     [in]  Initialization structure
*
* \return   status
*
*******************************************************************************/
int32_t RflanStream_Initialize( rflan_stream_t *Instance, rflan_stream_init_t *Init );


int32_t RflanStream_Disable       ( rflan_stream_t *Instance, rflan_stream_channel_t Channel );
int32_t RflanStream_StartTransfer ( rflan_stream_t *Instance, uint32_t Addr, uint32_t WordCnt, rflan_stream_channel_t Channel, bool Cyclic );
int32_t RflanStream_Transfer      ( rflan_stream_t *Instance, uint32_t Addr, uint32_t WordCnt, rflan_stream_channel_t Channel );
int32_t RflanStream_StopTransfer  ( rflan_stream_t *Instance, rflan_stream_channel_t Channel );

#endif
#endif
