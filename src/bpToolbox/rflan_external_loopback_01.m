%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% This script will load the transmiter DMA stream buffer based on the 
% matlab generated waveform and begin transmitting. This is followed by a 
% receiver DMA stream enable and several receiver DMA buffer reads.  Each 
% produces a spectral plot of the received buffer.
%
% Externally connect transmit and receiver ports together using a coaxial
% cable.  
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
clear all; close all; clc;

h = rflan();
h.Open('COM16');

% Select Transmit and Receive Ports
TxPort = h.Tx2;
RxPort = h.Rx2;

% Select Capture Length of DMA
RxBufLength = 4096;

% Set Tx Output Power
h.SetTxAttn(TxPort, 20);
h.SetTxBoost(TxPort, 0);

% Get Sample Rate 
fs = h.GetSampleRate(TxPort);

% Generate transmit tone as a factor of sample rate
f_tone = fs/32;
t = (0:8*fs/f_tone-1)'/fs;
txiq = 1/2 * (sin(2*pi*f_tone*t)+1i*cos(2*pi*f_tone*t));

% Load Transmit Buffer with signal
h.RflanStreamBufPut(TxPort,0,txiq);

% Enable continuous transmit of the iq data
h.RflanStreamStart(TxPort, 1, length(txiq));

% Read Carrier Frequency
fs = h.GetSampleRate(RxPort);

% Capture and plot several frames
figure();
for i = 1:5    
    
% Start DMA Burst
h.RflanStreamStart(RxPort, 0, RxBufLength);

% Read DMA Buffer
iq = h.RflanStreamBufGet(RxPort,0,RxBufLength);

bins = length(iq)/2; 
h2 = spectrum.welch('Hamming',bins);
spec = msspectrum(h2,iq,...
        'Fs',fs,...
        'SpectrumType','twosided',...
        'CenterDC',true);    
    
f = spec.Frequencies/1e6;
a = 10*log10(spec.Data);    

plot(f,a);
title([RxPort ' Spectrum ']);
xlabel('Frequency(MHz)');
ylabel('Power (dBm)');

end

h.RflanStreamStop(RxPort);
h.RflanStreamStop(TxPort);

