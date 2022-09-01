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

h.SetRadioState(h.Tx1, h.Calibrated);

h.SetCarrierFrequency(h.Tx1, v);
v2 = h.GetCarrierFrequency(h.Tx1);
if( abs(v2 - v) > 10 )
    error("Error");
end

h.SetRadioState(h.Tx1, h.Primed);

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

%% Close
h.Close();