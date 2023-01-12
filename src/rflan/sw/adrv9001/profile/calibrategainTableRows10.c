#include "adi_adrv9001_auxdac.h"
#include "adi_adrv9001_cals.h"
#include "adi_adrv9001_rx.h"
#include "adi_adrv9001_rx_gaincontrol.h"
#include "adi_adrv9001_spi.h"
#include "adi_adrv9001_ssi.h"
#include "adi_adrv9001_tx.h"
#include <stdlib.h>
adi_adrv9001_RxGainTableRow_t calibrate_gainTableRows_10[] = { {
		.rxFeGain = 0, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 0, 
		.phaseOffset = 0}, {
		.rxFeGain = 0, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 0, 
		.phaseOffset = 0}, {
		.rxFeGain = 0, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 0, 
		.phaseOffset = 0}, {
		.rxFeGain = 0, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 0, 
		.phaseOffset = 0}, {
		.rxFeGain = 0, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 0, 
		.phaseOffset = 0}, {
		.rxFeGain = 0, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 0, 
		.phaseOffset = 0}, {
		.rxFeGain = 0, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 0, 
		.phaseOffset = 0}, {
		.rxFeGain = 0, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 0, 
		.phaseOffset = 0}, {
		.rxFeGain = 0, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 0, 
		.phaseOffset = 0}, {
		.rxFeGain = 0, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 0, 
		.phaseOffset = 0}, {
		.rxFeGain = 0, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 0, 
		.phaseOffset = 0}, {
		.rxFeGain = 0, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 0, 
		.phaseOffset = 0}, {
		.rxFeGain = 0, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 0, 
		.phaseOffset = 0}, {
		.rxFeGain = 0, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 0, 
		.phaseOffset = 0}, {
		.rxFeGain = 0, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 0, 
		.phaseOffset = 0}, {
		.rxFeGain = 0, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 0, 
		.phaseOffset = 0}, {
		.rxFeGain = 0, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 0, 
		.phaseOffset = 0}, {
		.rxFeGain = 0, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 0, 
		.phaseOffset = 0}, {
		.rxFeGain = 0, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 0, 
		.phaseOffset = 0}, {
		.rxFeGain = 0, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 0, 
		.phaseOffset = 0}, {
		.rxFeGain = 0, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 0, 
		.phaseOffset = 0}, {
		.rxFeGain = 0, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 0, 
		.phaseOffset = 0}, {
		.rxFeGain = 0, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 0, 
		.phaseOffset = 0}, {
		.rxFeGain = 0, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 0, 
		.phaseOffset = 0}, {
		.rxFeGain = 0, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 0, 
		.phaseOffset = 0}, {
		.rxFeGain = 0, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 0, 
		.phaseOffset = 0}, {
		.rxFeGain = 0, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 0, 
		.phaseOffset = 0}, {
		.rxFeGain = 0, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 0, 
		.phaseOffset = 0}, {
		.rxFeGain = 0, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 0, 
		.phaseOffset = 0}, {
		.rxFeGain = 0, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 0, 
		.phaseOffset = 0}, {
		.rxFeGain = 0, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 0, 
		.phaseOffset = 0}, {
		.rxFeGain = 0, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 0, 
		.phaseOffset = 0}, {
		.rxFeGain = 0, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 0, 
		.phaseOffset = 0}, {
		.rxFeGain = 0, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 0, 
		.phaseOffset = 0}, {
		.rxFeGain = 0, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 0, 
		.phaseOffset = 0}, {
		.rxFeGain = 0, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 0, 
		.phaseOffset = 0}, {
		.rxFeGain = 0, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 0, 
		.phaseOffset = 0}, {
		.rxFeGain = 0, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 0, 
		.phaseOffset = 0}, {
		.rxFeGain = 0, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 0, 
		.phaseOffset = 0}, {
		.rxFeGain = 0, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 0, 
		.phaseOffset = 0}, {
		.rxFeGain = 0, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 0, 
		.phaseOffset = 0}, {
		.rxFeGain = 0, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 0, 
		.phaseOffset = 0}, {
		.rxFeGain = 0, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 0, 
		.phaseOffset = 0}, {
		.rxFeGain = 0, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 0, 
		.phaseOffset = 0}, {
		.rxFeGain = 0, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 0, 
		.phaseOffset = 0}, {
		.rxFeGain = 0, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 0, 
		.phaseOffset = 0}, {
		.rxFeGain = 0, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 0, 
		.phaseOffset = 0}, {
		.rxFeGain = 0, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 0, 
		.phaseOffset = 0}, {
		.rxFeGain = 0, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 0, 
		.phaseOffset = 0}, {
		.rxFeGain = 0, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 0, 
		.phaseOffset = 0}, {
		.rxFeGain = 0, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 0, 
		.phaseOffset = 0}, {
		.rxFeGain = 0, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 0, 
		.phaseOffset = 0}, {
		.rxFeGain = 0, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 0, 
		.phaseOffset = 0}, {
		.rxFeGain = 0, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 0, 
		.phaseOffset = 0}, {
		.rxFeGain = 0, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 0, 
		.phaseOffset = 0}, {
		.rxFeGain = 0, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 0, 
		.phaseOffset = 0}, {
		.rxFeGain = 0, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 0, 
		.phaseOffset = 0}, {
		.rxFeGain = 0, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 0, 
		.phaseOffset = 0}, {
		.rxFeGain = 0, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 0, 
		.phaseOffset = 0}, {
		.rxFeGain = 0, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 0, 
		.phaseOffset = 0}, {
		.rxFeGain = 0, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 0, 
		.phaseOffset = 0}, {
		.rxFeGain = 0, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 0, 
		.phaseOffset = 0}, {
		.rxFeGain = 0, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 0, 
		.phaseOffset = 0}, {
		.rxFeGain = 0, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 0, 
		.phaseOffset = 0}, {
		.rxFeGain = 0, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 0, 
		.phaseOffset = 0}, {
		.rxFeGain = 0, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 0, 
		.phaseOffset = 0}, {
		.rxFeGain = 0, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 0, 
		.phaseOffset = 0}, {
		.rxFeGain = 0, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 0, 
		.phaseOffset = 0}, {
		.rxFeGain = 0, 
		.extControl = 0, 
		.adcTiaGain = 0, 
		.digGain = 0, 
		.phaseOffset = 0}  };
