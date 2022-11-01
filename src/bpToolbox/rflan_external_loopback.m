%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% This script will load the transmiter DMA stream buffer from the specified 
% pre-generated iq file on the sd card and begin transmitting. This is 
% followed by a receiver DMA stream enable and several receiver DMA buffer 
% reads.  Each produces a spectral plot of the received buffer.
%
% Externally connect transmit and receiver ports together using a coaxial
% cable.  
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
clear all; close all; clc;

h = rflan();
h.Open('COM16');

%% Perform Loopback

TxPort = h.Tx2;
RxPort = h.Rx2;
RxBufLength = 4096;

% Set Tx Output Power
h.SetTxAttn(TxPort, 20);
h.SetTxBoost(TxPort, 0);

% Load Transmit buffer with IQ data from file on SD card
h.RflanStreamBufLoad(TxPort,'sample_rate_24K_Tone_3K.csv');

% Enable continuous transmit of firt 4096 points of the file
h.RflanStreamStart(TxPort, 1, 4096);

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

