#ifndef _PARAMETERS_H_
#define _PARAMETERS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "xparameters.h"
#include "ff.h"


#define FF_LOGICAL_DRIVE_PATH           ("1:/")
#define FF_FILENAME_MAX_LEN             (FF_LFN_BUF)

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


#define DMA_BUF_ADDR                    (0x40000000)      // Upper half of DDR. Reserved in linker script
#define ADRV9001_TX1_DMA_BUF_ADDR       (DMA_BUF_ADDR)
#define ADRV9001_TX1_DMA_BUF_SIZE       (0x400000)
#define ADRV9001_TX2_DMA_BUF_ADDR       (ADRV9001_TX1_DMA_BUF_ADDR + ADRV9001_TX1_DMA_BUF_SIZE)
#define ADRV9001_TX2_DMA_BUF_SIZE       (0x400000)
#define ADRV9001_RX1_DMA_BUF_ADDR       (ADRV9001_TX2_DMA_BUF_ADDR + ADRV9001_TX2_DMA_BUF_SIZE)
#define ADRV9001_RX1_DMA_BUF_SIZE       (0x10000000)
#define ADRV9001_RX2_DMA_BUF_ADDR       (ADRV9001_RX1_DMA_BUF_ADDR + ADRV9001_RX1_DMA_BUF_SIZE)
#define ADRV9001_RX2_DMA_BUF_SIZE       (0x10000000)

#define GPIO_DEVICE_ID                  (XPAR_PSU_GPIO_0_DEVICE_ID)
#define GPIO_OFFSET                     (78)
#define ADRV9001_GPIO_RX2_EN            (4 + GPIO_OFFSET)
#define ADRV9001_GPIO_RX1_EN            (3 + GPIO_OFFSET)
#define ADRV9001_GPIO_TX2_EN            (2 + GPIO_OFFSET)
#define ADRV9001_GPIO_TX1_EN            (1 + GPIO_OFFSET)
#define ADRV9001_GPIO_RSTN              (0 + GPIO_OFFSET)
#define ADRV9001_SPI_DEVICE_ID          (XPAR_PSU_SPI_0_DEVICE_ID)
#define ADRV9001_SPI_CS                 (0)

#endif
