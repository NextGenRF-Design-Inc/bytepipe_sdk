%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% This script enables SSI loopback follwed by the receiver DMA stream and plots the spectrum of 
% several receiver DMA captures.
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
clear all; close all; clc;

h = rflan();
h.Open('COM16');

TxPort = h.Tx1;

if( TxPort == h.Tx1 )
    RxPort = h.Rx1;
    h.SetTxRx1SsiLoopBack(1);
else
    RxPort = h.Rx2;
    h.SetTxRx2SsiLoopBack(1);
end

h.RflanStreamStop(RxPort);
h.RflanStreamStop(TxPort);

txIq = complex([0:1000]',[-1000:0]') / 2^15;

% Load Transmit buffer
h.RflanStreamBufPut(TxPort,0,txIq);

% Enable continuous transmit stream
h.RflanStreamStart(TxPort, 1, length(txIq));

% Enable one shot receive stream
h.RflanStreamStart(RxPort, 1, length(txIq));

% Read Receiver Buffer
rxIq = h.RflanStreamBufGet(RxPort,0,length(txIq));

figure();
plot(real(txIq));
hold all; grid on;
plot(real(rxIq));
legend('Tx','Rx');

h.RflanStreamStop(TxPort);
h.SetTxRx1SsiLoopBack(0);
h.SetTxRx2SsiLoopBack(0);


