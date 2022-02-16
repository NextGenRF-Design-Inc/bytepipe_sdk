close all;
clear all;

fs = 15.36e6;

iq = BytePipe_WavformFileRead("rx1.csv");

bins = 1000; 
h2 = spectrum.welch('Hamming',bins);
spec = msspectrum(h2,iq,...
        'Fs',fs,...
        'SpectrumType','twosided',...
        'CenterDC',true);    
f = spec.Frequencies/1e6;
a = 10*log10(spec.Data);    

figure; 
plot(f,a);
grid on; xlabel('Frequency (MHz)'); ylabel('Power (dB)'); title('Spectrum');


