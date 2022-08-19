#ifndef _PARAMETERS_H_
#define _PARAMETERS_H_
/***************************************************************************//**
*  \ingroup    APP
*  \defgroup   PARAMETERS Application parameters
*  @{
*******************************************************************************/
/***************************************************************************//**
*  \file       parameters.h
*
*  \details    This file contains RFLAN application definitions.
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

#ifdef __cplusplus
extern "C"
{
#endif

#include "xparameters.h"
#include "ff.h"

#define PLATFORM_ZYNQMP

#define ADRV9001_STATUS_OFFSET          (0)
#define CLI_STATUS_OFFSET               (-3000)
#define RFLAN_LWIP_STATUS_OFFSET        (-4000)
#define RFLAN_STREAM_STATUS_OFFSET      (-2000)
#define PIB_STATUS_OFFSET               (-5000)

//#define INTC_DEVICE_ID                  XPAR_SCUGIC_SINGLE_DEVICE_ID

#define FF_LOGICAL_DRIVE_PATH           ("1:/")
#define FF_FILENAME_MAX_LEN             (FF_LFN_BUF)

#define GPIO_DEVICE_ID                  (XPAR_PSU_GPIO_0_DEVICE_ID)
#define GPIO_LED_PIN                    (7)
#define GPIO_HWV0_PIN                   (0)
#define GPIO_HWV1_PIN                   (2)
#define GPIO_HWV2_PIN                   (3)

#define GTR0_REFCLK_FREQ_HZ             (52000000)
#define GTR1_REFCLK_FREQ_HZ             (125000000)
#define GTR2_REFCLK_FREQ_HZ             (27000000)
#define GTR3_REFCLK_FREQ_HZ             (300000000)
#define GTR0_REFCLK_VERSACLK5_PORT      (2)
#define GTR1_REFCLK_VERSACLK5_PORT      (3)
#define GTR2_REFCLK_VERSACLK5_PORT      (1)
#define GTR3_REFCLK_VERSACLK5_PORT      (0)

#define SYSMON_DEVICE_ID                XPAR_XSYSMONPSU_0_DEVICE_ID

#define RFLAN_TASK_STACK_SIZE           4096
#define RFLAN_TASK_PRIORITY             tskIDLE_PRIORITY
#define RFLAN_SETUP_SCRIPT_FILENAME     ("1:/rflan_setup.txt")

#define CLI_RX_STACK_SIZE               4096
#define CLI_TX_STACK_SIZE               1024
#define CLI_RX_TASK_PRIORITY            tskIDLE_PRIORITY + 2
#define CLI_TX_TASK_PRIORITY            tskIDLE_PRIORITY + 3
#define CLI_RX_QUEUE_SIZE               2048
#define CLI_TX_QUEUE_SIZE               32768
#define CLI_PRINT_BUF_SIZE              2048
#define CLI_CMD_BUF_SIZE                1024
#define CLI_CMD_LIST_SIZE               100
#define CLI_HISTORY_BUF_SIZE            256

#define RFLAN_UART_DEVICE_ID			      XPAR_PSU_UART_0_DEVICE_ID
#define RFLAN_UART_INTR_ID         	    XPAR_XUARTPS_0_INTR
#define RFLAN_UART_BAUDRATE             115200

#define RFLAN_LWIP_STACK_SIZE           1024
#define RFLAN_LWIP_TASK_PRIORITY        tskIDLE_PRIORITY + 3
#define RFLAN_LWIP_MAC_ADDR             { 0x00, 0x00, 0x00, 0x0a, 0x35, 0x00, 0x01, 0x02 }
#define RFLAN_LWIP_CLI_PORT             (2233)
#define RFLAN_LWIP_RECV_BUF_SIZE        (1500)
#define RFLAN_LWIP_EMAC_BASEADDR        XPAR_XEMACPS_0_BASEADDR

#define ADRV9001_SCRIPT_FILENAME        ("1:/adrv9001_setup.txt")
#define ADRV9001_SPI_DEVICE_ID          (XPAR_PSU_SPI_0_DEVICE_ID)
#define ADRV9001_SPI_CS                 (0)
#define ADRV9001_CTRL_BASE_ADDR         XPAR_ADRV9002_0_BASEADDR

#define PHY_SCRIPT_FILENAME             ("1:/phy_setup.txt")
#define PHY_TASK_PRIORITY               tskIDLE_PRIORITY + 5
#define PHY_TASK_STACK_SIZE             4096
#define PHY_QUEUE_SIZE                  16

#define RFLAN_DMA_TX1_BASEADDR          (XPAR_TX1_DMA_BASEADDR)
#define RFLAN_DMA_TX2_BASEADDR          (XPAR_TX2_DMA_BASEADDR)
#define RFLAN_DMA_RX1_BASEADDR          (XPAR_RX1_DMA_BASEADDR)
#define RFLAN_DMA_RX2_BASEADDR          (XPAR_RX2_DMA_BASEADDR)
#define RFLAN_DMA_TX1_IRQ               (XPAR_FABRIC_TX1_DMA_IRQ_INTR)
#define RFLAN_DMA_TX2_IRQ               (XPAR_FABRIC_TX2_DMA_IRQ_INTR)
#define RFLAN_DMA_RX1_IRQ               (XPAR_FABRIC_RX1_DMA_IRQ_INTR)
#define RFLAN_DMA_RX2_IRQ               (XPAR_FABRIC_RX2_DMA_IRQ_INTR)


#define RFLAN_DMA_BUF_ADDR              (0x3f000000)
#define RFLAN_DMA_TX1_BUF_ADDR          (RFLAN_DMA_BUF_ADDR)
#define RFLAN_DMA_TX1_BUF_SIZE          (0x400000)
#define RFLAN_DMA_TX2_BUF_ADDR          (RFLAN_DMA_TX1_BUF_ADDR + RFLAN_DMA_TX1_BUF_SIZE)
#define RFLAN_DMA_TX2_BUF_SIZE          (0x400000)
#define RFLAN_DMA_RX1_BUF_ADDR          (RFLAN_DMA_TX2_BUF_ADDR + RFLAN_DMA_TX2_BUF_SIZE)
#define RFLAN_DMA_RX1_BUF_SIZE          (0x400000)
#define RFLAN_DMA_RX2_BUF_ADDR          (RFLAN_DMA_RX1_BUF_ADDR + RFLAN_DMA_RX1_BUF_SIZE)
#define RFLAN_DMA_RX2_BUF_SIZE          (0x400000)


#endif
