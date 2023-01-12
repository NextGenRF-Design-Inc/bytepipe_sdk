#include "adrv9001.h"
/* This file contains code automatically generated by a machine. */
/* It has not been independently verified by any human. */
/* All code is provided as-is for example purposes only. */
/* The customer assumes all risks related to the use of this code. */


/* */
/* Silicon Revision: Presumed C0*/
/* */
/* Tx / Rx optimal carrier frequencies: 30 MHz to 6 GHz*/
/* External LO optimal frequencies: 60 MHz to 12 GHz*/
/* */
/* FPGA: v0.0.0*/
/* Device Driver API: v0.0.0*/
/* Device Driver Client: v68.0.6*/
/* Firmware: v0.21.0.9*/
/* Profile Generator: v0.51.3.0*/
/* Stream Generator Assembly: v0.7.9.0*/
/* Transceiver Evaluation Software: v0.22.0*/
/* ADRV9001 Plugin: v0.22.0*/

#ifndef _INITIALIZE_H_
#define _INITIALIZE_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "adi_adrv9001.h"
#include "adi_adrv9001_arm.h"
#include "adi_adrv9001_cals.h"
#include "adi_adrv9001_gpio.h"
#include "adi_adrv9001_mcs.h"
#include "adi_adrv9001_powermanagement.h"
#include "adi_adrv9001_powersavingandmonitormode.h"
#include "adi_adrv9001_radio.h"
#include "adi_adrv9001_rx.h"
#include "adi_adrv9001_ssi.h"
#include "adi_adrv9001_stream.h"
#include "adi_adrv9001_tx.h"
#include <stdlib.h>
extern adrv9001_params_t Adrv9001Params;
extern uint8_t initialize_binary_9[];
extern uint8_t initialize_binary_10[];
extern adi_adrv9001_RxGainTableRow_t initialize_gainTableRows_13[];
extern adi_adrv9001_RxGainTableRow_t initialize_gainTableRows_14[];
extern adi_adrv9001_RxGainTableRow_t initialize_gainTableRows_15[];
extern adi_adrv9001_RxGainTableRow_t initialize_gainTableRows_16[];
extern adi_adrv9001_TxAttenTableRow_t initialize_attenTableRows_17[];
extern adi_adrv9001_GpioCtrlInitCfg_t initialize_gpioCtrlInitCfg_25;

#define READ_BINARY_FILE(filename, buffer) { FILE *ptr; ptr = fopen(filename, "rb"); fread(buffer, sizeof(buffer), sizeof(char), ptr); }

int initialize(adi_adrv9001_Device_t * adrv9001Device_0);

#ifdef __cplusplus
}
#endif

#endif
