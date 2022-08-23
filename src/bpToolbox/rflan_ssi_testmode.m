%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% This script enables receiver SSI test mode where the ADRV9001 streams
% incrementing IQ data.  The following captures DMA samples and checks for
% incrementing pattern.
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
clear all; close all; clc;

h = rflan();
h.Open('COM16');
RxPort = h.Rx2;

% Select Capture Length of DMA
RxBufLength = 256;

% Set Test Mode to Incrementing
h.SetTestMode(RxPort,3);

% Enable DMA Burst
h.RflanStreamStart(RxPort, 0, RxBufLength);

% Read DMA buffer
iq = h.RflanStreamBufGet(RxPort,0,RxBufLength);

idata = real(iq)*2^15;
qdata = imag(iq)*2^15;

disp(['Number of I data Errors = ' num2str(sum(idata(2:end)-idata(1:end-1) - 1))]);

disp(['Number of Q data Errors = ' num2str(sum(qdata(2:end)-qdata(1:end-1) - 1))]);

figure();
plot(idata);
hold all; grid on;
plot(qdata);
legend;

% Set Test Mode to normal
h.SetTestMode(RxPort,0);