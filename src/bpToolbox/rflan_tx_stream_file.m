%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% This script enables the transmitter DMA stream.
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
clear all; close all; clc;

h = rflan();
h.Open('COM16');

%% Enable Stream

% Select Transmit Port
TxPort = h.Tx2;

% Transmit burst or continous using cyclic flag
cyclic = 1;

% Set Tx Settings
h.SetTxAttn(TxPort, 0);
h.SetTxBoost(TxPort, 1);

% Load Transmit buffer with IQ data from file on SD card
h.RflanStreamBufLoad(TxPort,'CFR_sample_rate_15p36M_bw_10M.csv');

% Delay before enabling SSI
h.SetSsiEnableDelay(TxPort, 500);

% Delay before disabling tx to flush ssi data
h.SetTxDisableDelay(TxPort, 200);

% Get Sample Rate 
fs = h.GetSampleRate(TxPort);

% Enable continuous transmit of the iq data
h.RflanStreamStart(TxPort, cyclic, 76800);

% Get Carrier Frequency 
fc = h.GetCarrierFrequency(TxPort);

%% Disable Stream

h.RflanStreamStop(TxPort);
