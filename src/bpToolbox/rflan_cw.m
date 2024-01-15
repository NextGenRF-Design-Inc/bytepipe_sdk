%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% This script enables a CW transmit tone using the ADRV9002 HDL instead
% of streaming data using the DMA.
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
clear all; close all; clc;
h = rflan();
h.Open('COM4');

%% Enable CW 
TxPort = h.Tx1;

% Set Carrier Frequency (optional)
h.SetCarrierFrequency(TxPort, 2.1e9);

% Set Tx Settings
h.SetTxAttn(TxPort, 10);
h.SetTxBoost(TxPort, 0);

% Set Data Source
h.SetFixedPattern(h.Tx1, hex2dec('3ff03fff'));
h.SetTxDataSrc(TxPort, h.TxDataSrcFixed);
h.GetTxDataSrc(TxPort);

% Enable Transmitter
h.Adrv9001ToRfEnabled( TxPort );

h.GetRadioState(TxPort);

%% Change Attn

h.SetTxAttn(TxPort, 2.55);

h.GetTxAttn(TxPort)

%% Disable CW

h.Adrv9001ToRfPrimed( TxPort );
h.SetTxDataSrc(TxPort, h.TxDataSrcAxis);
h.GetTxDataSrc(TxPort);
