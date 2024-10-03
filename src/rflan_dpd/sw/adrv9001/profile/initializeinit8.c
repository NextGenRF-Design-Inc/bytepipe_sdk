#include "adrv9001.h"
#include "adi_adrv9001.h"
#include "adi_adrv9001_arm.h"
#include "adi_adrv9001_cals.h"
#include "adi_adrv9001_dpd.h"
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
adi_adrv9001_Init_t initialize_init_8 = { 
		.clocks = { 
		.deviceClock_kHz = 38400, 
		.clkPllVcoFreq_daHz = 884736000, 
		.clkPllHsDiv = ADI_ADRV9001_HSDIV_4, 
		.clkPllMode = ADI_ADRV9001_CLK_PLL_HP_MODE, 
		.clk1105Div = ADI_ADRV9001_INTERNAL_CLOCK_DIV_2, 
		.armClkDiv = ADI_ADRV9001_INTERNAL_CLOCK_DIV_6, 
		.armPowerSavingClkDiv = 1, 
		.refClockOutEnable = true, 
		.auxPllPower = ADI_ADRV9001_COMPONENT_POWER_LEVEL_HIGH, 
		.clkPllPower = ADI_ADRV9001_COMPONENT_POWER_LEVEL_HIGH, 
		.padRefClkDrv = 0, 
		.extLo1OutFreq_kHz = 0, 
		.extLo2OutFreq_kHz = 0, 
		.rfPll1LoMode = ADI_ADRV9001_INT_LO1 | ADI_ADRV9001_INT_LO2, 
		.rfPll2LoMode = ADI_ADRV9001_INT_LO1 | ADI_ADRV9001_INT_LO2, 
		.ext1LoType = ADI_ADRV9001_EXT_LO_DIFFERENTIAL, 
		.ext2LoType = ADI_ADRV9001_EXT_LO_DIFFERENTIAL, 
		.rx1RfInputSel = ADI_ADRV9001_RX_A, 
		.rx2RfInputSel = ADI_ADRV9001_RX_B, 
		.extLo1Divider = 2, 
		.extLo2Divider = 2, 
		.rfPllPhaseSyncMode = ADI_ADRV9001_RFPLLMCS_NOSYNC, 
		.rx1LoSelect = ADI_ADRV9001_LOSEL_LO1, 
		.rx2LoSelect = ADI_ADRV9001_LOSEL_LO2, 
		.tx1LoSelect = ADI_ADRV9001_LOSEL_LO1, 
		.tx2LoSelect = ADI_ADRV9001_LOSEL_LO2, 
		.rx1LoDivMode = ADI_ADRV9001_LO_DIV_MODE_LOW_POWER, 
		.rx2LoDivMode = ADI_ADRV9001_LO_DIV_MODE_LOW_POWER, 
		.tx1LoDivMode = ADI_ADRV9001_LO_DIV_MODE_LOW_POWER, 
		.tx2LoDivMode = ADI_ADRV9001_LO_DIV_MODE_LOW_POWER, 
		.loGen1Select = ADI_ADRV9001_LOGENPOWER_RFPLL_LDO, 
		.loGen2Select = ADI_ADRV9001_LOGENPOWER_RFPLL_LDO }
	, 
		.rx = { 
		.rxInitChannelMask = 640, 
		.rxChannelCfg = { { 
		.profile = { 
		.primarySigBandwidth_Hz = 12500, 
		.rxOutputRate_Hz = 0, 
		.rxInterfaceSampleRate_Hz = 0, 
		.rxOffsetLo_kHz = 0, 
		.rxNcoEnable = false, 
		.outputSignaling = ADI_ADRV9001_RX_IQ, 
		.filterOrder = 1, 
		.filterOrderLp = 1, 
		.hpAdcCorner = 0, 
		.lpAdcCorner = 0, 
		.adcClk_kHz = 0, 
		.rxCorner3dB_kHz = 0, 
		.rxCorner3dBLp_kHz = 0, 
		.tiaPower = ADI_ADRV9001_COMPONENT_POWER_LEVEL_HIGH, 
		.tiaPowerLp = ADI_ADRV9001_COMPONENT_POWER_LEVEL_HIGH, 
		.channelType = 0, 
		.adcType = ADI_ADRV9001_ADC_HP, 
		.lpAdcCalMode = ADI_ADRV9001_ADC_LOWPOWER_PERIODIC, 
		.gainTableType = ADI_ADRV9001_RX_GAIN_CORRECTION_TABLE, 
		.rxDpProfile = { 
		.rxNbDecTop = { 
		.scicBlk23En = 0, 
		.scicBlk23DivFactor = 0, 
		.scicBlk23LowRippleEn = 0, 
		.decBy2Blk35En = 0, 
		.decBy2Blk37En = 0, 
		.decBy2Blk39En = 0, 
		.decBy2Blk41En = 0, 
		.decBy2Blk43En = 0, 
		.decBy3Blk45En = 0, 
		.decBy2Blk47En = 0 }
					, 
		.rxWbDecTop = { 
		.decBy2Blk25En = 0, 
		.decBy2Blk27En = 0, 
		.decBy2Blk29En = 0, 
		.decBy2Blk31En = 0, 
		.decBy2Blk33En = 0, 
		.wbLpfBlk33p1En = 0 }
					, 
		.rxDecTop = { 
		.decBy3Blk15En = 0, 
		.decBy2Hb3Blk17p1En = 0, 
		.decBy2Hb4Blk17p2En = 0, 
		.decBy2Hb5Blk19p1En = 0, 
		.decBy2Hb6Blk19p2En = 0 }
					, 
		.rxSincHBTop = { 
		.sincGainMux = ADI_ADRV9001_RX_SINC_GAIN_MUX_6_DB, 
		.sincMux = ADI_ADRV9001_RX_SINC_MUX5_OUTPUT_ZERO, 
		.hbMux = ADI_ADRV9001_RX_HB_MUX_OUTPUT_HB1, 
		.isGainCompEnabled = 0, 
		.gainComp9GainI = { 16384, 16384, 16384, 16384, 16384, 16384  }, 
		.gainComp9GainQ = { 0, 0, 0, 0, 0, 0  } }
					, 
		.rxNbDem = { 
		.dpInFifo = { 
		.dpInFifoEn = 0, 
		.dpInFifoMode = ADI_ADRV9001_DP_IN_FIFO_MODE_DETECTING, 
		.dpInFifoTestDataSel = ADI_ADRV9001_DP_IN_FIFO_INPUT_DP_SEL }
						, 
		.rxNbNco = { 
		.rxNbNcoEn = 0, 
		.rxNbNcoConfig = { 
		.freq = 0, 
		.sampleFreq = 0, 
		.phase = 0, 
		.realOut = 0 }
							 }
						, 
		.rxWbNbCompPFir = { 
		.bankSel = ADI_ADRV9001_PFIR_BANK_A, 
		.rxWbNbCompPFirInMuxSel = ADI_ADRV9001_RP_FIR_IN_MUX_INT_IN, 
		.rxWbNbCompPFirEn = 0 }
						, 
		.resamp = { 
		.rxResampEn = 0, 
		.resampPhaseI = 0, 
		.resampPhaseQ = 0 }
						, 
		.gsOutMuxSel = ADI_ADRV9001_GS_OUT_MUX_BYPASS, 
		.rxOutSel = ADI_ADRV9001_RX_OUT_IQ_SEL, 
		.rxRoundMode = ADI_ADRV9001_RX_ROUNDMODE_IQ, 
		.dpArmSel = ADI_ADRV9001_DP_SEL }
					 }
				, 
		.lnaConfig = { 
		.externalLnaPresent = false, 
		.gpioSourceSel = ADI_ADRV9001_GPIO_ANALOG_PIN_NIBBLE_UNASSIGNED, 
		.externalLnaPinSel = ADI_ADRV9001_EXTERNAL_LNA_PIN_RX1_LOWER_RX2_UPPER, 
		.settlingDelay = 0, 
		.numberLnaGainSteps = 0, 
		.lnaGainSteps_mdB = { 0, 0, 0, 0  }, 
		.lnaDigitalGainDelay = 0, 
		.minGainIndex = 0, 
		.lnaType = ADI_ADRV9001_EXTERNAL_LNA_TYPE_SINGLE }
				, 
		.rxSsiConfig = { 
		.ssiType = ADI_ADRV9001_SSI_TYPE_DISABLE, 
		.ssiDataFormatSel = ADI_ADRV9001_SSI_FORMAT_2_BIT_SYMBOL_DATA, 
		.numLaneSel = ADI_ADRV9001_SSI_1_LANE, 
		.strobeType = ADI_ADRV9001_SSI_SHORT_STROBE, 
		.lsbFirst = 0, 
		.qFirst = 0, 
		.txRefClockPin = ADI_ADRV9001_SSI_TX_REF_CLOCK_PIN_DISABLED, 
		.lvdsIBitInversion = false, 
		.lvdsQBitInversion = false, 
		.lvdsStrobeBitInversion = false, 
		.lvdsUseLsbIn12bitMode = 0, 
		.lvdsRxClkInversionEn = false, 
		.cmosDdrPosClkEn = false, 
		.cmosClkInversionEn = false, 
		.ddrEn = false, 
		.rxMaskStrobeEn = false }
				 }
			 }, { 
		.profile = { 
		.primarySigBandwidth_Hz = 12500, 
		.rxOutputRate_Hz = 0, 
		.rxInterfaceSampleRate_Hz = 0, 
		.rxOffsetLo_kHz = 0, 
		.rxNcoEnable = false, 
		.outputSignaling = ADI_ADRV9001_RX_IQ, 
		.filterOrder = 1, 
		.filterOrderLp = 1, 
		.hpAdcCorner = 0, 
		.lpAdcCorner = 0, 
		.adcClk_kHz = 0, 
		.rxCorner3dB_kHz = 0, 
		.rxCorner3dBLp_kHz = 0, 
		.tiaPower = ADI_ADRV9001_COMPONENT_POWER_LEVEL_HIGH, 
		.tiaPowerLp = ADI_ADRV9001_COMPONENT_POWER_LEVEL_HIGH, 
		.channelType = 0, 
		.adcType = ADI_ADRV9001_ADC_HP, 
		.lpAdcCalMode = ADI_ADRV9001_ADC_LOWPOWER_PERIODIC, 
		.gainTableType = ADI_ADRV9001_RX_GAIN_CORRECTION_TABLE, 
		.rxDpProfile = { 
		.rxNbDecTop = { 
		.scicBlk23En = 0, 
		.scicBlk23DivFactor = 0, 
		.scicBlk23LowRippleEn = 0, 
		.decBy2Blk35En = 0, 
		.decBy2Blk37En = 0, 
		.decBy2Blk39En = 0, 
		.decBy2Blk41En = 0, 
		.decBy2Blk43En = 0, 
		.decBy3Blk45En = 0, 
		.decBy2Blk47En = 0 }
					, 
		.rxWbDecTop = { 
		.decBy2Blk25En = 0, 
		.decBy2Blk27En = 0, 
		.decBy2Blk29En = 0, 
		.decBy2Blk31En = 0, 
		.decBy2Blk33En = 0, 
		.wbLpfBlk33p1En = 0 }
					, 
		.rxDecTop = { 
		.decBy3Blk15En = 0, 
		.decBy2Hb3Blk17p1En = 0, 
		.decBy2Hb4Blk17p2En = 0, 
		.decBy2Hb5Blk19p1En = 0, 
		.decBy2Hb6Blk19p2En = 0 }
					, 
		.rxSincHBTop = { 
		.sincGainMux = ADI_ADRV9001_RX_SINC_GAIN_MUX_6_DB, 
		.sincMux = ADI_ADRV9001_RX_SINC_MUX5_OUTPUT_ZERO, 
		.hbMux = ADI_ADRV9001_RX_HB_MUX_OUTPUT_HB1, 
		.isGainCompEnabled = 0, 
		.gainComp9GainI = { 16384, 16384, 16384, 16384, 16384, 16384  }, 
		.gainComp9GainQ = { 0, 0, 0, 0, 0, 0  } }
					, 
		.rxNbDem = { 
		.dpInFifo = { 
		.dpInFifoEn = 0, 
		.dpInFifoMode = ADI_ADRV9001_DP_IN_FIFO_MODE_DETECTING, 
		.dpInFifoTestDataSel = ADI_ADRV9001_DP_IN_FIFO_INPUT_DP_SEL }
						, 
		.rxNbNco = { 
		.rxNbNcoEn = 0, 
		.rxNbNcoConfig = { 
		.freq = 0, 
		.sampleFreq = 0, 
		.phase = 0, 
		.realOut = 0 }
							 }
						, 
		.rxWbNbCompPFir = { 
		.bankSel = ADI_ADRV9001_PFIR_BANK_A, 
		.rxWbNbCompPFirInMuxSel = ADI_ADRV9001_RP_FIR_IN_MUX_INT_IN, 
		.rxWbNbCompPFirEn = 0 }
						, 
		.resamp = { 
		.rxResampEn = 0, 
		.resampPhaseI = 0, 
		.resampPhaseQ = 0 }
						, 
		.gsOutMuxSel = ADI_ADRV9001_GS_OUT_MUX_BYPASS, 
		.rxOutSel = ADI_ADRV9001_RX_OUT_IQ_SEL, 
		.rxRoundMode = ADI_ADRV9001_RX_ROUNDMODE_IQ, 
		.dpArmSel = ADI_ADRV9001_DP_SEL }
					 }
				, 
		.lnaConfig = { 
		.externalLnaPresent = false, 
		.gpioSourceSel = ADI_ADRV9001_GPIO_ANALOG_PIN_NIBBLE_UNASSIGNED, 
		.externalLnaPinSel = ADI_ADRV9001_EXTERNAL_LNA_PIN_RX1_LOWER_RX2_UPPER, 
		.settlingDelay = 0, 
		.numberLnaGainSteps = 0, 
		.lnaGainSteps_mdB = { 0, 0, 0, 0  }, 
		.lnaDigitalGainDelay = 0, 
		.minGainIndex = 0, 
		.lnaType = ADI_ADRV9001_EXTERNAL_LNA_TYPE_SINGLE }
				, 
		.rxSsiConfig = { 
		.ssiType = ADI_ADRV9001_SSI_TYPE_DISABLE, 
		.ssiDataFormatSel = ADI_ADRV9001_SSI_FORMAT_2_BIT_SYMBOL_DATA, 
		.numLaneSel = ADI_ADRV9001_SSI_1_LANE, 
		.strobeType = ADI_ADRV9001_SSI_SHORT_STROBE, 
		.lsbFirst = 0, 
		.qFirst = 0, 
		.txRefClockPin = ADI_ADRV9001_SSI_TX_REF_CLOCK_PIN_DISABLED, 
		.lvdsIBitInversion = false, 
		.lvdsQBitInversion = false, 
		.lvdsStrobeBitInversion = false, 
		.lvdsUseLsbIn12bitMode = 0, 
		.lvdsRxClkInversionEn = false, 
		.cmosDdrPosClkEn = false, 
		.cmosClkInversionEn = false, 
		.ddrEn = false, 
		.rxMaskStrobeEn = false }
				 }
			 }, { 
		.profile = { 
		.primarySigBandwidth_Hz = 12500, 
		.rxOutputRate_Hz = 0, 
		.rxInterfaceSampleRate_Hz = 0, 
		.rxOffsetLo_kHz = 0, 
		.rxNcoEnable = false, 
		.outputSignaling = ADI_ADRV9001_RX_IQ, 
		.filterOrder = 1, 
		.filterOrderLp = 1, 
		.hpAdcCorner = 0, 
		.lpAdcCorner = 0, 
		.adcClk_kHz = 0, 
		.rxCorner3dB_kHz = 0, 
		.rxCorner3dBLp_kHz = 0, 
		.tiaPower = ADI_ADRV9001_COMPONENT_POWER_LEVEL_HIGH, 
		.tiaPowerLp = ADI_ADRV9001_COMPONENT_POWER_LEVEL_HIGH, 
		.channelType = 0, 
		.adcType = ADI_ADRV9001_ADC_HP, 
		.lpAdcCalMode = ADI_ADRV9001_ADC_LOWPOWER_PERIODIC, 
		.gainTableType = ADI_ADRV9001_RX_GAIN_CORRECTION_TABLE, 
		.rxDpProfile = { 
		.rxNbDecTop = { 
		.scicBlk23En = 0, 
		.scicBlk23DivFactor = 0, 
		.scicBlk23LowRippleEn = 0, 
		.decBy2Blk35En = 0, 
		.decBy2Blk37En = 0, 
		.decBy2Blk39En = 0, 
		.decBy2Blk41En = 0, 
		.decBy2Blk43En = 0, 
		.decBy3Blk45En = 0, 
		.decBy2Blk47En = 0 }
					, 
		.rxWbDecTop = { 
		.decBy2Blk25En = 0, 
		.decBy2Blk27En = 0, 
		.decBy2Blk29En = 0, 
		.decBy2Blk31En = 0, 
		.decBy2Blk33En = 0, 
		.wbLpfBlk33p1En = 0 }
					, 
		.rxDecTop = { 
		.decBy3Blk15En = 0, 
		.decBy2Hb3Blk17p1En = 0, 
		.decBy2Hb4Blk17p2En = 0, 
		.decBy2Hb5Blk19p1En = 0, 
		.decBy2Hb6Blk19p2En = 0 }
					, 
		.rxSincHBTop = { 
		.sincGainMux = ADI_ADRV9001_RX_SINC_GAIN_MUX_6_DB, 
		.sincMux = ADI_ADRV9001_RX_SINC_MUX5_OUTPUT_ZERO, 
		.hbMux = ADI_ADRV9001_RX_HB_MUX_OUTPUT_HB1, 
		.isGainCompEnabled = 0, 
		.gainComp9GainI = { 16384, 16384, 16384, 16384, 16384, 16384  }, 
		.gainComp9GainQ = { 0, 0, 0, 0, 0, 0  } }
					, 
		.rxNbDem = { 
		.dpInFifo = { 
		.dpInFifoEn = 0, 
		.dpInFifoMode = ADI_ADRV9001_DP_IN_FIFO_MODE_DETECTING, 
		.dpInFifoTestDataSel = ADI_ADRV9001_DP_IN_FIFO_INPUT_DP_SEL }
						, 
		.rxNbNco = { 
		.rxNbNcoEn = 0, 
		.rxNbNcoConfig = { 
		.freq = 0, 
		.sampleFreq = 0, 
		.phase = 0, 
		.realOut = 0 }
							 }
						, 
		.rxWbNbCompPFir = { 
		.bankSel = ADI_ADRV9001_PFIR_BANK_A, 
		.rxWbNbCompPFirInMuxSel = ADI_ADRV9001_RP_FIR_IN_MUX_INT_IN, 
		.rxWbNbCompPFirEn = 0 }
						, 
		.resamp = { 
		.rxResampEn = 0, 
		.resampPhaseI = 0, 
		.resampPhaseQ = 0 }
						, 
		.gsOutMuxSel = ADI_ADRV9001_GS_OUT_MUX_BYPASS, 
		.rxOutSel = ADI_ADRV9001_RX_OUT_IQ_SEL, 
		.rxRoundMode = ADI_ADRV9001_RX_ROUNDMODE_IQ, 
		.dpArmSel = ADI_ADRV9001_DP_SEL }
					 }
				, 
		.lnaConfig = { 
		.externalLnaPresent = false, 
		.gpioSourceSel = ADI_ADRV9001_GPIO_ANALOG_PIN_NIBBLE_UNASSIGNED, 
		.externalLnaPinSel = ADI_ADRV9001_EXTERNAL_LNA_PIN_RX1_LOWER_RX2_UPPER, 
		.settlingDelay = 0, 
		.numberLnaGainSteps = 0, 
		.lnaGainSteps_mdB = { 0, 0, 0, 0  }, 
		.lnaDigitalGainDelay = 0, 
		.minGainIndex = 0, 
		.lnaType = ADI_ADRV9001_EXTERNAL_LNA_TYPE_SINGLE }
				, 
		.rxSsiConfig = { 
		.ssiType = ADI_ADRV9001_SSI_TYPE_DISABLE, 
		.ssiDataFormatSel = ADI_ADRV9001_SSI_FORMAT_2_BIT_SYMBOL_DATA, 
		.numLaneSel = ADI_ADRV9001_SSI_1_LANE, 
		.strobeType = ADI_ADRV9001_SSI_SHORT_STROBE, 
		.lsbFirst = 0, 
		.qFirst = 0, 
		.txRefClockPin = ADI_ADRV9001_SSI_TX_REF_CLOCK_PIN_DISABLED, 
		.lvdsIBitInversion = false, 
		.lvdsQBitInversion = false, 
		.lvdsStrobeBitInversion = false, 
		.lvdsUseLsbIn12bitMode = 0, 
		.lvdsRxClkInversionEn = false, 
		.cmosDdrPosClkEn = false, 
		.cmosClkInversionEn = false, 
		.ddrEn = false, 
		.rxMaskStrobeEn = false }
				 }
			 }, { 
		.profile = { 
		.primarySigBandwidth_Hz = 12500, 
		.rxOutputRate_Hz = 0, 
		.rxInterfaceSampleRate_Hz = 0, 
		.rxOffsetLo_kHz = 0, 
		.rxNcoEnable = false, 
		.outputSignaling = ADI_ADRV9001_RX_IQ, 
		.filterOrder = 1, 
		.filterOrderLp = 1, 
		.hpAdcCorner = 0, 
		.lpAdcCorner = 0, 
		.adcClk_kHz = 0, 
		.rxCorner3dB_kHz = 0, 
		.rxCorner3dBLp_kHz = 0, 
		.tiaPower = ADI_ADRV9001_COMPONENT_POWER_LEVEL_HIGH, 
		.tiaPowerLp = ADI_ADRV9001_COMPONENT_POWER_LEVEL_HIGH, 
		.channelType = 0, 
		.adcType = ADI_ADRV9001_ADC_HP, 
		.lpAdcCalMode = ADI_ADRV9001_ADC_LOWPOWER_PERIODIC, 
		.gainTableType = ADI_ADRV9001_RX_GAIN_CORRECTION_TABLE, 
		.rxDpProfile = { 
		.rxNbDecTop = { 
		.scicBlk23En = 0, 
		.scicBlk23DivFactor = 0, 
		.scicBlk23LowRippleEn = 0, 
		.decBy2Blk35En = 0, 
		.decBy2Blk37En = 0, 
		.decBy2Blk39En = 0, 
		.decBy2Blk41En = 0, 
		.decBy2Blk43En = 0, 
		.decBy3Blk45En = 0, 
		.decBy2Blk47En = 0 }
					, 
		.rxWbDecTop = { 
		.decBy2Blk25En = 0, 
		.decBy2Blk27En = 0, 
		.decBy2Blk29En = 0, 
		.decBy2Blk31En = 0, 
		.decBy2Blk33En = 0, 
		.wbLpfBlk33p1En = 0 }
					, 
		.rxDecTop = { 
		.decBy3Blk15En = 0, 
		.decBy2Hb3Blk17p1En = 0, 
		.decBy2Hb4Blk17p2En = 0, 
		.decBy2Hb5Blk19p1En = 0, 
		.decBy2Hb6Blk19p2En = 0 }
					, 
		.rxSincHBTop = { 
		.sincGainMux = ADI_ADRV9001_RX_SINC_GAIN_MUX_6_DB, 
		.sincMux = ADI_ADRV9001_RX_SINC_MUX5_OUTPUT_ZERO, 
		.hbMux = ADI_ADRV9001_RX_HB_MUX_OUTPUT_HB1, 
		.isGainCompEnabled = 0, 
		.gainComp9GainI = { 16384, 16384, 16384, 16384, 16384, 16384  }, 
		.gainComp9GainQ = { 0, 0, 0, 0, 0, 0  } }
					, 
		.rxNbDem = { 
		.dpInFifo = { 
		.dpInFifoEn = 0, 
		.dpInFifoMode = ADI_ADRV9001_DP_IN_FIFO_MODE_DETECTING, 
		.dpInFifoTestDataSel = ADI_ADRV9001_DP_IN_FIFO_INPUT_DP_SEL }
						, 
		.rxNbNco = { 
		.rxNbNcoEn = 0, 
		.rxNbNcoConfig = { 
		.freq = 0, 
		.sampleFreq = 0, 
		.phase = 0, 
		.realOut = 0 }
							 }
						, 
		.rxWbNbCompPFir = { 
		.bankSel = ADI_ADRV9001_PFIR_BANK_A, 
		.rxWbNbCompPFirInMuxSel = ADI_ADRV9001_RP_FIR_IN_MUX_INT_IN, 
		.rxWbNbCompPFirEn = 0 }
						, 
		.resamp = { 
		.rxResampEn = 0, 
		.resampPhaseI = 0, 
		.resampPhaseQ = 0 }
						, 
		.gsOutMuxSel = ADI_ADRV9001_GS_OUT_MUX_BYPASS, 
		.rxOutSel = ADI_ADRV9001_RX_OUT_IQ_SEL, 
		.rxRoundMode = ADI_ADRV9001_RX_ROUNDMODE_IQ, 
		.dpArmSel = ADI_ADRV9001_DP_SEL }
					 }
				, 
		.lnaConfig = { 
		.externalLnaPresent = false, 
		.gpioSourceSel = ADI_ADRV9001_GPIO_ANALOG_PIN_NIBBLE_UNASSIGNED, 
		.externalLnaPinSel = ADI_ADRV9001_EXTERNAL_LNA_PIN_RX1_LOWER_RX2_UPPER, 
		.settlingDelay = 0, 
		.numberLnaGainSteps = 0, 
		.lnaGainSteps_mdB = { 0, 0, 0, 0  }, 
		.lnaDigitalGainDelay = 0, 
		.minGainIndex = 0, 
		.lnaType = ADI_ADRV9001_EXTERNAL_LNA_TYPE_SINGLE }
				, 
		.rxSsiConfig = { 
		.ssiType = ADI_ADRV9001_SSI_TYPE_DISABLE, 
		.ssiDataFormatSel = ADI_ADRV9001_SSI_FORMAT_2_BIT_SYMBOL_DATA, 
		.numLaneSel = ADI_ADRV9001_SSI_1_LANE, 
		.strobeType = ADI_ADRV9001_SSI_SHORT_STROBE, 
		.lsbFirst = 0, 
		.qFirst = 0, 
		.txRefClockPin = ADI_ADRV9001_SSI_TX_REF_CLOCK_PIN_DISABLED, 
		.lvdsIBitInversion = false, 
		.lvdsQBitInversion = false, 
		.lvdsStrobeBitInversion = false, 
		.lvdsUseLsbIn12bitMode = 0, 
		.lvdsRxClkInversionEn = false, 
		.cmosDdrPosClkEn = false, 
		.cmosClkInversionEn = false, 
		.ddrEn = false, 
		.rxMaskStrobeEn = false }
				 }
			 }, { 
		.profile = { 
		.primarySigBandwidth_Hz = 12500, 
		.rxOutputRate_Hz = 0, 
		.rxInterfaceSampleRate_Hz = 0, 
		.rxOffsetLo_kHz = 0, 
		.rxNcoEnable = false, 
		.outputSignaling = ADI_ADRV9001_RX_IQ, 
		.filterOrder = 1, 
		.filterOrderLp = 1, 
		.hpAdcCorner = 0, 
		.lpAdcCorner = 0, 
		.adcClk_kHz = 0, 
		.rxCorner3dB_kHz = 0, 
		.rxCorner3dBLp_kHz = 0, 
		.tiaPower = ADI_ADRV9001_COMPONENT_POWER_LEVEL_HIGH, 
		.tiaPowerLp = ADI_ADRV9001_COMPONENT_POWER_LEVEL_HIGH, 
		.channelType = 0, 
		.adcType = ADI_ADRV9001_ADC_HP, 
		.lpAdcCalMode = ADI_ADRV9001_ADC_LOWPOWER_PERIODIC, 
		.gainTableType = ADI_ADRV9001_RX_GAIN_CORRECTION_TABLE, 
		.rxDpProfile = { 
		.rxNbDecTop = { 
		.scicBlk23En = 0, 
		.scicBlk23DivFactor = 0, 
		.scicBlk23LowRippleEn = 0, 
		.decBy2Blk35En = 0, 
		.decBy2Blk37En = 0, 
		.decBy2Blk39En = 0, 
		.decBy2Blk41En = 0, 
		.decBy2Blk43En = 0, 
		.decBy3Blk45En = 0, 
		.decBy2Blk47En = 0 }
					, 
		.rxWbDecTop = { 
		.decBy2Blk25En = 0, 
		.decBy2Blk27En = 0, 
		.decBy2Blk29En = 0, 
		.decBy2Blk31En = 0, 
		.decBy2Blk33En = 0, 
		.wbLpfBlk33p1En = 0 }
					, 
		.rxDecTop = { 
		.decBy3Blk15En = 0, 
		.decBy2Hb3Blk17p1En = 0, 
		.decBy2Hb4Blk17p2En = 0, 
		.decBy2Hb5Blk19p1En = 0, 
		.decBy2Hb6Blk19p2En = 0 }
					, 
		.rxSincHBTop = { 
		.sincGainMux = ADI_ADRV9001_RX_SINC_GAIN_MUX_6_DB, 
		.sincMux = ADI_ADRV9001_RX_SINC_MUX5_OUTPUT_ZERO, 
		.hbMux = ADI_ADRV9001_RX_HB_MUX_OUTPUT_HB1, 
		.isGainCompEnabled = 0, 
		.gainComp9GainI = { 16384, 16384, 16384, 16384, 16384, 16384  }, 
		.gainComp9GainQ = { 0, 0, 0, 0, 0, 0  } }
					, 
		.rxNbDem = { 
		.dpInFifo = { 
		.dpInFifoEn = 0, 
		.dpInFifoMode = ADI_ADRV9001_DP_IN_FIFO_MODE_DETECTING, 
		.dpInFifoTestDataSel = ADI_ADRV9001_DP_IN_FIFO_INPUT_DP_SEL }
						, 
		.rxNbNco = { 
		.rxNbNcoEn = 0, 
		.rxNbNcoConfig = { 
		.freq = 0, 
		.sampleFreq = 0, 
		.phase = 0, 
		.realOut = 0 }
							 }
						, 
		.rxWbNbCompPFir = { 
		.bankSel = ADI_ADRV9001_PFIR_BANK_A, 
		.rxWbNbCompPFirInMuxSel = ADI_ADRV9001_RP_FIR_IN_MUX_INT_IN, 
		.rxWbNbCompPFirEn = 0 }
						, 
		.resamp = { 
		.rxResampEn = 0, 
		.resampPhaseI = 0, 
		.resampPhaseQ = 0 }
						, 
		.gsOutMuxSel = ADI_ADRV9001_GS_OUT_MUX_BYPASS, 
		.rxOutSel = ADI_ADRV9001_RX_OUT_IQ_SEL, 
		.rxRoundMode = ADI_ADRV9001_RX_ROUNDMODE_IQ, 
		.dpArmSel = ADI_ADRV9001_DP_SEL }
					 }
				, 
		.lnaConfig = { 
		.externalLnaPresent = false, 
		.gpioSourceSel = ADI_ADRV9001_GPIO_ANALOG_PIN_NIBBLE_UNASSIGNED, 
		.externalLnaPinSel = ADI_ADRV9001_EXTERNAL_LNA_PIN_RX1_LOWER_RX2_UPPER, 
		.settlingDelay = 0, 
		.numberLnaGainSteps = 0, 
		.lnaGainSteps_mdB = { 0, 0, 0, 0  }, 
		.lnaDigitalGainDelay = 0, 
		.minGainIndex = 0, 
		.lnaType = ADI_ADRV9001_EXTERNAL_LNA_TYPE_SINGLE }
				, 
		.rxSsiConfig = { 
		.ssiType = ADI_ADRV9001_SSI_TYPE_DISABLE, 
		.ssiDataFormatSel = ADI_ADRV9001_SSI_FORMAT_2_BIT_SYMBOL_DATA, 
		.numLaneSel = ADI_ADRV9001_SSI_1_LANE, 
		.strobeType = ADI_ADRV9001_SSI_SHORT_STROBE, 
		.lsbFirst = 0, 
		.qFirst = 0, 
		.txRefClockPin = ADI_ADRV9001_SSI_TX_REF_CLOCK_PIN_DISABLED, 
		.lvdsIBitInversion = false, 
		.lvdsQBitInversion = false, 
		.lvdsStrobeBitInversion = false, 
		.lvdsUseLsbIn12bitMode = 0, 
		.lvdsRxClkInversionEn = false, 
		.cmosDdrPosClkEn = false, 
		.cmosClkInversionEn = false, 
		.ddrEn = false, 
		.rxMaskStrobeEn = false }
				 }
			 }, { 
		.profile = { 
		.primarySigBandwidth_Hz = 9000000, 
		.rxOutputRate_Hz = 15360000, 
		.rxInterfaceSampleRate_Hz = 15360000, 
		.rxOffsetLo_kHz = 0, 
		.rxNcoEnable = false, 
		.outputSignaling = ADI_ADRV9001_RX_IQ, 
		.filterOrder = 1, 
		.filterOrderLp = 1, 
		.hpAdcCorner = 50000000, 
		.lpAdcCorner = 0, 
		.adcClk_kHz = 2211840, 
		.rxCorner3dB_kHz = 100000, 
		.rxCorner3dBLp_kHz = 100000, 
		.tiaPower = ADI_ADRV9001_COMPONENT_POWER_LEVEL_HIGH, 
		.tiaPowerLp = ADI_ADRV9001_COMPONENT_POWER_LEVEL_HIGH, 
		.channelType = 128, 
		.adcType = ADI_ADRV9001_ADC_HP, 
		.lpAdcCalMode = ADI_ADRV9001_ADC_LOWPOWER_PERIODIC, 
		.gainTableType = ADI_ADRV9001_RX_GAIN_CORRECTION_TABLE, 
		.rxDpProfile = { 
		.rxNbDecTop = { 
		.scicBlk23En = 0, 
		.scicBlk23DivFactor = 1, 
		.scicBlk23LowRippleEn = 0, 
		.decBy2Blk35En = 0, 
		.decBy2Blk37En = 0, 
		.decBy2Blk39En = 0, 
		.decBy2Blk41En = 0, 
		.decBy2Blk43En = 0, 
		.decBy3Blk45En = 0, 
		.decBy2Blk47En = 0 }
					, 
		.rxWbDecTop = { 
		.decBy2Blk25En = 0, 
		.decBy2Blk27En = 0, 
		.decBy2Blk29En = 0, 
		.decBy2Blk31En = 1, 
		.decBy2Blk33En = 1, 
		.wbLpfBlk33p1En = 0 }
					, 
		.rxDecTop = { 
		.decBy3Blk15En = 1, 
		.decBy2Hb3Blk17p1En = 0, 
		.decBy2Hb4Blk17p2En = 0, 
		.decBy2Hb5Blk19p1En = 0, 
		.decBy2Hb6Blk19p2En = 0 }
					, 
		.rxSincHBTop = { 
		.sincGainMux = ADI_ADRV9001_RX_SINC_GAIN_MUX_6_DB, 
		.sincMux = ADI_ADRV9001_RX_SINC_MUX5_OUTPUT_SINC6, 
		.hbMux = ADI_ADRV9001_RX_HB_MUX_OUTPUT_HB2, 
		.isGainCompEnabled = 0, 
		.gainComp9GainI = { 16384, 16384, 16384, 16384, 16384, 16384  }, 
		.gainComp9GainQ = { 0, 0, 0, 0, 0, 0  } }
					, 
		.rxNbDem = { 
		.dpInFifo = { 
		.dpInFifoEn = 0, 
		.dpInFifoMode = ADI_ADRV9001_DP_IN_FIFO_MODE_DETECTING, 
		.dpInFifoTestDataSel = ADI_ADRV9001_DP_IN_FIFO_INPUT_DP_SEL }
						, 
		.rxNbNco = { 
		.rxNbNcoEn = 0, 
		.rxNbNcoConfig = { 
		.freq = 0, 
		.sampleFreq = 0, 
		.phase = 0, 
		.realOut = 0 }
							 }
						, 
		.rxWbNbCompPFir = { 
		.bankSel = ADI_ADRV9001_PFIR_BANK_D, 
		.rxWbNbCompPFirInMuxSel = ADI_ADRV9001_RP_FIR_IN_MUX_INT_IN, 
		.rxWbNbCompPFirEn = 0 }
						, 
		.resamp = { 
		.rxResampEn = 0, 
		.resampPhaseI = 0, 
		.resampPhaseQ = 0 }
						, 
		.gsOutMuxSel = ADI_ADRV9001_GS_OUT_MUX_BYPASS, 
		.rxOutSel = ADI_ADRV9001_RX_OUT_IQ_SEL, 
		.rxRoundMode = ADI_ADRV9001_RX_ROUNDMODE_IQ, 
		.dpArmSel = ADI_ADRV9001_DP_SEL }
					 }
				, 
		.lnaConfig = { 
		.externalLnaPresent = false, 
		.gpioSourceSel = ADI_ADRV9001_GPIO_ANALOG_PIN_NIBBLE_UNASSIGNED, 
		.externalLnaPinSel = ADI_ADRV9001_EXTERNAL_LNA_PIN_RX1_LOWER_RX2_UPPER, 
		.settlingDelay = 0, 
		.numberLnaGainSteps = 0, 
		.lnaGainSteps_mdB = { 0, 0, 0, 0  }, 
		.lnaDigitalGainDelay = 0, 
		.minGainIndex = 0, 
		.lnaType = ADI_ADRV9001_EXTERNAL_LNA_TYPE_SINGLE }
				, 
		.rxSsiConfig = { 
		.ssiType = ADI_ADRV9001_SSI_TYPE_LVDS, 
		.ssiDataFormatSel = ADI_ADRV9001_SSI_FORMAT_16_BIT_I_Q_DATA, 
		.numLaneSel = ADI_ADRV9001_SSI_2_LANE, 
		.strobeType = ADI_ADRV9001_SSI_SHORT_STROBE, 
		.lsbFirst = 0, 
		.qFirst = 0, 
		.txRefClockPin = ADI_ADRV9001_SSI_TX_REF_CLOCK_PIN_DISABLED, 
		.lvdsIBitInversion = false, 
		.lvdsQBitInversion = false, 
		.lvdsStrobeBitInversion = false, 
		.lvdsUseLsbIn12bitMode = 0, 
		.lvdsRxClkInversionEn = false, 
		.cmosDdrPosClkEn = false, 
		.cmosClkInversionEn = false, 
		.ddrEn = true, 
		.rxMaskStrobeEn = false }
				 }
			 }, { 
		.profile = { 
		.primarySigBandwidth_Hz = 12500, 
		.rxOutputRate_Hz = 0, 
		.rxInterfaceSampleRate_Hz = 0, 
		.rxOffsetLo_kHz = 0, 
		.rxNcoEnable = false, 
		.outputSignaling = ADI_ADRV9001_RX_IQ, 
		.filterOrder = 1, 
		.filterOrderLp = 1, 
		.hpAdcCorner = 0, 
		.lpAdcCorner = 0, 
		.adcClk_kHz = 0, 
		.rxCorner3dB_kHz = 0, 
		.rxCorner3dBLp_kHz = 0, 
		.tiaPower = ADI_ADRV9001_COMPONENT_POWER_LEVEL_HIGH, 
		.tiaPowerLp = ADI_ADRV9001_COMPONENT_POWER_LEVEL_HIGH, 
		.channelType = 0, 
		.adcType = ADI_ADRV9001_ADC_HP, 
		.lpAdcCalMode = ADI_ADRV9001_ADC_LOWPOWER_PERIODIC, 
		.gainTableType = ADI_ADRV9001_RX_GAIN_CORRECTION_TABLE, 
		.rxDpProfile = { 
		.rxNbDecTop = { 
		.scicBlk23En = 0, 
		.scicBlk23DivFactor = 0, 
		.scicBlk23LowRippleEn = 0, 
		.decBy2Blk35En = 0, 
		.decBy2Blk37En = 0, 
		.decBy2Blk39En = 0, 
		.decBy2Blk41En = 0, 
		.decBy2Blk43En = 0, 
		.decBy3Blk45En = 0, 
		.decBy2Blk47En = 0 }
					, 
		.rxWbDecTop = { 
		.decBy2Blk25En = 0, 
		.decBy2Blk27En = 0, 
		.decBy2Blk29En = 0, 
		.decBy2Blk31En = 0, 
		.decBy2Blk33En = 0, 
		.wbLpfBlk33p1En = 0 }
					, 
		.rxDecTop = { 
		.decBy3Blk15En = 0, 
		.decBy2Hb3Blk17p1En = 0, 
		.decBy2Hb4Blk17p2En = 0, 
		.decBy2Hb5Blk19p1En = 0, 
		.decBy2Hb6Blk19p2En = 0 }
					, 
		.rxSincHBTop = { 
		.sincGainMux = ADI_ADRV9001_RX_SINC_GAIN_MUX_6_DB, 
		.sincMux = ADI_ADRV9001_RX_SINC_MUX5_OUTPUT_ZERO, 
		.hbMux = ADI_ADRV9001_RX_HB_MUX_OUTPUT_HB1, 
		.isGainCompEnabled = 0, 
		.gainComp9GainI = { 16384, 16384, 16384, 16384, 16384, 16384  }, 
		.gainComp9GainQ = { 0, 0, 0, 0, 0, 0  } }
					, 
		.rxNbDem = { 
		.dpInFifo = { 
		.dpInFifoEn = 0, 
		.dpInFifoMode = ADI_ADRV9001_DP_IN_FIFO_MODE_DETECTING, 
		.dpInFifoTestDataSel = ADI_ADRV9001_DP_IN_FIFO_INPUT_DP_SEL }
						, 
		.rxNbNco = { 
		.rxNbNcoEn = 0, 
		.rxNbNcoConfig = { 
		.freq = 0, 
		.sampleFreq = 0, 
		.phase = 0, 
		.realOut = 0 }
							 }
						, 
		.rxWbNbCompPFir = { 
		.bankSel = ADI_ADRV9001_PFIR_BANK_A, 
		.rxWbNbCompPFirInMuxSel = ADI_ADRV9001_RP_FIR_IN_MUX_INT_IN, 
		.rxWbNbCompPFirEn = 0 }
						, 
		.resamp = { 
		.rxResampEn = 0, 
		.resampPhaseI = 0, 
		.resampPhaseQ = 0 }
						, 
		.gsOutMuxSel = ADI_ADRV9001_GS_OUT_MUX_BYPASS, 
		.rxOutSel = ADI_ADRV9001_RX_OUT_IQ_SEL, 
		.rxRoundMode = ADI_ADRV9001_RX_ROUNDMODE_IQ, 
		.dpArmSel = ADI_ADRV9001_DP_SEL }
					 }
				, 
		.lnaConfig = { 
		.externalLnaPresent = false, 
		.gpioSourceSel = ADI_ADRV9001_GPIO_ANALOG_PIN_NIBBLE_UNASSIGNED, 
		.externalLnaPinSel = ADI_ADRV9001_EXTERNAL_LNA_PIN_RX1_LOWER_RX2_UPPER, 
		.settlingDelay = 0, 
		.numberLnaGainSteps = 0, 
		.lnaGainSteps_mdB = { 0, 0, 0, 0  }, 
		.lnaDigitalGainDelay = 0, 
		.minGainIndex = 0, 
		.lnaType = ADI_ADRV9001_EXTERNAL_LNA_TYPE_SINGLE }
				, 
		.rxSsiConfig = { 
		.ssiType = ADI_ADRV9001_SSI_TYPE_DISABLE, 
		.ssiDataFormatSel = ADI_ADRV9001_SSI_FORMAT_2_BIT_SYMBOL_DATA, 
		.numLaneSel = ADI_ADRV9001_SSI_1_LANE, 
		.strobeType = ADI_ADRV9001_SSI_SHORT_STROBE, 
		.lsbFirst = 0, 
		.qFirst = 0, 
		.txRefClockPin = ADI_ADRV9001_SSI_TX_REF_CLOCK_PIN_DISABLED, 
		.lvdsIBitInversion = false, 
		.lvdsQBitInversion = false, 
		.lvdsStrobeBitInversion = false, 
		.lvdsUseLsbIn12bitMode = 0, 
		.lvdsRxClkInversionEn = false, 
		.cmosDdrPosClkEn = false, 
		.cmosClkInversionEn = false, 
		.ddrEn = false, 
		.rxMaskStrobeEn = false }
				 }
			 }, { 
		.profile = { 
		.primarySigBandwidth_Hz = 9000000, 
		.rxOutputRate_Hz = 15360000, 
		.rxInterfaceSampleRate_Hz = 15360000, 
		.rxOffsetLo_kHz = 0, 
		.rxNcoEnable = false, 
		.outputSignaling = ADI_ADRV9001_RX_IQ, 
		.filterOrder = 1, 
		.filterOrderLp = 1, 
		.hpAdcCorner = 50000000, 
		.lpAdcCorner = 0, 
		.adcClk_kHz = 2211840, 
		.rxCorner3dB_kHz = 100000, 
		.rxCorner3dBLp_kHz = 100000, 
		.tiaPower = ADI_ADRV9001_COMPONENT_POWER_LEVEL_HIGH, 
		.tiaPowerLp = ADI_ADRV9001_COMPONENT_POWER_LEVEL_HIGH, 
		.channelType = 512, 
		.adcType = ADI_ADRV9001_ADC_HP, 
		.lpAdcCalMode = ADI_ADRV9001_ADC_LOWPOWER_PERIODIC, 
		.gainTableType = ADI_ADRV9001_RX_GAIN_CORRECTION_TABLE, 
		.rxDpProfile = { 
		.rxNbDecTop = { 
		.scicBlk23En = 0, 
		.scicBlk23DivFactor = 1, 
		.scicBlk23LowRippleEn = 0, 
		.decBy2Blk35En = 0, 
		.decBy2Blk37En = 0, 
		.decBy2Blk39En = 0, 
		.decBy2Blk41En = 0, 
		.decBy2Blk43En = 0, 
		.decBy3Blk45En = 0, 
		.decBy2Blk47En = 0 }
					, 
		.rxWbDecTop = { 
		.decBy2Blk25En = 0, 
		.decBy2Blk27En = 0, 
		.decBy2Blk29En = 0, 
		.decBy2Blk31En = 1, 
		.decBy2Blk33En = 1, 
		.wbLpfBlk33p1En = 0 }
					, 
		.rxDecTop = { 
		.decBy3Blk15En = 1, 
		.decBy2Hb3Blk17p1En = 0, 
		.decBy2Hb4Blk17p2En = 0, 
		.decBy2Hb5Blk19p1En = 0, 
		.decBy2Hb6Blk19p2En = 0 }
					, 
		.rxSincHBTop = { 
		.sincGainMux = ADI_ADRV9001_RX_SINC_GAIN_MUX_6_DB, 
		.sincMux = ADI_ADRV9001_RX_SINC_MUX5_OUTPUT_SINC6, 
		.hbMux = ADI_ADRV9001_RX_HB_MUX_OUTPUT_HB2, 
		.isGainCompEnabled = 0, 
		.gainComp9GainI = { 16384, 16384, 16384, 16384, 16384, 16384  }, 
		.gainComp9GainQ = { 0, 0, 0, 0, 0, 0  } }
					, 
		.rxNbDem = { 
		.dpInFifo = { 
		.dpInFifoEn = 0, 
		.dpInFifoMode = ADI_ADRV9001_DP_IN_FIFO_MODE_DETECTING, 
		.dpInFifoTestDataSel = ADI_ADRV9001_DP_IN_FIFO_INPUT_DP_SEL }
						, 
		.rxNbNco = { 
		.rxNbNcoEn = 0, 
		.rxNbNcoConfig = { 
		.freq = 0, 
		.sampleFreq = 0, 
		.phase = 0, 
		.realOut = 0 }
							 }
						, 
		.rxWbNbCompPFir = { 
		.bankSel = ADI_ADRV9001_PFIR_BANK_D, 
		.rxWbNbCompPFirInMuxSel = ADI_ADRV9001_RP_FIR_IN_MUX_INT_IN, 
		.rxWbNbCompPFirEn = 0 }
						, 
		.resamp = { 
		.rxResampEn = 0, 
		.resampPhaseI = 0, 
		.resampPhaseQ = 0 }
						, 
		.gsOutMuxSel = ADI_ADRV9001_GS_OUT_MUX_BYPASS, 
		.rxOutSel = ADI_ADRV9001_RX_OUT_IQ_SEL, 
		.rxRoundMode = ADI_ADRV9001_RX_ROUNDMODE_IQ, 
		.dpArmSel = ADI_ADRV9001_DP_SEL }
					 }
				, 
		.lnaConfig = { 
		.externalLnaPresent = false, 
		.gpioSourceSel = ADI_ADRV9001_GPIO_ANALOG_PIN_NIBBLE_UNASSIGNED, 
		.externalLnaPinSel = ADI_ADRV9001_EXTERNAL_LNA_PIN_RX1_LOWER_RX2_UPPER, 
		.settlingDelay = 0, 
		.numberLnaGainSteps = 0, 
		.lnaGainSteps_mdB = { 0, 0, 0, 0  }, 
		.lnaDigitalGainDelay = 0, 
		.minGainIndex = 0, 
		.lnaType = ADI_ADRV9001_EXTERNAL_LNA_TYPE_SINGLE }
				, 
		.rxSsiConfig = { 
		.ssiType = ADI_ADRV9001_SSI_TYPE_LVDS, 
		.ssiDataFormatSel = ADI_ADRV9001_SSI_FORMAT_16_BIT_I_Q_DATA, 
		.numLaneSel = ADI_ADRV9001_SSI_2_LANE, 
		.strobeType = ADI_ADRV9001_SSI_SHORT_STROBE, 
		.lsbFirst = 0, 
		.qFirst = 0, 
		.txRefClockPin = ADI_ADRV9001_SSI_TX_REF_CLOCK_PIN_DISABLED, 
		.lvdsIBitInversion = false, 
		.lvdsQBitInversion = false, 
		.lvdsStrobeBitInversion = false, 
		.lvdsUseLsbIn12bitMode = 0, 
		.lvdsRxClkInversionEn = false, 
		.cmosDdrPosClkEn = false, 
		.cmosClkInversionEn = false, 
		.ddrEn = true, 
		.rxMaskStrobeEn = false }
				 }
			 }  } }
	, 
		.tx = { 
		.txInitChannelMask = 8, 
		.txProfile = { { 
		.primarySigBandwidth_Hz = 12500, 
		.txInputRate_Hz = 0, 
		.txInterfaceSampleRate_Hz = 0, 
		.txOffsetLo_kHz = 0, 
		.validDataDelay = 0, 
		.txBbf3dBCorner_kHz = 33000, 
		.outputSignaling = ADI_ADRV9001_TX_IQ, 
		.txPdBiasCurrent = 0, 
		.txPdGainEnable = 0, 
		.txPrePdRealPole_kHz = 0, 
		.txPostPdRealPole_kHz = 0, 
		.txBbfPower = ADI_ADRV9001_COMPONENT_POWER_LEVEL_HIGH, 
		.txExtLoopBackType = 0, 
		.txExtLoopBackForInitCal = 0, 
		.txPeakLoopBackPower = 0, 
		.frequencyDeviation_Hz = 0, 
		.txDpProfile = { 
		.txPreProc = { 
		.txPreProcSymbol0 = 0, 
		.txPreProcSymbol1 = 0, 
		.txPreProcSymbol2 = 0, 
		.txPreProcSymbol3 = 0, 
		.txPreProcSymMapDivFactor = 1, 
		.txPreProcMode = ADI_ADRV9001_TX_DP_PREPROC_IQ_DATA_WITH_PFIRS, 
		.txPreProcWbNbPfirIBankSel = ADI_ADRV9001_PFIR_BANK_A, 
		.txPreProcWbNbPfirQBankSel = ADI_ADRV9001_PFIR_BANK_B }
				, 
		.txWbIntTop = { 
		.txInterpBy2Blk30En = 0, 
		.txInterpBy2Blk28En = 0, 
		.txInterpBy2Blk26En = 0, 
		.txInterpBy2Blk24En = 0, 
		.txInterpBy2Blk22En = 0, 
		.txWbLpfBlk22p1En = 0 }
				, 
		.txNbIntTop = { 
		.txInterpBy2Blk20En = 0, 
		.txInterpBy2Blk18En = 0, 
		.txInterpBy2Blk16En = 0, 
		.txInterpBy2Blk14En = 0, 
		.txInterpBy2Blk12En = 0, 
		.txInterpBy3Blk10En = 0, 
		.txInterpBy2Blk8En = 0, 
		.txScicBlk32En = 0, 
		.txScicBlk32DivFactor = 0 }
				, 
		.txIntTop = { 
		.interpBy3Blk44p1En = 0, 
		.sinc3Blk44En = 0, 
		.sinc2Blk42En = 0, 
		.interpBy3Blk40En = 0, 
		.interpBy2Blk38En = 0, 
		.interpBy2Blk36En = 0 }
				, 
		.txIntTopFreqDevMap = { 
		.rrc2Frac = 0, 
		.mpll = 0, 
		.nchLsw = 0, 
		.nchMsb = 0, 
		.freqDevMapEn = 0, 
		.txRoundEn = 1 }
				, 
		.txIqdmDuc = { 
		.iqdmDucMode = ADI_ADRV9001_TX_DP_IQDMDUC_MODE0, 
		.iqdmDev = 0, 
		.iqdmDevOffset = 0, 
		.iqdmScalar = 0, 
		.iqdmThreshold = 0, 
		.iqdmNco = { 
		.freq = 0, 
		.sampleFreq = 0, 
		.phase = 0, 
		.realOut = 0 }
					 }
				 }
			, 
		.txSsiConfig = { 
		.ssiType = ADI_ADRV9001_SSI_TYPE_DISABLE, 
		.ssiDataFormatSel = ADI_ADRV9001_SSI_FORMAT_2_BIT_SYMBOL_DATA, 
		.numLaneSel = ADI_ADRV9001_SSI_1_LANE, 
		.strobeType = ADI_ADRV9001_SSI_SHORT_STROBE, 
		.lsbFirst = 0, 
		.qFirst = 0, 
		.txRefClockPin = ADI_ADRV9001_SSI_TX_REF_CLOCK_PIN_DISABLED, 
		.lvdsIBitInversion = false, 
		.lvdsQBitInversion = false, 
		.lvdsStrobeBitInversion = false, 
		.lvdsUseLsbIn12bitMode = 0, 
		.lvdsRxClkInversionEn = false, 
		.cmosDdrPosClkEn = false, 
		.cmosClkInversionEn = false, 
		.ddrEn = false, 
		.rxMaskStrobeEn = false }
			 }, { 
		.primarySigBandwidth_Hz = 9000000, 
		.txInputRate_Hz = 15360000, 
		.txInterfaceSampleRate_Hz = 15360000, 
		.txOffsetLo_kHz = 0, 
		.validDataDelay = 0, 
		.txBbf3dBCorner_kHz = 50000, 
		.outputSignaling = ADI_ADRV9001_TX_IQ, 
		.txPdBiasCurrent = 1, 
		.txPdGainEnable = 0, 
		.txPrePdRealPole_kHz = 1000000, 
		.txPostPdRealPole_kHz = 530000, 
		.txBbfPower = ADI_ADRV9001_COMPONENT_POWER_LEVEL_HIGH, 
		.txExtLoopBackType = 2, 
		.txExtLoopBackForInitCal = 0, 
		.txPeakLoopBackPower = -180, 
		.frequencyDeviation_Hz = 0, 
		.txDpProfile = { 
		.txPreProc = { 
		.txPreProcSymbol0 = 0, 
		.txPreProcSymbol1 = 0, 
		.txPreProcSymbol2 = 0, 
		.txPreProcSymbol3 = 0, 
		.txPreProcSymMapDivFactor = 1, 
		.txPreProcMode = ADI_ADRV9001_TX_DP_PREPROC_IQ_DATA_WITH_PFIRS, 
		.txPreProcWbNbPfirIBankSel = ADI_ADRV9001_PFIR_BANK_C, 
		.txPreProcWbNbPfirQBankSel = ADI_ADRV9001_PFIR_BANK_D }
				, 
		.txWbIntTop = { 
		.txInterpBy2Blk30En = 0, 
		.txInterpBy2Blk28En = 0, 
		.txInterpBy2Blk26En = 0, 
		.txInterpBy2Blk24En = 1, 
		.txInterpBy2Blk22En = 1, 
		.txWbLpfBlk22p1En = 0 }
				, 
		.txNbIntTop = { 
		.txInterpBy2Blk20En = 0, 
		.txInterpBy2Blk18En = 0, 
		.txInterpBy2Blk16En = 0, 
		.txInterpBy2Blk14En = 0, 
		.txInterpBy2Blk12En = 0, 
		.txInterpBy3Blk10En = 0, 
		.txInterpBy2Blk8En = 0, 
		.txScicBlk32En = 0, 
		.txScicBlk32DivFactor = 1 }
				, 
		.txIntTop = { 
		.interpBy3Blk44p1En = 1, 
		.sinc3Blk44En = 0, 
		.sinc2Blk42En = 0, 
		.interpBy3Blk40En = 1, 
		.interpBy2Blk38En = 0, 
		.interpBy2Blk36En = 0 }
				, 
		.txIntTopFreqDevMap = { 
		.rrc2Frac = 0, 
		.mpll = 0, 
		.nchLsw = 0, 
		.nchMsb = 0, 
		.freqDevMapEn = 0, 
		.txRoundEn = 1 }
				, 
		.txIqdmDuc = { 
		.iqdmDucMode = ADI_ADRV9001_TX_DP_IQDMDUC_MODE0, 
		.iqdmDev = 0, 
		.iqdmDevOffset = 0, 
		.iqdmScalar = 0, 
		.iqdmThreshold = 0, 
		.iqdmNco = { 
		.freq = 0, 
		.sampleFreq = 61440000, 
		.phase = 0, 
		.realOut = 0 }
					 }
				 }
			, 
		.txSsiConfig = { 
		.ssiType = ADI_ADRV9001_SSI_TYPE_LVDS, 
		.ssiDataFormatSel = ADI_ADRV9001_SSI_FORMAT_16_BIT_I_Q_DATA, 
		.numLaneSel = ADI_ADRV9001_SSI_2_LANE, 
		.strobeType = ADI_ADRV9001_SSI_SHORT_STROBE, 
		.lsbFirst = 0, 
		.qFirst = 0, 
		.txRefClockPin = ADI_ADRV9001_SSI_TX_REF_CLOCK_PIN_DCLK_OUT_ENABLED, 
		.lvdsIBitInversion = false, 
		.lvdsQBitInversion = false, 
		.lvdsStrobeBitInversion = false, 
		.lvdsUseLsbIn12bitMode = 0, 
		.lvdsRxClkInversionEn = false, 
		.cmosDdrPosClkEn = false, 
		.cmosClkInversionEn = false, 
		.ddrEn = true, 
		.rxMaskStrobeEn = false }
			 }  } }
	, 
		.sysConfig = { 
		.duplexMode = ADI_ADRV9001_TDD_MODE, 
		.fhModeOn = 0, 
		.numDynamicProfiles = ADI_ADRV9001_NUM_DYNAMIC_PROFILES_DISABLED, 
		.mcsMode = ADI_ADRV9001_MCSMODE_DISABLED, 
		.mcsInterfaceType = ADI_ADRV9001_SSI_TYPE_DISABLE, 
		.adcTypeMonitor = ADI_ADRV9001_ADC_HP, 
		.pllLockTime_us = 380, 
		.pllPhaseSyncWait_us = 0, 
		.pllModulus = { 
		.modulus = { 8388593, 8388593, 8388593, 8388593, 8388593  }, 
		.dmModulus = { 8388593, 8388593  } }
		, 
		.warmBootEnable = false }
	, 
		.pfirBuffer = { 
		.pfirRxWbNbChFilterCoeff_A = { 
		.numCoeff = 128, 
		.symmetricSel = ADI_ADRV9001_PFIR_COEF_NON_SYMMETRIC, 
		.tapsSel = ADI_ADRV9001_PFIR_128_TAPS | ADI_ADRV9001_PFIR_TAPS_MAX_ID, 
		.gainSel = ADI_ADRV9001_PFIR_GAIN_ZERO_DB, 
		.coefficients = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8388608, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  } }
		, 
		.pfirRxWbNbChFilterCoeff_B = { 
		.numCoeff = 128, 
		.symmetricSel = ADI_ADRV9001_PFIR_COEF_NON_SYMMETRIC, 
		.tapsSel = ADI_ADRV9001_PFIR_128_TAPS | ADI_ADRV9001_PFIR_TAPS_MAX_ID, 
		.gainSel = ADI_ADRV9001_PFIR_GAIN_ZERO_DB, 
		.coefficients = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8388608, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  } }
		, 
		.pfirRxWbNbChFilterCoeff_C = { 
		.numCoeff = 128, 
		.symmetricSel = ADI_ADRV9001_PFIR_COEF_NON_SYMMETRIC, 
		.tapsSel = ADI_ADRV9001_PFIR_128_TAPS | ADI_ADRV9001_PFIR_TAPS_MAX_ID, 
		.gainSel = ADI_ADRV9001_PFIR_GAIN_ZERO_DB, 
		.coefficients = { 475, 312, -782, -39, 1201, -777, -1182, 1981, 177, -2874, 1941, 2393, -4416, 225, 5594, -4581, -3668, 8650, -1992, -9342, 9646, 4213, -15137, 6404, 13615, -18199, -2610, 23969, -15142, -17198, 31204, -3269, -34604, 30213, 17955, -49337, 16361, 45636, -53954, -12567, 72920, -40769, -54562, 89506, -4148, -102269, 83183, 57280, -142874, 41767, 139213, -158628, -45955, 231679, -125964, -193870, 320642, -4532, -442087, 390927, 347244, -1055854, 429729, 4391599, 4391599, 429729, -1055854, 347244, 390927, -442087, -4532, 320642, -193870, -125964, 231679, -45955, -158628, 139213, 41767, -142874, 57280, 83183, -102269, -4148, 89506, -54562, -40769, 72920, -12567, -53954, 45636, 16361, -49337, 17955, 30213, -34604, -3269, 31204, -17198, -15142, 23969, -2610, -18199, 13615, 6404, -15137, 4213, 9646, -9342, -1992, 8650, -3668, -4581, 5594, 225, -4416, 2393, 1941, -2874, 177, 1981, -1182, -777, 1201, -39, -782, 312, 0  } }
		, 
		.pfirRxWbNbChFilterCoeff_D = { 
		.numCoeff = 128, 
		.symmetricSel = ADI_ADRV9001_PFIR_COEF_NON_SYMMETRIC, 
		.tapsSel = ADI_ADRV9001_PFIR_128_TAPS | ADI_ADRV9001_PFIR_TAPS_MAX_ID, 
		.gainSel = ADI_ADRV9001_PFIR_GAIN_ZERO_DB, 
		.coefficients = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8388608, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  } }
		, 
		.pfirTxWbNbPulShpCoeff_A = { 
		.numCoeff = 0, 
		.symmetricSel = ADI_ADRV9001_PFIR_COEF_NON_SYMMETRIC, 
		.tapsSel = ADI_ADRV9001_PFIR_32_TAPS, 
		.gainSel = ADI_ADRV9001_PFIR_GAIN_NEG_12_DB, 
		.coefficients = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  } }
		, 
		.pfirTxWbNbPulShpCoeff_B = { 
		.numCoeff = 0, 
		.symmetricSel = ADI_ADRV9001_PFIR_COEF_NON_SYMMETRIC, 
		.tapsSel = ADI_ADRV9001_PFIR_32_TAPS, 
		.gainSel = ADI_ADRV9001_PFIR_GAIN_NEG_12_DB, 
		.coefficients = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  } }
		, 
		.pfirTxWbNbPulShpCoeff_C = { 
		.numCoeff = 128, 
		.symmetricSel = ADI_ADRV9001_PFIR_COEF_NON_SYMMETRIC, 
		.tapsSel = ADI_ADRV9001_PFIR_128_TAPS | ADI_ADRV9001_PFIR_TAPS_MAX_ID, 
		.gainSel = ADI_ADRV9001_PFIR_GAIN_ZERO_DB, 
		.coefficients = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  } }
		, 
		.pfirTxWbNbPulShpCoeff_D = { 
		.numCoeff = 128, 
		.symmetricSel = ADI_ADRV9001_PFIR_COEF_NON_SYMMETRIC, 
		.tapsSel = ADI_ADRV9001_PFIR_128_TAPS | ADI_ADRV9001_PFIR_TAPS_MAX_ID, 
		.gainSel = ADI_ADRV9001_PFIR_GAIN_ZERO_DB, 
		.coefficients = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  } }
		, 
		.pfirRxNbPulShp = { { 
		.numCoeff = 0, 
		.symmetricSel = ADI_ADRV9001_PFIR_COEF_NON_SYMMETRIC, 
		.taps = 0, 
		.gainSel = ADI_ADRV9001_PFIR_GAIN_NEG_12_DB, 
		.coefficients = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  } }, { 
		.numCoeff = 128, 
		.symmetricSel = ADI_ADRV9001_PFIR_COEF_NON_SYMMETRIC, 
		.taps = 128, 
		.gainSel = ADI_ADRV9001_PFIR_GAIN_ZERO_DB, 
		.coefficients = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8388608, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  } }  }, 
		.pfirRxMagLowTiaLowSRHp = { { 
		.numCoeff = 0, 
		.coefficients = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  } }, { 
		.numCoeff = 21, 
		.coefficients = { -346, 1330, -1179, -1282, 1309, 2728, -1342, -5853, -1019, 12237, 19601, 12237, -1019, -5853, -1342, 2728, 1309, -1282, -1179, 1330, -346  } }  }, 
		.pfirRxMagLowTiaHighSRHp = { { 
		.numCoeff = 0, 
		.coefficients = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  } }, { 
		.numCoeff = 21, 
		.coefficients = { -2474, 894, 6052, 4321, -7606, -15890, -6071, 13321, 18936, 6332, -2859, 6332, 18936, 13321, -6071, -15890, -7606, 4321, 6052, 894, -2474  } }  }, 
		.pfirRxMagHighTiaHighSRHp = { { 
		.numCoeff = 0, 
		.coefficients = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  } }, { 
		.numCoeff = 21, 
		.coefficients = { 39, -229, 714, -1485, 2134, -1844, -219, 4147, -8514, 8496, 26292, 8496, -8514, 4147, -219, -1844, 2134, -1485, 714, -229, 39  } }  }, 
		.pfirRxMagLowTiaLowSRLp = { { 
		.numCoeff = 0, 
		.coefficients = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  } }, { 
		.numCoeff = 21, 
		.coefficients = { -346, 1330, -1179, -1282, 1308, 2728, -1342, -5852, -1018, 12237, 19601, 12237, -1018, -5852, -1342, 2728, 1308, -1282, -1179, 1330, -346  } }  }, 
		.pfirRxMagLowTiaHighSRLp = { { 
		.numCoeff = 0, 
		.coefficients = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  } }, { 
		.numCoeff = 21, 
		.coefficients = { -2474, 893, 6051, 4320, -7605, -15887, -6070, 13320, 18934, 6331, -2859, 6331, 18934, 13320, -6070, -15887, -7605, 4320, 6051, 893, -2474  } }  }, 
		.pfirRxMagHighTiaHighSRLp = { { 
		.numCoeff = 0, 
		.coefficients = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  } }, { 
		.numCoeff = 21, 
		.coefficients = { 39, -229, 712, -1481, 2128, -1841, -215, 4131, -8490, 8497, 26266, 8497, -8490, 4131, -215, -1841, 2128, -1481, 712, -229, 39  } }  }, 
		.pfirTxMagComp1 = { 
		.numCoeff = 0, 
		.coefficients = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  } }
		, 
		.pfirTxMagComp2 = { 
		.numCoeff = 21, 
		.coefficients = { 69, -384, 1125, -2089, 2300, -165, -5248, 12368, -13473, 4864, 34039, 4864, -13473, 12368, -5248, -165, 2300, -2089, 1125, -384, 69  } }
		, 
		.pfirTxMagCompNb = { { 
		.numCoeff = 0, 
		.coefficients = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  } }, { 
		.numCoeff = 13, 
		.coefficients = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  } }  }, 
		.pfirRxMagCompNb = { { 
		.numCoeff = 0, 
		.coefficients = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  } }, { 
		.numCoeff = 13, 
		.coefficients = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  } }  } }
	 };
