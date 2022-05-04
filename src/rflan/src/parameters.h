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


#define FF_LOGICAL_DRIVE_PATH           ("1:/")
#define FF_FILENAME_MAX_LEN             (FF_LFN_BUF)
#define APP_LOG_FILENAME                ("1:/bytepipe_sdk_log.txt")

#define INTC_DEVICE_ID			            XPAR_SCUGIC_SINGLE_DEVICE_ID

#define APP_TASK_PRIORITY               tskIDLE_PRIORITY
#define APP_CLI_RX_TASK_PRIORITY        tskIDLE_PRIORITY
#define APP_CLI_TX_TASK_PRIORITY        tskIDLE_PRIORITY + 3
#define PHY_TASK_PRIORITY               tskIDLE_PRIORITY + 2

#define APP_TASK_STACK_SIZE             0x8000
#define PHY_TASK_STACK_SIZE             0x8000
#define APP_CLI_RX_STACK_SIZE           8192
#define APP_CLI_TX_STACK_SIZE           8192

#define APP_TASK_NAME                   "App"
#define APP_CLI_RX_TASK_NAME            "CliRx"
#define APP_CLI_TX_TASK_NAME            "CliTx"
#define PHY_TASK_NAME                   "Phy"

#define APP_CLI_RX_QUEUE_SIZE           2048
#define APP_CLI_TX_QUEUE_SIZE           32768
#define PHY_QUEUE_SIZE                  64

#define APP_CLI_UART_DEVICE_ID          XPAR_PSU_UART_0_DEVICE_ID
#define APP_CLI_UART_INTR_ID            XPAR_XUARTPS_0_INTR
#define APP_CLI_PRINT_BUF_SIZE          2048
#define APP_CLI_CMD_BUF_SIZE            2048
#define APP_CLI_CMD_LIST_SIZE           100
#define APP_CLI_HISTORY_BUF_SIZE        256

#define GPIO_DEVICE_ID                  (XPAR_PSU_GPIO_0_DEVICE_ID)
#define GPIO_OFFSET                     (78)

#define GTR0_REFCLK_FREQ_HZ             (52000000)
#define GTR1_REFCLK_FREQ_HZ             (125000000)
#define GTR2_REFCLK_FREQ_HZ             (27000000)
#define GTR3_REFCLK_FREQ_HZ             (300000000)

#define GTR0_REFCLK_VERSACLK5_PORT      (2)
#define GTR1_REFCLK_VERSACLK5_PORT      (3)
#define GTR2_REFCLK_VERSACLK5_PORT      (1)
#define GTR3_REFCLK_VERSACLK5_PORT      (0)


#define SYSMON_DEVICE_ID                XPAR_XSYSMONPSU_0_DEVICE_ID

#define ADRV9001_SPI_DEVICE_ID          (XPAR_PSU_SPI_0_DEVICE_ID)
#define ADRV9001_SPI_CS                 (0)

#define ADRV9001_TEST_MODE_PATTERN      (0xABCDABCD)

#define AXI_ADRV9001_CTRL_BASE_ADDR     XPAR_ADRV9002_0_BASEADDR

#define AXI_ADRV9001_CTRL_REG_OFFSET    (0x00000000)
#define AXI_ADRV9001_CTRL_TX1EN_MASK    (0x00000001)
#define AXI_ADRV9001_CTRL_TX2EN_MASK    (0x00000002)
#define AXI_ADRV9001_CTRL_RX1EN_MASK    (0x00000004)
#define AXI_ADRV9001_CTRL_RX2EN_MASK    (0x00000008)
#define AXI_ADRV9001_CTRL_TX1RST_MASK   (0x00000010)
#define AXI_ADRV9001_CTRL_TX2RST_MASK   (0x00000020)
#define AXI_ADRV9001_CTRL_RX1RST_MASK   (0x00000040)
#define AXI_ADRV9001_CTRL_RX2RST_MASK   (0x00000080)
#define AXI_ADRV9001_CTRL_RSTN_MASK     (0x00000100)

#define PHY_STATUS_OFFSET               (-2000)
#define PIB_STATUS_OFFSET               (-3000)

#define PHY_PROFILE_NAME_MAX_LEN        (64)
#define PHY_PROFILE_NAME_LIST_DELIMITER (",")
#define PHY_PROFILE_MAX_CNT             (4)
#define PHY_RX_CNT                      (2)
#define PHY_TX_CNT                      (2)
#define PHY_LO_CNT                      (2)

#define PHY_STREAM_DMA_TX1_BASEADDR     (XPAR_TX1_DMA_BASEADDR)
#define PHY_STREAM_DMA_TX2_BASEADDR     (XPAR_TX2_DMA_BASEADDR)
#define PHY_STREAM_DMA_RX1_BASEADDR     (XPAR_RX1_DMA_BASEADDR)
#define PHY_STREAM_DMA_RX2_BASEADDR     (XPAR_RX2_DMA_BASEADDR)

#define PHY_STREAM_DMA_TX1_IRQ          (XPAR_FABRIC_TX1_DMA_IRQ_INTR)
#define PHY_STREAM_DMA_TX2_IRQ          (XPAR_FABRIC_TX2_DMA_IRQ_INTR)
#define PHY_STREAM_DMA_RX1_IRQ          (XPAR_FABRIC_RX1_DMA_IRQ_INTR)
#define PHY_STREAM_DMA_RX2_IRQ          (XPAR_FABRIC_RX2_DMA_IRQ_INTR)

#define PHY_STREAM_DMA_CONSTANT_LENGTH        (16)
#define PHY_STREAM_DMA_RAMP_LENGTH            (65536)

#define PHY_STREAM_DMA_BUF_ADDR               (0x40000000)      // Upper half of DDR. Reserved in linker script
#define PHY_STREAM_DMA_TX1_BUF_ADDR           (PHY_STREAM_DMA_BUF_ADDR)
#define PHY_STREAM_DMA_TX1_BUF_SIZE           (0x400000)
#define PHY_STREAM_DMA_TX2_BUF_ADDR           (PHY_STREAM_DMA_TX1_BUF_ADDR + PHY_STREAM_DMA_TX1_BUF_SIZE)
#define PHY_STREAM_DMA_TX2_BUF_SIZE           (0x400000)
#define PHY_STREAM_DMA_RX1_BUF_ADDR           (PHY_STREAM_DMA_TX2_BUF_ADDR + PHY_STREAM_DMA_TX2_BUF_SIZE)
#define PHY_STREAM_DMA_RX1_BUF_SIZE           (0x10000000)
#define PHY_STREAM_DMA_RX2_BUF_ADDR           (PHY_STREAM_DMA_RX1_BUF_ADDR + PHY_STREAM_DMA_RX1_BUF_SIZE)
#define PHY_STREAM_DMA_RX2_BUF_SIZE           (0x10000000)

#endif
