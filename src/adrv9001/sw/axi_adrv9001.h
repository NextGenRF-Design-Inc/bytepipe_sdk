#ifndef SRC_AXI_ADRV9001_H_
#define SRC_AXI_ADRV9001_H_
/***************************************************************************//**
*  \ingroup    ADRV9001
*  \defgroup   AXI_ADRV9001 AXI ADRV9001 Interface Driver
*  @{
*******************************************************************************/
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

#define ADRV9001_DGPIO_PIN_0              (0)       ///< DGPIO[0] pin number
#define ADRV9001_DGPIO_PIN_1              (1)       ///< DGPIO[1] pin number
#define ADRV9001_DGPIO_PIN_2              (2)       ///< DGPIO[2] pin number
#define ADRV9001_DGPIO_PIN_3              (3)       ///< DGPIO[3] pin number
#define ADRV9001_DGPIO_PIN_4              (4)       ///< DGPIO[4] pin number
#define ADRV9001_DGPIO_PIN_5              (5)       ///< DGPIO[5] pin number
#define ADRV9001_DGPIO_PIN_6              (6)       ///< DGPIO[6] pin number
#define ADRV9001_DGPIO_PIN_7              (7)       ///< DGPIO[7] pin number
#define ADRV9001_DGPIO_PIN_8              (8)       ///< DGPIO[8] pin number
#define ADRV9001_DGPIO_PIN_9              (9)       ///< DGPIO[9] pin number
#define ADRV9001_DGPIO_PIN_10             (10)      ///< DGPIO[10] pin number
#define ADRV9001_DGPIO_PIN_11             (11)      ///< DGPIO[11] pin number
#define ADRV9001_DGPIO_PIN_12             (12)      ///< DGPIO[12] pin number
#define ADRV9001_DGPIO_PIN_13             (13)      ///< DGPIO[13] pin number
#define ADRV9001_DGPIO_PIN_14             (14)      ///< DGPIO[14] pin number
#define ADRV9001_DGPIO_PIN_15             (15)      ///< DGPIO[15] pin number

/**
**  Data Path Structure
*/
typedef enum
{
  AXI_ADRV9001_DATA_PATH_AXIS = 0,      ///< Route SSI data to and from AXIS stream
  AXI_ADRV9001_DATA_PATH_AXI = 1        ///< Route SSI data to and from AXI register
} axi_adrv9001_data_path_t;

/**
**  Initialization Structure
**
**  This structure is used to initialization the module.  The application can
**  destroy the corresponding parameter after calling initializing the module.
*/
typedef struct{
  uint32_t            Base;             ///< AXI bus base address
  uint32_t            IrqId;            ///< Processor interrupt ID.
  XScuGic            *IrqInstance;      ///< Processor Interrupt Controller Instance
}axi_adrv9001_init_t;

/**
**  Instance structure
**
**  This structure holds the variables associated with this module.  This
**  structure must be allocated and maintained by the application.  The application
**  should not access this structure directly.  The application must pass this
**  variable when calling all APIs.
*/
typedef struct{
  uint32_t            Base;             ///< AXI bus base address
  uint32_t            IrqId;            ///< Processor interrupt ID.
  uint8_t volatile    SpiDone;          ///< Flag for waiting on SPI interrupt
  XScuGic            *IrqInstance;      ///< Processor Interrupt Controller Instance
}axi_adrv9001_t;


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
int32_t AxiAdrv9001_Initialize( axi_adrv9001_t *Instance, axi_adrv9001_init_t *Init );


/***************************************************************************//**
*
* \details  Set Reset Pin connected to ADRV9001
*
* \param    Instance [in]  Driver Instance
* \param    Level    [in]  0 = low, 1 = high
*
* \return   none
*
*******************************************************************************/
void AxiAdrv9001_ResetbPinSet( axi_adrv9001_t *Instance, uint8_t Level );

/***************************************************************************//**
*
* \details  Set DGPIO Pin values
*
* \param    Instance [in]  Driver Instance
* \param    Value    [in]  Bitmask corresponding to each pin
*
* \return   none
*
*******************************************************************************/
void AxiAdrv9001_SetDgpio( axi_adrv9001_t *Instance, uint32_t Value );

/***************************************************************************//**
*
* \details  Get DGPIO Pin values
*
* \param    Instance [in]  Driver Instance
* \param    Value    [out] Bitmask corresponding to each pin
*
* \return   none
*
*******************************************************************************/
void AxiAdrv9001_GetDgpio( axi_adrv9001_t *Instance, uint32_t *Value );

/***************************************************************************//**
*
* \details  Get DGPIO Pin direction
*
* \param    Instance [in]  Driver Instance
* \param    Value    [out] Bitmask corresponding to each pin, 0 = output,
*                          1 = input
*
* \return   none
*
*******************************************************************************/
void AxiAdrv9001_GetDgpioDir( axi_adrv9001_t *Instance, uint32_t *Value );

/***************************************************************************//**
*
* \details  Set DGPIO Pin direction
*
* \param    Instance [in]  Driver Instance
* \param    Value    [in]  Bitmask corresponding to each pin, 0 = output,
*                          1 = input
*
* \return   none
*
*******************************************************************************/
void AxiAdrv9001_SetDgpioDir( axi_adrv9001_t *Instance, uint32_t Value );

/***************************************************************************//**
*
* \details  Set DGPIO Pin value
*
* \param    Instance [in]  Driver Instance
* \param    Pin      [in]  Pin number.
* \param    Level    [in]  Level, hi = 1, lo = 0
*
* \return   none
*
*******************************************************************************/
void AxiAdrv9001_SetDgpioPin(axi_adrv9001_t *Instance, uint8_t Pin, uint8_t Level );

/***************************************************************************//**
*
* \details  Toggle DGPIO Pin
*
* \param    Instance [in]  Driver Instance
* \param    Pin      [in]  Pin number.
*
* \return   none
*
*******************************************************************************/
void AxiAdrv9001_ToggleDgpioPin(axi_adrv9001_t *Instance, uint8_t Pin );


void AxiAdrv9001_SetTxDataPath       ( axi_adrv9001_t *Instance, adi_common_ChannelNumber_e Channel, axi_adrv9001_data_path_t Value );
void AxiAdrv9001_GetTxDataPath       ( axi_adrv9001_t *Instance, adi_common_ChannelNumber_e Channel, axi_adrv9001_data_path_t *Value );
void AxiAdrv9001_SetTxData           ( axi_adrv9001_t *Instance, adi_common_ChannelNumber_e Channel, uint32_t Value );
void AxiAdrv9001_GetRxData           ( axi_adrv9001_t *Instance, adi_common_ChannelNumber_e Channel, uint32_t *Value );
void AxiAdrv9001_GetEnableDelay      ( axi_adrv9001_t *Instance, adi_common_Port_e Port, adi_common_ChannelNumber_e Channel, uint16_t *SampleCnt );
void AxiAdrv9001_GetDisableDelay     ( axi_adrv9001_t *Instance, adi_common_Port_e Port, adi_common_ChannelNumber_e Channel, uint16_t *SampleCnt );
void AxiAdrv9001_SetEnableMode       ( axi_adrv9001_t *Instance, adi_common_Port_e Port, adi_common_ChannelNumber_e Channel, adi_adrv9001_ChannelEnableMode_e mode );
void AxiAdrv9001_GetCaptureControlCnt( axi_adrv9001_t *Instance, uint32_t *Value );

void AxiAdrv9001_SetEnableDelay      ( axi_adrv9001_t *Instance, adi_common_Port_e Port, adi_common_ChannelNumber_e Channel, uint16_t SampleCnt );
void AxiAdrv9001_SetDisableDelay     ( axi_adrv9001_t *Instance, adi_common_Port_e Port, adi_common_ChannelNumber_e Channel, uint16_t SampleCnt );
void AxiAdrv9001_SetCaptureControlCnt( axi_adrv9001_t *Instance, uint32_t Value );

int32_t AxiAdrv9001_MspiTransfer     ( axi_adrv9001_t *Instance, uint8_t *TxBuf, uint8_t *RxBuf, uint32_t Length);

#endif
