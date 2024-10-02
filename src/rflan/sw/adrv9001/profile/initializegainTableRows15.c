#include "adrv9001.h"
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
adi_adrv9001_RxGainTableRow_t initialize_gainTableRows_15[] = { { 
		.rxFeGain = 248, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -2, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 248, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -2, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 242, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -10, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 242, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -10, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 230, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -22, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 230, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -22, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 210, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -20, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 210, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -20, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 175, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -16, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 175, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -16, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 112, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -10, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 112, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -10, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 0, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 0, 
		.phaseOffset = 0 }  };
