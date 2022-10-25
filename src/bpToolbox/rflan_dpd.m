%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% This script enables DPD
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
clear all; close all; clc;

h = rflan();
h.Open('COM4');

TxPort = h.Tx2;

% Set Tx Attenuation
h.SetTxAttn(TxPort, 7);
h.SetTxBoost(TxPort, 1);

% Load Transmit buffer with IQ data from file on SD card
h.RflanStreamBufLoad(TxPort,'CFR_sample_rate_15p36M_bw_10M.csv');

% Delay in samples between rising edge of enable and tx_axis_tready
h.SetEnableDelay(TxPort, 500);

%% Disable DPD
h.SetDpdEnable(TxPort, 0);

%% Configure DPD Parameters
h.SetDpdEnable(TxPort, 1);
h.SetExternalLoopbackDelay(TxPort, 3800);
h.SetExternalLoopbackPower(TxPort, -18);
h.SetDpdNumberofSamples(TxPort, 4096);
h.SetDpdRxTxNormalizationLowerThreshold_dB(TxPort,-11);
h.SetDpdRxTxNormalizationUpperThreshold_dB(TxPort,-6);
h.SetDpdDetectionPowerThreshold_dB(TxPort,-16);
h.SetDpdDetectionPeakThreshold_dB(TxPort,-10);

%% Restart Transmit
h.RflanStreamStop(TxPort);
h.RflanStreamStart(TxPort, 1, 38400);

%% Stop Transmitting
h.RflanStreamStop(TxPort);

%% Get Config
h.GetDpdConfig(TxPort)


