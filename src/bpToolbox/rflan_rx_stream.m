%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% This script enables the receiver DMA stream and plots the spectrum of 
% several receiver DMA captures.
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
clear all; close all; clc;

h = rflan();
h.Open('COM16');

%% Enable Stream

% Select Receive Port
RxPort = h.Rx1;

% Select Capture Length of DMA
RxBufLength = 2000;

% Read Sample Rate
fs = h.GetSampleRate(RxPort);

% Delay in samples between rising edge of enable and rx_axis_tvalid
h.SetEnableDelay(RxPort, 500);

% Delay in samples between falling edge of enable and falling edge of rx_axis_tvalid 
h.SetDisableDelay(RxPort, 10);

% Read DMA buffer and plot data several times
figure();
for i = 1:15
% Start DMA Burst
h.RflanStreamStart(RxPort, 0, RxBufLength + 10);

pause(1);

% Read DMA Buffer
iq = h.RflanStreamBufGet(RxPort,10,RxBufLength);

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

end




