#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include "adrv9001_pib.h"
#include "axi_adrv9001.h"


#define ADRV9001_PIB_FLAG_REBOOT        (0x08)
#define ADRV9001_PIB_FLAG_VIRTUAL       (0x10)
#define ADRV9001_PIB_FLAG_SET_ACTION    (0x20)
#define ADRV9001_PIB_FLAG_GET_ACTION    (0x40)

/* PIB Definition */
static pib_def_t Adrv9001PibDef[] =
{
//  Name                                                      Address                                                                Storage Type                       Flags
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
  { "LogPath",                              offsetof(adrv9001_params_t, LogPath),                                                     PibTypeStr,       PIB_FLAGS_DEFAULT },                                      // File system path to store adrv9001 logs
  { "Tx1Attn",                              offsetof(adrv9001_params_t, Tx1Attn),                                                     PibTypeU16,       PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_SET_ACTION },       // Transmitter attenuation in milli dB ( 7500 = 7.5dB )
  { "Tx2Attn",                              offsetof(adrv9001_params_t, Tx2Attn),                                                     PibTypeU16,       PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_SET_ACTION },
  { "Tx1Boost",                             offsetof(adrv9001_params_t, Tx1Boost),                                                    PibTypeU8,        PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_SET_ACTION },
  { "Tx2Boost",                             offsetof(adrv9001_params_t, Tx2Boost),                                                    PibTypeU8,        PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_SET_ACTION },
  { "Tx1DpdEnable",                         offsetof(adrv9001_params_t, Tx1DpdInitCfg.enable),                                        PibTypeU8,        PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_REBOOT },           // DPD enable (profile must support DPD)
  { "Tx1DpdAmplifierType",                  offsetof(adrv9001_params_t, Tx1DpdInitCfg.amplifierType),                                 PibTypeU8,        PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_REBOOT },           // DPD amplifier type (see TES)
  { "Tx1DpdLutSize",                        offsetof(adrv9001_params_t, Tx1DpdInitCfg.lutSize),                                       PibTypeU8,        PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_REBOOT },           // DPD LUT size  (see TES)
  { "Tx1DpdModel",                          offsetof(adrv9001_params_t, Tx1DpdInitCfg.model),                                         PibTypeU8,        PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_REBOOT },           // DPD model  (see TES)
  { "Tx1DpdChangeModelTapOrders",           offsetof(adrv9001_params_t, Tx1DpdInitCfg.changeModelTapOrders),                          PibTypeU8,        PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_REBOOT },           
  { "Tx1DpdClgcEnable",                     offsetof(adrv9001_params_t, Tx1DpdInitCfg.clgcEnable),                                    PibTypeU8,        PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_REBOOT },
  { "Tx1DpdPreLutScale",                    offsetof(adrv9001_params_t, Tx1DpdInitCfg.preLutScale),                                   PibTypeU8,        PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_REBOOT },
  { "Tx1DpdNumberofSamples",                offsetof(adrv9001_params_t, Tx1DpdCfg.numberOfSamples),                                   PibTypeU32,       PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_REBOOT },
  { "Tx1DpdAdditionalPowerScale",           offsetof(adrv9001_params_t, Tx1DpdCfg.additionalPowerScale),                              PibTypeU32,       PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_REBOOT },
  { "Tx1DpdCountsLessThanPowerThreshold",   offsetof(adrv9001_params_t, Tx1DpdCfg.countsLessThanPowerThreshold),                      PibTypeU16,       PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_REBOOT },
  { "Tx1DpdCountsGreaterThanPeakThreshold", offsetof(adrv9001_params_t, Tx1DpdCfg.countsGreaterThanPeakThreshold),                    PibTypeU16,       PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_REBOOT },
  { "Tx1DpdImmediateLutSwitching",          offsetof(adrv9001_params_t, Tx1DpdCfg.immediateLutSwitching),                             PibTypeU8,        PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_REBOOT },
  { "Tx1DpdUseSpecialFrame",                offsetof(adrv9001_params_t, Tx1DpdCfg.useSpecialFrame),                                   PibTypeU8,        PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_REBOOT },
  { "Tx1DpdResetLuts",                      offsetof(adrv9001_params_t, Tx1DpdCfg.resetLuts),                                         PibTypeU8,        PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_REBOOT },
  { "Tx1DpdSamplingRate_Hz",                offsetof(adrv9001_params_t, Tx1DpdCfg.dpdSamplingRate_Hz),                                PibTypeU32,       PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_REBOOT },
  { "Tx1DpdRxTxNormalizationLowerThreshold",offsetof(adrv9001_params_t, Tx1DpdCfg.rxTxNormalizationLowerThreshold),                   PibTypeU32,       PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_REBOOT },
  { "Tx1DpdRxTxNormalizationUpperThreshold",offsetof(adrv9001_params_t, Tx1DpdCfg.rxTxNormalizationUpperThreshold),                   PibTypeU32,       PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_REBOOT },
  { "Tx1DpdDetectionPowerThreshold",        offsetof(adrv9001_params_t, Tx1DpdCfg.detectionPowerThreshold),                           PibTypeU32,       PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_REBOOT },
  { "Tx1DpdDetectionPeakThreshold",         offsetof(adrv9001_params_t, Tx1DpdCfg.detectionPeakThreshold),                            PibTypeU32,       PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_REBOOT },
  { "Tx1DpdTimeFilterCoefficient",          offsetof(adrv9001_params_t, Tx1DpdCfg.timeFilterCoefficient),                             PibTypeU32,       PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_REBOOT },
  { "Tx1DpdClgcLoopOpen",                   offsetof(adrv9001_params_t, Tx1DpdCfg.clgcLoopOpen),                                      PibTypeU8,        PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_REBOOT },
  { "Tx1DpdClgcFilterAlpha",                offsetof(adrv9001_params_t, Tx1DpdCfg.clgcFilterAlpha),                                   PibTypeU32,       PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_REBOOT },
  { "Tx1DpdClgcGainTarget_dB",              offsetof(adrv9001_params_t, Tx1DpdCfg.clgcGainTarget_HundredthdB),                        PibTypeS32,       PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_REBOOT },
  { "Tx1DpdClgcLastGain_HundredthdB",       offsetof(adrv9001_params_t, Tx1DpdCfg.clgcLastGain_HundredthdB),                          PibTypeS32,       PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_REBOOT },
  { "Tx1DpdClgcFilteredGain_HundredthdB",   offsetof(adrv9001_params_t, Tx1DpdCfg.clgcFilteredGain_HundredthdB),                      PibTypeS32,       PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_REBOOT },
  { "Tx2DpdEnable",                         offsetof(adrv9001_params_t, Tx2DpdInitCfg.enable),                                        PibTypeU8,        PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_REBOOT },
  { "Tx2DpdAmplifierType",                  offsetof(adrv9001_params_t, Tx2DpdInitCfg.amplifierType),                                 PibTypeU8,        PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_REBOOT },
  { "Tx2DpdLutSize",                        offsetof(adrv9001_params_t, Tx2DpdInitCfg.lutSize),                                       PibTypeU8,        PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_REBOOT },
  { "Tx2DpdModel",                          offsetof(adrv9001_params_t, Tx2DpdInitCfg.model),                                         PibTypeU8,        PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_REBOOT },
  { "Tx2DpdChangeModelTapOrders",           offsetof(adrv9001_params_t, Tx2DpdInitCfg.changeModelTapOrders),                          PibTypeU8,        PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_REBOOT },
  { "Tx2DpdClgcEnable",                     offsetof(adrv9001_params_t, Tx2DpdInitCfg.clgcEnable),                                    PibTypeU8,        PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_REBOOT },
  { "Tx2DpdPreLutScale",                    offsetof(adrv9001_params_t, Tx2DpdInitCfg.preLutScale),                                   PibTypeU8,        PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_REBOOT },
  { "Tx2DpdNumberofSamples",                offsetof(adrv9001_params_t, Tx2DpdCfg.numberOfSamples),                                   PibTypeU32,       PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_REBOOT },
  { "Tx2DpdAdditionalPowerScale",           offsetof(adrv9001_params_t, Tx2DpdCfg.additionalPowerScale),                              PibTypeU32,       PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_REBOOT },
  { "Tx2DpdCountsLessThanPowerThreshold",   offsetof(adrv9001_params_t, Tx2DpdCfg.countsLessThanPowerThreshold),                      PibTypeU16,       PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_REBOOT },
  { "Tx2DpdCountsGreaterThanPeakThreshold", offsetof(adrv9001_params_t, Tx2DpdCfg.countsGreaterThanPeakThreshold),                    PibTypeU16,       PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_REBOOT },
  { "Tx2DpdImmediateLutSwitching",          offsetof(adrv9001_params_t, Tx2DpdCfg.immediateLutSwitching),                             PibTypeU8,        PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_REBOOT },
  { "Tx2DpdUseSpecialFrame",                offsetof(adrv9001_params_t, Tx2DpdCfg.useSpecialFrame),                                   PibTypeU8,        PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_REBOOT },
  { "Tx2DpdResetLuts",                      offsetof(adrv9001_params_t, Tx2DpdCfg.resetLuts),                                         PibTypeU8,        PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_REBOOT },
  { "Tx2DpdSamplingRate_Hz",                offsetof(adrv9001_params_t, Tx2DpdCfg.dpdSamplingRate_Hz),                                PibTypeU32,       PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_REBOOT },
  { "Tx2DpdRxTxNormalizationLowerThreshold",offsetof(adrv9001_params_t, Tx2DpdCfg.rxTxNormalizationLowerThreshold),                   PibTypeU32,       PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_REBOOT },
  { "Tx2DpdRxTxNormalizationUpperThreshold",offsetof(adrv9001_params_t, Tx2DpdCfg.rxTxNormalizationUpperThreshold),                   PibTypeU32,       PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_REBOOT },
  { "Tx2DpdDetectionPowerThreshold",        offsetof(adrv9001_params_t, Tx2DpdCfg.detectionPowerThreshold),                           PibTypeU32,       PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_REBOOT },
  { "Tx2DpdDetectionPeakThreshold",         offsetof(adrv9001_params_t, Tx2DpdCfg.detectionPeakThreshold),                            PibTypeU32,       PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_REBOOT },
  { "Tx2DpdTimeFilterCoefficient",          offsetof(adrv9001_params_t, Tx2DpdCfg.timeFilterCoefficient),                             PibTypeU32,       PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_REBOOT },
  { "Tx2DpdClgcLoopOpen",                   offsetof(adrv9001_params_t, Tx2DpdCfg.clgcLoopOpen),                                      PibTypeU8,        PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_REBOOT },
  { "Tx2DpdClgcFilterAlpha",                offsetof(adrv9001_params_t, Tx2DpdCfg.clgcFilterAlpha),                                   PibTypeU32,       PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_REBOOT },
  { "Tx2DpdClgcGainTarget_dB",              offsetof(adrv9001_params_t, Tx2DpdCfg.clgcGainTarget_HundredthdB),                        PibTypeS32,       PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_REBOOT },
  { "Tx2DpdClgcLastGain_HundredthdB",       offsetof(adrv9001_params_t, Tx2DpdCfg.clgcLastGain_HundredthdB),                          PibTypeS32,       PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_REBOOT },
  { "Tx2DpdClgcFilteredGain_HundredthdB",   offsetof(adrv9001_params_t, Tx2DpdCfg.clgcFilteredGain_HundredthdB),                      PibTypeS32,       PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_REBOOT },
  { "Tx1ExternalLoopbackPower",             offsetof(adrv9001_params_t, tx.txProfile[0].txPeakLoopBackPower),                         PibTypeS16,       PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_REBOOT },
  { "Tx1ExternalPathDelay",                 offsetof(adrv9001_params_t, Tx1ExternalPathDelay),                                        PibTypeU32,       PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_REBOOT },
  { "Tx2ExternalLoopbackPower",             offsetof(adrv9001_params_t, tx.txProfile[1].txPeakLoopBackPower),                         PibTypeS16,       PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_REBOOT },
  { "Tx2ExternalPathDelay",                 offsetof(adrv9001_params_t, Tx2ExternalPathDelay),                                        PibTypeU32,       PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_REBOOT },
  { "Tx1CarrierFrequency",                  0,                                                                                        PibTypeU64,       PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_VIRTUAL },          // Carrier Frequency
  { "Tx2CarrierFrequency",                  0,                                                                                        PibTypeU64,       PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_VIRTUAL },
  { "Rx1CarrierFrequency",                  0,                                                                                        PibTypeU64,       PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_VIRTUAL },
  { "Rx2CarrierFrequency",                  0,                                                                                        PibTypeU64,       PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_VIRTUAL },
  { "Tx1IqData",                            0,                                                                                        PibTypeU32,       PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_VIRTUAL },          // Transmit IQ data used when IqDataPath = 1
  { "Tx2IqData",                            0,                                                                                        PibTypeU32,       PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_VIRTUAL },
  { "Rx1IqData",                            0,                                                                                        PibTypeU32,       PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_VIRTUAL },
  { "Rx2IqData",                            0,                                                                                        PibTypeU32,       PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_VIRTUAL },
  { "Tx1IqDataPath",                        0,                                                                                        PibTypeU8,        PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_VIRTUAL },          // Transmit data path, 0 = axis, 1 = IqData
  { "Tx2IqDataPath",                        0,                                                                                        PibTypeU8,        PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_VIRTUAL },
  { "Dgpio",                                0,                                                                                        PibTypeU32,       PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_VIRTUAL },          // Digital GPIO output value on write and input on read
  { "DgpioDir",                             0,                                                                                        PibTypeU32,       PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_VIRTUAL },          // Digital GPIO direction
  { "VcTcxo",                               0,                                                                                        PibTypeFloat,     PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_VIRTUAL },          // VCTCXO control voltage (0 to 1.8)
  { "Tx1RadioState",                        0,                                                                                        PibTypeU8,        PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_VIRTUAL },          // Radio state
  { "Tx2RadioState",                        0,                                                                                        PibTypeU8,        PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_VIRTUAL },
  { "Rx1RadioState",                        0,                                                                                        PibTypeU8,        PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_VIRTUAL },
  { "Rx2RadioState",                        0,                                                                                        PibTypeU8,        PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_VIRTUAL },
  { "TxRx1SsiLoopBack",                     0,                                                                                        PibTypeU8,        PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_VIRTUAL },          // Tx/Rx SSI loopback 
  { "TxRx2SsiLoopBack",                     0,                                                                                        PibTypeU8,        PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_VIRTUAL },
  { "FirmwareVersion",                      0,                                                                                        PibTypeStr,       PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_VIRTUAL | PIB_FLAG_READ_ONLY }, // ARM processor firmware revision
  { "APIVersion",                           0,                                                                                        PibTypeStr,       PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_VIRTUAL | PIB_FLAG_READ_ONLY }, // API version
  { "SiliconVersion",                       0,                                                                                        PibTypeStr,       PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_VIRTUAL | PIB_FLAG_READ_ONLY }, // Silicon Version
  { "Temp",                                 0,                                                                                        PibTypeS16,       PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_VIRTUAL | PIB_FLAG_READ_ONLY }, // Temperature
  { "Rx1Rssi",                              0,                                                                                        PibTypeFloat,     PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_VIRTUAL | PIB_FLAG_READ_ONLY }, // Receiver Signal Strength
  { "Rx2Rssi",                              0,                                                                                        PibTypeFloat,     PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_VIRTUAL | PIB_FLAG_READ_ONLY }, 
  { "Rx1CurGainIndex",                      0,                                                                                        PibTypeU8,        PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_VIRTUAL | PIB_FLAG_READ_ONLY }, // Receiver gain index
  { "Rx2CurGainIndex",                      0,                                                                                        PibTypeU8,        PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_VIRTUAL | PIB_FLAG_READ_ONLY },
  { "DeviceClock_kHz",                      offsetof(adrv9001_params_t, clocks.deviceClock_kHz),                                      PibTypeU32,       PIB_FLAGS_DEFAULT | PIB_FLAG_READ_ONLY },                             // Device clock frequency
  { "ClkPllVcoFreq_daHz",                   offsetof(adrv9001_params_t, clocks.clkPllVcoFreq_daHz),                                   PibTypeU32,       PIB_FLAGS_DEFAULT | PIB_FLAG_READ_ONLY },                             // See TES
  { "ClkPllHsDiv",                          offsetof(adrv9001_params_t, clocks.clkPllHsDiv),                                          PibTypeS8,        PIB_FLAGS_DEFAULT | PIB_FLAG_READ_ONLY },                             // See TES
  { "ClkPllMode",                           offsetof(adrv9001_params_t, clocks.clkPllMode),                                           PibTypeS8,        PIB_FLAGS_DEFAULT | PIB_FLAG_READ_ONLY },                             // See TES
  { "Clk1105Div",                           offsetof(adrv9001_params_t, clocks.clk1105Div),                                           PibTypeS8,        PIB_FLAGS_DEFAULT | PIB_FLAG_READ_ONLY },                             // See TES
  { "ArmClkDiv",                            offsetof(adrv9001_params_t, clocks.armClkDiv),                                            PibTypeS8,        PIB_FLAGS_DEFAULT | PIB_FLAG_READ_ONLY },                             // See TES
  { "ArmPowerSavingClkDiv",                 offsetof(adrv9001_params_t, clocks.armPowerSavingClkDiv),                                 PibTypeU16,       PIB_FLAGS_DEFAULT | PIB_FLAG_READ_ONLY },                             // See TES
  { "RefClockOutEnable",                    offsetof(adrv9001_params_t, clocks.refClockOutEnable),                                    PibTypeU8,        PIB_FLAGS_DEFAULT | PIB_FLAG_READ_ONLY },                             // See TES
  { "AuxPllPower",                          offsetof(adrv9001_params_t, clocks.auxPllPower),                                          PibTypeS8,        PIB_FLAGS_DEFAULT | PIB_FLAG_READ_ONLY },                             // See TES
  { "ClkPllPower",                          offsetof(adrv9001_params_t, clocks.clkPllPower),                                          PibTypeS8,        PIB_FLAGS_DEFAULT | PIB_FLAG_READ_ONLY },                             // See TES
  { "PadRefClkDrv",                         offsetof(adrv9001_params_t, clocks.padRefClkDrv),                                         PibTypeU8,        PIB_FLAGS_DEFAULT | PIB_FLAG_READ_ONLY },                             // See TES
  { "ExtLo1OutFreq_kHz",                    offsetof(adrv9001_params_t, clocks.extLo1OutFreq_kHz),                                    PibTypeU32,       PIB_FLAGS_DEFAULT | PIB_FLAG_READ_ONLY },                             // See TES
  { "ExtLo2OutFreq_kHz",                    offsetof(adrv9001_params_t, clocks.extLo2OutFreq_kHz),                                    PibTypeU32,       PIB_FLAGS_DEFAULT | PIB_FLAG_READ_ONLY },                             // See TES
  { "RfPll1LoMode",                         offsetof(adrv9001_params_t, clocks.rfPll1LoMode),                                         PibTypeS8,        PIB_FLAGS_DEFAULT | PIB_FLAG_READ_ONLY },                             // See TES
  { "RfPll2LoMode",                         offsetof(adrv9001_params_t, clocks.rfPll2LoMode),                                         PibTypeS8,        PIB_FLAGS_DEFAULT | PIB_FLAG_READ_ONLY },                             // See TES
  { "Ext1LoType",                           offsetof(adrv9001_params_t, clocks.ext1LoType),                                           PibTypeS8,        PIB_FLAGS_DEFAULT | PIB_FLAG_READ_ONLY },                             // See TES
  { "Ext2LoType",                           offsetof(adrv9001_params_t, clocks.ext2LoType),                                           PibTypeS8,        PIB_FLAGS_DEFAULT | PIB_FLAG_READ_ONLY },                             // See TES
  { "Rx1RfInputSel",                        offsetof(adrv9001_params_t, clocks.rx1RfInputSel),                                        PibTypeS8,        PIB_FLAGS_DEFAULT | PIB_FLAG_READ_ONLY },                             // See TES
  { "Rx2RfInputSel",                        offsetof(adrv9001_params_t, clocks.rx2RfInputSel),                                        PibTypeS8,        PIB_FLAGS_DEFAULT | PIB_FLAG_READ_ONLY },                             // See TES
  { "ExtLo1Divider",                        offsetof(adrv9001_params_t, clocks.extLo1Divider),                                        PibTypeU16,       PIB_FLAGS_DEFAULT | PIB_FLAG_READ_ONLY },                             // See TES
  { "ExtLo2Divider",                        offsetof(adrv9001_params_t, clocks.extLo2Divider),                                        PibTypeU16,       PIB_FLAGS_DEFAULT | PIB_FLAG_READ_ONLY },                             // See TES
  { "RfPllPhaseSyncMode",                   offsetof(adrv9001_params_t, clocks.rfPllPhaseSyncMode),                                   PibTypeS8,        PIB_FLAGS_DEFAULT | PIB_FLAG_READ_ONLY },                             // See TES
  { "Rx1LoSelect",                          offsetof(adrv9001_params_t, clocks.rx1LoSelect),                                          PibTypeS8,        PIB_FLAGS_DEFAULT | PIB_FLAG_READ_ONLY },                             // See TES
  { "Rx2LoSelect",                          offsetof(adrv9001_params_t, clocks.rx2LoSelect),                                          PibTypeS8,        PIB_FLAGS_DEFAULT | PIB_FLAG_READ_ONLY },                             // See TES
  { "Tx1LoSelect",                          offsetof(adrv9001_params_t, clocks.tx1LoSelect),                                          PibTypeS8,        PIB_FLAGS_DEFAULT | PIB_FLAG_READ_ONLY },                             // See TES
  { "Tx2LoSelect",                          offsetof(adrv9001_params_t, clocks.tx2LoSelect),                                          PibTypeS8,        PIB_FLAGS_DEFAULT | PIB_FLAG_READ_ONLY },                             // See TES
  { "Rx1LoDivMode",                         offsetof(adrv9001_params_t, clocks.rx1LoDivMode),                                         PibTypeS8,        PIB_FLAGS_DEFAULT | PIB_FLAG_READ_ONLY },                             // See TES
  { "Rx2LoDivMode",                         offsetof(adrv9001_params_t, clocks.rx2LoDivMode),                                         PibTypeS8,        PIB_FLAGS_DEFAULT | PIB_FLAG_READ_ONLY },                             // See TES
  { "Tx1LoDivMode",                         offsetof(adrv9001_params_t, clocks.tx1LoDivMode),                                         PibTypeS8,        PIB_FLAGS_DEFAULT | PIB_FLAG_READ_ONLY },                             // See TES
  { "Tx2LoDivMode",                         offsetof(adrv9001_params_t, clocks.tx2LoDivMode),                                         PibTypeS8,        PIB_FLAGS_DEFAULT | PIB_FLAG_READ_ONLY },                             // See TES
  { "LoGen1Select",                         offsetof(adrv9001_params_t, clocks.loGen1Select),                                         PibTypeS8,        PIB_FLAGS_DEFAULT | PIB_FLAG_READ_ONLY },                             // See TES
  { "LoGen2Select",                         offsetof(adrv9001_params_t, clocks.loGen2Select),                                         PibTypeS8,        PIB_FLAGS_DEFAULT | PIB_FLAG_READ_ONLY },                             // See TES
  { "Tx1SampleRate",                        offsetof(adrv9001_params_t, tx.txProfile[0].txInterfaceSampleRate_Hz),                    PibTypeU32,       PIB_FLAGS_DEFAULT | PIB_FLAG_READ_ONLY },                             // See TES
  { "Tx2SampleRate",                        offsetof(adrv9001_params_t, tx.txProfile[1].txInterfaceSampleRate_Hz),                    PibTypeU32,       PIB_FLAGS_DEFAULT | PIB_FLAG_READ_ONLY },                             // See TES
  { "Rx1SampleRate",                        offsetof(adrv9001_params_t, rx.rxChannelCfg[0].profile.rxInterfaceSampleRate_Hz),         PibTypeU32,       PIB_FLAGS_DEFAULT | PIB_FLAG_READ_ONLY },                             // See TES
  { "Rx2SampleRate",                        offsetof(adrv9001_params_t, rx.rxChannelCfg[1].profile.rxInterfaceSampleRate_Hz),         PibTypeU32,       PIB_FLAGS_DEFAULT | PIB_FLAG_READ_ONLY },                             // See TES
  { "Tx1TestMode",                          0,                                                                                        PibTypeU8,        PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_VIRTUAL },                      // ADRV9001 Test mode
  { "Tx2TestMode",                          0,                                                                                        PibTypeU8,        PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_VIRTUAL },
  { "Rx1TestMode",                          0,                                                                                        PibTypeU8,        PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_VIRTUAL },
  { "Rx2TestMode",                          0,                                                                                        PibTypeU8,        PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_VIRTUAL },
  { "Tx1TestData",                          offsetof(adrv9001_params_t, Tx1TestModeData),                                             PibTypeU32,       PIB_FLAGS_DEFAULT },                                                  // ADRV9001 IQ data when test mode = constant
  { "Tx2TestData",                          offsetof(adrv9001_params_t, Tx2TestModeData),                                             PibTypeU32,       PIB_FLAGS_DEFAULT },
  { "Rx1TestData",                          offsetof(adrv9001_params_t, Rx1TestModeData),                                             PibTypeU32,       PIB_FLAGS_DEFAULT },
  { "Rx2TestData",                          offsetof(adrv9001_params_t, Rx2TestModeData),                                             PibTypeU32,       PIB_FLAGS_DEFAULT },
  { "Tx1EnableDly",                         0,                                                                                        PibTypeU16,       PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_VIRTUAL },
  { "Tx2EnableDly",                         0,                                                                                        PibTypeU16,       PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_VIRTUAL },
  { "Rx1EnableDly",                         0,                                                                                        PibTypeU16,       PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_VIRTUAL },
  { "Rx2EnableDly",                         0,                                                                                        PibTypeU16,       PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_VIRTUAL },
  { "Tx1DisableDly",                        0,                                                                                        PibTypeU16,       PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_VIRTUAL },
  { "Tx2DisableDly",                        0,                                                                                        PibTypeU16,       PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_VIRTUAL },
  { "Rx1DisableDly",                        0,                                                                                        PibTypeU16,       PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_VIRTUAL },
  { "Rx2DisableDly",                        0,                                                                                        PibTypeU16,       PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_VIRTUAL },
  { "Tx1EnableMode",                        offsetof(adrv9001_params_t, Tx1EnableMode),                                               PibTypeU8,        PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_SET_ACTION },
  { "Tx2EnableMode",                        offsetof(adrv9001_params_t, Tx2EnableMode),                                               PibTypeU8,        PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_SET_ACTION },
  { "Rx1EnableMode",                        offsetof(adrv9001_params_t, Rx1EnableMode),                                               PibTypeU8,        PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_SET_ACTION },
  { "Rx2EnableMode",                        offsetof(adrv9001_params_t, Rx2EnableMode),                                               PibTypeU8,        PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_SET_ACTION },
  { "Rx1FrontendEnablePin",                 offsetof(adrv9001_params_t, Rx1FrontendEnablePin),                                        PibTypeU8,        PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_SET_ACTION },
  { "Rx2FrontendEnablePin",                 offsetof(adrv9001_params_t, Rx2FrontendEnablePin),                                        PibTypeU8,        PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_SET_ACTION },
  { "Tx1FrontendEnablePin",                 offsetof(adrv9001_params_t, Tx1FrontendEnablePin),                                        PibTypeU8,        PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_SET_ACTION },
  { "Tx2FrontendEnablePin",                 offsetof(adrv9001_params_t, Tx2FrontendEnablePin),                                        PibTypeU8,        PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_SET_ACTION },
  { "TcxoEnablePin",                        offsetof(adrv9001_params_t, TcxoEnablePin),                                               PibTypeU8,        PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_SET_ACTION },
  { "TcxoDacChannel",                       offsetof(adrv9001_params_t, TcxoDacChannel),                                              PibTypeU8,        PIB_FLAGS_DEFAULT | ADRV9001_PIB_FLAG_SET_ACTION },
};

static int32_t Adrv9001Pib_SetActionByNameByString( adrv9001_t *Instance, char *name, char *str )
{
  int32_t status = 0;
  adi_common_ChannelNumber_e Channel;
  adi_common_Port_e Port;
  int32_t id;

  /* Get ID */
  if((status = Pib_GetItemId( &Instance->Pib, name, &id )) != 0)
    return status;

  if(strncmp(name, "Tx1", 3) == 0 )
  {
    Port = ADI_TX;
    Channel = ADI_CHANNEL_1;
  }
  else if(strncmp(name, "Tx2", 3) == 0 )
  {
    Port = ADI_TX;
    Channel = ADI_CHANNEL_2;
  }
  else if(strncmp(name, "Rx1", 3) == 0 )
  {
    Port = ADI_RX;
    Channel = ADI_CHANNEL_1;
  }
  else if(strncmp(name, "Rx2", 3) == 0 )
  {
    Port = ADI_RX;
    Channel = ADI_CHANNEL_2;
  }

  if( strcmp( &name[3], "Attn") == 0 )
  {
    if( Channel == ADI_CHANNEL_1 )
      status = adi_adrv9001_Tx_Attenuation_Set(&Instance->Device, Channel, Instance->Params->Tx1Attn);
    else
      status = adi_adrv9001_Tx_Attenuation_Set(&Instance->Device, Channel, Instance->Params->Tx2Attn);
  }

  else if( strcmp( &name[3], "Boost") == 0 )
  {
    if( Channel == ADI_CHANNEL_1 )
      status = adi_adrv9001_Tx_OutputPowerBoost_Set(&Instance->Device, Channel, Instance->Params->Tx1Boost);
    else
      status = adi_adrv9001_Tx_OutputPowerBoost_Set(&Instance->Device, Channel, Instance->Params->Tx2Boost);
  }

  else if( strcmp( &name[3], "EnableMode") == 0 )
  {
	  if( (Port == ADI_TX) && (Channel == ADI_CHANNEL_1) )
	  {
		  status = Adrv9001_SetEnableMode(Instance, Port, Channel, Instance->Params->Tx1EnableMode);
	  }
	  else if( (Port == ADI_TX) && (Channel == ADI_CHANNEL_2) )
	  {
		  status = Adrv9001_SetEnableMode(Instance, Port, Channel, Instance->Params->Tx2EnableMode);
	  }
	  else if( (Port == ADI_RX) && (Channel == ADI_CHANNEL_1) )
	  {
		  status = Adrv9001_SetEnableMode(Instance, Port, Channel, Instance->Params->Rx1EnableMode);
	  }
	  else if( (Port == ADI_RX) && (Channel == ADI_CHANNEL_2) )
	  {
		  status = Adrv9001_SetEnableMode(Instance, Port, Channel, Instance->Params->Rx2EnableMode);
	  }
	  else
	  {
		  status = Adrv9001Status_InvalidPib;
	  }
  }
  else if( strcmp( &name[3], "FrontendEnablePin") == 0 )
  {
    if( (Port == ADI_TX) && (Channel == ADI_CHANNEL_1) )
    {
      if( Instance->Params->Tx1FrontendEnablePin != ADI_ADRV9001_GPIO_UNASSIGNED )
        if( adi_adrv9001_gpio_OutputPinLevel_Set(&Instance->Device, Instance->Params->Tx1FrontendEnablePin, ADI_ADRV9001_GPIO_PIN_LEVEL_LOW) != 0)
          return Adrv9001Status_GpioErr;
    }
    else if( (Port == ADI_TX) && (Channel == ADI_CHANNEL_2) )
    {
      if( Instance->Params->Tx2FrontendEnablePin != ADI_ADRV9001_GPIO_UNASSIGNED )
        if( adi_adrv9001_gpio_OutputPinLevel_Set(&Instance->Device, Instance->Params->Tx2FrontendEnablePin, ADI_ADRV9001_GPIO_PIN_LEVEL_LOW) != 0)
          return Adrv9001Status_GpioErr;
    }
    else if( (Port == ADI_RX) && (Channel == ADI_CHANNEL_1) )
    {
      if( Instance->Params->Rx1FrontendEnablePin != ADI_ADRV9001_GPIO_UNASSIGNED )
        if( adi_adrv9001_gpio_OutputPinLevel_Set(&Instance->Device, Instance->Params->Rx1FrontendEnablePin, ADI_ADRV9001_GPIO_PIN_LEVEL_LOW) != 0)
          return Adrv9001Status_GpioErr;
    }
    else if( (Port == ADI_RX) && (Channel == ADI_CHANNEL_2) )
    {
      if( Instance->Params->Rx2FrontendEnablePin != ADI_ADRV9001_GPIO_UNASSIGNED )
        if( adi_adrv9001_gpio_OutputPinLevel_Set(&Instance->Device, Instance->Params->Rx2FrontendEnablePin, ADI_ADRV9001_GPIO_PIN_LEVEL_LOW) != 0)
          return Adrv9001Status_GpioErr;
    }
    else
    {
      status = Adrv9001Status_InvalidPib;
    }
  }
  else if( strcmp( &name[3], "TcxoEnablePin") == 0 )
  {
    if( Instance->Params->TcxoEnablePin != ADI_ADRV9001_GPIO_UNASSIGNED )
      if( adi_adrv9001_gpio_OutputPinLevel_Set(&Instance->Device, Instance->Params->TcxoEnablePin, ADI_ADRV9001_GPIO_PIN_LEVEL_HIGH) != 0)
        return Adrv9001Status_GpioErr;
  }
  else if( strcmp( &name[3], "TcxoDacChannel") == 0 )
  {
    if(( status = Adrv9001_EnableDac(Instance, Instance->Params->TcxoDacChannel, true )) != 0 )
      return status;
  }
  
  else
  {
    status = Adrv9001Status_InvalidPib;
  }

  return Adrv9001Status_Success;
}

static int32_t Adrv9001Pib_SetVirtualByNameByString( adrv9001_t *Instance, char *name, char *str )
{
  int32_t status = 0;
  adi_common_ChannelNumber_e Channel;
  adi_common_Port_e Port;
  int32_t id;


  /* Get ID */
  if((status = Pib_GetItemId( &Instance->Pib, name, &id )) != 0)
    return status;

  if(strncmp(name, "Tx1", 3) == 0 )
  {
    Port = ADI_TX;
    Channel = ADI_CHANNEL_1;
  }
  else if(strncmp(name, "Tx2", 3) == 0 )
  {
    Port = ADI_TX;
    Channel = ADI_CHANNEL_2;
  }
  else if(strncmp(name, "Rx1", 3) == 0 )
  {
    Port = ADI_RX;
    Channel = ADI_CHANNEL_1;
  }
  else if(strncmp(name, "Rx2", 3) == 0 )
  {
    Port = ADI_RX;
    Channel = ADI_CHANNEL_2;
  }

  if( strcmp( &name[3], "CarrierFrequency") == 0 )
  {
    adi_adrv9001_Carrier_t *tmp;

    if( (Port == ADI_TX) && (Channel == ADI_CHANNEL_1) )
      if( Instance->Params->clocks.tx1LoSelect == ADI_ADRV9001_LOSEL_LO1 )
        tmp = &Instance->Params->Lo1Carrier;
      else
        tmp = &Instance->Params->Lo2Carrier;
    else if( (Port == ADI_TX) && (Channel == ADI_CHANNEL_2) )
      if( Instance->Params->clocks.tx2LoSelect == ADI_ADRV9001_LOSEL_LO1 )
        tmp = &Instance->Params->Lo1Carrier;
      else
        tmp = &Instance->Params->Lo2Carrier;
    else if( (Port == ADI_RX) && (Channel == ADI_CHANNEL_1) )
      if( Instance->Params->clocks.rx1LoSelect == ADI_ADRV9001_LOSEL_LO1 )
        tmp = &Instance->Params->Lo1Carrier;
      else
        tmp = &Instance->Params->Lo2Carrier;
    else if( (Port == ADI_RX) && (Channel == ADI_CHANNEL_2) )
      if( Instance->Params->clocks.rx2LoSelect == ADI_ADRV9001_LOSEL_LO1 )
        tmp = &Instance->Params->Lo1Carrier;
      else
        tmp = &Instance->Params->Lo2Carrier;
    else
      return Adrv9001Status_InvalidPib;

    if( adi_adrv9001_Radio_Carrier_Inspect( &Instance->Device, Port, Channel, tmp ) != 0)
      return Adrv9001Status_CarrierFreqErr;

    adi_adrv9001_ChannelState_e State;

    Pib_StrToNum(str, Instance->Pib.Def[id].var_type, &tmp->carrierFrequency_Hz);

    if( adi_adrv9001_Radio_Channel_State_Get( &Instance->Device, Port, Channel, &State ) != 0)
      return Adrv9001Status_InvalidState;

    if( State == ADI_ADRV9001_CHANNEL_CALIBRATED )
    {
      if( adi_adrv9001_Radio_Carrier_Configure(&Instance->Device, Port, Channel, tmp) != 0)
        return Adrv9001Status_CarrierFreqErr;
    }
    else
    {
      status = Adrv9001Status_InvalidState;
    }
  }

  else if( strcmp( &name[3], "IqData") == 0 )
  {
    uint32_t tmp;
    Pib_StrToNum(str, Instance->Pib.Def[id].var_type, &tmp);
    AxiAdrv9001_SetTxData(Instance->CtrlBase, Channel, tmp);
  }

  else if( strcmp( &name[3], "IqDataPath") == 0 )
  {
    axi_adrv9001_data_path_t tmp;
    Pib_StrToNum(str, Instance->Pib.Def[id].var_type, &tmp);
    AxiAdrv9001_SetTxDataPath(Instance->CtrlBase, Channel, tmp);
  }

  else if( strcmp( name, "Dgpio") == 0 )
  {
    uint32_t tmp;
    Pib_StrToNum(str, Instance->Pib.Def[id].var_type, &tmp);
    AxiAdrv9001_SetDgpio(Instance->CtrlBase, tmp);
  }

  else if( strcmp( name, "DgpioDir") == 0 )
  {
    uint32_t tmp;
    Pib_StrToNum(str, Instance->Pib.Def[id].var_type, &tmp);
    AxiAdrv9001_SetDgpioDir(Instance->CtrlBase, tmp);
  }

  else if( strcmp( name, "VcTcxo") == 0 )
  {
    float tmp;
    Pib_StrToNum(str, Instance->Pib.Def[id].var_type, &tmp);
    if((status = Adrv9001_SetVcTcxo(Instance, tmp)) != 0)
      return status;
  }

  else if( strcmp( &name[3], "RadioState") == 0 )
  {
    adi_adrv9001_ChannelState_e State;

    Pib_StrToNum(str, Instance->Pib.Def[id].var_type, &State);

    if( State == ADI_ADRV9001_CHANNEL_CALIBRATED )
      status = Adrv9001_ToCalibrated(Instance, Port, Channel);
    else if( State == ADI_ADRV9001_CHANNEL_PRIMED )
      status = Adrv9001_ToPrimed(Instance, Port, Channel);
    else if( State == ADI_ADRV9001_CHANNEL_RF_ENABLED )
      status = Adrv9001_ToRfEnabled(Instance, Port, Channel );
    else
      status = Adrv9001Status_InvalidState;

    if( status != 0 )
      return status;
  }

  else if( strcmp( name, "TxRx1SsiLoopBack") == 0 )
  {
    uint8_t tmp;
    Pib_StrToNum(str, Instance->Pib.Def[id].var_type, &tmp);
    if( adi_adrv9001_Ssi_Loopback_Set(&Instance->Device, ADI_CHANNEL_1, ADI_ADRV9001_SSI_TYPE_LVDS, tmp) != 0)
      return Adrv9001Status_SsiSetErr;
  }

  else if( strcmp( name, "TxRx2SsiLoopBack") == 0 )
  {
    uint8_t tmp;
    Pib_StrToNum(str, Instance->Pib.Def[id].var_type, &tmp);
    if( adi_adrv9001_Ssi_Loopback_Set(&Instance->Device, ADI_CHANNEL_2, ADI_ADRV9001_SSI_TYPE_LVDS, tmp) != 0)
      return Adrv9001Status_SsiSetErr;
  }

  else if( strcmp( &name[3], "TestMode") == 0 )
  {
    uint32_t tmp;

    Pib_StrToNum(str, Instance->Pib.Def[id].var_type, &tmp);

    if( Port == ADI_TX )
    {
      adi_adrv9001_TxSsiTestModeCfg_t Cfg = {
          .fixedDataPatternToCheck = (Channel == ADI_CHANNEL_1) ? Instance->Params->Tx1TestModeData : Instance->Params->Tx2TestModeData,
          .testData = tmp };

      if( adi_adrv9001_Ssi_Tx_TestMode_Configure(&Instance->Device, Channel, ADI_ADRV9001_SSI_TYPE_LVDS, ADI_ADRV9001_SSI_FORMAT_16_BIT_I_Q_DATA, &Cfg ) != 0)
        return Adrv9001Status_SsiTestModeErr;
    }
    else
    {
      adi_adrv9001_RxSsiTestModeCfg_t Cfg = {
          .fixedDataPatternToTransmit = (Channel == ADI_CHANNEL_1) ? Instance->Params->Rx1TestModeData : Instance->Params->Rx2TestModeData,
          .testData = tmp
      };

      if( adi_adrv9001_Ssi_Rx_TestMode_Configure(&Instance->Device, Channel, ADI_ADRV9001_SSI_TYPE_LVDS, ADI_ADRV9001_SSI_FORMAT_16_BIT_I_Q_DATA, &Cfg ) != 0)
        return Adrv9001Status_SsiTestModeErr;
    }
  }
  
  else if( strcmp( &name[3], "EnableDly") == 0 )
  {
    uint16_t tmp;
    Pib_StrToNum(str, Instance->Pib.Def[id].var_type, &tmp);    
		AxiAdrv9001_SetEnableDelay(Instance->CtrlBase, Port, Channel, tmp);
  }
  
  else if( strcmp( &name[3], "DisableDly") == 0 )
  {
    uint16_t tmp;
    Pib_StrToNum(str, Instance->Pib.Def[id].var_type, &tmp);    
    AxiAdrv9001_SetDisableDelay(Instance->CtrlBase, Port, Channel, tmp);
  } 

  else if( strcmp( &name[3], "TestData") == 0 )
  {
    uint32_t tmp;
    Pib_StrToNum(str, Instance->Pib.Def[id].var_type, &tmp);    
    AxiAdrv9001_SetTxData(Instance->CtrlBase, Channel, tmp);
  }   

  else
  {
	  status = Adrv9001Status_InvalidPib;
  }

  return Adrv9001Status_Success;
}

static int32_t Adrv9001Pib_GetVirtualStringByName( adrv9001_t *Instance, char *name, char *str )
{
  int32_t status = 0;
  adi_common_ChannelNumber_e Channel;
  adi_common_Port_e Port;
  int32_t id;


  /* Get ID */
  if((status = Pib_GetItemId( &Instance->Pib, name, &id )) != 0)
    return status;

  if(strncmp(name, "Tx1", 3) == 0 )
  {
    Port = ADI_TX;
    Channel = ADI_CHANNEL_1;
  }
  else if(strncmp(name, "Tx2", 3) == 0 )
  {
    Port = ADI_TX;
    Channel = ADI_CHANNEL_2;
  }
  else if(strncmp(name, "Rx1", 3) == 0 )
  {
    Port = ADI_RX;
    Channel = ADI_CHANNEL_1;
  }
  else if(strncmp(name, "Rx2", 3) == 0 )
  {
    Port = ADI_RX;
    Channel = ADI_CHANNEL_2;
  }

  if( strcmp( &name[3], "CarrierFrequency") == 0 )
  {
    adi_adrv9001_Carrier_t AdiCarrier;
    if( adi_adrv9001_Radio_Carrier_Inspect( &Instance->Device, Port, Channel, &AdiCarrier ) != 0)
      return Adrv9001Status_CarrierFreqErr;

    Pib_ValueToString( &Instance->Pib, id, (uint8_t*)&AdiCarrier.carrierFrequency_Hz, str );
  }

  else if( strcmp( &name[3], "IqData") == 0 )
  {
    uint32_t tmp;
    AxiAdrv9001_GetRxData(Instance->CtrlBase, Channel, &tmp );
    Pib_ValueToString( &Instance->Pib, id, (uint8_t*)&tmp, str );
  }

  else if( strcmp( &name[3], "IqDataPath") == 0 )
  {
    axi_adrv9001_data_path_t tmp;
    AxiAdrv9001_GetTxDataPath(Instance->CtrlBase, Channel, &tmp );
    Pib_ValueToString( &Instance->Pib, id, (uint8_t*)&tmp, str );
  }

  else if( strcmp( name, "Dgpio") == 0 )
  {
    uint32_t tmp;
    AxiAdrv9001_GetDgpio(Instance->CtrlBase, &tmp);
    Pib_ValueToString( &Instance->Pib, id, (uint8_t*)&tmp, str );
  }

  else if( strcmp( name, "DgpioDir") == 0 )
  {
    uint32_t tmp;
    AxiAdrv9001_GetDgpioDir(Instance->CtrlBase, &tmp);
    Pib_ValueToString( &Instance->Pib, id, (uint8_t*)&tmp, str );
  }

  else if( strcmp( &name[3], "RadioState") == 0 )
  {
    adi_adrv9001_ChannelState_e tmp;
    if( adi_adrv9001_Radio_Channel_State_Get( &Instance->Device, Port, Channel, &tmp ) != 0)
      return Adrv9001Status_InvalidState;

    Pib_ValueToString( &Instance->Pib, id, (uint8_t*)&tmp, str );
  }

  else if( strcmp( name, "SiliconVersion") == 0 )
  {
    adi_adrv9001_SiliconVersion_t tmp;
    if( adi_adrv9001_SiliconVersion_Get( &Instance->Device, &tmp ) != 0)
      return Adrv9001Status_ReadErr;

    sprintf(str, "%X%X", tmp.major, tmp.minor);
  }

  else if( strcmp( name, "FirmwareVersion") == 0 )
  {
    adi_adrv9001_ArmVersion_t tmp;
    if( adi_adrv9001_arm_Version( &Instance->Device, &tmp ) != 0)
      return Adrv9001Status_ReadErr;

    sprintf(str,"%u.%u.%u.%u", tmp.majorVer, tmp.minorVer, tmp.maintVer, tmp.rcVer );
  }

  else if( strcmp( name, "APIVersion") == 0 )
  {
    adi_common_ApiVersion_t tmp;
    if( adi_adrv9001_ApiVersion_Get( &Instance->Device, &tmp ) != 0)
      return Adrv9001Status_ReadErr;

    sprintf(str,"%lu.%lu.%lu\r\n\r\n", tmp.major, tmp.minor, tmp.patch);
  }

  else if( strcmp( name, "Temp") == 0 )
  {
    int16_t tmp;
    if( adi_adrv9001_Temperature_Get(&Instance->Device, &tmp) != 0 )
      return Adrv9001Status_ReadErr;

    Pib_ValueToString( &Instance->Pib, id, (uint8_t*)&tmp, str );
  }

  else if( strcmp( &name[3], "Rssi") == 0 )
  {
    uint32_t tmp;
    if( adi_adrv9001_Rx_Rssi_Read( &Instance->Device, Channel, &tmp ) != 0)
      return Adrv9001Status_ReadErr;

    float tmpf = ((float)tmp) / 1000;
    Pib_ValueToString( &Instance->Pib, id, (uint8_t*)&tmpf, str );
  }

  else if( strcmp( name, "VcTcxo") == 0 )
  {
    float tmp;
    if( Adrv9001_GetVcTcxo(Instance, &tmp) != 0)
      return Adrv9001Status_ReadErr;

    Pib_ValueToString( &Instance->Pib, id, (uint8_t*)&tmp, str );
  }

  else if( strcmp( &name[3], "CurGainIndex") == 0 )
  {
    uint8_t tmp;
    if( adi_adrv9001_Rx_Gain_Get(&Instance->Device, Channel, &tmp) != 0)
      return Adrv9001Status_ReadErr;

    Pib_ValueToString( &Instance->Pib, id, (uint8_t*)&tmp, str );
  }
  
  else if( strcmp( &name[3], "EnableDly") == 0 )
  {
    uint16_t tmp;    
		AxiAdrv9001_GetEnableDelay(Instance->CtrlBase, Port, Channel, &tmp);
    Pib_ValueToString( &Instance->Pib, id, (uint8_t*)&tmp, str );
  }
  
  else if( strcmp( &name[3], "DisableDly") == 0 )
  {
    uint16_t tmp;    
    AxiAdrv9001_GetDisableDelay(Instance->CtrlBase, Port, Channel, &tmp);
    Pib_ValueToString( &Instance->Pib, id, (uint8_t*)&tmp, str );
  }    

  else if( strcmp( &name[3], "TestData") == 0 )
  {
    uint32_t tmp;    
    AxiAdrv9001_GetRxData(Instance->CtrlBase, Channel, &tmp);
    Pib_ValueToString( &Instance->Pib, id, (uint8_t*)&tmp, str );    
  }  
  
  return status;
}

int32_t Adrv9001Pib_GetStringByName( adrv9001_t *Instance, char *name, char *str )
{
  int32_t id;
  int32_t status;

  /* Get ID */
  if((status = Pib_GetItemId( &Instance->Pib, name, &id )) != 0)
    return status;

  if( ( Instance->Pib.Def[id].flags & ADRV9001_PIB_FLAG_VIRTUAL ) == ADRV9001_PIB_FLAG_VIRTUAL )
  {
    status = Adrv9001Pib_GetVirtualStringByName( Instance, name, str );
  }
  else
  {
    status = Pib_GetStringByName( &Instance->Pib, name, str );
  }

  return status;
}

int32_t Adrv9001Pib_SetByNameByString( adrv9001_t *Instance, char *name, char *str )
{
  int32_t id;
  int32_t status;

  /* Get ID */
  if((status = Pib_GetItemId( &Instance->Pib, name, &id )) != 0)
    return status;

  if( (Instance->Pib.Def[id].flags & ADRV9001_PIB_FLAG_REBOOT ) == ADRV9001_PIB_FLAG_REBOOT )
    Instance->PendingReboot = 1;

  if( ( Instance->Pib.Def[id].flags & ADRV9001_PIB_FLAG_VIRTUAL ) == ADRV9001_PIB_FLAG_VIRTUAL )
  {
    status = Adrv9001Pib_SetVirtualByNameByString( Instance, name, str );
  }
  else
  {
    status = Pib_SetByNameByString( &Instance->Pib, name, str );

    if(( Instance->Pib.Def[id].flags & ADRV9001_PIB_FLAG_SET_ACTION ) == ADRV9001_PIB_FLAG_SET_ACTION )
      status = Adrv9001Pib_SetActionByNameByString( Instance, name, str );
  }

  return status;
}

int32_t Adrv9001Pib_Initialize( adrv9001_t *Instance )
{
  int32_t status;

  /* Create PIB Config */
  pib_init_t PibInit = {
    .Params             = &Adrv9001Params,
    .Defaults           = NULL,
    .PibLen             = sizeof(Adrv9001PibDef) / sizeof(pib_def_t),
    .ParamsSize         = sizeof(adrv9001_params_t),
    .Def                = Adrv9001PibDef,
  };

  /* Initialize PIB Instance */
  if((status = Pib_Init( &Instance->Pib, &PibInit )) != 0)
    return status;

  return status;
}





/** @} */
