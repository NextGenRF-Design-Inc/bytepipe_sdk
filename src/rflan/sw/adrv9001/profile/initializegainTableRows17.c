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
adi_adrv9001_RxGainTableRow_t initialize_gainTableRows_17[] = { { 
		.rxFeGain = 251, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 13, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 251, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 23, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 250, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -4, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 250, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 6, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 250, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 16, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 249, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -7, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 249, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 3, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 248, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -12, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 248, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -2, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 247, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -17, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 247, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -7, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 246, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -16, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 246, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -6, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 245, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -15, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 245, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -5, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 244, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -10, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 243, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -16, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 242, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -19, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 242, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -10, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 241, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -13, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 240, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -14, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 239, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -16, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 238, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -16, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 237, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -16, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 236, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -15, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 234, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -22, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 233, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -20, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 232, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -17, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 230, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -22, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 229, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -19, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 227, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -21, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 226, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -17, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 224, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -18, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 222, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -18, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 220, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -18, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 218, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -17, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 215, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -21, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 213, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -19, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 210, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -21, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 208, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -18, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 205, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -18, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 202, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -18, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 199, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -17, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 195, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -19, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 192, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -17, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 188, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -16, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 184, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -16, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 180, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -15, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 175, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -16, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 170, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -15, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 165, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -15, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 160, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -14, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 154, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -14, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 148, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -13, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 142, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -12, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 135, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -12, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 128, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -11, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 120, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -10, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 112, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -10, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 104, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -8, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 94, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -9, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 85, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -7, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 75, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -6, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 64, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -5, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 53, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -4, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 41, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -3, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 28, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -2, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 14, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = -1, 
		.phaseOffset = 0 }, { 
		.rxFeGain = 0, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 0, 
		.phaseOffset = 0 }  };
