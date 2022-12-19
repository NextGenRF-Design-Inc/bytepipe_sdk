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
/* Device Driver Client: v67.1.1*/
/* Firmware: v0.21.0.8*/
/* Profile Generator: v0.50.11.0*/
/* Stream Generator Assembly: v0.7.9.0*/
/* Transceiver Evaluation Software: v0.21.0*/
/* ADRV9001 Plugin: v0.21.0*/

#ifndef _CALIBRATE_H_
#define _CALIBRATE_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "adi_adrv9001_auxdac.h"
#include "adi_adrv9001_cals.h"
#include "adi_adrv9001_spi.h"
#include "adi_adrv9001_ssi.h"
#include "adi_adrv9001_tx.h"
#include <stdlib.h>

#define AUTOGENERATOR_ERROR_HANDLER(errorCode) if (0 != errorCode) { return errorCode; }

#define READ_BINARY_FILE(filename, buffer) { FILE *ptr; ptr = fopen(filename, "rb"); fread(buffer, sizeof(buffer), sizeof(char), ptr); }

int calibrate(adi_adrv9001_Device_t * adrv9001Device_0);

#ifdef __cplusplus
}
#endif

#endif
