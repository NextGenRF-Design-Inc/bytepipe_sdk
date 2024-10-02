/***************************************************************************//**
 *  \file       adrv9001.c
 *
 *  \details    This file contains the ADRV9001 implementation
 *
 *  \copyright
 *
 *  Copyright 2021(c) NextGen RF Design, Inc.
 *
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *   - Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   - Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *   - The use of this software may or may not infringe the patent rights of one
 *     or more patent holders.  This license does not release you from the
 *     requirement that you obtain separate licenses from these patent holders
 *     to use this software.
 *   - Use of the software either in source or binary form, must be run on or
 *     directly connected to a NextGen RF Design, Inc. product.
 *
 *  THIS SOFTWARE IS PROVIDED BY NEXTGEN RF DESIGN "AS IS" AND ANY EXPRESS OR
 *  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-INFRINGEMENT,
 *  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 *  EVENT SHALL NEXTGEN RF DESIGN BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  INTELLECTUAL PROPERTY RIGHTS, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************/
#include <stdio.h>
#include <string.h>
#include "adrv9001.h"
#include "initialize.h"
#include "calibrate.h"
#include "configure.h"
#include "sleep.h"
#include "math.h"


#define ADI_CHANNEL_RX1_INIT_MASK               (0x40)
#define ADI_CHANNEL_RX2_INIT_MASK               (0x80)
#define ADI_CHANNEL_TX1_INIT_MASK               (0x04)
#define ADI_CHANNEL_TX2_INIT_MASK               (0x08)


#define ADI_ADRV9001_RX_INIT_CALS (ADI_ADRV9001_INIT_CAL_RX_HPADC_FLASH | ADI_ADRV9001_INIT_CAL_RX_LPADC | ADI_ADRV9001_INIT_CAL_RX_TIA_CUTOFF | ADI_ADRV9001_INIT_CAL_RX_GROUP_DELAY | ADI_ADRV9001_INIT_CAL_RX_QEC_TCAL | ADI_ADRV9001_INIT_CAL_RX_QEC_FIC | ADI_ADRV9001_INIT_CAL_RX_RF_DC_OFFSET | ADI_ADRV9001_INIT_CAL_RX_GAIN_PATH_DELAY)
#define ADI_ADRV9001_TX_INIT_CALS (ADI_ADRV9001_INIT_CAL_TX_QEC | ADI_ADRV9001_INIT_CAL_TX_LO_LEAKAGE | ADI_ADRV9001_INIT_CAL_TX_LB_PD | ADI_ADRV9001_INIT_CAL_TX_BBAF | ADI_ADRV9001_INIT_CAL_TX_BBAF_GD | ADI_ADRV9001_INIT_CAL_TX_ATTEN_DELAY | ADI_ADRV9001_INIT_CAL_TX_DAC | ADI_ADRV9001_INIT_CAL_TX_PATH_DELAY)


static int32_t Adrv9001_ConfigureProfile( adrv9001_t *Instance, int32_t *Rx1ChfCoeff, int32_t *Rx2ChfCoeff )
{
  int32_t status;

  if((status = Adrv9001_ToPrimed( Instance, ADI_RX, ADI_CHANNEL_1 )) != 0)
    return Adrv9001Status_ToPrimedErr;

  if((status = Adrv9001_ToPrimed( Instance, ADI_RX, ADI_CHANNEL_2 )) != 0)
    return Adrv9001Status_ToPrimedErr;

  if((status = Adrv9001_ToPrimed( Instance, ADI_TX, ADI_CHANNEL_1 )) != 0)
    return Adrv9001Status_ToPrimedErr;

  if((status = Adrv9001_ToPrimed( Instance, ADI_TX, ADI_CHANNEL_2 )) != 0)
    return Adrv9001Status_ToPrimedErr;

  /* Calibrate SSI */
  if((status = Adrv9001_CalibrateSsiDelay( Instance, ADI_TX, ADI_CHANNEL_1, ADI_ADRV9001_SSI_TESTMODE_DATA_RAMP_16_BIT, ADRV9001_TEST_MODE_PATTERN )) != 0)
    printf("Adrv9001 Tx1 Failed SSI Calibration\r\n");

  if((status = Adrv9001_CalibrateSsiDelay( Instance, ADI_TX, ADI_CHANNEL_2, ADI_ADRV9001_SSI_TESTMODE_DATA_RAMP_16_BIT, ADRV9001_TEST_MODE_PATTERN )) != 0)
    printf("Adrv9001 Tx2 Failed SSI Calibration\r\n");

  if((status = Adrv9001_CalibrateSsiDelay( Instance, ADI_RX, ADI_CHANNEL_1, ADI_ADRV9001_SSI_TESTMODE_DATA_PRBS15, ADRV9001_TEST_MODE_PATTERN )) != 0)
    printf("Adrv9001 Rx1 Failed SSI Calibration\r\n");

  if((status = Adrv9001_CalibrateSsiDelay( Instance, ADI_RX, ADI_CHANNEL_2, ADI_ADRV9001_SSI_TESTMODE_DATA_PRBS15, ADRV9001_TEST_MODE_PATTERN )) != 0)
    printf("Adrv9001 Rx2 Failed SSI Calibration\r\n");

  printf("%s Version Information:\r\n","ADRV9002" );

  adi_adrv9001_SiliconVersion_t SiliconVer;
  if(adi_adrv9001_SiliconVersion_Get( &Instance->Device, &SiliconVer) == 0)
    printf("  -Silicon Version: %X%X\r\n", SiliconVer.major, SiliconVer.minor );
  else
    printf("  -Silicon Version: Failed\r\n" );

  adi_adrv9001_ArmVersion_t ArmVer;
  if(adi_adrv9001_arm_Version( &Instance->Device, &ArmVer) == 0)
    printf("  -Firmware Version: %u.%u.%u.%u\r\n",ArmVer.majorVer, ArmVer.minorVer, ArmVer.maintVer, ArmVer.armBuildType );
  else
    printf("  -Firmware Version: Failed\r\n");

  adi_common_ApiVersion_t ApiVer;
  if(adi_adrv9001_ApiVersion_Get( &Instance->Device, &ApiVer) == 0)
    printf("  -API Version: %lu.%lu.%lu\r\n\r\n", ApiVer.major, ApiVer.minor, ApiVer.patch );
  else
    printf("  -API Version: Failed\r\n\r\n");

  /* Set Enable Mode */
  if( Adrv9001_SetEnableMode(Instance, ADI_TX, ADI_CHANNEL_1, Instance->TxEnableMode) != 0)
    return Adrv9001Status_EnableModeErr;

  if( Adrv9001_SetEnableMode(Instance, ADI_TX, ADI_CHANNEL_2, Instance->TxEnableMode) != 0)
    return Adrv9001Status_EnableModeErr;

  if( Adrv9001_SetEnableMode(Instance, ADI_RX, ADI_CHANNEL_1, Instance->RxEnableMode) != 0)
    return Adrv9001Status_EnableModeErr;

  if( Adrv9001_SetEnableMode(Instance, ADI_RX, ADI_CHANNEL_2, Instance->RxEnableMode) != 0)
    return Adrv9001Status_EnableModeErr;


  if( Instance->Params->sysConfig.fhModeOn )
  {
    Instance->HopTableSize = ADRV9001_MAX_HOP_TABLE_SIZE;

    Instance->HopTable = calloc(1, sizeof(adi_adrv9001_FhHopFrame_t) * Instance->HopTableSize );

    if( Instance->HopTable == NULL )
      return Adrv9001Status_MemoryErr;

    uint64_t HopFreq = ADRV9001_HOP_TABLE_START_FREQ;

    for( int i = 0; i < ADRV9001_MAX_HOP_TABLE_SIZE; i++ )
    {
      Instance->HopTable[i].rx1GainIndex = 255;
      Instance->HopTable[i].rx2GainIndex = 255;
      Instance->HopTable[i].hopFrequencyHz = HopFreq;

      HopFreq = HopFreq + ADRV9001_HOP_TABLE_STEP_FREQ;
    }
  }
  else
  {
    Instance->HopTableSize = 0;
  }

  if( Rx1ChfCoeff != NULL )
  {
    Adrv9001_LoadRxChannelFilter( Instance, ADI_CHANNEL_1, Rx1ChfCoeff );
  }

  if( Rx2ChfCoeff != NULL )
  {
    Adrv9001_LoadRxChannelFilter( Instance, ADI_CHANNEL_2, Rx2ChfCoeff );
  }

  Instance->Initialized = 1;

  return Adrv9001Status_Success;
}

int32_t Adrv9001_ClearError( adrv9001_t *Instance )
{
  adi_common_ErrorClear(&Instance->Device.common);

  return Adrv9001Status_Success;
}

int32_t Adrv9001_LoadProfile( adrv9001_t *Instance, adrv9001_profile_t *Profile )
{
  int32_t status;

  /* Clear any errors */
  Adrv9001_ClearError( Instance );

  adi_adrv9001_RxProfile_t *Rx1Profile = &Profile->Init->rx.rxChannelCfg[ADI_ADRV9001_MAX_RX_CHANNEL_START].profile;
  adi_adrv9001_RxProfile_t *Rx2Profile = &Profile->Init->rx.rxChannelCfg[ADI_ADRV9001_MAX_RX_CHANNEL_START + 1].profile;
  adi_adrv9001_RxProfile_t *oRx1Profile = &Profile->Init->rx.rxChannelCfg[ADI_ADRV9001_MAX_ORX_CHANNEL_START].profile;
  adi_adrv9001_RxProfile_t *oRx2Profile = &Profile->Init->rx.rxChannelCfg[ADI_ADRV9001_MAX_ORX_CHANNEL_START + 1].profile;

  bool Tx1ProfileEnable = ((Profile->Init->tx.txInitChannelMask & ADI_ADRV9001_TX1 ) == ADI_ADRV9001_TX1 );
  bool Tx2ProfileEnable = ((Profile->Init->tx.txInitChannelMask & ADI_ADRV9001_TX2 ) == ADI_ADRV9001_TX2 );
  bool Rx1ProfileEnable = ((Profile->Init->rx.rxInitChannelMask & ADI_ADRV9001_RX1 ) == ADI_ADRV9001_RX1 );
  bool Rx2ProfileEnable = ((Profile->Init->rx.rxInitChannelMask & ADI_ADRV9001_RX2 ) == ADI_ADRV9001_RX2 );

  if( adi_adrv9001_HwClose(&Instance->Device) != 0)
    return Adrv9001Status_ProfileInitErr;

  adi_adrv9001_SpiSettings_t SpiSettings = {
      .msbFirst = 1,
      .enSpiStreaming = 0,
      .autoIncAddrUp = 1,
      .fourWireMode = 1,
      .cmosPadDrvStrength = ADI_ADRV9001_CMOSPAD_DRV_STRONG };

  if( adi_adrv9001_HwOpen(&Instance->Device, &SpiSettings) != 0)
    return Adrv9001Status_ProfileInitErr;

  if( adi_adrv9001_HwReset(&Instance->Device) != 0)
    return Adrv9001Status_ProfileInitErr;

  /* Initialize Analog */
  if( adi_adrv9001_InitAnalog(&Instance->Device, Profile->Init, ADI_ADRV9001_DEVICECLOCKDIVISOR_2) != 0)
    return Adrv9001Status_InitAnalogErr;

  if( adi_adrv9001_arm_AhbSpiBridge_Enable(&Instance->Device) != 0)
    return Adrv9001Status_SpiErr;

  if( adi_adrv9001_Stream_Image_Write(&Instance->Device, 0, Profile->StreamImageBuf, 32768, ADI_ADRV9001_ARM_SINGLE_SPI_WRITE_MODE_STANDARD_BYTES_252) != 0)
    return Adrv9001Status_StreamImageWriteErr;

  if( adi_adrv9001_arm_Image_Write(&Instance->Device, 0, initialize_binary_10, 310272, ADI_ADRV9001_ARM_SINGLE_SPI_WRITE_MODE_STANDARD_BYTES_252) != 0)
    return Adrv9001Status_ArmImageWriteErr;

  if( adi_adrv9001_arm_Profile_Write(&Instance->Device, Profile->Init) != 0)
    return Adrv9001Status_ProfileWriteErr;

  if( adi_adrv9001_arm_PfirProfiles_Write(&Instance->Device, Profile->Init) != 0)
    return Adrv9001Status_PfirErr;


  /* Configure Rx Gain Tables */
  if( adi_adrv9001_Rx_GainTable_Write(&Instance->Device, ADI_ORX, ADI_CHANNEL_1, 14, Profile->oRx1GainTable, 13, &oRx1Profile->lnaConfig, oRx1Profile->gainTableType) != 0)
    return Adrv9001Status_RxGainTableErr;

  if( adi_adrv9001_Rx_GainTable_Write(&Instance->Device, ADI_ORX, ADI_CHANNEL_2, 14, Profile->oRx2GainTable, 13, &oRx2Profile->lnaConfig, oRx2Profile->gainTableType) != 0)
    return Adrv9001Status_RxGainTableErr;

  if( adi_adrv9001_Rx_GainTable_Write(&Instance->Device, ADI_RX, ADI_CHANNEL_1, 255, Profile->Rx1GainTable, 69, &Rx1Profile->lnaConfig, Rx1Profile->gainTableType) != 0)
    return Adrv9001Status_RxGainTableErr;

  if( adi_adrv9001_Rx_GainTable_Write(&Instance->Device, ADI_RX, ADI_CHANNEL_2, 255, Profile->Rx2GainTable, 69, &Rx2Profile->lnaConfig, Rx2Profile->gainTableType) != 0)
    return Adrv9001Status_RxGainTableErr;


  /* Configure TX Attenuation Table */
  if( adi_adrv9001_Tx_AttenuationTable_Write(&Instance->Device, 3, 0, Profile->TxAttnTable, 960) != 0)
    return Adrv9001Status_TxAttnErr;

  /* Configure Power Management Settings */
  adi_adrv9001_PowerManagementSettings_t PowerManagementSettings = {
    .ldoPowerSavingModes = { ADI_ADRV9001_LDO_POWER_SAVING_MODE_1, ADI_ADRV9001_LDO_POWER_SAVING_MODE_1, ADI_ADRV9001_LDO_POWER_SAVING_MODE_1, ADI_ADRV9001_LDO_POWER_SAVING_MODE_1, ADI_ADRV9001_LDO_POWER_SAVING_MODE_1, ADI_ADRV9001_LDO_POWER_SAVING_MODE_1, ADI_ADRV9001_LDO_POWER_SAVING_MODE_1, ADI_ADRV9001_LDO_POWER_SAVING_MODE_1, ADI_ADRV9001_LDO_POWER_SAVING_MODE_1, ADI_ADRV9001_LDO_POWER_SAVING_MODE_1, ADI_ADRV9001_LDO_POWER_SAVING_MODE_1, ADI_ADRV9001_LDO_POWER_SAVING_MODE_1, ADI_ADRV9001_LDO_POWER_SAVING_MODE_1, ADI_ADRV9001_LDO_POWER_SAVING_MODE_1, ADI_ADRV9001_LDO_POWER_SAVING_MODE_1, ADI_ADRV9001_LDO_POWER_SAVING_MODE_1, ADI_ADRV9001_LDO_POWER_SAVING_MODE_1, ADI_ADRV9001_LDO_POWER_SAVING_MODE_1, ADI_ADRV9001_LDO_POWER_SAVING_MODE_1  } };

  if( adi_adrv9001_powermanagement_Configure(&Instance->Device, &PowerManagementSettings) != 0)
    return Adrv9001Status_PowerMgmtErr;


  /* Start ARM processor */
  if( adi_adrv9001_arm_Start(&Instance->Device) != 0)
    return Adrv9001Status_ArmErr;

  if( adi_adrv9001_arm_StartStatus_Check(&Instance->Device, 5000000) != 0)
    return Adrv9001Status_ArmErr;

  /* Configure Boost */
  if(adi_adrv9001_Tx_OutputPowerBoost_Set(&Instance->Device, ADI_CHANNEL_1, Profile->Tx1Boost) != 0)
    return Adrv9001Status_TxBoostErr;

  if(adi_adrv9001_Tx_OutputPowerBoost_Set(&Instance->Device, ADI_CHANNEL_2, Profile->Tx2Boost) != 0)
    return Adrv9001Status_TxBoostErr;

  /* Configure PLL Loop Filters */
  struct adi_adrv9001_PllLoopFilterCfg pll_loop_filter = {
    .effectiveLoopBandwidth_kHz = 0,
    .loopBandwidth_kHz = 300,
    .phaseMargin_degrees = 60,
    .powerScale = 5
  };

  if( adi_adrv9001_Radio_PllLoopFilter_Set(&Instance->Device, ADI_ADRV9001_PLL_LO1, &pll_loop_filter ) != 0)
    return Adrv9001Status_PllErr;

  if( adi_adrv9001_Radio_PllLoopFilter_Set(&Instance->Device, ADI_ADRV9001_PLL_LO2, &pll_loop_filter ) != 0)
    return Adrv9001Status_PllErr;

  if( adi_adrv9001_Radio_PllLoopFilter_Set(&Instance->Device, ADI_ADRV9001_PLL_AUX, &pll_loop_filter ) != 0)
    return Adrv9001Status_PllErr;

  adi_adrv9001_PowerSavingAndMonitorMode_MonitorModeRssiCfg_t MonitorModeRssiCfg = {
    .numberOfMeasurementsToAverage = 4,
    .measurementsStartPeriod_ms = 1,
    .detectionThreshold_mdBFS = -80000,
    .measurementDuration_samples = 10 };

  if( adi_adrv9001_powerSavingAndMonitorMode_MonitorMode_Rssi_Configure(&Instance->Device, &MonitorModeRssiCfg) != 0)
    return Adrv9001Status_PowerMgmtErr;

  adi_adrv9001_PowerSavingAndMonitorMode_MonitorModeDmrSearchCfg_t DmrSearchCfg = {
    .pathDelay = 0,
    .magcorrTh = 250,
    .detCnt1 = 375,
    .detCnt2 = 500,
    .detTgtMin = 880000 };

  if( adi_adrv9001_powerSavingAndMonitorMode_MonitorMode_DmrSearch_Configure(&Instance->Device, &DmrSearchCfg) != 0)
    return Adrv9001Status_PowerMgmtErr;

  adi_adrv9001_RxPortSwitchCfg_t RxPortSwitchCfg = {
    .minFreqPortA_Hz = 890000000,
    .maxFreqPortA_Hz = 910000000,
    .minFreqPortB_Hz = 1890000000,
    .maxFreqPortB_Hz = 1910000000,
    .enable = false,
    .manualRxPortSwitch = false };

  if( adi_adrv9001_Rx_PortSwitch_Configure(&Instance->Device, &RxPortSwitchCfg) != 0)
    return Adrv9001Status_PowerMgmtErr;


  if( Rx1ProfileEnable )
  {
    adi_adrv9001_Carrier_t Rx1Carrier = {
        .loGenOptimization = Profile->Rx1LoGenOptimization,
        .carrierFrequency_Hz = Profile->Rx1CarrierFreqHz,
        .intermediateFrequency_Hz = Profile->Rx1IfFreqHz,
        .manualRxport = Profile->Init->clocks.rx1RfInputSel };

    if( adi_adrv9001_Radio_Carrier_Configure(&Instance->Device, ADI_RX, ADI_CHANNEL_1, &Rx1Carrier) != 0)
      return Adrv9001Status_CarrierFreqErr;

    adi_adrv9001_ChannelEnablementDelays_t Rx1EnablementDelay = {
        .riseToOnDelay = 0,
        .riseToAnalogOnDelay = 0,
        .fallToOffDelay = 0,
        .guardDelay = 0,
        .holdDelay = 0 };

    if( adi_adrv9001_Radio_ChannelEnablementDelays_Configure(&Instance->Device, ADI_RX, ADI_CHANNEL_1, &Rx1EnablementDelay) != 0)
      return Adrv9001Status_PathDelayErr;

    if( adi_adrv9001_Rx_AdcSwitchEnable_Set(&Instance->Device, ADI_CHANNEL_1, false) != 0)
      return Adrv9001Status_AdcErr;

    if( adi_adrv9001_Ssi_PowerDown_Set(&Instance->Device, ADI_RX, ADI_CHANNEL_1, ADI_ADRV9001_SSI_POWER_DOWN_DISABLED) != 0)
      return Adrv9001Status_PowerMgmtErr;
  }

  if( Rx2ProfileEnable )
  {
    adi_adrv9001_Carrier_t Rx2Carrier = {
        .loGenOptimization = Profile->Rx2LoGenOptimization,
        .carrierFrequency_Hz = Profile->Rx2CarrierFreqHz,
        .intermediateFrequency_Hz = Profile->Rx2IfFreqHz,
        .manualRxport = Profile->Init->clocks.rx2RfInputSel };

    if( adi_adrv9001_Radio_Carrier_Configure(&Instance->Device, ADI_RX, ADI_CHANNEL_2, &Rx2Carrier) != 0)
      return Adrv9001Status_CarrierFreqErr;

    adi_adrv9001_ChannelEnablementDelays_t Rx2EnablementDelay = {
        .riseToOnDelay = 0,
        .riseToAnalogOnDelay = 0,
        .fallToOffDelay = 0,
        .guardDelay = 0,
        .holdDelay = 0 };

    if( adi_adrv9001_Radio_ChannelEnablementDelays_Configure(&Instance->Device, ADI_RX, ADI_CHANNEL_2, &Rx2EnablementDelay) != 0)
      return Adrv9001Status_PathDelayErr;

    if( adi_adrv9001_Rx_AdcSwitchEnable_Set(&Instance->Device, ADI_CHANNEL_2, false) != 0)
      return Adrv9001Status_AdcErr;

    if( adi_adrv9001_Ssi_PowerDown_Set(&Instance->Device, ADI_RX, ADI_CHANNEL_2, ADI_ADRV9001_SSI_POWER_DOWN_DISABLED) != 0)
      return Adrv9001Status_PowerMgmtErr;
  }

  if( Tx1ProfileEnable )
  {
    adi_adrv9001_Carrier_t Tx1Carrier = {
        .loGenOptimization = Profile->Tx1LoGenOptimization,
        .carrierFrequency_Hz = Profile->Tx1CarrierFreqHz,
        .intermediateFrequency_Hz = Profile->Tx1IfFreqHz,
        .manualRxport = ADI_ADRV9001_RX_A};

    if( adi_adrv9001_Radio_Carrier_Configure(&Instance->Device, ADI_TX, ADI_CHANNEL_1, &Tx1Carrier) != 0)
      return Adrv9001Status_CarrierFreqErr;

    adi_adrv9001_ChannelEnablementDelays_t Tx1EnablementDelay = {
        .riseToOnDelay = 0,
        .riseToAnalogOnDelay = 0,
        .fallToOffDelay = 0,
        .guardDelay = 0,
        .holdDelay = 0 };

    if( adi_adrv9001_Radio_ChannelEnablementDelays_Configure(&Instance->Device, ADI_TX, ADI_CHANNEL_1, &Tx1EnablementDelay) != 0)
      return Adrv9001Status_PathDelayErr;

    if( adi_adrv9001_cals_ExternalPathDelay_Set(&Instance->Device, ADI_CHANNEL_1, 0) != 0)
      return Adrv9001Status_PathDelayErr;

    if( adi_adrv9001_Ssi_PowerDown_Set(&Instance->Device, ADI_TX, ADI_CHANNEL_1, ADI_ADRV9001_SSI_POWER_DOWN_DISABLED) != 0)
      return Adrv9001Status_PowerMgmtErr;

    adi_adrv9001_SlewRateLimiterCfg_t Tx1SlewRateLimiterCfg = {
      .srlEnable = false,
      .srlStatisticsEnable = false,
      .srlTableSelect = ADI_ADRV9001_SRL_TABLE0,
      .srlSlewOffset = 0,
      .srlStatisticsMode = ADI_ADRV9001_SRL_STATISTICS_MIN_SLEW_FACTOR_OBSERVED };

    if( adi_adrv9001_Tx_SlewRateLimiter_Configure(&Instance->Device, ADI_CHANNEL_1, &Tx1SlewRateLimiterCfg) != 0)
      return Adrv9001Status_SlewRateLimiterErr;
  }

  if( Tx2ProfileEnable )
  {
    adi_adrv9001_Carrier_t Tx2Carrier = {
        .loGenOptimization = Profile->Tx2LoGenOptimization,
        .carrierFrequency_Hz = Profile->Tx2CarrierFreqHz,
        .intermediateFrequency_Hz = Profile->Tx2IfFreqHz,
        .manualRxport = ADI_ADRV9001_RX_A };

    if( adi_adrv9001_Radio_Carrier_Configure(&Instance->Device, ADI_TX, ADI_CHANNEL_2, &Tx2Carrier) != 0)
      return Adrv9001Status_CarrierFreqErr;

    adi_adrv9001_ChannelEnablementDelays_t Tx2EnablementDelay = {
        .riseToOnDelay = 0,
        .riseToAnalogOnDelay = 0,
        .fallToOffDelay = 0,
        .guardDelay = 0,
        .holdDelay = 0 };

    if( adi_adrv9001_Radio_ChannelEnablementDelays_Configure(&Instance->Device, ADI_TX, ADI_CHANNEL_2, &Tx2EnablementDelay) != 0)
      return Adrv9001Status_PathDelayErr;

    if( adi_adrv9001_cals_ExternalPathDelay_Set(&Instance->Device, ADI_CHANNEL_2, 0) != 0)
      return Adrv9001Status_PathDelayErr;

    if( adi_adrv9001_Ssi_PowerDown_Set(&Instance->Device, ADI_TX, ADI_CHANNEL_2, ADI_ADRV9001_SSI_POWER_DOWN_DISABLED) != 0)
      return Adrv9001Status_PowerMgmtErr;

    adi_adrv9001_SlewRateLimiterCfg_t Tx2SlewRateLimiterCfg = {
      .srlEnable = false,
      .srlStatisticsEnable = false,
      .srlTableSelect = ADI_ADRV9001_SRL_TABLE0,
      .srlSlewOffset = 0,
      .srlStatisticsMode = ADI_ADRV9001_SRL_STATISTICS_MIN_SLEW_FACTOR_OBSERVED };

    if( adi_adrv9001_Tx_SlewRateLimiter_Configure(&Instance->Device, ADI_CHANNEL_2, &Tx2SlewRateLimiterCfg) != 0)
      return Adrv9001Status_SlewRateLimiterErr;
  }

  /* Set PLL Configuration */
  adi_adrv9001_PllConfig_t Lo1PllConfig = {
      .pllCalibration = Profile->Lo1PllCalibration,
      .pllPower = Profile->Lo1PllPower };

  if( adi_adrv9001_Radio_Pll_Configure(&Instance->Device, ADI_ADRV9001_PLL_LO1, &Lo1PllConfig) != 0)
    return Adrv9001Status_PllErr;

  adi_adrv9001_PllConfig_t Lo2PllConfig = {
      .pllCalibration = Profile->Lo2PllCalibration,
      .pllPower = Profile->Lo2PllPower };

  if( adi_adrv9001_Radio_Pll_Configure(&Instance->Device, ADI_ADRV9001_PLL_LO2, &Lo2PllConfig) != 0)
    return Adrv9001Status_PllErr;

  /* Configure MCS Delay */
  adi_adrv9001_McsDelay_t McsDelay = {
    .readDelay = 5,
    .sampleDelay = 0 };

  if( adi_adrv9001_Mcs_ChannelMcsDelay_Set(&Instance->Device, ADI_RX, ADI_CHANNEL_1, &McsDelay) != 0)
    return Adrv9001Status_PathDelayErr;

  if( adi_adrv9001_Mcs_ChannelMcsDelay_Set(&Instance->Device, ADI_RX, ADI_CHANNEL_2, &McsDelay) != 0)
    return Adrv9001Status_PathDelayErr;

  if( adi_adrv9001_Mcs_ChannelMcsDelay_Set(&Instance->Device, ADI_TX, ADI_CHANNEL_1, &McsDelay) != 0)
    return Adrv9001Status_PathDelayErr;

  if( adi_adrv9001_Mcs_ChannelMcsDelay_Set(&Instance->Device, ADI_TX, ADI_CHANNEL_2, &McsDelay) != 0)
    return Adrv9001Status_PathDelayErr;

  /* Configure ARM */
  if( adi_adrv9001_arm_System_Program(&Instance->Device, 15) != 0)
    return Adrv9001Status_ArmErr;

  /* Configure Rx Loid */
  adi_adrv9001_RxrfdcLoidCfg_t LoidCfg = {
    .loidEnable = false,
    .loidThreshold_negdBFS = 61 };

  if( adi_adrv9001_Rx_Loid_Configure(&Instance->Device, ADI_CHANNEL_1, &LoidCfg) != 0)
    return Adrv9001Status_LoidErr;

  if( adi_adrv9001_Rx_Loid_Configure(&Instance->Device, ADI_CHANNEL_2, &LoidCfg) != 0)
    return Adrv9001Status_LoidErr;

  /* Configure AGC */
  if( adi_adrv9001_Rx_GainControl_Configure(&Instance->Device, ADI_CHANNEL_1, Profile->Rx1Agc) != 0)
    return Adrv9001Status_AgcErr;

  if( adi_adrv9001_Rx_GainControl_Configure(&Instance->Device, ADI_CHANNEL_2, Profile->Rx2Agc) != 0)
    return Adrv9001Status_AgcErr;

  /* Configure IRQ Sources */
  if( adi_adrv9001_gpio_GpIntMask_Set(&Instance->Device, 16) != 0)
    return Adrv9001Status_IrqErr;

  /* Configure Attenuation */
  if( adi_adrv9001_Tx_Attenuation_PinControl_Configure(&Instance->Device, ADI_CHANNEL_1, &Profile->Tx1AttnPinMode) != 0)
    return Adrv9001Status_TxAttnErr;

  if( adi_adrv9001_Tx_Attenuation_PinControl_Configure(&Instance->Device, ADI_CHANNEL_2, &Profile->Tx2AttnPinMode) != 0)
    return Adrv9001Status_TxAttnErr;

  if( adi_adrv9001_Tx_Attenuation_Set(&Instance->Device, ADI_CHANNEL_1, 0) != 0)
    return Adrv9001Status_TxAttnErr;

  if( adi_adrv9001_Tx_Attenuation_Set(&Instance->Device, ADI_CHANNEL_2, 0) != 0)
    return Adrv9001Status_TxAttnErr;

  /* Configure Interface Gain */
  adi_adrv9001_RxInterfaceGainCtrl_t RxInterfaceGainCfg = {
    .updateInstance = ADI_ADRV9001_RX_INTERFACE_GAIN_UPDATE_TIMING_NOW,
    .controlMode = ADI_ADRV9001_RX_INTERFACE_GAIN_CONTROL_MANUAL,
    .gain = ADI_ADRV9001_RX_INTERFACE_GAIN_0_DB,
    .rssiDuration = 255,
    .rssiMovingAverageDuration = 10,
    .gainControlAutomaticThreshold_dBFS = -2,
    .signalPAR = 15,
    .enableFastAttack = false };

  if( adi_adrv9001_Rx_InterfaceGain_Configure(&Instance->Device, ADI_CHANNEL_1, &RxInterfaceGainCfg) != 0)
    return Adrv9001Status_AgcErr;

  if( adi_adrv9001_Rx_InterfaceGain_Configure(&Instance->Device, ADI_CHANNEL_2, &RxInterfaceGainCfg) != 0)
    return Adrv9001Status_AgcErr;

  /* Configure Power Savings Monitor Mode */
  adi_adrv9001_PowerSavingAndMonitorMode_ChannelPowerSavingCfg_t PowerSavingCfg = {
    .channelDisabledPowerDownMode = ADI_ADRV9001_POWERSAVINGANDMONITORMODE_CHANNEL_MODE_DISABLED,
    .gpioPinPowerDownMode = ADI_ADRV9001_POWERSAVINGANDMONITORMODE_CHANNEL_MODE_DISABLED };

  if( adi_adrv9001_powerSavingAndMonitorMode_ChannelPowerSaving_Configure(&Instance->Device, ADI_CHANNEL_1, &PowerSavingCfg) != 0)
    return Adrv9001Status_PowerMgmtErr;

  if( adi_adrv9001_powerSavingAndMonitorMode_ChannelPowerSaving_Configure(&Instance->Device, ADI_CHANNEL_2, &PowerSavingCfg) != 0)
    return Adrv9001Status_PowerMgmtErr;

  /* Configure Rx enable delays */
  adi_adrv9001_ChannelEnablementDelays_t RxEnablementDelays = {
    .riseToOnDelay = 139715,
    .riseToAnalogOnDelay = 0,
    .fallToOffDelay = 0,
    .guardDelay = 0,
    .holdDelay = 373 };

  if( adi_adrv9001_Radio_ChannelEnablementDelays_Configure(&Instance->Device, ADI_RX, ADI_CHANNEL_1, &RxEnablementDelays) != 0)
    return Adrv9001Status_PathDelayErr;

  if( adi_adrv9001_Radio_ChannelEnablementDelays_Configure(&Instance->Device, ADI_RX, ADI_CHANNEL_2, &RxEnablementDelays) != 0)
      return Adrv9001Status_PathDelayErr;

  /* Configure Tx enable delays */
  adi_adrv9001_ChannelEnablementDelays_t TxEnablementDelays = {
    .riseToOnDelay = 139715,
    .riseToAnalogOnDelay = 0,
    .fallToOffDelay = 343,
    .guardDelay = 0,
    .holdDelay = 0 };

  if( adi_adrv9001_Radio_ChannelEnablementDelays_Configure(&Instance->Device, ADI_TX, ADI_CHANNEL_1, &TxEnablementDelays) != 0)
    return Adrv9001Status_PathDelayErr;

  if( adi_adrv9001_Radio_ChannelEnablementDelays_Configure(&Instance->Device, ADI_TX, ADI_CHANNEL_2, &TxEnablementDelays) != 0)
    return Adrv9001Status_PathDelayErr;

  /* Configure BBDC Loop gain */
  if( adi_adrv9010_bbdc_LoopGain_Set(&Instance->Device, ADI_CHANNEL_1, 65536) != 0)
    return Adrv9001Status_BbdcErr;

  if( adi_adrv9010_bbdc_LoopGain_Set(&Instance->Device, ADI_CHANNEL_2, 65536) != 0)
    return Adrv9001Status_BbdcErr;

  /* Configure Tracking Cals */
  adi_adrv9001_TrackingCals_t TrackingCals = {
    .chanTrackingCalMask = { ADI_ADRV9001_TRACKING_CAL_TX_QEC | ADI_ADRV9001_TRACKING_CAL_TX_LO_LEAKAGE | ADI_ADRV9001_TRACKING_CAL_RX_QEC_WBPOLY | ADI_ADRV9001_TRACKING_CAL_ORX_QEC_WBPOLY | ADI_ADRV9001_TRACKING_CAL_RX_BBDC | ADI_ADRV9001_TRACKING_CAL_RX_RFDC | ADI_ADRV9001_TRACKING_CAL_RX_QEC_FIC | ADI_ADRV9001_TRACKING_CAL_RX_GAIN_CONTROL_DETECTORS | ADI_ADRV9001_TRACKING_CAL_RX_RSSI, ADI_ADRV9001_TRACKING_CAL_TX_QEC | ADI_ADRV9001_TRACKING_CAL_TX_LO_LEAKAGE | ADI_ADRV9001_TRACKING_CAL_RX_QEC_WBPOLY | ADI_ADRV9001_TRACKING_CAL_ORX_QEC_WBPOLY | ADI_ADRV9001_TRACKING_CAL_RX_BBDC | ADI_ADRV9001_TRACKING_CAL_RX_RFDC | ADI_ADRV9001_TRACKING_CAL_RX_QEC_FIC | ADI_ADRV9001_TRACKING_CAL_RX_GAIN_CONTROL_DETECTORS | ADI_ADRV9001_TRACKING_CAL_RX_RSSI  } };

  if( adi_adrv9001_cals_Tracking_Set(&Instance->Device, &TrackingCals) != 0)
    return Adrv9001Status_TrackingCalsErr;

  /* Configure Rx Gain Control */
  if( adi_adrv9001_Rx_GainControl_Mode_Set(&Instance->Device, ADI_CHANNEL_1, Profile->Rx1AgcCtrlMode ) != 0)
    return Adrv9001Status_AgcErr;

  if( Profile->Rx1AgcCtrlMode == ADI_ADRV9001_RX_GAIN_CONTROL_MODE_SPI )
  {
    if( adi_adrv9001_Rx_Gain_Set(&Instance->Device, ADI_CHANNEL_1, 247) != 0)
      return Adrv9001Status_AgcErr;
  }
  else
  {
    if( adi_adrv9001_Rx_GainControl_PinMode_Configure(&Instance->Device, ADI_CHANNEL_1, &Profile->Rx1AgcPinMode) != 0)
      return Adrv9001Status_AgcErr;
  }

  if( adi_adrv9001_Rx_GainControl_Mode_Set(&Instance->Device, ADI_CHANNEL_2, Profile->Rx2AgcCtrlMode) != 0)
    return Adrv9001Status_AgcErr;

  if( Profile->Rx2AgcCtrlMode == ADI_ADRV9001_RX_GAIN_CONTROL_MODE_SPI )
  {
    if( adi_adrv9001_Rx_Gain_Set(&Instance->Device, ADI_CHANNEL_2, 247) != 0)
      return Adrv9001Status_AgcErr;
  }
  else
  {
    if( adi_adrv9001_Rx_GainControl_PinMode_Configure(&Instance->Device, ADI_CHANNEL_2, &Profile->Rx2AgcPinMode) != 0)
      return Adrv9001Status_AgcErr;
  }

  /* Calibrate */
  adi_adrv9001_InitCals_t InitCals = {
    .sysInitCalMask = (adi_adrv9001_InitCalibrations_e) 0,
    .chanInitCalMask = { 0, 0},
    .calMode = ADI_ADRV9001_INIT_CAL_MODE_ALL,
    .force = false };

  if( Rx1ProfileEnable )
    InitCals.chanInitCalMask[0] |= ADI_ADRV9001_RX_INIT_CALS;

  if( Tx1ProfileEnable )
    InitCals.chanInitCalMask[0] |= ADI_ADRV9001_TX_INIT_CALS;

  if( Rx2ProfileEnable )
    InitCals.chanInitCalMask[1] |= ADI_ADRV9001_RX_INIT_CALS;

  if( Tx2ProfileEnable )
    InitCals.chanInitCalMask[1] |= ADI_ADRV9001_TX_INIT_CALS;

  uint8_t InitCalsError = 0;
  if( adi_adrv9001_cals_InitCals_Run(&Instance->Device, &InitCals, 300000, &InitCalsError) != 0)
    return Adrv9001Status_InitCalsErr;

  if( InitCalsError != 0 )
    return Adrv9001Status_InitCalsErr;

  /* Disable Loopbacks */
  if( adi_adrv9001_Tx_DataPath_Loopback_Set(&Instance->Device, ADI_CHANNEL_1, false) != 0)
    return Adrv9001Status_LoopBackErr;

  if( adi_adrv9001_Ssi_Loopback_Set(&Instance->Device, ADI_CHANNEL_1, ADI_ADRV9001_SSI_TYPE_LVDS, false) != 0)
    return Adrv9001Status_LoopBackErr;

  if( adi_adrv9001_Tx_DataPath_Loopback_Set(&Instance->Device, ADI_CHANNEL_2, false) != 0)
    return Adrv9001Status_LoopBackErr;

  if( adi_adrv9001_Ssi_Loopback_Set(&Instance->Device, ADI_CHANNEL_2, ADI_ADRV9001_SSI_TYPE_LVDS, false) != 0)
    return Adrv9001Status_LoopBackErr;

  /* Disable Aux DAC by default */
  if( adi_adrv9001_AuxDac_Configure(&Instance->Device, ADI_ADRV9001_AUXDAC0, false) != 0)
    return Adrv9001Status_DacErr;

  if( adi_adrv9001_AuxDac_Configure(&Instance->Device, ADI_ADRV9001_AUXDAC1, false) != 0)
    return Adrv9001Status_DacErr;

  if( adi_adrv9001_AuxDac_Configure(&Instance->Device, ADI_ADRV9001_AUXDAC2, false) != 0)
    return Adrv9001Status_DacErr;

  if( adi_adrv9001_AuxDac_Configure(&Instance->Device, ADI_ADRV9001_AUXDAC3, false) != 0)
    return Adrv9001Status_DacErr;

  /* Configure Profile */
  if((status = Adrv9001_ConfigureProfile( Instance, NULL, NULL )) != 0)
    return status;

	  return Adrv9001Status_Success;
}

int32_t Adrv9001_LoadRxChannelFilter( adrv9001_t *Instance, adi_common_ChannelNumber_e channel, int32_t *Coeff )
{
  adi_adrv9001_PfirWbNbBuffer_t pfirCoeff = {
      .numCoeff = 128,
      .symmetricSel = ADI_ADRV9001_PFIR_COEF_NON_SYMMETRIC,
      .tapsSel = ADI_ADRV9001_PFIR_128_TAPS,
      .gainSel = ADI_ADRV9001_PFIR_GAIN_ZERO_DB,
  };

  memcpy((uint8_t*)pfirCoeff.coefficients, (uint8_t*)Coeff, sizeof( pfirCoeff.coefficients) );

  uint8_t mask = 0;

  if( channel == ADI_CHANNEL_1 )
    mask = ADI_ADRV9001_RX1;
  else
    mask = ADI_ADRV9001_RX2;

  if( adi_adrv9001_arm_NextPfir_Set( &Instance->Device, mask, &pfirCoeff ) != 0)
    return Adrv9001Status_PfirErr;

  if( adi_adrv9001_arm_Profile_Switch(&Instance->Device) != 0)
    return Adrv9001Status_PfirErr;

  return Adrv9001Status_Success;
}

int32_t Adrv9001_LoadDefaultProfile( adrv9001_t *Instance )
{
  int32_t status;

  printf("\r\nAdrv9001 Load Profile\r\n\r\n");

  if((status = initialize( &Instance->Device )) != 0)
    return Adrv9001Status_ProfileInitErr;

  if((status = calibrate( &Instance->Device )) != 0)
    return Adrv9001Status_ProfileCalErr;

  if((status = configure( &Instance->Device )) != 0)
    return Adrv9001Status_ProfileCfgErr;

  return Adrv9001Status_Success;
}

int32_t Adrv9001_SetRxRssiOffset( adrv9001_t *Instance, adi_common_ChannelNumber_e channel, float Value )
{
  if( channel == ADI_CHANNEL_1 )
  {
    Instance->Rx1RssiOffsetdB = Value;
  }
  else
  {
    Instance->Rx2RssiOffsetdB = Value;
  }

  return Adrv9001Status_Success;
}

int32_t Adrv9001_GetRxRssiOffset( adrv9001_t *Instance, adi_common_ChannelNumber_e channel, float *Value )
{
  if( channel == ADI_CHANNEL_1 )
  {
    *Value = Instance->Rx1RssiOffsetdB;
  }
  else
  {
    *Value = Instance->Rx2RssiOffsetdB;
  }

  return Adrv9001Status_Success;
}

int32_t Adrv9001_GetRxRssi( adrv9001_t *Instance, adi_common_ChannelNumber_e channel, float *Value )
{
  uint32_t rxRssiPower_mdB;
  uint8_t gainIndex;
  adi_adrv9001_RxGainTableRow_t Row;

  /* Read RSSI */
  if( adi_adrv9001_Rx_Rssi_Read( &Instance->Device, channel, &rxRssiPower_mdB ) != 0)
    return Adrv9001Status_ReadErr;

  /* Read Gain index */
  if( adi_adrv9001_Rx_Gain_Get( &Instance->Device, channel, &gainIndex ) != 0)
    return Adrv9001Status_ReadErr;

  /* Read Gain Table */
  if( adi_adrv9001_Rx_GainTable_Read(&Instance->Device, channel, gainIndex, &Row, 1, NULL) != 0)
    return Adrv9001Status_RxGainTableErr;

  /* Convert to dbm relative to calibration offset */
  float rssi = -(float)rxRssiPower_mdB / 1000.0;
  if( channel == ADI_CHANNEL_1 )
  {
    rssi += Instance->Rx1RssiOffsetdB;
  }
  else
  {
    rssi += Instance->Rx2RssiOffsetdB;
  }

  /* Calculate Front End gain */
  float feGain = ((float)(256 - Row.rxFeGain)) / ((float)256);
  feGain = (float)log10((double)feGain);
  feGain = 20.0 * feGain;

  /* Account for Front End gain */
  rssi -= feGain;

  /* Account for digital gain */
  rssi -= (float)Row.digGain * 0.05;

  *Value = rssi;

  return Adrv9001Status_Success;
}

int32_t Adrv9001_SetRxToTxLoopBack(adrv9001_t *Instance, adi_common_ChannelNumber_e channel, bool Value)
{
  if( channel == ADI_CHANNEL_1 )
    Instance->Rx1ToTx1Loopback = Value;
  else
    Instance->Rx2ToTx2Loopback = Value;

  if( adi_adrv9001_Tx_DataPath_Loopback_Set(&Instance->Device, channel, Value) != 0)
    return Adrv9001Status_WriteErr;

  return Adrv9001Status_Success;
}

int32_t Adrv9001_GetRxToTxLoopBack(adrv9001_t *Instance, adi_common_ChannelNumber_e channel, bool *Value)
{
  if( channel == ADI_CHANNEL_1 )
    *Value = Instance->Rx1ToTx1Loopback;
  else
    *Value = Instance->Rx2ToTx2Loopback;

  return Adrv9001Status_Success;
}

int32_t Adrv9001_SetTxToRxLoopBack( adrv9001_t *Instance, adi_common_ChannelNumber_e channel, bool Value )
{
  if( channel == ADI_CHANNEL_1 )
    Instance->Tx1ToRx1Loopback = Value;
  else
    Instance->Tx2ToRx2Loopback = Value;

  if( adi_adrv9001_Ssi_Loopback_Set( &Instance->Device, channel, ADI_ADRV9001_SSI_TYPE_LVDS, Value) != 0)
    return Adrv9001Status_WriteErr;

  return Adrv9001Status_Success;
}

int32_t Adrv9001_GetTxToRxLoopBack( adrv9001_t *Instance, adi_common_ChannelNumber_e channel, bool *Value )
{
  if( channel == ADI_CHANNEL_1 )
    *Value = Instance->Tx1ToRx1Loopback;
  else
    *Value = Instance->Tx2ToRx2Loopback;

  return Adrv9001Status_Success;
}

int32_t Adrv9001_GetSiliconVersion( adrv9001_t *Instance, adi_adrv9001_SiliconVersion_t *Value )
{
  if( adi_adrv9001_SiliconVersion_Get( &Instance->Device, Value ) != 0)
    return Adrv9001Status_ReadErr;

  return Adrv9001Status_Success;
}

int32_t Adrv9001_GetFirmwareVersion( adrv9001_t *Instance, adi_adrv9001_ArmVersion_t *Value )
{
  if( adi_adrv9001_arm_Version( &Instance->Device, Value ) != 0)
    return Adrv9001Status_ReadErr;

  return Adrv9001Status_Success;
}

int32_t Adrv9001_GetApiVersion( adrv9001_t *Instance, adi_common_ApiVersion_t *Value )
{
  if( adi_adrv9001_ApiVersion_Get( &Instance->Device, Value ) != 0)
    return Adrv9001Status_ReadErr;

  return Adrv9001Status_Success;
}

int32_t Adrv9001_GetTemperature( adrv9001_t *Instance, int16_t *Value )
{
  if( adi_adrv9001_Temperature_Get( &Instance->Device, Value ) != 0)
    return Adrv9001Status_ReadErr;

  return Adrv9001Status_Success;
}

int32_t Adrv9001_GetSsiStatus( adrv9001_t *Instance, adi_common_Port_e port, adi_common_ChannelNumber_e channel, bool *Value )
{
  if( port == ADI_TX )
  {
    if( channel == ADI_CHANNEL_1 )
      *Value = Instance->Tx1SsiSweepStatus;
    else
      *Value = Instance->Tx2SsiSweepStatus;
  }
  else
  {
    if( channel == ADI_CHANNEL_1 )
      *Value = Instance->Rx1SsiSweepStatus;
    else
      *Value = Instance->Rx2SsiSweepStatus;
  }

  return Adrv9001Status_Success;
}

int32_t Adrv9001_GetSsiDataDelay( adrv9001_t *Instance, adi_common_Port_e port, adi_common_ChannelNumber_e channel, uint16_t *Value )
{
  adi_adrv9001_SsiCalibrationCfg_t cal;

  if( adi_adrv9001_Ssi_Delay_Inspect( &Instance->Device, ADI_ADRV9001_SSI_TYPE_LVDS, &cal ) != 0)
    return Adrv9001Status_ReadErr;

  uint8_t delay;

  if( port == ADI_TX )
  {
    if( channel == ADI_CHANNEL_1 )
      delay = cal.txIDataDelay[0];
    else
      delay = cal.txIDataDelay[1];
  }
  else
  {
    if( channel == ADI_CHANNEL_1 )
      delay = cal.rxIDataDelay[0];
    else
      delay = cal.rxIDataDelay[1];
  }

  *Value = (uint16_t)delay * 90;

  return Adrv9001Status_Success;
}

int32_t Adrv9001_GetSsiClockDelay( adrv9001_t *Instance, adi_common_Port_e port, adi_common_ChannelNumber_e channel, uint16_t *Value )
{
  adi_adrv9001_SsiCalibrationCfg_t cal;

  if( adi_adrv9001_Ssi_Delay_Inspect( &Instance->Device, ADI_ADRV9001_SSI_TYPE_LVDS, &cal ) != 0)
    return Adrv9001Status_ReadErr;

  uint8_t delay;

  if( port == ADI_TX )
  {
    if( channel == ADI_CHANNEL_1 )
      delay = cal.txClkDelay[0];
    else
      delay = cal.txClkDelay[1];
  }
  else
  {
    if( channel == ADI_CHANNEL_1 )
      delay = cal.rxClkDelay[0];
    else
      delay = cal.rxClkDelay[1];
  }

  *Value = (uint16_t)delay * 90;

  return Adrv9001Status_Success;
}

int32_t Adrv9001_SetTxBoost( adrv9001_t *Instance, adi_common_ChannelNumber_e channel, bool Value )
{
  if( Adrv9001_IsPortEnabled(Instance, ADI_TX, channel ) == false )
    return Adrv9001Status_Success;

  Instance->TxBoost[channel -1] = Value;

  if( adi_adrv9001_Tx_OutputPowerBoost_Set(&Instance->Device, channel, Value) != 0)
    return Adrv9001Status_TxAttnErr;

  return Adrv9001Status_Success;
}

int32_t Adrv9001_SetRxCurGainIndex( adrv9001_t *Instance, adi_common_ChannelNumber_e channel, uint8_t Value )
{
  if( adi_adrv9001_Rx_Gain_Set( &Instance->Device, channel, Value ) != 0)
    return Adrv9001Status_WriteErr;

  return Adrv9001Status_Success;
}

int32_t Adrv9001_GetRxCurGainIndex( adrv9001_t *Instance, adi_common_ChannelNumber_e channel, uint8_t *Value )
{
  if( adi_adrv9001_Rx_Gain_Get( &Instance->Device, channel, Value ) != 0)
    return Adrv9001Status_ReadErr;

  return Adrv9001Status_Success;
}

int32_t Adrv9001_GetRxGainTableRow( adrv9001_t *Instance, adi_common_ChannelNumber_e channel, uint8_t TableIndex, adi_adrv9001_RxGainTableRow_t *Value )
{
  if( adi_adrv9001_Rx_GainTable_Read(&Instance->Device, channel, TableIndex, Value, 1, NULL) != 0)
    return Adrv9001Status_RxGainTableErr;

  return Adrv9001Status_Success;
}

int32_t Adrv9001_GetRxGainTableFeAttn( adrv9001_t *Instance, adi_common_ChannelNumber_e channel, uint8_t TableIndex, float *Value )
{
  int32_t status;
  adi_adrv9001_RxGainTableRow_t Row;

  if((status = Adrv9001_GetRxGainTableRow(Instance, channel, TableIndex, &Row )) != 0)
    return status;

  /* Calculate Front End gain */
  float gain = ((float)(256 - Row.rxFeGain)) / ((float)256);
  gain = (float)log10((double)gain);
  gain = 20.0 * gain;

  memcpy((uint8_t*)Value, (uint8_t*)&gain, sizeof(gain));

  return Adrv9001Status_Success;
}

int32_t Adrv9001_GetRxGainTableDigAttn( adrv9001_t *Instance, adi_common_ChannelNumber_e channel, uint8_t TableIndex, float *Value )
{
  int32_t status;
  adi_adrv9001_RxGainTableRow_t Row;

  if((status = Adrv9001_GetRxGainTableRow(Instance, channel, TableIndex, &Row )) != 0)
    return status;

  /* Calculate Front End gain */
  float gain = (float)Row.digGain * 0.05;

  memcpy((uint8_t*)Value, (uint8_t*)&gain, sizeof(gain));

  return Adrv9001Status_Success;
}

int32_t Adrv9001_GetRxGainTableExtCtrl( adrv9001_t *Instance, adi_common_ChannelNumber_e channel, uint8_t TableIndex, uint8_t *Value )
{
  int32_t status;
  adi_adrv9001_RxGainTableRow_t tmp;

  if((status = Adrv9001_GetRxGainTableRow(Instance, channel, TableIndex, &tmp )) != 0)
    return status;

  *Value = tmp.extControl;

  return Adrv9001Status_Success;
}

int32_t Adrv9001_SetTxDataSrc( adrv9001_t *Instance, adi_common_ChannelNumber_e channel, adrv9001_tx_data_src_t Value )
{
  /* Configure Programmable Logic */
  if( Value == Adrv9001TxDataSrc_Ones )
  {
    AxiAdrv9001_SetTxDataSrc( &Instance->Axi, AxiAdrv9001TxDataSrc_Ones, channel );
  }
  else if( Value == Adrv9001TxDataSrc_Zeros )
  {
    AxiAdrv9001_SetTxDataSrc( &Instance->Axi, AxiAdrv9001TxDataSrc_Zeros, channel );
  }
  else if( Value == Adrv9001TxDataSrc_Ramp )
  {
    AxiAdrv9001_SetTxDataSrc( &Instance->Axi, AxiAdrv9001TxDataSrc_Ramp, channel );
  }
  else if( Value == Adrv9001TxDataSrc_Pn15 )
  {
    AxiAdrv9001_SetTxDataSrc( &Instance->Axi, AxiAdrv9001TxDataSrc_Pn15, channel);
  }
  else if( Value == Adrv9001TxDataSrc_FixedPattern )
  {
    AxiAdrv9001_SetTxDataSrc( &Instance->Axi, AxiAdrv9001TxDataSrc_FixedPattern, channel );
  }
  else if( Value == Adrv9001TxDataSrc_Axis )
  {
    AxiAdrv9001_SetTxDataSrc( &Instance->Axi, AxiAdrv9001TxDataSrc_Axis, channel );
  }

  /* Configure ADRV9001 */
  if( Value == Adrv9001TxDataSrc_RxLoopback )
  {
    if( Adrv9001_SetRxToTxLoopBack( Instance, channel, true) != 0 )
      return Adrv9001Status_WriteErr;
  }
  else
  {
    if( Adrv9001_SetRxToTxLoopBack( Instance, channel, false) != 0 )
      return Adrv9001Status_WriteErr;
  }

  return Adrv9001Status_Success;
}

int32_t Adrv9001_GetTxDataSrc( adrv9001_t *Instance, adi_common_ChannelNumber_e channel, adrv9001_tx_data_src_t *Value )
{
  axi_adrv9001_data_src_t AxiAdrv9001DataSrc = AxiAdrv9001_GetTxDataSrc( &Instance->Axi, channel);

  bool Loopback;
  if( Adrv9001_GetTxToRxLoopBack( Instance, channel, &Loopback ) != 0)
    return Adrv9001Status_ReadErr;

  if( Loopback )
  {
    *Value = Adrv9001TxDataSrc_RxLoopback;
  }
  else if( AxiAdrv9001DataSrc == AxiAdrv9001TxDataSrc_Axis )
  {
    *Value = Adrv9001TxDataSrc_Axis;
  }
  else if( AxiAdrv9001DataSrc == AxiAdrv9001TxDataSrc_Zeros )
  {
    *Value = Adrv9001TxDataSrc_Zeros;
  }
  else if( AxiAdrv9001DataSrc == AxiAdrv9001TxDataSrc_Ones )
  {
    *Value = Adrv9001TxDataSrc_Ones;
  }
  else if( AxiAdrv9001DataSrc == AxiAdrv9001TxDataSrc_Ramp )
  {
    *Value = Adrv9001TxDataSrc_Ramp;
  }
  else if( AxiAdrv9001DataSrc == AxiAdrv9001TxDataSrc_Pn15 )
  {
    *Value = Adrv9001TxDataSrc_Pn15;
  }
  else if( AxiAdrv9001DataSrc == AxiAdrv9001TxDataSrc_FixedPattern )
  {
    *Value = Adrv9001TxDataSrc_FixedPattern;
  }
  else
  {
    return Adrv9001Status_InvalidParameter;
  }

  return Adrv9001Status_Success;
}

int32_t Adrv9001_GetTxBoost( adrv9001_t *Instance, adi_common_ChannelNumber_e channel, bool *Value )
{
  *Value = Instance->TxBoost[channel - 1];

  return Adrv9001Status_Success;
}

int32_t Adrv9001_SetTxAttn( adrv9001_t *Instance, adi_common_ChannelNumber_e channel, float Value )
{
  if( Adrv9001_IsPortEnabled(Instance, ADI_TX, channel ) == false )
    return Adrv9001Status_Success;

  Instance->TxAttn[channel - 1] = Value;

  uint16_t attenuation_mdB = (uint16_t)(Value * 1000.0);

  if( adi_adrv9001_Tx_Attenuation_Set(&Instance->Device, channel, attenuation_mdB) != 0)
    return Adrv9001Status_TxAttnErr;

  return Adrv9001Status_Success;
}

int32_t Adrv9001_GetTxAttn( adrv9001_t *Instance, adi_common_ChannelNumber_e channel, float *Value )
{
  *Value = Instance->TxAttn[channel - 1];

  return Adrv9001Status_Success;
}

int32_t Adrv9001_SetEnableMode( adrv9001_t *Instance, adi_common_Port_e port, adi_common_ChannelNumber_e channel, adi_adrv9001_ChannelEnableMode_e mode )
{
  if( Adrv9001_IsPortEnabled(Instance, port, channel ) == false )
    return Adrv9001Status_Success;

  if( adi_adrv9001_Radio_ChannelEnableMode_Set(&Instance->Device, port, channel, mode) != 0)
    return Adrv9001Status_EnableModeErr;

  return Adrv9001Status_Success;
}

int32_t Adrv9001_GetEnableMode( adrv9001_t *Instance, adi_common_Port_e port, adi_common_ChannelNumber_e channel, adi_adrv9001_ChannelEnableMode_e *mode )
{
  if(adi_adrv9001_Radio_ChannelEnableMode_Get( &Instance->Device, port, channel, mode) != 0)
    return Adrv9001Status_ReadErr;

  return Adrv9001Status_Success;
}

static void Adrv9001_HopIrq( adrv9001_t *Instance )
{
  AxiAdrv9001_ClearHopIrq( &Instance->Axi );

  if( Instance->HopIrqCallback != NULL )
    Instance->HopIrqCallback( Instance->HopIrqCallbackRef );
}

void Adrv9001_RegisterRadioStateCallback( adrv9001_t *Instance, adrv9001_rf_state_cb_t Callback, void *CallbackRef )
{
  Instance->StateCallback = Callback;
  Instance->StateCallbackRef = CallbackRef;
}

int32_t Adrv9001_Initialize( adrv9001_t *Instance, adrv9001_init_t *Init )
{
  int32_t status;

  memset(Instance, 0, sizeof(adrv9001_t));

  if( Init->Init == NULL )
	return Adrv9001Status_InvalidParameter;

  Instance->Params = Init->Init;
  Instance->IrqInstance = Init->IrqInstance;
  Instance->StateCallback = Init->StateCallback;
  Instance->StateCallbackRef = Init->StateCallbackRef;
  Instance->TxAttn[0] = Init->TxAttn[0];
  Instance->TxAttn[1] = Init->TxAttn[1];
  Instance->TxBoost[0] = Init->TxBoost[0];
  Instance->TxBoost[1] = Init->TxBoost[1];
  Instance->HopIrqCallback = Init->HopIrqCallback;
  Instance->HopIrqCallbackRef = Init->HopIrqCallbackRef;
  Instance->RxEnableMode = Init->RxEnableMode;
  Instance->TxEnableMode = Init->TxEnableMode;
  Instance->Rx1RssiOffsetdB = Init->Rx1RssiOffsetdB;
  Instance->Rx2RssiOffsetdB = Init->Rx2RssiOffsetdB;

  /* Assign Hal Reference to adrv9001 */
  Instance->Device.common.devHalInfo = (void*)Instance;

  /* Enable Logging */
  Instance->Device.common.error.logEnable = 1;

  /* Initialize Log Path */
  if( (Init->LogFilename != NULL) && (Init->BasePath != NULL) )
  {
    strcpy( Instance->LogPath, Init->BasePath );
    strcpy( &Instance->LogPath[strlen(Instance->LogPath)], Init->LogFilename );
  }
  else
  {
    Instance->LogPath[0] = 0;
  }

  axi_adrv9001_init_t AxiInit = {
      .Base = Init->AxiBase,
      .IrqId = Init->AxiIrqId,
      .IrqInstance = Init->IrqInstance,
      .ClockFreqHz = Init->AxiClockFreqHz
  };

  /* Initialize AXI ADRV9001 */
  if((status = AxiAdrv9001_Initialize( &Instance->Axi, &AxiInit )) != 0)
    return status;

  /* Load Profile */
  if((status = Adrv9001_LoadDefaultProfile( Instance )) != 0)
    return status;

  /* Configure Profile */
  if((status = Adrv9001_ConfigureProfile( Instance, Init->Rx1ChfCoeff, Init->Rx2ChfCoeff )) != 0)
    return status;

  if( Init->HopIrqId != 0x00 )
  {
    if(XScuGic_Connect(Instance->IrqInstance, Init->HopIrqId, (XInterruptHandler)Adrv9001_HopIrq, Instance ) != 0)
      return Adrv9001Status_IrqErr;

    XScuGic_Enable(Instance->IrqInstance, Init->HopIrqId);
  }


  return Adrv9001Status_Success;
}

int32_t Adrv9001_GetSampleRate( adrv9001_t *Instance, adi_common_Port_e port, adi_common_ChannelNumber_e channel, uint32_t *Value )
{
  if( port == ADI_RX )
  {
    if( channel == ADI_CHANNEL_1 )
    {
      memcpy(Value, (uint8_t*)&Instance->Params->rx.rxChannelCfg[0].profile.rxInterfaceSampleRate_Hz, sizeof( uint32_t ));
    }
    else if( channel == ADI_CHANNEL_2 )
    {
      memcpy(Value, (uint8_t*)&Instance->Params->rx.rxChannelCfg[1].profile.rxInterfaceSampleRate_Hz, sizeof( uint32_t ));
    }
    else
    {
    	return Adrv9001Status_InvalidParameter;
    }
  }
  else if( port == ADI_TX )
  {
    if( channel == ADI_CHANNEL_1 )
    {
      memcpy(Value, (uint8_t*)&Instance->Params->tx.txProfile[0].txInterfaceSampleRate_Hz, sizeof( uint32_t ));
    }
    else if( channel == ADI_CHANNEL_2 )
    {
      memcpy(Value, (uint8_t*)&Instance->Params->tx.txProfile[1].txInterfaceSampleRate_Hz, sizeof( uint32_t ));
    }
    else
    {
    	return Adrv9001Status_InvalidParameter;
    }
  }
  else
  {
	  return Adrv9001Status_InvalidParameter;
  }

  return Adrv9001Status_Success;
}

int32_t Adrv9001_GetCarrierFrequency( adrv9001_t *Instance, adi_common_Port_e port, adi_common_ChannelNumber_e channel, uint64_t *FreqHz )
{
  adi_adrv9001_Carrier_t AdiCarrier;
  if( adi_adrv9001_Radio_Carrier_Inspect( &Instance->Device, port, channel, &AdiCarrier ) != 0)
    return Adrv9001Status_CarrierFreqErr;

  memcpy((uint8_t*)FreqHz, (uint8_t*)&AdiCarrier.carrierFrequency_Hz, sizeof(uint64_t));

  return Adrv9001Status_Success;
}

int32_t Adrv9001_SetCarrierFrequency( adrv9001_t *Instance, adi_common_Port_e port, adi_common_ChannelNumber_e channel, uint64_t FreqHz )
{
  int32_t status;

  adi_adrv9001_ChannelState_e State;
  if( adi_adrv9001_Radio_Channel_State_Get( &Instance->Device, port, channel, &State ) != 0)
    return Adrv9001Status_ReadErr;

  adi_adrv9001_ChannelEnableMode_e mode;
  if(adi_adrv9001_Radio_ChannelEnableMode_Get( &Instance->Device, port, channel, &mode) != 0)
    return Adrv9001Status_ReadErr;

  if( mode == ADI_ADRV9001_PIN_MODE )
  {
    /* Set SPI Mode */
    if(adi_adrv9001_Radio_ChannelEnableMode_Set(&Instance->Device, port, channel, ADI_ADRV9001_SPI_MODE) != 0)
      return Adrv9001Status_WriteErr;
  }

  if( State == ADI_ADRV9001_CHANNEL_RF_ENABLED )
  {
    if((status = Adrv9001_ToPrimed( Instance, port, channel )) != 0)
      return status;
  }
	
  if( State != ADI_ADRV9001_CHANNEL_CALIBRATED )
  {
    if((status = Adrv9001_ToCalibrated( Instance, port, channel )) != 0)
      return status;
  }

  adi_adrv9001_Carrier_t carrier;

  if( adi_adrv9001_Radio_Carrier_Inspect( &Instance->Device, port, channel, &carrier ) != 0)
    return Adrv9001Status_ReadErr;

  memcpy((uint8_t*)&carrier.carrierFrequency_Hz, (uint8_t*)&FreqHz, sizeof( carrier.carrierFrequency_Hz ) );

  if( adi_adrv9001_Radio_Carrier_Configure(&Instance->Device, port, channel, &carrier) != 0)
    return Adrv9001Status_WriteErr;

  if( (mode == ADI_ADRV9001_PIN_MODE) || ( State == ADI_ADRV9001_CHANNEL_PRIMED ) || (State == ADI_ADRV9001_CHANNEL_RF_ENABLED) )
  {
    if((status = Adrv9001_ToPrimed( Instance, port, channel )) != 0)
      return status;
  }

  if(mode == ADI_ADRV9001_PIN_MODE)
  {
    if(adi_adrv9001_Radio_ChannelEnableMode_Set(&Instance->Device, port, channel, ADI_ADRV9001_PIN_MODE) != 0)
      return Adrv9001Status_WriteErr;
  }

  if( State == ADI_ADRV9001_CHANNEL_RF_ENABLED )
  {
    if((status = Adrv9001_ToRfEnabled( Instance, port, channel )) != 0)
      return status;
  }

  return Adrv9001Status_Success;
}

int32_t Adrv9001_ToPrimed( adrv9001_t *Instance, adi_common_Port_e port, adi_common_ChannelNumber_e channel )
{
  if( Adrv9001_IsPortEnabled(Instance, port, channel ) == false )
    return Adrv9001Status_Success;

  adi_adrv9001_ChannelEnableMode_e mode;
  if(adi_adrv9001_Radio_ChannelEnableMode_Get( &Instance->Device, port, channel, &mode) != 0)
    return Adrv9001Status_ReadErr;

  /* Allow user to set RF frontend */
  if( (Instance->StateCallback != NULL) && (Instance->Initialized != 0) )
    Instance->StateCallback( Instance->StateCallbackRef, ADI_ADRV9001_CHANNEL_RF_ENABLED, port, channel );

  if( mode == ADI_ADRV9001_SPI_MODE )
  {
    /* Set Radio State */
    if( adi_adrv9001_Radio_Channel_ToPrimed(&Instance->Device, port, channel) != 0 )
      return Adrv9001Status_ToEnabledErr;
  }

  AxiAdrv9001_SetPsEnable(&Instance->Axi, port, channel, 0);

  return Adrv9001Status_Success;
}

int32_t Adrv9001_ToCalibrated( adrv9001_t *Instance, adi_common_Port_e port, adi_common_ChannelNumber_e channel )
{
  if( Adrv9001_IsPortEnabled(Instance, port, channel ) == false )
    return Adrv9001Status_Success;

  adi_adrv9001_ChannelEnableMode_e mode;
  if(adi_adrv9001_Radio_ChannelEnableMode_Get( &Instance->Device, port, channel, &mode) != 0)
    return Adrv9001Status_ReadErr;

  /* Allow user to set RF frontend */
  if( (Instance->StateCallback != NULL) && (Instance->Initialized != 0) )
    Instance->StateCallback( Instance->StateCallbackRef, ADI_ADRV9001_CHANNEL_RF_ENABLED, port, channel );

  if( mode == ADI_ADRV9001_PIN_MODE )
  {
    /* Set SPI Mode */
    if(adi_adrv9001_Radio_ChannelEnableMode_Set(&Instance->Device, port, channel, ADI_ADRV9001_SPI_MODE) != 0)
      return Adrv9001Status_WriteErr;
  }

  /* Set Radio State */
  if( adi_adrv9001_Radio_Channel_ToCalibrated(&Instance->Device, port, channel) != 0 )
    return Adrv9001Status_ToCalErr;

  return Adrv9001Status_Success;
}

int32_t Adrv9001_ToRfEnabled( adrv9001_t *Instance, adi_common_Port_e port, adi_common_ChannelNumber_e channel )
{
  if( Adrv9001_IsPortEnabled(Instance, port, channel ) == false )
    return Adrv9001Status_Success;

  adi_adrv9001_ChannelEnableMode_e mode;
  if(adi_adrv9001_Radio_ChannelEnableMode_Get( &Instance->Device, port, channel, &mode) != 0)
    return Adrv9001Status_ReadErr;

  /* Allow user to set RF frontend */
  if( (Instance->StateCallback != NULL) && (Instance->Initialized != 0) )
    Instance->StateCallback( Instance->StateCallbackRef, ADI_ADRV9001_CHANNEL_RF_ENABLED, port, channel );

  if( mode == ADI_ADRV9001_SPI_MODE )
  {
    /* Set Radio State */
    if( adi_adrv9001_Radio_Channel_EnableRf(&Instance->Device, port, channel, true) != 0 )
      return Adrv9001Status_ToEnabledErr;
  }

  AxiAdrv9001_SetPsEnable(&Instance->Axi, port, channel, 1);

  if( port == ADI_TX )
  {
    Adrv9001_SetTxAttn( Instance, channel, Instance->TxAttn[channel -1] );
    Adrv9001_SetTxBoost( Instance, channel, Instance->TxBoost[channel -1] );
  }

  return Adrv9001Status_Success;
}

bool Adrv9001_IsRfEnabled( adrv9001_t *Instance, adi_common_Port_e port, adi_common_ChannelNumber_e channel )
{
  adi_adrv9001_ChannelState_e State;
  if( adi_adrv9001_Radio_Channel_State_Get( &Instance->Device, port, channel, &State ) != 0)
    return false;

  if( State == ADI_ADRV9001_CHANNEL_RF_ENABLED )
    return true;
  else
    return false;
}

int32_t Adrv9001_GetRadioState( adrv9001_t *Instance, adi_common_Port_e port, adi_common_ChannelNumber_e channel, adi_adrv9001_ChannelState_e *Value )
{
   if( adi_adrv9001_Radio_Channel_State_Get( &Instance->Device, port, channel, Value ) != 0)
     return Adrv9001Status_ReadErr;

   return Adrv9001Status_Success;
}

int32_t Adrv9001_SetTxTestMode( adrv9001_t *Instance, adi_common_ChannelNumber_e channel, adi_adrv9001_SsiTestModeData_e testMode )
{
  int32_t status;

  if((status = Adrv9001_ToPrimed( Instance, ADI_TX, channel )) != 0)
    return status;

  adi_adrv9001_TxSsiTestModeCfg_t Cfg = {
      .testData = testMode,
      .fixedDataPatternToCheck = 0xAABBCCDD
  };

  if( adi_adrv9001_Ssi_Tx_TestMode_Configure(&Instance->Device, channel, ADI_ADRV9001_SSI_TYPE_LVDS, ADI_ADRV9001_SSI_FORMAT_16_BIT_I_Q_DATA, &Cfg ) != 0)
    return Adrv9001Status_SsiTestModeErr;

  if( channel == ADI_CHANNEL_1 )
    Instance->Tx1TestMode = testMode;
  else
    Instance->Tx2TestMode = testMode;

  return Adrv9001Status_Success;
}

int32_t Adrv9001_GetTxTestMode( adrv9001_t *Instance, adi_common_ChannelNumber_e channel, adi_adrv9001_SsiTestModeData_e *testMode )
{
  if( channel == ADI_CHANNEL_1 )
    *testMode = Instance->Tx1TestMode;
  else
    *testMode = Instance->Tx2TestMode;

  return Adrv9001Status_Success;
}

int32_t Adrv9001_SetRxTestMode( adrv9001_t *Instance, adi_common_ChannelNumber_e channel, adi_adrv9001_SsiTestModeData_e testMode )
{
  int32_t status;

  if((status = Adrv9001_ToPrimed( Instance, ADI_RX, channel )) != 0)
    return status;

  adi_adrv9001_RxSsiTestModeCfg_t Cfg = {
      .testData = testMode,
      .fixedDataPatternToTransmit = 0xAABBCCDD
  };

  if( adi_adrv9001_Ssi_Rx_TestMode_Configure(&Instance->Device, channel, ADI_ADRV9001_SSI_TYPE_LVDS, ADI_ADRV9001_SSI_FORMAT_16_BIT_I_Q_DATA, &Cfg ) != 0)
    return Adrv9001Status_SsiTestModeErr;

  if( channel == ADI_CHANNEL_1 )
    Instance->Rx1TestMode = testMode;
  else
    Instance->Rx2TestMode = testMode;

  return Adrv9001Status_Success;
}

int32_t Adrv9001_GetRxTestMode( adrv9001_t *Instance, adi_common_ChannelNumber_e channel, adi_adrv9001_SsiTestModeData_e *testMode )
{
  if( channel == ADI_CHANNEL_1 )
    *testMode = Instance->Rx1TestMode;
  else
    *testMode = Instance->Rx2TestMode;

  return Adrv9001Status_Success;
}

int32_t Adrv9001_RxSsiTest( adrv9001_t *Instance, adi_common_ChannelNumber_e channel, adi_adrv9001_SsiTestModeData_e testMode, uint32_t fixedPattern )
{
  int32_t status;

  if( testMode == ADI_ADRV9001_SSI_TESTMODE_DATA_FIXED_PATTERN )
  {
    AxiAdrv9001_SetFixedPattern( &Instance->Axi, ADI_RX, channel, fixedPattern );
  }

  if((status = Adrv9001_ToPrimed( Instance, ADI_RX, channel )) != 0)
    return status;

  adi_adrv9001_RxSsiTestModeCfg_t Cfg = {
      .testData = testMode,
      .fixedDataPatternToTransmit = fixedPattern
  };

  if( adi_adrv9001_Ssi_Rx_TestMode_Configure(&Instance->Device, channel, ADI_ADRV9001_SSI_TYPE_LVDS, ADI_ADRV9001_SSI_FORMAT_16_BIT_I_Q_DATA, &Cfg ) != 0)
    return Adrv9001Status_SsiTestModeErr;

  if((status = Adrv9001_ToRfEnabled( Instance, ADI_RX, channel )) != 0)
    return status;

  usleep(10000);

  if( testMode == ADI_ADRV9001_SSI_TESTMODE_DATA_FIXED_PATTERN )
  {
    if( AxiAdrv9001_GetFixedPatternMonDet( &Instance->Axi, channel) == false )
      status = Adrv9001Status_SsiTestModeErr;
    else
      status = Adrv9001Status_Success;
  }
  else if( testMode == ADI_ADRV9001_SSI_TESTMODE_DATA_RAMP_16_BIT )
  {
    if( AxiAdrv9001_GetRampMonDet( &Instance->Axi, channel) == false )
      status = Adrv9001Status_SsiTestModeErr;
    else
      status = Adrv9001Status_Success;
  }
  else if( testMode == ADI_ADRV9001_SSI_TESTMODE_DATA_PRBS15 )
  {
    if( AxiAdrv9001_GetPn15MonDet( &Instance->Axi, channel) == false )
      status = Adrv9001Status_SsiTestModeErr;
    else
      status = Adrv9001Status_Success;
  }
  else
  {
    status = Adrv9001Status_InvalidParameter;
  }

  Adrv9001_ToPrimed( Instance, ADI_RX, channel );

  return status;
}

int32_t Adrv9001_TxTestModeCheck( adrv9001_t *Instance, adi_common_ChannelNumber_e channel, adi_adrv9001_SsiTestModeData_e testMode, uint32_t fixedPattern )
{
  adi_adrv9001_TxSsiTestModeCfg_t Cfg = {
      .testData = testMode,
      .fixedDataPatternToCheck = fixedPattern
  };

  if( adi_adrv9001_Ssi_Tx_TestMode_Configure(&Instance->Device, channel, ADI_ADRV9001_SSI_TYPE_LVDS, ADI_ADRV9001_SSI_FORMAT_16_BIT_I_Q_DATA, &Cfg ) != 0)
    return Adrv9001Status_SsiTestModeErr;

  usleep(5000);

  /* Read Test Mode Status */
  adi_adrv9001_TxSsiTestModeStatus_t s;
  if( adi_adrv9001_Ssi_Tx_TestMode_Status_Inspect( &Instance->Device, channel, ADI_ADRV9001_SSI_TYPE_LVDS, ADI_ADRV9001_SSI_FORMAT_16_BIT_I_Q_DATA, &Cfg, &s ) != 0)
    return Adrv9001Status_SsiTestModeErr;

  if( s.dataError != 0 )
    return Adrv9001Status_SsiTestModeErr;

  return Adrv9001Status_Success;
}

int32_t Adrv9001_TxSsiTest( adrv9001_t *Instance, adi_common_ChannelNumber_e channel, adi_adrv9001_SsiTestModeData_e testMode, uint32_t fixedPattern )
{
  int32_t status;

  axi_adrv9001_data_src_t prevDataSrc = AxiAdrv9001_GetTxDataSrc(&Instance->Axi, channel);

  if((status = Adrv9001_ToPrimed( Instance, ADI_TX, channel )) != 0)
    return status;

  if( testMode == ADI_ADRV9001_SSI_TESTMODE_DATA_FIXED_PATTERN )
  {
    AxiAdrv9001_SetTxDataSrc( &Instance->Axi, AxiAdrv9001TxDataSrc_FixedPattern, channel );
    AxiAdrv9001_SetFixedPattern( &Instance->Axi, ADI_TX, channel, fixedPattern );
  }
  else if( testMode == ADI_ADRV9001_SSI_TESTMODE_DATA_RAMP_16_BIT )
  {
    AxiAdrv9001_SetTxDataSrc( &Instance->Axi, AxiAdrv9001TxDataSrc_Ramp, channel );
  }
  else if( testMode == ADI_ADRV9001_SSI_TESTMODE_DATA_PRBS15 )
  {
    AxiAdrv9001_SetTxDataSrc( &Instance->Axi, AxiAdrv9001TxDataSrc_Pn15, channel );
  }
  else
  {
    return Adrv9001Status_InvalidParameter;
  }

  if((status = Adrv9001_ToRfEnabled( Instance, ADI_TX, channel )) != 0)
    return status;

  usleep(1000);

  status = Adrv9001_TxTestModeCheck( Instance, channel, testMode, fixedPattern );

  Adrv9001_ToPrimed( Instance, ADI_TX, channel );

  AxiAdrv9001_SetTxDataSrc( &Instance->Axi, prevDataSrc, channel );

  return status;
}

int32_t Adrv9001_PerformSsiSweep( adrv9001_t *Instance, adi_common_Port_e port, adi_common_ChannelNumber_e channel, adi_adrv9001_SsiTestModeData_e testMode, uint32_t fixedPattern, uint16_t results[8][8] )
{
  int32_t status;

  adi_adrv9001_ChannelEnableMode_e mode;

  if(adi_adrv9001_Radio_ChannelEnableMode_Get( &Instance->Device, port, channel, &mode) != 0)
    return Adrv9001Status_ReadErr;

  if( Adrv9001_SetEnableMode(Instance, port, channel, ADI_ADRV9001_SPI_MODE) != 0)
    return Adrv9001Status_EnableModeErr;

  adrv9001_ssi_port_delay_t Delay;

  for(uint8_t clkDly = 0; clkDly < 8; clkDly++)
  {
    Delay.Clk = clkDly;

    for(uint8_t dataDly = 0; dataDly < 8; dataDly++)
    {
      Delay.Strobe = dataDly;
      Delay.Idata = dataDly;
      Delay.Qdata = dataDly;

      if((status = Adrv9001_SetSsiDelay( Instance, port, channel, Delay)) != 0)
        return status;

      if(port == ADI_TX)
      {
        if( Adrv9001_TxSsiTest(Instance, channel, testMode, fixedPattern) != 0)
          results[clkDly][dataDly] = 0;
        else
          results[clkDly][dataDly] = 1;
      }
      else
      {
        if( Adrv9001_RxSsiTest(Instance, channel, testMode, fixedPattern ) != 0)
          results[clkDly][dataDly] = 0;
        else
          results[clkDly][dataDly] = 1;
      }
    }
  }

  adi_adrv9001_RxSsiTestModeCfg_t RxCfg = { .testData = ADI_ADRV9001_SSI_TESTMODE_DATA_NORMAL };
  if( adi_adrv9001_Ssi_Rx_TestMode_Configure(&Instance->Device, channel, ADI_ADRV9001_SSI_TYPE_LVDS, ADI_ADRV9001_SSI_FORMAT_16_BIT_I_Q_DATA, &RxCfg ) != 0)
    return Adrv9001Status_SsiTestModeErr;

  adi_adrv9001_TxSsiTestModeCfg_t TxCfg = { .testData = ADI_ADRV9001_SSI_TESTMODE_DATA_NORMAL };
  if( adi_adrv9001_Ssi_Tx_TestMode_Configure(&Instance->Device, channel, ADI_ADRV9001_SSI_TYPE_LVDS, ADI_ADRV9001_SSI_FORMAT_16_BIT_I_Q_DATA, &TxCfg ) != 0)
    return Adrv9001Status_SsiTestModeErr;

  if( Adrv9001_SetEnableMode(Instance, port, channel, mode) != 0)
    return Adrv9001Status_EnableModeErr;

  return status;
}

static int32_t Adrv9001_FindBestSsiDelay( uint16_t results[8][8], uint8_t *DataIdx, uint8_t *ClkIdx )
{
  for (int y = 0; y < 8; y++)
  {
    /* Find longest string of passes */
    for( int x = 0; x < 8; x++)
    {
      if( results[y][x] > 0 )
      {
        for( int xr = (x + 1); xr < 8; xr++)
        {
          if( results[y][xr] > 0 )
            results[y][x] += 128 >> (xr - x - 1);
          else
            break;
        }

        for( int xl = (x - 1); xl > -1; xl--)
        {
          if( results[y][xl] > 0 )
            results[y][x] += 128 >> (x - xl - 1);
          else
            break;
        }
      }
    }
  }

  uint16_t max = 0;
  for(uint8_t clkDelay = 0; clkDelay < 8; clkDelay++)
  {
    for(uint8_t dataDelay = 0; dataDelay < 8; dataDelay++)
    {
      if( results[clkDelay][dataDelay] > max )
      {
        max = results[clkDelay][dataDelay];
        *ClkIdx = clkDelay;
        *DataIdx = dataDelay;
      }
    }
  }

  if( max == 0 )
    return Adrv9001Status_SsiSweepErr;

  return Adrv9001Status_Success;
}

bool Adrv9001_IsPortEnabled( adrv9001_t *Instance, adi_common_Port_e port, adi_common_ChannelNumber_e channel )
{
  if( (port == ADI_TX) && (channel == ADI_CHANNEL_1))
  {
    if( (Instance->Params->tx.txInitChannelMask & ADI_ADRV9001_TX1 ) == ADI_ADRV9001_TX1 )
      return true;
  }
  else if( (port == ADI_TX) && (channel == ADI_CHANNEL_2))
  {
    if( (Instance->Params->tx.txInitChannelMask & ADI_ADRV9001_TX2 ) == ADI_ADRV9001_TX2 )
      return true;
  }
  else if( (port == ADI_RX) && (channel == ADI_CHANNEL_1))
  {
    if( (Instance->Params->rx.rxInitChannelMask & ADI_ADRV9001_RX1 ) == ADI_ADRV9001_RX1 )
      return true;
  }
  else if( (port == ADI_RX) && (channel == ADI_CHANNEL_2))
  {
    if( (Instance->Params->rx.rxInitChannelMask & ADI_ADRV9001_RX2 ) == ADI_ADRV9001_RX2 )
      return true;
  }

  return false;
}

int32_t Adrv9001_CalibrateSsiDelay( adrv9001_t *Instance, adi_common_Port_e port, adi_common_ChannelNumber_e channel, adi_adrv9001_SsiTestModeData_e testMode, uint32_t fixedPattern )
{
  int32_t status;
  uint16_t results[8][8];
  uint8_t clkMax = 0;
  uint8_t dataMax = 0;

  uint8_t *Result;
  bool *SsiStatus;

  if( (port == ADI_TX) && (channel == ADI_CHANNEL_1) )
  {
    Result = Instance->Tx1SsiSweep;
    SsiStatus = &Instance->Tx1SsiSweepStatus;
  }
  else if((port == ADI_TX) && (channel == ADI_CHANNEL_2))
  {
    Result = Instance->Tx2SsiSweep;
    SsiStatus = &Instance->Tx2SsiSweepStatus;
  }
  else if((port == ADI_RX) && (channel == ADI_CHANNEL_1))
  {
    Result = Instance->Rx1SsiSweep;
    SsiStatus = &Instance->Rx1SsiSweepStatus;
  }
  else
  {
    Result = Instance->Rx2SsiSweep;
    SsiStatus = &Instance->Rx2SsiSweepStatus;
  }

  *SsiStatus = false;

  if( Adrv9001_IsPortEnabled( Instance, port, channel ) == false )
    return Adrv9001Status_Success;

  if((status = Adrv9001_PerformSsiSweep( Instance, port, channel, testMode, fixedPattern, results )) != 0)
    return status;

  if( Result != NULL )
  {
    uint8_t cnt = 0;
    for( int i = 0; i < 8; i++)
    {
      for( int j = 0; j < 8; j++)
      {
        Result[cnt++] = results[i][j];
      }
    }
  }

  if((status = Adrv9001_FindBestSsiDelay( results, &dataMax, &clkMax )) != 0)
    return status;

  adrv9001_ssi_port_delay_t Delay = {
      .Clk = clkMax,
      .Strobe = dataMax,
      .Idata = dataMax,
      .Qdata = dataMax
  };

  if((status = Adrv9001_SetSsiDelay( Instance, port, channel, Delay)) != 0)
    return status;

  *SsiStatus = true;

  return Adrv9001Status_Success;
}

int32_t Adrv9001_SetSsiDataDelay( adrv9001_t *Instance, adi_common_Port_e port, adi_common_ChannelNumber_e channel, uint16_t Value )
{
  adi_adrv9001_ChannelState_e State;

  uint8_t Delay = (uint8_t)(Value / 90);

  if( adi_adrv9001_Radio_Channel_State_Get( &Instance->Device, port, channel, &State ) != 0)
    return Adrv9001Status_SsiSetErr;

  if( State == ADI_ADRV9001_CHANNEL_RF_ENABLED )
    return Adrv9001Status_SsiSetErr;

  adi_adrv9001_SsiCalibrationCfg_t cal;

  if( adi_adrv9001_Ssi_Delay_Inspect(&Instance->Device, ADI_ADRV9001_SSI_TYPE_LVDS, &cal ) != 0)
    return Adrv9001Status_SsiSetErr;

  if((port == ADI_RX) && (channel == ADI_CHANNEL_1))
  {
    cal.rxIDataDelay[0] = Delay;
    cal.rxQDataDelay[0] = Delay;
    cal.rxStrobeDelay[0] = Delay;
  }
  else if((port == ADI_RX) && (channel == ADI_CHANNEL_2))
  {
    cal.rxIDataDelay[1] = Delay;
    cal.rxQDataDelay[1] = Delay;
    cal.rxStrobeDelay[1] = Delay;
  }
  else if((port == ADI_TX) && (channel == ADI_CHANNEL_1))
  {
    cal.txIDataDelay[0] = Delay;
    cal.txQDataDelay[0] = Delay;
    cal.txStrobeDelay[0] = Delay;
  }
  else if((port == ADI_TX) && (channel == ADI_CHANNEL_2))
  {
    cal.txIDataDelay[1] = Delay;
    cal.txQDataDelay[1] = Delay;
    cal.txStrobeDelay[1] = Delay;
  }

  if( adi_adrv9001_Ssi_Delay_Configure(&Instance->Device, ADI_ADRV9001_SSI_TYPE_LVDS, &cal) != 0)
    return Adrv9001Status_SsiSetErr;

  return Adrv9001Status_Success;
}

int32_t Adrv9001_SetSsiClkDelay( adrv9001_t *Instance, adi_common_Port_e port, adi_common_ChannelNumber_e channel, uint16_t Value )
{
  adi_adrv9001_ChannelState_e State;

  uint8_t Delay = (uint8_t)(Value / 90);

  if( adi_adrv9001_Radio_Channel_State_Get( &Instance->Device, port, channel, &State ) != 0)
    return Adrv9001Status_SsiSetErr;

  if( State == ADI_ADRV9001_CHANNEL_RF_ENABLED )
    return Adrv9001Status_SsiSetErr;

  adi_adrv9001_SsiCalibrationCfg_t cal;

  if( adi_adrv9001_Ssi_Delay_Inspect(&Instance->Device, ADI_ADRV9001_SSI_TYPE_LVDS, &cal) != 0)
    return Adrv9001Status_SsiSetErr;

  if((port == ADI_RX) && (channel == ADI_CHANNEL_1))
  {
    cal.rxClkDelay[0] = Delay;
  }
  else if((port == ADI_RX) && (channel == ADI_CHANNEL_2))
  {
    cal.rxClkDelay[1] = Delay;
  }
  else if((port == ADI_TX) && (channel == ADI_CHANNEL_1))
  {
    cal.txClkDelay[0] = Delay;
  }
  else if((port == ADI_TX) && (channel == ADI_CHANNEL_2))
  {
    cal.txClkDelay[1] = Delay;
  }

  if( adi_adrv9001_Ssi_Delay_Configure(&Instance->Device, ADI_ADRV9001_SSI_TYPE_LVDS, &cal) != 0)
    return Adrv9001Status_SsiSetErr;

  return Adrv9001Status_Success;
}

int32_t Adrv9001_SetSsiDelay( adrv9001_t *Instance, adi_common_Port_e port, adi_common_ChannelNumber_e channel, adrv9001_ssi_port_delay_t Delay )
{
  adi_adrv9001_ChannelState_e State;

  if( adi_adrv9001_Radio_Channel_State_Get( &Instance->Device, port, channel, &State ) != 0)
    return Adrv9001Status_SsiSetErr;

  if( State == ADI_ADRV9001_CHANNEL_RF_ENABLED )
    return Adrv9001Status_SsiSetErr;

  adi_adrv9001_SsiCalibrationCfg_t cal;

  if( adi_adrv9001_Ssi_Delay_Inspect(&Instance->Device, ADI_ADRV9001_SSI_TYPE_LVDS, &cal) != 0)
    return Adrv9001Status_SsiSetErr;

  if((port == ADI_RX) && (channel == ADI_CHANNEL_1))
  {
    cal.rxClkDelay[0] = Delay.Clk;
    cal.rxStrobeDelay[0] = Delay.Strobe;
    cal.rxIDataDelay[0] = Delay.Idata;
    cal.rxQDataDelay[0] = Delay.Qdata;
  }
  else if((port == ADI_RX) && (channel == ADI_CHANNEL_2))
  {
    cal.rxClkDelay[1] = Delay.Clk;
    cal.rxStrobeDelay[1] = Delay.Strobe;
    cal.rxIDataDelay[1] = Delay.Idata;
    cal.rxQDataDelay[1] = Delay.Qdata;
  }
  else if((port == ADI_TX) && (channel == ADI_CHANNEL_1))
  {
    cal.txClkDelay[0] = Delay.Clk;
    cal.txStrobeDelay[0] = Delay.Strobe;
    cal.txIDataDelay[0] = Delay.Idata;
    cal.txQDataDelay[0] = Delay.Qdata;
  }
  else if((port == ADI_TX) && (channel == ADI_CHANNEL_2))
  {
    cal.txClkDelay[1] = Delay.Clk;
    cal.txStrobeDelay[1] = Delay.Strobe;
    cal.txIDataDelay[1] = Delay.Idata;
    cal.txQDataDelay[1] = Delay.Qdata;
  }

  if( adi_adrv9001_Ssi_Delay_Configure(&Instance->Device, ADI_ADRV9001_SSI_TYPE_LVDS, &cal) != 0)
    return Adrv9001Status_SsiSetErr;

  return Adrv9001Status_Success;
}

int32_t Adrv9001_GetAdcVoltage(adrv9001_t *Instance, adi_adrv9001_AuxAdc_e Channel, float *Value )
{
  uint16_t auxAdc_mV;

  if(adi_adrv9001_AuxAdc_Voltage_Get(&Instance->Device, Channel, &auxAdc_mV) != 0)
    return Adrv9001Status_AdcErr;

  *Value = (float)auxAdc_mV / 1000.0;

  return Adrv9001Status_Success;
}

int32_t Adrv9001_GetAdcEnable( adrv9001_t *Instance, adi_adrv9001_AuxAdc_e Channel, bool *Value )
{
  if( adi_adrv9001_AuxAdc_Inspect(&Instance->Device, Channel, Value) != 0)
    return Adrv9001Status_AdcErr;

  return Adrv9001Status_Success;
}

int32_t Adrv9001_SetAdcEnable( adrv9001_t *Instance, adi_adrv9001_AuxAdc_e Channel, bool Value )
{
  if( adi_adrv9001_AuxAdc_Configure(&Instance->Device, Channel, Value) != 0)
    return Adrv9001Status_AdcErr;

  return Adrv9001Status_Success;
}

int32_t Adrv9001_SetDacEnable( adrv9001_t *Instance, adi_adrv9001_AuxDac_e Id, bool Enable )
{
  adi_adrv9001_GpioSignal_e signal;

  adi_adrv9001_GpioCfg_t Cfg = {
      .polarity = ADI_ADRV9001_GPIO_POLARITY_NORMAL,
      .master = ADI_ADRV9001_GPIO_MASTER_ADRV9001
  };

  switch( Id )
  {
    case ADI_ADRV9001_AUXDAC0 : signal = ADI_ADRV9001_GPIO_SIGNAL_AUX_DAC_0; Cfg.pin = ADI_ADRV9001_GPIO_ANALOG_00; break;
    case ADI_ADRV9001_AUXDAC1 : signal = ADI_ADRV9001_GPIO_SIGNAL_AUX_DAC_1; Cfg.pin = ADI_ADRV9001_GPIO_ANALOG_01; break;
    case ADI_ADRV9001_AUXDAC2 : signal = ADI_ADRV9001_GPIO_SIGNAL_AUX_DAC_2; Cfg.pin = ADI_ADRV9001_GPIO_ANALOG_02; break;
    case ADI_ADRV9001_AUXDAC3 : signal = ADI_ADRV9001_GPIO_SIGNAL_AUX_DAC_3; Cfg.pin = ADI_ADRV9001_GPIO_ANALOG_03; break;
    default: return Adrv9001Status_DacErr;
  }

  if( adi_adrv9001_gpio_Configure(&Instance->Device, signal, &Cfg) != 0)
    return Adrv9001Status_GpioErr;

  if( adi_adrv9001_AuxDac_Configure(&Instance->Device, Id, Enable) != 0 )
    return Adrv9001Status_GpioErr;

  return Adrv9001Status_Success;
}

int32_t Adrv9001_GetDacEnable( adrv9001_t *Instance, adi_adrv9001_AuxDac_e Id, bool *Enable )
{
  if( adi_adrv9001_AuxDac_Inspect(&Instance->Device, Id, Enable) != 0 )
    return Adrv9001Status_GpioErr;

  return Adrv9001Status_Success;
}

int32_t Adrv9001_SetDgitalGpioDirection( adrv9001_t *Instance, adi_adrv9001_GpioPinCrumbSel_e Bank, adi_adrv9001_GpioPinDirection_e Dir )
{
  if( Dir == ADI_ADRV9001_GPIO_PIN_DIRECTION_OUTPUT )
  {
    if( adi_adrv9001_gpio_ManualOutput_Configure(&Instance->Device, Bank ) != 0 )
      return Adrv9001Status_GpioErr;
  }
  else
  {
    if( adi_adrv9001_gpio_ManualInput_Configure(&Instance->Device, Bank ) != 0 )
      return Adrv9001Status_GpioErr;
  }

  return Adrv9001Status_Success;
}

int32_t Adrv9001_SetAnalogGpioDirection( adrv9001_t *Instance, adi_adrv9001_GpioAnalogPinNibbleSel_e Bank, adi_adrv9001_GpioPinDirection_e Dir )
{
  if( Dir == ADI_ADRV9001_GPIO_PIN_DIRECTION_OUTPUT )
  {
    if( adi_adrv9001_gpio_ManualAnalogOutput_Configure(&Instance->Device, Bank ) != 0 )
      return Adrv9001Status_GpioErr;
  }
  else
  {
    if( adi_adrv9001_gpio_ManualAnalogInput_Configure(&Instance->Device, Bank ) != 0 )
      return Adrv9001Status_GpioErr;
  }

  return Adrv9001Status_Success;
}

int32_t Adrv9001_SetGpioDirection( adrv9001_t *Instance, adi_adrv9001_GpioPin_e Pin, adi_adrv9001_GpioPinDirection_e Dir )
{
  int32_t status;

  switch( Pin )
  {
    case ADI_ADRV9001_GPIO_DIGITAL_00:
    case ADI_ADRV9001_GPIO_DIGITAL_01:
      status = Adrv9001_SetDgitalGpioDirection( Instance, ADI_ADRV9001_GPIO_PIN_CRUMB_01_00, Dir );
      break;

    case ADI_ADRV9001_GPIO_DIGITAL_02:
    case ADI_ADRV9001_GPIO_DIGITAL_03:
      status = Adrv9001_SetDgitalGpioDirection( Instance, ADI_ADRV9001_GPIO_PIN_CRUMB_03_02, Dir );
      break;

    case ADI_ADRV9001_GPIO_DIGITAL_04:
    case ADI_ADRV9001_GPIO_DIGITAL_05:
      status = Adrv9001_SetDgitalGpioDirection( Instance, ADI_ADRV9001_GPIO_PIN_CRUMB_05_04, Dir );
      break;

    case ADI_ADRV9001_GPIO_DIGITAL_06:
    case ADI_ADRV9001_GPIO_DIGITAL_07:
      status = Adrv9001_SetDgitalGpioDirection( Instance, ADI_ADRV9001_GPIO_PIN_CRUMB_07_06, Dir );
      break;

    case ADI_ADRV9001_GPIO_DIGITAL_08:
    case ADI_ADRV9001_GPIO_DIGITAL_09:
      status = Adrv9001_SetDgitalGpioDirection( Instance, ADI_ADRV9001_GPIO_PIN_CRUMB_09_08, Dir );
      break;

    case ADI_ADRV9001_GPIO_DIGITAL_10:
    case ADI_ADRV9001_GPIO_DIGITAL_11:
      status = Adrv9001_SetDgitalGpioDirection( Instance, ADI_ADRV9001_GPIO_PIN_CRUMB_11_10, Dir );
      break;

    case ADI_ADRV9001_GPIO_ANALOG_00:
    case ADI_ADRV9001_GPIO_ANALOG_01:
    case ADI_ADRV9001_GPIO_ANALOG_02:
    case ADI_ADRV9001_GPIO_ANALOG_03:
      status = Adrv9001_SetAnalogGpioDirection( Instance, ADI_ADRV9001_GPIO_ANALOG_PIN_NIBBLE_03_00, Dir );
      break;

    case ADI_ADRV9001_GPIO_ANALOG_04:
    case ADI_ADRV9001_GPIO_ANALOG_05:
    case ADI_ADRV9001_GPIO_ANALOG_06:
    case ADI_ADRV9001_GPIO_ANALOG_07:
      status = Adrv9001_SetAnalogGpioDirection( Instance, ADI_ADRV9001_GPIO_ANALOG_PIN_NIBBLE_07_04, Dir );
      break;

    case ADI_ADRV9001_GPIO_ANALOG_08:
    case ADI_ADRV9001_GPIO_ANALOG_09:
    case ADI_ADRV9001_GPIO_ANALOG_10:
    case ADI_ADRV9001_GPIO_ANALOG_11:
      status = Adrv9001_SetAnalogGpioDirection( Instance, ADI_ADRV9001_GPIO_ANALOG_PIN_NIBBLE_11_08, Dir );
      break;

    default:
      status = Adrv9001Status_InvalidParameter;
      break;

  }

  return status;
}

int32_t Adrv9001_GetGpioDirection( adrv9001_t *Instance, adi_adrv9001_GpioPin_e Pin, adi_adrv9001_GpioPinDirection_e *Dir )
{
  if( adi_adrv9001_gpio_PinDirection_Get(&Instance->Device, Pin, Dir ) != 0)
    return Adrv9001Status_GpioErr;

  return Adrv9001Status_Success;
}

int32_t Adrv9001_SetGpioPinLevel( adrv9001_t *Instance, adi_adrv9001_GpioPin_e Pin, adi_adrv9001_GpioPinLevel_e Level )
{
  if( adi_adrv9001_gpio_OutputPinLevel_Set(&Instance->Device, Pin, Level) != 0)
    return Adrv9001Status_GpioErr;

  return Adrv9001Status_Success;
}

int32_t Adrv9001_GetGpioPinLevel( adrv9001_t *Instance, adi_adrv9001_GpioPin_e Pin, adi_adrv9001_GpioPinLevel_e *Level )
{
  adi_adrv9001_GpioPinDirection_e Dir;
  if( adi_adrv9001_gpio_PinDirection_Get(&Instance->Device, Pin, &Dir ) != 0)
    return Adrv9001Status_GpioErr;

  if( Dir == ADI_ADRV9001_GPIO_PIN_DIRECTION_INPUT)
  {
    if( adi_adrv9001_gpio_InputPinLevel_Get(&Instance->Device, Pin, Level) != 0)
      return Adrv9001Status_GpioErr;
  }
  else
  {
    if( adi_adrv9001_gpio_OutputPinLevel_Get(&Instance->Device, Pin, Level) != 0)
      return Adrv9001Status_GpioErr;
  }

  return Adrv9001Status_Success;
}

int32_t Adrv9001_SetDacVoltage(adrv9001_t *Instance, adi_adrv9001_AuxDac_e Channel, float Voltage )
{
  uint16_t Value;

  if( Voltage >= ADRV9001_MAX_DAC_VOLTAGE )
  {
    Value = 4095;
  }
  else
  {
    Value = (uint16_t)((((Voltage - 0.900)/ 1.700) * 4096.000) + 2048);
  }

  if( adi_adrv9001_AuxDac_Code_Set(&Instance->Device, Channel, Value) != 0)
    return Adrv9001Status_DacErr;

  return Adrv9001Status_Success;
}

int32_t Adrv9001_GetDacVoltage( adrv9001_t *Instance, adi_adrv9001_AuxDac_e Channel, float *Voltage )
{
  uint16_t Value;

  if( adi_adrv9001_AuxDac_Code_Get(&Instance->Device, Channel, &Value) != 0 )
    return Adrv9001Status_DacErr;

  *Voltage = 0.9000 + (((float)Value - 2048.000) / 4096.000) * 1.7;

  return Adrv9001Status_Success;
}
int32_t Adrv9001_SetLogPath(adrv9001_t *Instance, char* LogPath)
{
  if(strlen(LogPath) < ADRV9001_LOG_PATH_SIZE)
  {
    strcpy( Instance->LogPath, LogPath );
    return Adrv9001Status_Success;
  }
  else
    return Adrv9001Status_LogPathTooLong;
}

int32_t Adrv9001_GetLogPath(adrv9001_t *Instance, char* LogPath)
{
  strcpy( LogPath, Instance->LogPath );

  return Adrv9001Status_Success;
}

int32_t Adrv9001_LogWrite(void *devHalCfg, uint32_t logLevel, const char *comment, va_list argp)
{
#ifdef ADRV9001_USE_FS

  adrv9001_t *Adrv9001 = (adrv9001_t*)devHalCfg;

  if( strlen(Adrv9001->LogPath) > 0 )
  {
    UINT len = 0;

    char *str = calloc(1,1024);

    vsnprintf( str, 1023, comment, argp );

    if(f_write(&Adrv9001->LogFil, str, strlen(str), (UINT*)&len) != FR_OK)
      return Adrv9001Status_LogErr;

    f_sync(&Adrv9001->LogFil);

    free(str);
  }
#endif
  return Adrv9001Status_Success;
}

int32_t Adrv9001_DelayUs(void *devHalCfg, uint32_t time_us)
{
  //	while( time_us > 1000 )
  //	{
  //	  vTaskDelay( portTICK_PERIOD_MS );
  //
  //	  time_us -= 1000;
  //	}

  usleep(time_us);

  return Adrv9001Status_Success;
}

int32_t Adrv9001_Open( void *devHalCfg )
{
#ifdef ADRV9001_USE_FS
  adrv9001_t *Adrv9001 = (adrv9001_t*)devHalCfg;

  if( strlen(Adrv9001->LogPath) > 0 )
  {
    /* Delete PHY Log file */
    f_unlink(Adrv9001->LogPath);

    /* Open File */
    if( f_open(&Adrv9001->LogFil, Adrv9001->LogPath, FA_CREATE_NEW | FA_WRITE | FA_READ) != FR_OK)
      return ADI_COMMON_ACT_WARN_RESET_LOG;

    /* Point to beginning of file */
    f_lseek(&Adrv9001->LogFil, 0);

    f_sync(&Adrv9001->LogFil);
  }

#endif

  return Adrv9001Status_Success;
}

int32_t Adrv9001_Close( void *devHalCfg )
{
#ifdef ADRV9001_USE_FS
  adrv9001_t *Adrv9001 = (adrv9001_t*)devHalCfg;

  if( strlen(Adrv9001->LogPath) > 0 )
  {
    f_sync(&Adrv9001->LogFil);

    f_close(&Adrv9001->LogFil);
  }
#endif
  return Adrv9001Status_Success;
}

int32_t Adrv9001_SpiWrite( void *devHalCfg, const uint8_t txData[], uint32_t numTxBytes )
{
  adrv9001_t *Adrv9001 = (adrv9001_t*)devHalCfg;

  return AxiAdrv9001_MspiTransfer( &Adrv9001->Axi, (uint8_t*)txData, NULL, numTxBytes);
}

int32_t Adrv9001_SpiRead( void *devHalCfg, const uint8_t txData[], uint8_t rxData[], uint32_t numRxBytes )
{
  adrv9001_t *Adrv9001 = (adrv9001_t*)devHalCfg;

  return AxiAdrv9001_MspiTransfer( &Adrv9001->Axi, (uint8_t*)txData, rxData, numRxBytes);
}

int32_t Adrv9001_ResetbPinSet( void *devHalCfg, uint8_t pinLevel )
{
  adrv9001_t *Adrv9001 = (adrv9001_t*)devHalCfg;

  AxiAdrv9001_ResetbPinSet( &Adrv9001->Axi, pinLevel );

  return Adrv9001Status_Success;
}

int32_t Adrv9001_GetHopFrequency( adrv9001_t *Instance, uint64_t *Value )
{
  adi_adrv9001_FhHopFrame_t hopFrame;

  if( adi_adrv9001_fh_FrameInfo_Inspect(&Instance->Device, ADI_ADRV9001_FH_HOP_SIGNAL_1, ADI_ADRV9001_FHFRAMEINDEX_CURRENT_FRAME, &hopFrame) != 0)
    return Adrv9001Status_ReadErr;

  memcpy((uint8_t*)Value, (uint8_t*)&hopFrame.hopFrequencyHz, sizeof( hopFrame.hopFrequencyHz));

  return Adrv9001Status_Success;
}

int32_t Adrv9001_GetNextHopFrequency( adrv9001_t *Instance, uint64_t *Value )
{
  adi_adrv9001_FhHopFrame_t hopFrame;

  if( adi_adrv9001_fh_FrameInfo_Inspect(&Instance->Device, ADI_ADRV9001_FH_HOP_SIGNAL_1, ADI_ADRV9001_FHFRAMEINDEX_CURRENT_FRAME, &hopFrame) != 0)
    return Adrv9001Status_ReadErr;

  for( int i = 0; i < Instance->HopTableSize; i++ )
  {
    if( Instance->HopTable[i].hopFrequencyHz == hopFrame.hopFrequencyHz )
    {
      if( i == (Instance->HopTableSize - 1))
      {
        memcpy((uint8_t*)Value, (uint8_t*)&Instance->HopTable[0].hopFrequencyHz, sizeof( uint64_t ));
        return Adrv9001Status_Success;
      }
      else
      {
        memcpy((uint8_t*)Value, (uint8_t*)&Instance->HopTable[i + 1].hopFrequencyHz, sizeof( uint64_t ));
        return Adrv9001Status_Success;
      }
    }
  }

  return Adrv9001Status_InvalidHopFreq;
}

int32_t Adrv9001_LoadHopTable( adrv9001_t *Instance, uint8_t size )
{
  Instance->HopTableSize = size;

  return adi_adrv9001_fh_HopTable_Static_Configure(&Instance->Device, ADI_ADRV9001_FHMODE_LO_RETUNE_REALTIME_PROCESS, ADI_ADRV9001_FH_HOP_SIGNAL_1, ADI_ADRV9001_FHHOPTABLE_A, Instance->HopTable, size);
}

int32_t Adrv9001_SetHopTableFrequency( adrv9001_t *Instance, uint8_t index, uint64_t Freq )
{
  if( index > Instance->HopTableSize )
    return Adrv9001Status_InvalidParameter;

  memcpy((uint8_t*)&Instance->HopTable[index].hopFrequencyHz, (uint8_t*)&Freq, sizeof( Freq ));

  return Adrv9001Status_Success;
}

int32_t Adrv9001_SetHopTableAttn( adrv9001_t *Instance, uint8_t index, float Attn )
{
  if( index > Instance->HopTableSize )
    return Adrv9001Status_InvalidParameter;

  Attn = Attn * 5;

  uint8_t attn_fifthdb = (uint8_t)Attn;

  Instance->HopTable[index].tx1Attenuation_fifthdB = attn_fifthdb;

  return Adrv9001Status_Success;
}

int32_t Adrv9001_GetHopTableAttn( adrv9001_t *Instance, uint8_t index, float *Attn )
{
  if( index > Instance->HopTableSize )
    return Adrv9001Status_InvalidParameter;

  float v = (float)Instance->HopTable[index].tx1Attenuation_fifthdB;

  v = v / 5.0;

  *Attn = v;

  return Adrv9001Status_Success;
}

int32_t Adrv9001_GetHopTableFrequency( adrv9001_t *Instance, uint8_t index, uint64_t *Freq )
{
  if( index > Instance->HopTableSize )
    return Adrv9001Status_InvalidParameter;

  memcpy((uint8_t*)Freq, (uint8_t*)&Instance->HopTable[index].hopFrequencyHz, sizeof( uint64_t ));

  return Adrv9001Status_Success;
}


int32_t(*adi_common_hal_LogWrite)(void *devHalCfg, uint32_t logLevel, const char *formatStr, va_list argp)    = Adrv9001_LogWrite;
int32_t(*adi_common_hal_Wait_us)(void *devHalCfg, uint32_t time_us)                                           = Adrv9001_DelayUs;
int32_t(*adi_adrv9001_hal_open)(void *devHalCfg)                                                              = Adrv9001_Open;
int32_t(*adi_adrv9001_hal_close)(void *devHalCfg)                                                             = Adrv9001_Close;
int32_t(*adi_hal_SpiWrite)(void *devHalCfg, const uint8_t txData[], uint32_t numTxBytes)                      = Adrv9001_SpiWrite;
int32_t(*adi_hal_SpiRead)(void *devHalCfg, const uint8_t txData[], uint8_t rxData[], uint32_t numRxBytes)     = Adrv9001_SpiRead;
int32_t(*adi_adrv9001_hal_resetbPin_set)(void *devHalCfg, uint8_t pinLevel)                                   = Adrv9001_ResetbPinSet;

int32_t(*adi_hal_ArmImagePageGet)(void *devHalCfg, const char *armImagePath, uint32_t pageIndex, uint32_t pageSize, uint8_t *rdBuff) = NULL;
int32_t(*adi_hal_StreamImagePageGet)(void *devHalCfg, const char *streamImagePath, uint32_t pageIndex, uint32_t pageSize, uint8_t *rdBuff) = NULL;
int32_t(*adi_hal_RxGainTableEntryGet)(void *devHalCfg, const char *rxGainTablePath, uint16_t lineCount, uint8_t *gainIndex, uint8_t *rxFeGain, uint8_t *tiaControl, uint8_t *adcControl, uint8_t *extControl, uint16_t *phaseOffset, int16_t *digGain) = NULL;
int32_t(*adi_hal_TxAttenTableEntryGet)(void *devHalCfg, const char *txAttenTablePath, uint16_t lineCount, uint16_t *attenIndex, uint8_t *txAttenHp, uint16_t *txAttenMult) = NULL;



