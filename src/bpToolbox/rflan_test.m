% The following sections show example matlab commands supported by the
% BytePipe Toolbox.

%% Connect to BytePipe using serial port.
clear all; close all; clc;

h = rflan();
h.Open('COM16');

%% Help
clc;
h.Write('help');

%% ADRV9001 Version Information
clc;
AdrvSiliconVersion = h.GetAdrvSiliconVersion( );
if( ~strcmp(AdrvSiliconVersion,'C0'))
    error("Error");
end

AdrvFirmwareVersion = h.GetAdrvFirmwareVersion( );
if( ~strcmp(AdrvFirmwareVersion,'0.21.0.8'))
    error("Error");
end

AdrvAPIVersion = h.GetAdrvAPIVersion( );
if( ~strcmp(AdrvAPIVersion,'67.1.1'))
    error("Error");
end


%% ADRV9001 Temperature
clc;
Temp = h.GetTemp();
if( (Temp < -20) || (Temp > 90))
    error("Error");
end

%% VCTCXO

% The 38.4MHz VCTCXO on BytePipe can be configured for +-8ppm by applying
% 0.05 to 1.75V.
clc;
v1 = mod(h.GetVcTcxo( ) + 0.1, 1.7) + 0.05;
h.SetVcTcxo( v1 );
v2 = h.GetVcTcxo( );
if( abs(v2 - v1 ) > 0.01)
    error("Error");
end

%% Carrier Frequency
clc;
v = 2.899e9;
h.SetCarrierFrequency(h.Tx1, v);
v2 = h.GetCarrierFrequency(h.Tx1);
if( abs(v2 - v) > 10 )
    error("Error");
end

%% Transmit Attenuation
clc;
v = h.Enabled;
h.SetRadioState(h.Tx1, v);
if(~strcmp(h.GetRadioState(h.Tx1),v))
    error("Error");
end

v = h.GetTxAttn(h.Tx1) + 0.1;
h.SetTxAttn(h.Tx1, v);
if( h.GetTxAttn(h.Tx1) ~= v )
    error("Error");
end

v = h.Primed;
h.SetRadioState(h.Tx1, v);
if(~strcmp(h.GetRadioState(h.Tx1),v))
    error("Error");
end

%% Transmit Boost
clc;
v = ~h.GetTxBoost(h.Tx1);
h.SetTxBoost(h.Tx1, v);
if( h.GetTxBoost(h.Tx1) ~= v)
    error("Error");
end

%% Radio State

% The following sets and reads the radio state.
clc;
v = h.Calibrated;
h.SetRadioState(h.Tx1, v);
if(~strcmp(h.GetRadioState(h.Tx1),v))
    error("Error");
end

v = h.Primed;
h.SetRadioState(h.Tx1, v);
if(~strcmp(h.GetRadioState(h.Tx1),v))
    error("Error");
end

v = h.Enabled;
h.SetRadioState(h.Tx1, v);
if(~strcmp(h.GetRadioState(h.Tx1),v))
    error("Error");
end

v = h.Primed;
h.SetRadioState(h.Tx1, v);
if(~strcmp(h.GetRadioState(h.Tx1),v))
    error("Error");
end


%% RSSI
h.GetRssi(h.Rx1);


%% Tx Stream from File

% Load Transmit buffer with IQ data from file on SD card
% type "ls" to see files on sd card
h.RflanStreamBufLoad(h.Tx1,'CFR_sample_rate_15p36M_bw_10M.csv');

% Enable transmit stream of nSamp number of samples.  Optionally repeat
% this pattern if Cyclic is set to 1.  If Cyclic equals 0 then the IQ data
% is transmitted once and then turned off.
Cyclic = 1;
nSamp = 2e3;
h.RflanStreamStart(h.Tx1, Cyclic, nSamp);


%% Tx Stream from Matlab

% Load Transmit buffer with IQ data from file on SD card
% type "ls" to see files on sd card
h.RflanStreamBufLoad(h.Tx1,'CFR_sample_rate_15p36M_bw_10M.csv');

% Enable transmit stream of nSamp number of samples.  Optionally repeat
% this pattern if Cyclic is set to 1.  If Cyclic equals 0 then the IQ data
% is transmitted once and then turned off.
Cyclic = 1;
nSamp = 2e3;
h.RflanStreamStart(h.Tx1, Cyclic, nSamp);


%% Rx Stream Start

% Enable receiver stream of nSamp number of samples.  
Cyclic = 1;
nSamp = 2e3;
h.RflanStreamStart(h.Rx1, Cyclic, nSamp);

% Read IQ samples from stream buffer
iq = h.RflanStreamBufGet(h.Rx1,0, nSamp);
fs = h.GetSampleRate(h.Rx1);

%% Stream Stop

% Stop streaming, only used if stream was started in cyclic mode.
h.RflanStreamStop(h.Tx1);
h.RflanStreamStop(h.Tx2);
h.RflanStreamStop(h.Rx1);
h.RflanStreamStop(h.Rx2);

%% Plot IQ Data

close all; clc;

bins = length(iq)/2; 
h2 = spectrum.welch('Hamming',bins);
spec = msspectrum(h2,iq,...
        'Fs',fs,...
        'SpectrumType','twosided',...
        'CenterDC',true);    
    
f = spec.Frequencies/1e6;
a = 10*log10(spec.Data);    

figure();
plot(f,a);
grid on; xlabel('Frequency (MHz)'); ylabel('Power (dB)'); title('Spectrum');

figure();
t = [0:length(iq)-1]/fs;
plot(t,real(iq));
hold all;
plot(t,imag(iq));
grid on; xlabel('Time (sec)'); ylabel('Amplitude'); title('IQ vs Time');


%% Close
h.Close();