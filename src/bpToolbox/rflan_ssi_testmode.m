%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% This script enables receiver SSI test mode where the ADRV9001 streams
% incrementing IQ data.  The following captures DMA samples and checks for
% incrementing pattern.
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
clear all; close all; clc;

h = rflan();
h.Open('COM16');
RxPort = h.Rx1;

%% Incrementig Test

% Select Capture Length of DMA
RxBufLength = 2000;

% Delay in samples between rising edge of enable and rx_axis_tvalid
h.SetEnableDelay(RxPort, 6000);

% Delay in samples between falling edge of enable and falling edge of rx_axis_tvalid 
h.SetDisableDelay(RxPort, 100);

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

%% Constant Test

% Set Test Mode Data
h.SetTestModeData( RxPort, 1);

% Set Test Mode to Constant
h.SetTestMode(RxPort,1);

% Get Enable Mode
mode = h.GetEnableMode(RxPort);

% Set Enable Mode to SPI
h.SetEnableMode(RxPort,h.EnableModeSpi);

% Enable Radio
h.SetRadioState(RxPort, h.Enabled);

for i = 1:10
    v = h.GetRxTestData(RxPort);
    pause(0.1);
    disp(dec2hex(v));
end

% Return configuration 
h.SetRadioState(RxPort, h.Primed);
h.SetEnableMode(RxPort, mode);

%% Cleanup

% Set Test Mode to normal
h.SetTestMode(RxPort,0);