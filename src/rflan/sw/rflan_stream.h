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


#ifndef RFLAN_STREAM_STATUS_OFFSET
#define RFLAN_STREAM_STATUS_OFFSET (-2000)
#endif


typedef enum
{
  RflanStreamStatus_Success               = (0),
  RflanStreamStatus_MemoryError           = (RFLAN_STREAM_STATUS_OFFSET - 1),
  RflanStreamStatus_InvalidParameter      = (RFLAN_STREAM_STATUS_OFFSET - 2),
  RflanStreamStatus_DmaError              = (RFLAN_STREAM_STATUS_OFFSET - 3),
  RflanStreamStatus_Done                  = (RFLAN_STREAM_STATUS_OFFSET - 4),
} rflan_stream_status_t;


typedef enum
{
  RflanStreamChannel_Tx1      = (0),
  RflanStreamChannel_Tx2      = (1),
  RflanStreamChannel_Rx1      = (2),
  RflanStreamChannel_Rx2      = (3)
} rflan_stream_channel_t;


typedef void (*stream_callback_t)( uint32_t Buf, uint32_t Size, rflan_stream_channel_t Channel, void *CallbackRef );


/**
 * \brief Data for initialization of the stream.
 */
typedef struct{
  stream_callback_t   Callback;     ///< Parent Callback function
  void               *CallbackRef;  ///< Parent User Data
  adrv9001_t	       *Adrv9001;     ///< ADRV9001 Instance
}rflan_stream_init_t;

typedef struct{
  stream_callback_t   Callback;     ///< Parent Callback function
  void               *CallbackRef;  ///< Parent User Data
  adrv9001_t         *Adrv9001;     ///< ADRV9001 Instance
  axi_dma_t           Tx1Dma;       ///< DMA Instance
  axi_dma_t           Tx2Dma;       ///< DMA Instance
  axi_dma_t           Rx1Dma;       ///< DMA Instance
  axi_dma_t           Rx2Dma;       ///< DMA Instance
}rflan_stream_t;


int32_t RflanStream_Initialize    ( rflan_stream_t *Instance, rflan_stream_init_t *Init );
int32_t RflanStream_Disable       ( rflan_stream_t *Instance, rflan_stream_channel_t Channel );
int32_t RflanStream_StartTransfer ( rflan_stream_t *Instance, uint32_t Addr, uint32_t WordCnt, rflan_stream_channel_t Channel, bool Cyclic );
int32_t RflanStream_Transfer      ( rflan_stream_t *Instance, uint32_t Addr, uint32_t WordCnt, rflan_stream_channel_t Channel );
int32_t RflanStream_StopTransfer  ( rflan_stream_t *Instance, rflan_stream_channel_t Channel );

#endif
#endif
