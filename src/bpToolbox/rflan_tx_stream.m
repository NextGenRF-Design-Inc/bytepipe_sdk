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

% Stop Previous Streams
h.RflanStreamStop(TxPort);

% Set Tx Settings
h.SetTxAttn(TxPort, 0);
h.SetTxBoost(TxPort, 1);

% Delay before enabling SSI
h.SetSsiEnableDelay(TxPort, 500);

% Delay before disabling tx to flush ssi data
h.SetTxDisableDelay(TxPort, 200);

% Get Sample Rate 
fs = h.GetSampleRate(TxPort);

% Generate transmit tone as a factor of sample rate
f_tone = fs/32;
t = (0:4*fs/f_tone-1)'/fs;
iq = 1/2 * (sin(2*pi*f_tone*t)+1i*cos(2*pi*f_tone*t));

% Load Transmit Buffer with signal
h.RflanStreamBufPut(TxPort,0,iq);

% Enable continuous transmit of the iq data
h.RflanStreamStart(TxPort, cyclic, length(iq));

% Get Carrier Frequency 
fc = h.GetCarrierFrequency(TxPort) - f_tone;

disp(['Tone Frequency = ' num2str(fc)]);

figure();
bins = length(iq)/2; 
h2 = spectrum.welch('Hamming',bins);
spec = msspectrum(h2,iq,...
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
plot(real(iq),imag(iq));
xlabel('In-Phase');
ylabel('Quadrature-Phase');
title('Constillation');

subplot(2,2,[3 4]);
t = (1:length(iq))/(fs/1e6);
plot(t,real(iq));
hold on;
plot(t,imag(iq));
xlabel('time(us)');
ylabel('Magnitude');
title('Time Domain');

%% Disable Stream

h.RflanStreamStop(TxPort);

