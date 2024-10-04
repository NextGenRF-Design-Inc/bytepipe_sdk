%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% This script enables a CW transmit tone using the ADRV9002 HDL instead
% of streaming data using the DMA.
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
clear all; close all; clc;
h = rflan();
h.Open('COM12');

%% Enable CW 
TxPort = h.Tx1;

% Load constant IQ value
h.SetTxIqConstant(TxPort, hex2dec('7fff7fff'));

% Change data path from DMA to Constant
h.SetTxIqDataPath(TxPort, h.Adrv9001TxDataSrc_FixedPattern);
%h.SetTxIqDataPath(TxPort, h.Adrv9001TxDataSrc_Ones);

% Enable Transmitter
h.Adrv9001ToRfEnabled( TxPort );

%% Disable CW

h.Adrv9001ToRfPrimed( TxPort );
h.SetTxIqDataPath(TxPort, h.Adrv9001TxDataSrc_Axis);


