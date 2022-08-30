%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% This script enables the transmitter DMA stream.
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
clear all; close all; clc;

h = rflan();
h.Open('COM16');

% Select Transmit Port
TxPort = h.Tx1;

% Transmit burst or continous using cyclic flag
cyclic = 0;

% Stop Previous Streams
h.RflanStreamStop(TxPort);

% Set Tx Settings
h.SetTxAttn(TxPort, 20);
h.SetTxBoost(TxPort, 0);

% Delay before enabling SSI
h.SetSsiEnableDelay(TxPort, 500);

% Delay before disabling tx to flush ssi data
h.SetTxDisableDelay(TxPort, 200);

% Get Sample Rate 
fs = h.GetSampleRate(TxPort);

% Generate transmit tone as a factor of sample rate
f_tone = fs/32;
t = (0:40*fs/f_tone-1)'/fs;
txiq = 1/2 * (sin(2*pi*f_tone*t)+1i*cos(2*pi*f_tone*t));

% Load Transmit Buffer with signal
h.RflanStreamBufPut(TxPort,0,txiq);

% Enable continuous transmit of the iq data
h.RflanStreamStart(TxPort, cyclic, length(txiq));

figure();    
bins = length(txiq)/2; 
h2 = spectrum.welch('Hamming',bins);
spec = msspectrum(h2,txiq,...
        'Fs',fs,...
        'SpectrumType','twosided',...
        'CenterDC',true);    
    
f = spec.Frequencies/1e6;
a = 10*log10(spec.Data);    

hold off;
subplot(2,2,1);
plot(f,a);
title('Spectrum');
xlabel('Frequency(MHz)');
ylabel('Power (dBm)');
subplot(2,2,2);
plot(real(txiq),imag(txiq));
xlabel('In-Phase');
ylabel('Quadrature-Phase');
title('Constillation');

subplot(2,2,[3 4]);
t = (1:length(txiq))/(fs/1e6);
plot(t,real(txiq));
hold on;
plot(t,imag(txiq));
xlabel('time(us)');
ylabel('Magnitude');
title('Time Domain');






