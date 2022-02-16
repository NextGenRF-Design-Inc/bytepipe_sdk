%% Init
clear all;
close all;

% Instantiate Object
bp = bytepipe_rflan();

bp.Open('COM21');


%% Rx1/Tx1 Loopback
bp.DmaLoadConstant(bp.Tx1, hex2dec('12345678'));
bp.Adrv9001SetLoopBack(bp.Tx1, 1);
bp.Adrv9001ToRfEnabled(bp.Tx1);
bp.Adrv9001ToRfEnabled(bp.Rx1);

bp.Adrv9001ReadDma(bp.Rx1, 40);

%% Rx2/Tx2 Loopback
bp.DmaLoadConstant(bp.Tx2, hex2dec('12345678'));
bp.Adrv9001SetLoopBack(bp.Tx2, 1);
bp.Adrv9001ToRfEnabled(bp.Tx2);
bp.Adrv9001ToRfEnabled(bp.Rx2);

bp.Adrv9001ReadDma(bp.Rx2, 40);

%% Rx1/Tx1 Ramp
bp.DmaLoadRamp(bp.Tx1);
bp.Adrv9001SetLoopBack(bp.Tx1, 1);
bp.Adrv9001ToRfEnabled(bp.Tx1);
bp.Adrv9001ToRfEnabled(bp.Rx1);

bp.Adrv9001ReadDma(bp.Rx1, 40);

%% Tx2 Stream
bp.PhyIqFileStreamEnable(bp.Tx2, 'TETRA1_sample_rate_144K_bw_25K.csv', -1);

%% Tx1 Stream
bp.PhyIqFileStreamEnable(bp.Tx1, 'CFR_sample_rate_15p36M_bw_10M.csv', -1);

%% RSSI
for i = 1 : 1000
    bp.Adrv9001GetRssi(bp.Rx1);
    bp.Adrv9001GetRssi(bp.Rx2);
    pause(0.4);
end

%% 
fs = 15.36e6;

iq = BytePipe_WavformFileRead("rx1.csv");

bins = 100; 
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

