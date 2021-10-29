close all;
clear all;

fs = 15.36e6;

iq = BytePipe_WavformFileRead("rx.csv");

figure();
bins = 1000; 
h2 = spectrum.welch('Hamming',bins);
spec = msspectrum(h2,iq,...
        'Fs',fs,...
        'SpectrumType','twosided',...
        'CenterDC',true);    
f = spec.Frequencies/1e6;
a = 10*log10(spec.Data);    
plot(f,a);
grid on;
xlabel('Frequency (MHz)');
ylabel('Power (dB)');
title('Spectrum');

figure; 
subplot(2,2,1); plot(f,a);
grid on; xlabel('Frequency (MHz)'); ylabel('Power (dB)'); title('Spectrum');

subplot(2,2,2); plot(real(iq), imag(iq));
grid on; xlabel('Real'); ylabel('Imag'); title('Constillation');

subplot(2,2,3); plot(real(iq));  
grid on; xlabel('Value'); ylabel('Sample'); title('Idata');
subplot(2,2,4); plot(imag(iq));
grid on; xlabel('Value'); ylabel('Sample'); title('Qdata');

