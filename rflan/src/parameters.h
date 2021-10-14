#ifndef _PARAMETERS_H_
#define _PARAMETERS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "xparameters.h"
#include "adi_adrv9001_gpio.h"

#define MAC_STATUS_OFFSET         (-1000)

#define ADRV9001_NUM_SUBCHANNELS  2 /* I - in-phase and Q - quadrature channels */
#define ADRV9001_NUM_CHAIN_CHANNELS 2 /* channels per RX/TX chain */
#define ADRV9001_NUM_CHANNELS   (ADRV9001_NUM_CHAIN_CHANNELS * ADRV9001_NUM_SUBCHANNELS)

/* BBP GPIO */
#define GPIO_DEVICE_ID                          XPAR_PSU_GPIO_0_DEVICE_ID
#define GPIO_OFFSET			                        (78)
#define ADRV9001_GPIO_TX2                       (49 + GPIO_OFFSET)
#define ADRV9001_GPIO_TX1                       (48 + GPIO_OFFSET)
#define ADRV9001_GPIO_RX2                       (47 + GPIO_OFFSET)
#define ADRV9001_GPIO_RX1                       (46 + GPIO_OFFSET)
#define ADRV9001_GPIO_RESET                     (45 + GPIO_OFFSET)

/* ADRV9001 GPIO */
#define ADRV9001_GPIO_RX1A_LNA_ENABLE_PIN       (ADI_ADRV9001_GPIO_ANALOG_06)
#define ADRV9001_GPIO_RX2A_LNA_ENABLE_PIN       (ADI_ADRV9001_GPIO_ANALOG_05)

/* SPI */
#define ADRV9001_SPI_CS				                  (0)
#define ADRV9001_SPI_DEVICE_ID			            XPAR_PSU_SPI_0_DEVICE_ID

/* Interrupt */
#define INTC_DEVICE_ID			                    XPAR_SCUGIC_SINGLE_DEVICE_ID

/* DMA Controller */
#define RX1_DMA_BASEADDR		                    XPAR_ADRV9001_RX1_DMA_BASEADDR
#define RX2_DMA_BASEADDR		                    XPAR_ADRV9001_RX2_DMA_BASEADDR
#define TX1_DMA_BASEADDR		                    XPAR_ADRV9001_TX1_DMA_BASEADDR
#define TX2_DMA_BASEADDR		                    XPAR_ADRV9001_TX2_DMA_BASEADDR

/* Memory */
#define DDR_MEM_BASEADDR		                    XPAR_DDR_MEM_BASEADDR
#define RX1_DMA_BUFADDR		                      (DDR_MEM_BASEADDR + 0x800000)
#define RX2_DMA_BUFADDR 		                    (DDR_MEM_BASEADDR + 0x900000)
#define TX1_DMA_BUFADDR		                      (DDR_MEM_BASEADDR + 0xA000000)
#define TX2_DMA_BUFADDR		                      (DDR_MEM_BASEADDR + 0xA100000)

#define APP_TASK_PRIORITY           tskIDLE_PRIORITY
#define APP_CLI_RX_TASK_PRIORITY    tskIDLE_PRIORITY
#define APP_CLI_TX_TASK_PRIORITY    tskIDLE_PRIORITY + 1
#define MAC_TASK_PRIORITY           tskIDLE_PRIORITY + 2

#define APP_TASK_STACK_SIZE       0x8000
#define MAC_TASK_STACK_SIZE       0x8000
#define APP_CLI_RX_STACK_SIZE     8192
#define APP_CLI_TX_STACK_SIZE     8192

#define APP_TASK_NAME           "App"
#define APP_CLI_RX_TASK_NAME    "CliRx"
#define APP_CLI_TX_TASK_NAME    "CliTx"

#define APP_CLI_RX_QUEUE_SIZE   2048
#define APP_CLI_TX_QUEUE_SIZE   32768

#define APP_CLI_UART_DEVICE_ID      XPAR_PSU_UART_0_DEVICE_ID
#define APP_CLI_UART_INTR_ID        XPAR_XUARTPS_0_INTR
#define APP_CLI_PRINT_BUF_SIZE      2048
#define APP_CLI_CMD_BUF_SIZE        2048
#define APP_CLI_CMD_LIST_SIZE       50
#define APP_CLI_HISTORY_BUF_SIZE    256


#endif
