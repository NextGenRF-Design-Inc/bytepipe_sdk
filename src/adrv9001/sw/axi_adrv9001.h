#ifndef SRC_AXI_ADRV9001_H_
#define SRC_AXI_ADRV9001_H_
/***************************************************************************//**
*  \file       axi_adrv9001.h
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
#include "adi_adrv9001_common_types.h"
#include "adi_adrv9001_radio_types.h"
#include "xscugic.h"



/* Register Address */
#define ADRV9001_TX1_PS_ENABLE_ADDR         ((0 ) << 2)    ///< TX1 enable register address offset
#define ADRV9001_TX2_PS_ENABLE_ADDR         ((1 ) << 2)    ///< TX2 enable register address offset
#define ADRV9001_RX1_PS_ENABLE_ADDR         ((2 ) << 2)    ///< RX1 enable register address offset
#define ADRV9001_RX2_PS_ENABLE_ADDR         ((3 ) << 2)    ///< RX2 enable register address offset
#define ADRV9001_RSTN_ADDR                  ((4 ) << 2)    ///< Reset pin register address offset
#define ADRV9001_DGPIO_DIR_ADDR             ((5 ) << 2)    ///< DGPIO direction register address offset
#define ADRV9001_DGPIO_IO_ADDR              ((6 ) << 2)    ///< DGPIO value register address offset
#define ADRV9001_TX1_DATA_SRC_ADDR          ((7 ) << 2)    ///< TX1 Data Source
#define ADRV9001_TX2_DATA_SRC_ADDR          ((8 ) << 2)    ///< TX2 Data Source
#define ADRV9001_RX_RAMP_DET_ADDR           ((9 ) << 2)    ///< RX Ramp pattern detected
#define ADRV9001_RX_PN15_DET_ADDR           ((10) << 2)    ///< RX PN15 pattern detected
#define ADRV9001_RX_FIXED_PATTERN_DET_ADDR  ((11) << 2)    ///< RX Fixed pattern detected

#define ADRV9001_RX1_FIXED_PATTERN_ADDR     ((12) << 2)    ///< RX1 Fixed pattern
#define ADRV9001_RX2_FIXED_PATTERN_ADDR     ((13) << 2)    ///< RX2 Fixed pattern
#define ADRV9001_TX1_FIXED_PATTERN_ADDR     ((14) << 2)    ///< TX1 Fixed pattern
#define ADRV9001_TX2_FIXED_PATTERN_ADDR     ((15) << 2)    ///< TX2 Fixed pattern

#define ADRV9001_RX1_HOP_SETUP_CNT_ADDR     ((16) << 2)    ///<
#define ADRV9001_RX2_HOP_SETUP_CNT_ADDR     ((17) << 2)    ///<
#define ADRV9001_TX1_HOP_SETUP_CNT_ADDR     ((18) << 2)    ///<
#define ADRV9001_TX2_HOP_SETUP_CNT_ADDR     ((19) << 2)    ///<

#define ADRV9001_HOP_DGPIO_SETUP_CNT_ADDR   ((20) << 2)    ///<

#define ADRV9001_MSPI_ADDR                  ((21) << 2)    ///< SPI data register offset

#define ADRV9001_HOP_IRQ_ADDR               ((23) << 2)    ///<

#define ADRV9001_PS_HOP_TRIG_ADDR           ((24) << 2)    ///< Trigger a hop
#define ADRV9001_NEXT_HOP_ENABLE_MASK_ADDR  ((25) << 2)    ///< Mask determining which channels are enabled on next hop transition, bit0 = rx1, bit1 = rx2, bit2 = tx1, bit3 = tx2
#define ADRV9001_ENABLE_PL_HOP_TRIG_ADDR    ((26) << 2)    ///< Enable hopping trigger from PL

#define ADRV9001_SWAP_IQ_ADDR               ((27) << 2)    ///< Swap IQ. [tx1,tx2,rx1,rx2]

#define ADRV9001_HOPPING_MODE_ENABLE_ADDR   ((28) << 2)    ///< Enable hopping mode

#define ADRV9001_PS_SETUP_PIN_ADDR          ((30) << 2)    ///<

#define ADRV9001_ID_ADDR                    ((31) << 2)    ///< Driver id register offset

#define ADRV9001_DGPIO_PIN_0                (0)       ///< DGPIO[0] pin number
#define ADRV9001_DGPIO_PIN_1                (1)       ///< DGPIO[1] pin number
#define ADRV9001_DGPIO_PIN_2                (2)       ///< DGPIO[2] pin number
#define ADRV9001_DGPIO_PIN_3                (3)       ///< DGPIO[3] pin number
#define ADRV9001_DGPIO_PIN_4                (4)       ///< DGPIO[4] pin number
#define ADRV9001_DGPIO_PIN_5                (5)       ///< DGPIO[5] pin number
#define ADRV9001_DGPIO_PIN_6                (6)       ///< DGPIO[6] pin number
#define ADRV9001_DGPIO_PIN_7                (7)       ///< DGPIO[7] pin number
#define ADRV9001_DGPIO_PIN_8                (8)       ///< DGPIO[8] pin number
#define ADRV9001_DGPIO_PIN_9                (9)       ///< DGPIO[9] pin number
#define ADRV9001_DGPIO_PIN_10               (10)      ///< DGPIO[10] pin number
#define ADRV9001_DGPIO_PIN_11               (11)      ///< DGPIO[11] pin number
#define ADRV9001_DGPIO_PIN_12               (12)      ///< DGPIO[12] pin number
#define ADRV9001_DGPIO_PIN_13               (13)      ///< DGPIO[13] pin number
#define ADRV9001_DGPIO_PIN_14               (14)      ///< DGPIO[14] pin number
#define ADRV9001_DGPIO_PIN_15               (15)      ///< DGPIO[15] pin number

#define ADRV9001_RX1_HOP_ENABLE_MASK        (0x01)
#define ADRV9001_RX2_HOP_ENABLE_MASK        (0x02)
#define ADRV9001_TX1_HOP_ENABLE_MASK        (0x04)
#define ADRV9001_TX2_HOP_ENABLE_MASK        (0x08)

#define ADRV9001_RX2_SWAP_IQ_MASK           (0x01)
#define ADRV9001_RX1_SWAP_IQ_MASK           (0x02)
#define ADRV9001_TX2_SWAP_IQ_MASK           (0x04)
#define ADRV9001_TX1_SWAP_IQ_MASK           (0x08)

#define ADRV9001_RX2_SETUP_PIN_MASK         (0x01)
#define ADRV9001_RX1_SETUP_PIN_MASK         (0x02)
#define ADRV9001_TX2_SETUP_PIN_MASK         (0x04)
#define ADRV9001_TX1_SETUP_PIN_MASK         (0x08)

#define ADRV9001_HOP_TRIG_CLEAR_MASK        (0x01)
#define ADRV9001_HOP_TRIG_ENABLE_MASK       (0x02)
#define ADRV9001_HOP_TRIG_STATUS_MASK       (0x04)



typedef enum{
  AxiAdrv9001TxDataSrc_Axis = 0,
  AxiAdrv9001TxDataSrc_Zeros,
  AxiAdrv9001TxDataSrc_Ones,
  AxiAdrv9001TxDataSrc_Ramp,
  AxiAdrv9001TxDataSrc_Pn15,
  AxiAdrv9001TxDataSrc_FixedPattern
}axi_adrv9001_data_src_t;


typedef struct{
  uint32_t            Base;             ///< AXI bus base address
  uint32_t            IrqId;            ///< Processor interrupt ID.
  XScuGic            *IrqInstance;      ///< Processor Interrupt Controller Instance
  uint32_t            ClockFreqHz;
}axi_adrv9001_init_t;


typedef struct{
  uint32_t            Base;             ///< AXI bus base address
  uint32_t            IrqId;            ///< Processor interrupt ID.
  uint8_t volatile    SpiDone;          ///< Flag for waiting on SPI interrupt
  XScuGic            *IrqInstance;      ///< Processor Interrupt Controller Instance
  uint32_t            ClockFreqHz;
}axi_adrv9001_t;



int32_t AxiAdrv9001_Initialize( axi_adrv9001_t *Instance, axi_adrv9001_init_t *Init );
void AxiAdrv9001_ResetbPinSet( axi_adrv9001_t *Instance, uint8_t Level );
void AxiAdrv9001_SetDgpio( axi_adrv9001_t *Instance, uint32_t Value );
void AxiAdrv9001_GetDgpio( axi_adrv9001_t *Instance, uint32_t *Value );
void AxiAdrv9001_GetDgpioDir( axi_adrv9001_t *Instance, uint32_t *Value );
void AxiAdrv9001_SetDgpioDir( axi_adrv9001_t *Instance, uint32_t Value );
void AxiAdrv9001_SetDgpioPin(axi_adrv9001_t *Instance, uint8_t Pin, uint8_t Level );
void AxiAdrv9001_ToggleDgpioPin(axi_adrv9001_t *Instance, uint8_t Pin );

void AxiAdrv9001_SetTxDataSrc(axi_adrv9001_t *Instance, axi_adrv9001_data_src_t DataSrc, adi_common_ChannelNumber_e Channel );

axi_adrv9001_data_src_t AxiAdrv9001_GetTxDataSrc(axi_adrv9001_t *Instance, adi_common_ChannelNumber_e Channel);
void AxiAdrv9001_SetPsEnable(axi_adrv9001_t *Instance, adi_common_Port_e Port, adi_common_ChannelNumber_e Channel, uint8_t Enable );
bool AxiAdrv9001_GetPn15MonDet(axi_adrv9001_t *Instance, adi_common_ChannelNumber_e Channel);
bool AxiAdrv9001_GetRampMonDet(axi_adrv9001_t *Instance, adi_common_ChannelNumber_e Channel);
bool AxiAdrv9001_GetFixedPatternMonDet(axi_adrv9001_t *Instance, adi_common_ChannelNumber_e Channel);
uint32_t AxiAdrv9001_GetFixedPattern(axi_adrv9001_t *Instance, adi_common_Port_e Port, adi_common_ChannelNumber_e Channel);
void AxiAdrv9001_SetFixedPattern(axi_adrv9001_t *Instance, adi_common_Port_e Port, adi_common_ChannelNumber_e Channel, uint32_t Value);

bool AxiAdrv9001_GetPsEnable(axi_adrv9001_t *Instance, adi_common_Port_e Port, adi_common_ChannelNumber_e Channel);
int32_t AxiAdrv9001_MspiTransfer( axi_adrv9001_t *Instance, uint8_t *TxBuf, uint8_t *RxBuf, uint32_t Length);

bool AxiAdrv9001_GetSwapIq(axi_adrv9001_t *Instance, adi_common_Port_e Port, adi_common_ChannelNumber_e Channel);
void AxiAdrv9001_SetSwapIq(axi_adrv9001_t *Instance, adi_common_Port_e Port, adi_common_ChannelNumber_e Channel, bool Swap );

bool AxiAdrv9001_GetHoppingMode(axi_adrv9001_t *Instance );
void AxiAdrv9001_SetHoppingMode(axi_adrv9001_t *Instance, bool Enable );
void AxiAdrv9001_SetNextHopEnableMask(axi_adrv9001_t *Instance, uint8_t mask );
uint8_t AxiAdrv9001_GetNextHopEnableMask(axi_adrv9001_t *Instance );
void AxiAdrv9001_SetNextHopEnable(axi_adrv9001_t *Instance, adi_common_Port_e Port, adi_common_ChannelNumber_e Channel, bool Enable );
bool AxiAdrv9001_GetNextHopEnable(axi_adrv9001_t *Instance, adi_common_Port_e Port, adi_common_ChannelNumber_e Channel );

void AxiAdrv9001_SetHopSetupDelay(axi_adrv9001_t *Instance,adi_common_Port_e Port, adi_common_ChannelNumber_e Channel, float Value );
void AxiAdrv9001_GetHopSetupDelay(axi_adrv9001_t *Instance,adi_common_Port_e Port, adi_common_ChannelNumber_e Channel, float *Value );
void AxiAdrv9001_SetHopDelay(axi_adrv9001_t *Instance, float Value );
void AxiAdrv9001_GetHopDelay(axi_adrv9001_t *Instance, float *Value );

void AxiAdrv9001_TriggerSingleHop(axi_adrv9001_t *Instance );

uint8_t AxiAdrv9001_GetEnablePlHopTrig(axi_adrv9001_t *Instance );
void AxiAdrv9001_SetEnablePlHopTrig(axi_adrv9001_t *Instance, bool Enable );

void AxiAdrv9001_SetSetupPin(axi_adrv9001_t *Instance, adi_common_Port_e Port, adi_common_ChannelNumber_e Channel );

void AxiAdrv9001_ClearHopIrq( axi_adrv9001_t *Instance );
void AxiAdrv9001_SetHopIrqEnable( axi_adrv9001_t *Instance, bool Enable );
bool AxiAdrv9001_GetHopIrqEnable( axi_adrv9001_t *Instance );
bool AxiAdrv9001_GetHopIrqStatus( axi_adrv9001_t *Instance );

#endif
