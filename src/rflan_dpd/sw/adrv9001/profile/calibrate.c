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

#include "calibrate.h"

int calibrate(adi_adrv9001_Device_t * adrv9001Device_0)
{
	int32_t error_code = 0;

	AUTOGENERATOR_ERROR_HANDLER(error_code);
	adi_adrv9001_InitCals_t initCals_1 = {
		.sysInitCalMask = (adi_adrv9001_InitCalibrations_e) 0, 
		.chanInitCalMask = { (adi_adrv9001_InitCalibrations_e) 0, ADI_ADRV9001_INIT_CAL_TX_QEC | ADI_ADRV9001_INIT_CAL_TX_LO_LEAKAGE | ADI_ADRV9001_INIT_CAL_TX_LB_PD | ADI_ADRV9001_INIT_CAL_TX_BBAF | ADI_ADRV9001_INIT_CAL_TX_BBAF_GD | ADI_ADRV9001_INIT_CAL_TX_ATTEN_DELAY | ADI_ADRV9001_INIT_CAL_TX_DAC | ADI_ADRV9001_INIT_CAL_TX_PATH_DELAY | ADI_ADRV9001_INIT_CAL_RX_HPADC_FLASH | ADI_ADRV9001_INIT_CAL_RX_LPADC | ADI_ADRV9001_INIT_CAL_RX_TIA_CUTOFF | ADI_ADRV9001_INIT_CAL_RX_GROUP_DELAY | ADI_ADRV9001_INIT_CAL_RX_QEC_FIC | ADI_ADRV9001_INIT_CAL_RX_RF_DC_OFFSET | ADI_ADRV9001_INIT_CAL_RX_GAIN_PATH_DELAY  }, 
		.calMode = ADI_ADRV9001_INIT_CAL_MODE_ALL, 
		.force = false};
	uint8_t errorFlag_1 = 0;
	error_code = adi_adrv9001_cals_InitCals_Run(adrv9001Device_0, &initCals_1, 300000, &errorFlag_1);
	AUTOGENERATOR_ERROR_HANDLER(error_code);
	uint8_t readData_2 = 0;
	error_code = adi_adrv9001_spi_Byte_Read(adrv9001Device_0, 11, &readData_2);
	AUTOGENERATOR_ERROR_HANDLER(error_code);
	uint32_t internalPathDelays_ns_3[6] = { 0 };
	error_code = adi_adrv9001_cals_InternalPathDelay_Get(adrv9001Device_0, ADI_TX, ADI_CHANNEL_2, internalPathDelays_ns_3, 6);
	int32_t internalPathDelays_ns_3_indexer = 1;
	for (internalPathDelays_ns_3_indexer = 1; internalPathDelays_ns_3_indexer < 6; internalPathDelays_ns_3_indexer++)
	{
	}
	AUTOGENERATOR_ERROR_HANDLER(error_code);
	error_code = adi_adrv9001_Tx_DataPath_Loopback_Set(adrv9001Device_0, ADI_CHANNEL_2, false);
	AUTOGENERATOR_ERROR_HANDLER(error_code);
	error_code = adi_adrv9001_Ssi_Loopback_Set(adrv9001Device_0, ADI_CHANNEL_2, ADI_ADRV9001_SSI_TYPE_LVDS, false);
	AUTOGENERATOR_ERROR_HANDLER(error_code);
	AUTOGENERATOR_ERROR_HANDLER(error_code);

	return error_code;
}
