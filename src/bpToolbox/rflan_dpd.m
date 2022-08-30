%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% This script enables DPD
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
clear all; close all; clc;

h = rflan();
h.Open('COM16');

TxPort = h.Tx2;

% Set Tx Output Power
h.SetTxAttn(TxPort, 20);
h.SetTxBoost(TxPort, 0);

% Load Transmit buffer with IQ data from file on SD card
h.RflanStreamBufLoad(TxPort,'CFR_sample_rate_15p36M_bw_10M.csv');

% Enable continuous transmit of the file
h.RflanStreamStart(TxPort, 1, 38400);

