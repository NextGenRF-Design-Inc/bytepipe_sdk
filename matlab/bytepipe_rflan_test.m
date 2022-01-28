%% Init
clear all;
close all;

% Instantiate Object
bp = bytepipe_rflan();

bp.Open('COM4');


%% Rx1/Tx1 Loopback

bp.LoadDmaConstant(bp.Tx1, hex2dec('12345678'));
bp.Adrv9001SetLoopBack(bp.Tx1, 1);
bp.Adrv9001ToRfEnabled(bp.Tx1);
bp.Adrv9001ToRfEnabled(bp.Rx1);

bp.Adrv9001ReadDma(bp.Rx1, 40);

%% Rx2/Tx2 Loopback

bp.LoadDmaConstant(bp.Tx2, hex2dec('12345678'));
bp.Adrv9001SetLoopBack(bp.Tx2, 1);
bp.Adrv9001ToRfEnabled(bp.Tx2);
bp.Adrv9001ToRfEnabled(bp.Rx2);

bp.Adrv9001ReadDma(bp.Rx2, 40);

%% Rx1/Tx1 Ramp

bp.LoadDmaRamp(bp.Tx1);
bp.Adrv9001SetLoopBack(bp.Tx1, 1);
bp.Adrv9001ToRfEnabled(bp.Tx1);
bp.Adrv9001ToRfEnabled(bp.Rx1);

bp.Adrv9001ReadDma(bp.Rx1, 40);


%% Tx1 Stream

bp.PhyIqFileStreamEnable(bp.Tx1, 'TETRA1_sample_rate_144K_bw_25K.csv', -1);

%% RSSI

for i = 1 : 1000
    
    bp.Adrv9001GetRssi(bp.Rx1);
    bp.Adrv9001GetRssi(bp.Rx2);
    pause(0.4);
end


