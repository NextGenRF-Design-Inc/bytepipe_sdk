%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% This script enables the transmitter DMA stream.
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
clear all; close all; clc;

h = rflan();
h.Open('COM16');

%% Enable Stream

% Select Transmit Port
TxPort = h.Tx1;

% Transmit burst or continous using cyclic flag
cyclic = 1;

% Stop Previous Streams
h.RflanStreamStop(TxPort);

% Set Tx Settings
h.SetTxAttn(TxPort, 10);
h.SetTxBoost(TxPort, 0);

% Delay before enabling SSI
h.SetSsiEnableDelay(TxPort, 500);

% Delay before disabling tx to flush ssi data
h.SetTxDisableDelay(TxPort, 200);

% Get Sample Rate 
fs = h.GetSampleRate(TxPort);

% Generate transmit tone as a factor of sample rate
f_tone = fs/32;
t = (0:8*fs/f_tone-1)'/fs;
txiq = 1/2 * (sin(2*pi*f_tone*t)+1i*cos(2*pi*f_tone*t));

% Load Transmit Buffer with signal
h.RflanStreamBufPut(TxPort,0,txiq);

% Enable continuous transmit of the iq data
h.RflanStreamStart(TxPort, cyclic, length(txiq));

% Get Carrier Frequency 
fc = h.GetCarrierFrequency(TxPort) - f_tone;

disp(['Tone Frequency = ' num2str(fc)]);

%% Disable Stream

h.RflanStreamStop(TxPort);
