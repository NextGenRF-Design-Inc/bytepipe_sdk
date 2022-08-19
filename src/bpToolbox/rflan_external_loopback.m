clear all; close all; clc;

h = rflan();
h.Open('COM16');

figure();

% Read Carrier Frequency
h.GetCarrierFrequency(h.Tx1);
fc = h.GetCarrierFrequency(h.Rx1);
fs = h.GetSampleRate(h.Rx1);

% Set Tx Output Power
h.SetTxAttn(h.Tx1, 0);
h.SetTxBoost(h.Tx1, 0);

% Load Transmit buffer with IQ data from file on SD card
h.RflanStreamBufLoad(h.Tx1,'CFR_sample_rate_15p36M_bw_10M.csv');

% Start continuous receive
h.RflanStreamStart(h.Rx1, 1, 2e3);

% Read IQ samples from stream buffer
iq = h.RflanStreamBufGet(h.Rx1,0,2e3);

bins = length(iq)/2; 
h2 = spectrum.welch('Hamming',bins);
spec = msspectrum(h2,iq,...
        'Fs',fs,...
        'SpectrumType','twosided',...
        'CenterDC',true);    
    
f = spec.Frequencies/1e6;
a = 10*log10(spec.Data);    

plot(f,a);
hold all;
grid on; xlabel('Frequency (MHz)'); ylabel('Power (dB)'); title('Spectrum');

% Enable continuous transmit stream
h.RflanStreamStart(h.Tx1, 1, 10e3);


for i = 1:5    
pause(1);    
iq = h.RflanStreamBufGet(h.Rx1,0,2e3);

bins = length(iq)/2; 
h2 = spectrum.welch('Hamming',bins);
spec = msspectrum(h2,iq,...
        'Fs',fs,...
        'SpectrumType','twosided',...
        'CenterDC',true);    
    
f = spec.Frequencies/1e6;
a = 10*log10(spec.Data);    

plot(f,a);
end

h.RflanStreamStop(h.Rx1);

