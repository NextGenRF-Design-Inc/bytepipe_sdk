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
#define APP_CLI_TX_TASK_PRIORITY        tskIDLE_PRIORITY + 1
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


#endif
