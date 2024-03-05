%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% This script enables the receiver DMA stream and plots the spectrum of 
% several receiver DMA captures.
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
clear all; close all; clc;

h = rflan();
%h.Open('COM12');
h.Open('COM4');
% Select Receive Port
RxPort = h.Rx1;

%% Enable Stream

% Select Capture Length of DMA
RxBufLength = 2000;

% Read Sample Rate
fs = h.GetSampleRate(RxPort);

% Set Carrier Frequency (optional)
%h.SetCarrierFrequency(TxPort, 2.5e9);
h.SetCarrierFrequency(RxPort, 2.5e9);
% Read Carrier Frequency
h.GetCarrierFrequency(RxPort);

% Enable Receiver
h.Adrv9001ToRfEnabled( RxPort );

% Read DMA buffer and plot data several times
figure();
for i = 1:1
    
% Start DMA Burst
h.RflanStreamStart(RxPort, 0, RxBufLength);

pause(1);

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
title('typ');

subplot(2,2,[3 4]);
t = (1:length(iq))/(fs/1e6);
plot(t,real(iq));
hold on;
plot(t,imag(iq));
xlabel('time(us)');
ylabel('Magnitude');
title('Time Domain');

end

%% Disable Receiver

h.Adrv9001ToRfPrimed( RxPort );


