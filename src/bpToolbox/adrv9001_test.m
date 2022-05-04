%% Connect
close all; clear all;

dut = adrv9001( );
dut.Adrv9001_Connect();
info = dut.Adrv9001_GetVerInfo();
disp(info);

%% Load Profile
profile_directory = uigetdir('','Profile Directory' );
dut.Adrv9001_LoadProfile(profile_directory);

%% Configure ADRV9001
dut.Adrv9001_Initialize( );
dut.Adrv9001_Calibrate( );
dut.Adrv9001_Configure( );
dut.Adrv9001_Prime( );

%% Capture Rx Data
dut.Adrv9001_BeginReceiving();
state = dut.Adrv9001_GetRadioState(dut.PORT_RX, dut.CHANNEL_1);
disp(['Rx1 State = ', state]);

iqdata = dut.Adrv9001_DataCapture(4096, dut.CHANNEL_1);

dut.Adrv9001_StopReceiving();
state = dut.Adrv9001_GetRadioState(dut.PORT_RX, dut.CHANNEL_1);
disp(['Rx1 State = ', state]);

figure();
bins = 1024;
h2 = spectrum.welch('Hamming',bins);
spec = msspectrum(h2,iqdata,...
    'Fs',fs,...
    'SpectrumType','twosided',...
    'CenterDC',true);
f = spec.Frequencies/1e6;
a = 10*log10(spec.Data);
plot(f,a);


%% Transmit

dut.Adrv9001_BeginTransmitting();
state = dut.Adrv9001_GetRadioState(dut.PORT_TX, dut.CHANNEL_1);
disp(['Tx1 State = ', state]);

input('Press Enter to stop transmitting');

dut.Adrv9001_StopTransmitting();
state = dut.Adrv9001_GetRadioState(dut.PORT_TX, dut.CHANNEL_1);
disp(['Tx1 State = ', state]);

%% Plot PFir Filter Response
figure();
[x,y] = dut.Adrv9001_GetPFirResponse( dut.PORT_RX, dut.CHANNEL_1, 'A');
plot(x/1e6,y);
hold all; grid on;
[x,y] = dut.Adrv9001_GetPFirResponse( dut.PORT_RX, dut.CHANNEL_1, 'B');
plot(x/1e6,y);
[x,y] = dut.Adrv9001_GetPFirResponse( dut.PORT_RX, dut.CHANNEL_1, 'C');
plot(x/1e6,y);
[x,y] = dut.Adrv9001_GetPFirResponse( dut.PORT_RX, dut.CHANNEL_1, 'D');
plot(x/1e6,y);
title('Rx Programmable FIR Response');
xlabel('Frequency (MHz)')
ylabel('Magnitude (dBFS)');
legend('A', 'B', 'C', 'D');

figure();
[x,y] = dut.Adrv9001_GetPFirResponse( dut.PORT_TX, dut.CHANNEL_1, 'A');
plot(x/1e6,y);
hold all; grid on;
[x,y] = dut.Adrv9001_GetPFirResponse( dut.PORT_TX, dut.CHANNEL_1, 'B');
plot(x/1e6,y);
[x,y] = dut.Adrv9001_GetPFirResponse( dut.PORT_TX, dut.CHANNEL_1, 'C');
plot(x/1e6,y);
[x,y] = dut.Adrv9001_GetPFirResponse( dut.PORT_TX, dut.CHANNEL_1, 'D');
plot(x/1e6,y);
title('Tx Programmable FIR Response');
xlabel('Frequency (MHz)')
ylabel('Magnitude (dBFS)');
legend('A', 'B', 'C', 'D');



