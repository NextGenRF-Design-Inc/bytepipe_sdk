%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% BytePipe_x9002 RFLAN Interface Class
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
classdef rflan < handle
    
    % Read/Write 
    properties
        
        %         Temp = 0;
        %         AdrvSiliconVersion = '';
        %         AdrvFirmwareVersion = '';
        %         AdrvAPIVersion = '';
        %         VcTcxo;
        %
        %         DeviceClock_kHz;
        %         DlkPllVcoFreq_daHz;
        %         DlkPllHsDiv;
        %         ClkPllMode;
        %         Clk1105Div;
        %         ArmClkDiv;
        %         ArmPowerSavingClkDiv;
        %         RefClockOutEnable;
        %         AuxPllPower;
        %         ClkPllPower;
        %         PadRefClkDrv;
        %         ExtLo1OutFreq_kHz;
        %         ExtLo2OutFreq_kHz;
        %         RfPll1LoMode;
        %         RfPll2LoMode;
        %         Ext1LoType;
        %         Ext2LoType;
        %         Rx1RfInputSel;
        %         Rx2RfInputSel;
        %         ExtLo1Divider;
        %         ExtLo2Divider;
        %         RfPllPhaseSyncMode;
        %         Rx1LoSelect;
        %         Rx2LoSelect;
        %         Tx1LoSelect;
        %         Tx2LoSelect;
        %         Rx1LoDivMode;
        %         Rx2LoDivMode;
        %         Tx1LoDivMode;
        %         Tx2LoDivMode;
        %         LoGen1Select;
        %         LoGen2Select;
        %
        %
        %         Tx1DpdEnable;
        %         Tx1DpdAmplifierType;
        %         Tx1DpdLutSize;
        %         Tx1DpdModel;
        %         Tx1DpdChangeModelTapOrders;
        %         Tx1DpdClgcEnable;
        %         Tx1DpdPreLutScale;
        %         Tx1DpdNumberofSamples;
        %         Tx1DpdAdditionalPowerScale;
        %         Tx1DpdCountsLessThanPowerThreshold;
        %         Tx1DpdCountsGreaterThanPeakThreshold;
        %         Tx1DpdImmediateLutSwitching;
        %         Tx1DpdUseSpecialFrame;
        %         Tx1DpdResetLuts;
        %         Tx1DpdSamplingRate_Hz;
        %         Tx1DpdRxTxNormalizationLowerThreshold;
        %         Tx1DpdRxTxNormalizationUpperThreshold;
        %         Tx1DpdDetectionPowerThreshold;
        %         Tx1DpdDetectionPeakThreshold;
        %         Tx1DpdTimeFilterCoefficient;
        %         Tx1DpdClgcLoopOpen;
        %         Tx1DpdClgcFilterAlpha;
        %         Tx1DpdClgcGainTarget_HundredthdB;
        %         Tx1DpdClgcLastGain_HundredthdB;
        %         Tx1DpdClgcFilteredGain_HundredthdB;
        %
        %         Tx2DpdEnable;
        %         Tx2DpdAmplifierType;
        %         Tx2DpdLutSize;
        %         Tx2DpdModel;
        %         Tx2DpdChangeModelTapOrders;
        %         Tx2DpdClgcEnable;
        %         Tx2DpdPreLutScale;
        %         Tx2DpdNumberofSamples;
        %         Tx2DpdAdditionalPowerScale;
        %         Tx2DpdCountsLessThanPowerThreshold;
        %         Tx2DpdCountsGreaterThanPeakThreshold;
        %         Tx2DpdImmediateLutSwitching;
        %         Tx2DpdUseSpecialFrame;
        %         Tx2DpdResetLuts;
        %         Tx2DpdSamplingRate_Hz;
        %         Tx2DpdRxTxNormalizationLowerThreshold;
        %         Tx2DpdRxTxNormalizationUpperThreshold;
        %         Tx2DpdDetectionPowerThreshold;
        %         Tx2DpdDetectionPeakThreshold;
        %         Tx2DpdTimeFilterCoefficient;
        %         Tx2DpdClgcLoopOpen;
        %         Tx2DpdClgcFilterAlpha;
        %         Tx2DpdClgcGainTarget_HundredthdB;
        %         Tx2DpdClgcLastGain_HundredthdB;
        %         Tx2DpdClgcFilteredGain_HundredthdB;
        %
        %         Tx1SampleRate;
        %         Tx2SampleRate;
        %         Rx1SampleRate;
        %         Rx2SampleRate;
        %
        %         Tx1CarrierFrequency;
        %         Tx2CarrierFrequency;
        %         Rx1CarrierFrequency;
        %         Rx2CarrierFrequency;
        %
        %         Tx1Attn;
        %         Tx2Attn;
        %
        %         Tx1Boost;
        %         Tx2Boost;
        
        
    end
    
    % Read Only
    properties (GetAccess = 'public', SetAccess = 'private')
        SerialPort = [];
        Tx1 = 'Tx1';
        Tx2 = 'Tx2';
        Rx1 = 'Rx1';
        Rx2 = 'Rx2';
        
        MaxWriteIqLen = 64;
        RxSampleDelay = 20;
        
        Adrv9001TxDataPath_DMA = 0;
        ADRV9001TxDataPath_Constant = 1;
        ADRV9001TxDataPath_Incrementing = 2;
        
        Calibrated = 'Calibrated';
        Primed = 'Primed';
        Enabled = 'Enabled';
        Standby = 'Standby';
        
        s;
        
        UserCallback = [];
        
        AdrvVerInfo = struct(...
            'SiliconVer','unknown',...
            'FirmwareVer','unknown',...
            'ApiVer','unknown');
        
        adi_adrv9001_init_cal = struct(...
            'ADI_ADRV9001_INIT_CAL_TX_QEC',             hex2dec('00000001'),...
            'ADI_ADRV9001_INIT_CAL_TX_LO_LEAKAGE',      hex2dec('00000002'),...
            'ADI_ADRV9001_INIT_CAL_TX_LB_PD',           hex2dec('00000004'),... % Tx Loopback path delay
            'ADI_ADRV9001_INIT_CAL_TX_DCC',             hex2dec('00000008'),... % Tx Duty Cycle Correction
            'ADI_ADRV9001_INIT_CAL_TX_BBAF',            hex2dec('00000010'),... % Tx Baseband Analog Filter
            'ADI_ADRV9001_INIT_CAL_TX_BBAF_GD',         hex2dec('00000020'),... % Tx Baseband Analog Filter Group Delay
            'ADI_ADRV9001_INIT_CAL_TX_ATTEN_DELAY',     hex2dec('00000040'),... % Tx Attenuation Delay
            'ADI_ADRV9001_INIT_CAL_TX_DAC',             hex2dec('00000080'),... % Tx DAC
            'ADI_ADRV9001_INIT_CAL_TX_PATH_DELAY',      hex2dec('00000100'),... % Tx Path Delay
            'ADI_ADRV9001_INIT_CAL_RX_HPADC_RC',        hex2dec('00000200'),... % Rx HP ADC Resistance and Capacitance
            'ADI_ADRV9001_INIT_CAL_RX_HPADC_FLASH',     hex2dec('00000400'),... % Rx HP ADC Flash
            'ADI_ADRV9001_INIT_CAL_RX_HPADC_DAC',       hex2dec('00000800'),... % Rx HP ADC DAC
            'ADI_ADRV9001_INIT_CAL_RX_DCC',             hex2dec('00001000'),... % Rx Duty Cycle Correction
            'ADI_ADRV9001_INIT_CAL_RX_LPADC',           hex2dec('00002000'),... % Rx LP ADC
            'ADI_ADRV9001_INIT_CAL_RX_TIA_CUTOFF',      hex2dec('00004000'),... % Rx Trans-Impedance Amplifier Cutoff
            'ADI_ADRV9001_INIT_CAL_RX_GROUP_DELAY',     hex2dec('00008000'),... % Rx Trans-Impedance Amplifier Group Delay
            'ADI_ADRV9001_INIT_CAL_RX_QEC_TCAL',        hex2dec('00010000'),... % Rx QEC Tone Calibration
            'ADI_ADRV9001_INIT_CAL_RX_QEC_FIC',         hex2dec('00020000'),... % Rx QEC Frequency-Independent
            'ADI_ADRV9001_INIT_CAL_RX_QEC_ILB_LO_DELAY',hex2dec('00040000'),... % Rx Internal Loopback LO Delay
            'ADI_ADRV9001_INIT_CAL_RX_RF_DC_OFFSET',    hex2dec('00080000'),... % Rx RF DC Offset
            'ADI_ADRV9001_INIT_LO_RETUNE',              hex2dec('000B902B'),... % Minimium Subset of InitCals that must be run for LO Retune
            'ADI_ADRV9001_INIT_CAL_RX_GAIN_PATH_DELAY', hex2dec('00100000'),... % Rx Gain Path Delay
            'ADI_ADRV9001_INIT_CAL_RX_DMR_PATH_DELAY',  hex2dec('00200000'),... % Rx DMR Path Delay
            'ADI_ADRV9001_INIT_CAL_PLL',                hex2dec('00400000'),... % PLL
            'ADI_ADRV9001_INIT_CAL_AUX_PLL',            hex2dec('00800000'),... % AUX PLL
            'ADI_ADRV9001_INIT_CAL_TX_ALL',             hex2dec('000001FF'),... % Tx all Init Cals
            'ADI_ADRV9001_INIT_CAL_RX_ALL',             hex2dec('001FFE00'),... % Rx all Init Cals
            'ADI_ADRV9001_INIT_CAL_RX_TX_ALL',          hex2dec('001FFFFF'),... % Rx / Tx all Init Cals
            'ADI_ADRV9001_INIT_CAL_SYSTEM_ALL',         hex2dec('00C00000'));   % All system Init Cals
        
        
        
        
    end
    
    % Application 
    methods
        
        function obj = rflan(varargin)
            
            %Set Optional Properties
            SetProperties(obj,varargin);
            
        end
                
        function SetCallback( obj, cb )
            obj.UserCallback = cb;            
        end
        
        function AdrvClearError( obj )
            obj.Write('AdrvClearError');
        end
                              
        function SetProperties(obj,vars)
            
            for i = 1:2:length(vars)
                f = strsplit(vars{i},'.');
                
                if(sum(strcmp(fieldnames(obj),f{1})))
                    if(length(f) == 1)
                        obj = setfield(obj,f{1},vars{i+1});
                    else
                        obj_tmp = getfield(obj,f{1});
                        obj_tmp = SetProperties(obj_tmp,[{strjoin(f(2:end),'.')},vars(i+1)]);
                        obj = setfield(obj,f{1},obj_tmp);
                    end
                end
            end
            
        end
        
        function TaskInfo( obj )
            obj.Write('TaskInfo');            
        end
        
        function Adrv9001ToRfCalibrated( obj, Port )
            obj.Write(['Adrv9001ToRfCalibrated ' char(Port)]);
        end
        
        function Adrv9001ToRfPrimed( obj, Port )
            obj.Write(['Adrv9001ToRfPrimed ' char(Port)]);
        end
        
        function Adrv9001ToRfEnabled( obj, Port )
            obj.Write(['Adrv9001ToRfEnabled ' char(Port)]);
        end
            
        function Adrv9001CalibrateSsi( obj, Port )
            obj.Write(['Adrv9001CalibrateSsi ' char(Port)]);
        end
        
        function Adrv9001SweepSsi( obj, Port )
            obj.Write(['Adrv9001SweepSsi ' char(Port)]);
        end        
           
    end
    
    % Stream
    methods
        function RflanStreamStart(obj, Port, Cyclic, SampleCnt)
            
            if( strcmp(Port, obj.Tx1) || strcmp(Port, obj.Tx2))
                obj.Write(['RflanStreamStart ' char(Port) ' ' char(num2str(Cyclic)) ' ' char(num2str(SampleCnt))]);
            else
                obj.Write(['RflanStreamStart ' char(Port) ' ' char(num2str(Cyclic)) ' ' char(num2str(SampleCnt + obj.RxSampleDelay))]);
            end
        end
        
        function RflanStreamStop(obj, Port )
            obj.Write(['RflanStreamStop ' char(Port)]);
        end
        
        function RflanStreamBufPut(obj, Port, SampleOffset, iq)
            iq = [real(iq),imag(iq)]';
            iq = iq(:);            
            bufOffset = 0;
            
            remaining = length(iq);
            while( remaining > 0 )
                               
                if( remaining > obj.MaxWriteIqLen )
                    iqBuf = iq(bufOffset+1:bufOffset + obj.MaxWriteIqLen);
                else
                    iqBuf = iq(bufOffset+1:end);
                end
                                
                remaining = remaining - length(iqBuf);
                               
                obj.Write(['RflanStreamBufPut ' char(Port) ' ' char(num2str(SampleOffset + bufOffset/2)) ' ' char(strjoin(compose("%d",int16(iqBuf*2^15)),","))]);
            
                bufOffset = bufOffset + length(iqBuf);      
                                
                pause(0.1);
            end
        end        
        
        function RflanStreamBufLoad(obj, Port, filename)                   
            obj.Write(['RflanStreamBufLoad ' char(Port) ' ' char(filename)]);
        end        
                
        function v = RflanStreamBufGet(obj, Port, SampleOffset, SampleCnt)
            v = [];
            
            configureCallback(obj.s, "off");
            obj.s.flush();
            
            obj.Write(['RflanStreamBufGet ' char(Port) ' ' char(num2str(SampleOffset + obj.RxSampleDelay)) ' ' char(num2str(SampleCnt))]);
            
            tic;
            str = '';
            v = [];
            
            while( ~contains(str,[Port ' Stream Buffer:']) && (toc < 10))
                str = obj.Read();
                disp(str);
                
                if( ~isempty(str))
                    
                    if( contains(str,[Port ' Stream Buffer:']) )
                        str2 = strsplit(str,':');
                        v = strtrim(str2{2});
                        v = strsplit(v,',');
                        
                        v = str2double(v);
                        %v = typecast(uint16(v),'int16');
                        v = double(v) / 2^15;
                        v = complex(v(1:2:end),v(2:2:end));
                        reshape(v,[],1);
                    end
                end
            end
            
            obj.s.flush();
            
            configureCallback(obj.s,"terminator",@obj.ConnectionCallback);
        end        
    end
    
    % Interface
    methods
        
        function Open(obj,Port)
            
            obj.SerialPort = Port;
            
            Close(obj);
            
            obj.s = serialport(obj.SerialPort,115200);
            configureTerminator(obj.s,"CR/LF");
            configureCallback(obj.s,"terminator",@obj.ConnectionCallback);
            set(obj.s, 'Timeout', 10);
        end
        
        function Close(obj)
            
            obj.s = [];
            
            p = instrfind('Port',obj.SerialPort);
            if(~isempty(p))
                fclose(p);
                delete(p);
            end
            
        end
        
        function Write( obj, str )
            obj.s.writeline(char(str));
        end
        
        function str = Read( obj )
            str = char(obj.s.readline());
        end
        
        function ConnectionCallback(obj,ser,~)
            str = char(readline(ser));
                           
            if( contains(str,'=') )
                str2 = strsplit(str,'=');
                
                p = strtrim(str2{1});
                v = strtrim(str2{2});
                                    
                if(~isnan(str2double(v)))
                    v = str2double(v);
                end
                
                obj.SetProperties({p,v});
            end
            
            if( ~isempty(obj.UserCallback) )
                obj.UserCallback( str );
            end
            
            disp(strtrim(str));
        end
        
    end
    
    % Get Parameters
    methods
        
        function v = RflanGetParam( obj, p )
            configureCallback(obj.s, "off");
            obj.s.flush();
            
            obj.Write(['RflanGetParam ' p]);
            
            tic;
            str = '';
            v = [];
            
            while( ~contains(str,[ p ' = ']) && (toc < 3))
                str = obj.Read();
                disp(str);
                
                if( contains(str,'=') )
                    str2 = strsplit(str,'=');
                    
                    p = strtrim(str2{1});
                    v = strtrim(str2{2});
                    
                    if(~isnan(str2double(v)))
                        v = str2double(v);
                    end
                    
                    obj.SetProperties({p,v});
                end
            end
            
            obj.s.flush();
            
            configureCallback(obj.s,"terminator",@obj.ConnectionCallback);
            
        end
        
        function v = Adrv9001GetParam( obj, p )
            configureCallback(obj.s, "off");
            obj.s.flush();
            
            obj.Write(['Adrv9001GetParam ' p]);
            
            tic;
            str = '';
            v = [];
            
            while( ~contains(str,[ p ' = ']) && (toc < 3))
                str = obj.Read();
                disp(str);
                
                if( contains(str,'=') )
                    str2 = strsplit(str,'=');
                    
                    p = strtrim(str2{1});
                    v = strtrim(str2{2});
                    
                    if(~isnan(str2double(v)))
                        v = str2double(v);
                    end
                    
                    obj.SetProperties({p,v});
                end
            end
            
            obj.s.flush();
            
            configureCallback(obj.s,"terminator",@obj.ConnectionCallback);
            
        end
                
        function v = GetTemp( obj )
            v = obj.Adrv9001GetParam('Temp');
        end
        
        function v = GetVcTcxo( obj )
            v = obj.Adrv9001GetParam('VcTcxo');
        end
        
        function v = GetAdrvSiliconVersion( obj )
            v = obj.Adrv9001GetParam('SiliconVersion');
        end
        
        function v = GetAdrvFirmwareVersion( obj )
            v = obj.Adrv9001GetParam('FirmwareVersion');
        end
        
        function v = GetAdrvAPIVersion( obj )
            v = obj.Adrv9001GetParam('APIVersion');
        end
        
        function v = GetDpdEnable( obj, Port )
            v = obj.Adrv9001GetParam([Port 'DpdEnable']);
        end
        
        function v = GetDpdAmplifierType( obj, Port )
            v = obj.Adrv9001GetParam([Port 'DpdAmplifierType']);
        end
        
        function v = GetDpdLutSize( obj, Port )
            v = obj.Adrv9001GetParam([Port 'DpdLutSize']);
        end
        
        function v = GetDpdModel( obj, Port )
            v = obj.Adrv9001GetParam([Port 'DpdModel']);
        end
        
        function v = GetDpdChangeModelTapOrders( obj, Port )
            v = obj.Adrv9001GetParam([Port 'DpdChangeModelTapOrders']);
        end
        
        function v = GetDpdClgcEnable( obj, Port )
            v = obj.Adrv9001GetParam([Port 'DpdClgcEnable']);
        end
        
        function v = GetDpdPreLutScale( obj, Port )
            v = obj.Adrv9001GetParam([Port 'DpdPreLutScale']);
        end
        
        function v = GetDpdNumberofSamples( obj, Port )
            v = obj.Adrv9001GetParam([Port 'DpdNumberofSamples']);
        end
        
        function v = GetDpdAdditionalPowerScale( obj, Port )
            v = obj.Adrv9001GetParam([Port 'DpdAdditionalPowerScale']);
        end
        
        function v = GetDpdCountsLessThanPowerThreshold( obj, Port )
            v = obj.Adrv9001GetParam([Port 'DpdCountsLessThanPowerThreshold']);
        end
        
        function v = GetDpdCountsGreaterThanPeakThreshold( obj, Port )
            v = obj.Adrv9001GetParam([Port 'DpdCountsGreaterThanPeakThreshold']);
        end
        
        function v = GetDpdImmediateLutSwitching( obj, Port )
            v = obj.Adrv9001GetParam([Port 'DpdImmediateLutSwitching']);
        end
        
        function v = GetDpdUseSpecialFrame( obj, Port )
            v = obj.Adrv9001GetParam([Port 'DpdUseSpecialFrame']);
        end
        
        function v = GetDpdResetLuts( obj, Port )
            v = obj.Adrv9001GetParam([Port 'DpdResetLuts']);
        end
        
        function v = GetDpdSamplingRate_Hz( obj, Port )
            v = obj.Adrv9001GetParam([Port 'DpdSamplingRate_Hz']);
        end
        
        function v = GetDpdRxTxNormalizationLowerThreshold_dBm( obj, Port )
            v = obj.Adrv9001GetParam([Port 'DpdRxTxNormalizationLowerThreshold']);
            v = 10*log10(v / 2^30);
        end
        
        function v = GetDpdRxTxNormalizationUpperThreshold_dBm( obj, Port )
            v = obj.Adrv9001GetParam([Port 'DpdRxTxNormalizationUpperThreshold']);
            v = 10*log10(v / 2^30);
        end
        
        function v = GetDpdDetectionPowerThreshold_dBm( obj, Port )
            v = obj.Adrv9001GetParam([Port 'DpdDetectionPowerThreshold']);
            v = 10*log10(v / 2^31);
        end
        
        function v = GetDpdDetectionPeakThreshold_dBm( obj, Port )
            v = obj.Adrv9001GetParam([Port 'DpdDetectionPeakThreshold']);
            v = 10*log10(v / 2^31);
        end
        
        function v = GetDpdTimeFilterCoefficient( obj, Port )
            v = obj.Adrv9001GetParam([Port 'DpdTimeFilterCoefficient']);
            v = v / 2^31;
        end
        
        function v = GetDpdClgcLoopOpen( obj, Port )
            v = obj.Adrv9001GetParam([Port 'DpdClgcLoopOpen']);
        end
        
        function v = GetDpdClgcFilterAlpha( obj, Port )
            v = obj.Adrv9001GetParam([Port 'DpdClgcFilterAlpha']);
            v = v / 2^31;
        end
        
        function v = GetDpdClgcGainTarget_dB( obj, Port )
            v = obj.Adrv9001GetParam([Port 'DpdClgcGainTarget_dB']);
            v = v / 100;
        end
        
        function v = GetDpdClgcLastGain_dB( obj, Port )
            v = obj.Adrv9001GetParam([Port 'DpdClgcLastGain_HundredthdB']);
            v = v / 100;
        end
        
        function v = GetDpdClgcFilteredGain_dB( obj, Port )
            v = obj.Adrv9001GetParam([Port 'DpdClgcFilteredGain_HundredthdB']);
            v = v / 100;
        end
        
        function v = GetDpdCoefficients( obj, Port )
            v = obj.Adrv9001GetParam([Port 'DpdClgcFilteredGain_HundredthdB']);
            v = v / 100;
        end
        
        function v = GetSampleRate( obj, Port )
            v = obj.Adrv9001GetParam([Port 'SampleRate']);
        end
        
        function v = GetTxAttn( obj, Port )
            v = obj.Adrv9001GetParam([Port 'Attn'])/1000;
        end
        
        function v = GetTxBoost( obj, Port )
            v = obj.Adrv9001GetParam([Port 'Boost']);
        end
        
        function v = GetCarrierFrequency( obj, Port )
            v = obj.Adrv9001GetParam([Port 'CarrierFrequency']);
        end
        
        function v = GetRssi( obj, Port )
            v = obj.Adrv9001GetParam([Port 'Rssi']);
        end
        
        function v = GetRxGain( obj, Port )
            v = obj.Adrv9001GetParam([Port 'Rssi']);
        end
        
        function v = GetExternalLoopbackPower(obj, Port)
            v = obj.Adrv9001GetParam([Port 'ExternalLoopbackPower']);
            v = v /10;
        end
        
        function v = GetExternalLoopbackDelay(obj, Port)
            v = obj.Adrv9001GetParam([Port 'ExternalPathDelay']);
        end
        
        function v = GetDpdConfig( obj, Port )
            
            v = struct('DpdEnable', obj.GetDpdEnable(Port),...
                       'DpdAmplifierType', obj.GetDpdAmplifierType(Port),...
                       'DpdLutSize', obj.GetDpdLutSize(Port),...
                       'DpdModel', obj.GetDpdModel(Port),...
                       'DpdChangeModelTapOrders', obj.GetDpdChangeModelTapOrders(Port),...
                       'DpdClgcEnable', obj.GetDpdClgcEnable(Port),...
                       'DpdPreLutScale', obj.GetDpdPreLutScale(Port),...
                       'DpdNumberofSamples', obj.GetDpdNumberofSamples(Port),...
                       'DpdAdditionalPowerScale', obj.GetDpdAdditionalPowerScale(Port),...
                       'DpdCountsLessThanPowerThreshold', obj.GetDpdCountsLessThanPowerThreshold(Port),...
                       'DpdCountsGreaterThanPeakThreshold',obj.GetDpdCountsGreaterThanPeakThreshold(Port),...
                       'DpdImmediateLutSwitching',obj.GetDpdImmediateLutSwitching(Port),...
                       'DpdUseSpecialFrame',obj.GetDpdUseSpecialFrame(Port),...
                       'DpdResetLuts',obj.GetDpdResetLuts(Port),...
                       'DpdSamplingRate_Hz',obj.GetDpdSamplingRate_Hz(Port),...
                       'DpdRxTxNormalizationLowerThreshold_dBm',obj.GetDpdRxTxNormalizationLowerThreshold_dBm(Port),...
                       'DpdRxTxNormalizationUpperThreshold_dBm',obj.GetDpdRxTxNormalizationUpperThreshold_dBm(Port),...
                       'DpdDetectionPowerThreshold_dBm',obj.GetDpdDetectionPowerThreshold_dBm(Port),...
                       'DpdDetectionPeakThreshold_dBm',obj.GetDpdDetectionPeakThreshold_dBm(Port),...
                       'DpdTimeFilterCoefficient',obj.GetDpdTimeFilterCoefficient(Port),...
                       'DpdClgcLoopOpen',obj.GetDpdClgcLoopOpen(Port),...
                       'DpdClgcFilterAlpha',obj.GetDpdClgcFilterAlpha(Port),...
                       'DpdClgcGainTarget_dB',obj.GetDpdClgcGainTarget_dB(Port),...
                       'DpdClgcLastGain_dB',obj.GetDpdClgcLastGain_dB(Port),...
                       'DpdClgcFilteredGain_dB',obj.GetDpdClgcFilteredGain_dB(Port),...
                       'ExternalLoopbackPower',obj.GetExternalLoopbackPower(Port),...
                       'ExternalLoopbackDelay',obj.GetExternalLoopbackDelay(Port));
            
        end
            
        function v = GetRadioState( obj, Port )
            v = obj.Adrv9001GetParam([Port 'RadioState']);
            
            if( v == 1 )
                v = obj.Calibrated;
            elseif( v == 2 )
                v = obj.Primed;
            elseif( v == 3)
                v = obj.Enabled;
            else
                v = obj.Standby;
            end
        end
        
        function v = GetSsiClockDelay( obj, Port )
            v = obj.Adrv9001GetParam([Port 'SsiClockDelay']);            
        end
        
        function v = GetSsiDataDelay( obj, Port )
            v = obj.Adrv9001GetParam([Port 'SsiDataDelay']);            
        end        
        
        function v = GetFileList( obj, filter)
            configureCallback(obj.s, "off");
            obj.s.flush();
            
            obj.Write('ls');
            
            tic;
            str = '';
            v = {};
            
            while( toc < 2 )
                str = obj.Read();
                disp(str);
                
                if(isempty(filter))
                    v = [v; {strtrim(str(1:strfind(str,'.csv')+3))}];
                elseif( contains(str,filter) )
                    v = [v; {strtrim(str(1:strfind(str,'.csv')+3))}];
                end
            end
            
            obj.s.flush();
            
            configureCallback(obj.s,"terminator",@obj.ConnectionCallback);            
        end
    end
    
    % Set Parameters
    methods
        
        function RflanSetParam( obj, p, v )
            obj.Write(['RflanSetParam ' p ' ' num2str(v)]);
        end
        
        function Adrv9001SetParam( obj, p, v )
            obj.Write(['Adrv9001SetParam ' p ' ' num2str(v)]);
        end        
        
        function SetVcTcxo( obj, v )
            obj.Adrv9001SetParam('VcTcxo', num2str(v,"%.2f"));
        end

        function SetTestMode( obj, Port, v )
            obj.Adrv9001SetParam([Port 'TestMode'], num2str(v));
        end
        
        function SetDpdEnable( obj, Port, v )
            obj.Adrv9001SetParam([Port 'DpdEnable'], num2str(v));
        end
        
        function SetDpdAmplifierType( obj, Port, v )
            obj.Adrv9001SetParam([Port 'DpdAmplifierType'], num2str(v));
        end
        
        function SetDpdLutSize( obj, Port, v )
            obj.Adrv9001SetParam([Port 'DpdLutSize'], num2str(v));
        end
        
        function SetDpdModel( obj, Port, v )
            obj.Adrv9001SetParam([Port 'DpdModel'], num2str(v));
        end
        
        function SetDpdChangeModelTapOrders( obj, Port, v )
            obj.Adrv9001SetParam([Port 'DpdChangeModelTapOrders'], num2str(v));
        end
        
        function SetDpdClgcEnable( obj, Port, v )
            obj.Adrv9001SetParam([Port 'DpdClgcEnable'], num2str(v));
        end
        
        function SetDpdPreLutScale( obj, Port, v )
            obj.Adrv9001SetParam([Port 'DpdPreLutScale'], num2str(v));
        end
        
        function SetDpdNumberofSamples( obj, Port, v )
            obj.Adrv9001SetParam([Port 'DpdNumberofSamples'], num2str(v));
        end
        
        function SetDpdAdditionalPowerScale( obj, Port, v )
            obj.Adrv9001SetParam([Port 'DpdAdditionalPowerScale'], num2str(v));
        end
        
        function SetDpdCountsLessThanPowerThreshold( obj, Port, v )
            obj.Adrv9001SetParam([Port 'DpdCountsLessThanPowerThreshold'], num2str(v));
        end
        
        function SetDpdCountsGreaterThanPeakThreshold( obj, Port, v )
            obj.Adrv9001SetParam([Port 'DpdCountsGreaterThanPeakThreshold'], num2str(v));
        end
        
        function SetDpdImmediateLutSwitching( obj, Port, v )
            obj.Adrv9001SetParam([Port 'DpdImmediateLutSwitching'], num2str(v));
        end
        
        function SetDpdUseSpecialFrame( obj, Port, v )
            obj.Adrv9001SetParam([Port 'DpdUseSpecialFrame'], num2str(v));
        end
        
        function SetDpdResetLuts( obj, Port, v )
            obj.Adrv9001SetParam([Port 'DpdResetLuts'], num2str(v));
        end
        
        function SetDpdSamplingRate_Hz( obj, Port, v )
            obj.Adrv9001SetParam([Port 'DpdSamplingRate_Hz'], num2str(v));
        end
        
        function SetDpdRxTxNormalizationLowerThreshold_dB( obj, Port, v )
            obj.Adrv9001SetParam([Port 'DpdRxTxNormalizationLowerThreshold'], num2str(floor(10^(v/10)*2^30)));
        end
        
        function SetDpdRxTxNormalizationUpperThreshold_dB( obj, Port, v )
            obj.Adrv9001SetParam([Port 'DpdRxTxNormalizationUpperThreshold'], num2str(floor(10^(v/10)*2^30)));
        end
        
        function SetDpdDetectionPowerThreshold_dB( obj, Port, v )
            obj.Adrv9001SetParam([Port 'DpdDetectionPowerThreshold'], num2str(floor(10^(v/10)*2^31)));
        end
        
        function SetDpdDetectionPeakThreshold_dB( obj, Port, v )
            obj.Adrv9001SetParam([Port 'DpdDetectionPeakThreshold'], num2str(floor(10^(v/10)*2^31)));
        end
        
        function SetDpdTimeFilterCoefficient( obj, Port, v )
            obj.Adrv9001SetParam([Port 'DpdTimeFilterCoefficient'], num2str(v));
        end
        
        function SetDpdClgcLoopOpen( obj, Port, v )
            obj.Adrv9001SetParam([Port 'DpdClgcLoopOpen'], num2str(v));
        end
        
        function SetDpdClgcFilterAlpha( obj, Port, v )
            obj.Adrv9001SetParam([Port 'DpdClgcFilterAlpha'], num2str(v));
        end
        
        function SetDpdClgcGainTarget_dB( obj, Port, v )
            obj.Adrv9001SetParam([Port 'DpdClgcGainTarget_HundredthdB ' num2str(v * 100)]);
        end
        
        function SetDpdClgcLastGain_dB( obj, Port, v )
            obj.Adrv9001SetParam([Port 'DpdClgcLastGain_HundredthdB ' num2str(v * 100)]);
        end
        
        function SetDpdClgcFilteredGain_dB( obj, Port, v )
            obj.Adrv9001SetParam([Port 'DpdClgcFilteredGain_HundredthdB ' num2str(v * 100)]);
        end
        
        function SetTxAttn( obj, Port, v )
            obj.Adrv9001SetParam([Port 'Attn'], num2str(v*1000));
        end
        
        function SetTxBoost( obj, Port, v )
            obj.Adrv9001SetParam([Port 'Boost'], num2str(v));
        end
        
        function SetCarrierFrequency( obj, Port, v )
            obj.Adrv9001ToRfCalibrated( Port );
            obj.Adrv9001SetParam([Port 'CarrierFrequency'], num2str(v));
        end
        
        function SetRadioState( obj, Port, v )
            if( strcmp(v, obj.Calibrated ))
                obj.Adrv9001SetParam([Port 'RadioState'], '1');
            elseif( strcmp(v, obj.Primed) )
                obj.Adrv9001SetParam([Port 'RadioState'], '2');
            elseif( strcmp(v, obj.Enabled) )
                obj.Adrv9001SetParam([Port 'RadioState'], '3');
            end
        end
        
        function v = SetExternalLoopbackPower(obj, Port, v)
            obj.Adrv9001SetParam([Port 'ExternalLoopbackPower'], num2str(v * 10));
        end
        
        function v = SetExternalLoopbackDelay(obj, Port, v)
            obj.Adrv9001SetParam([Port 'ExternalPathDelay'], num2str(v));
        end
        
        function SetSsiClockDelay( obj, Port, v )
            obj.Adrv9001SetParam([Port 'SsiClockDelay'], num2str(v));            
        end
        
        function SetSsiDataDelay( obj, Port, v )
            obj.Adrv9001SetParam([Port 'SsiDataDelay'], num2str(v));            
        end
           
        function SetTxRx1SsiLoopBack( obj, v )
            obj.Adrv9001SetParam('TxRx1SsiLoopBack', num2str(v));            
        end 

        function SetTxRx2SsiLoopBack( obj, v )
            obj.Adrv9001SetParam('TxRx2SsiLoopBack', num2str(v));            
        end    
        
        function SetTxIqConstant( obj, Port, v )
            obj.Adrv9001SetParam([Port 'IqData'], num2str(v));            
        end   

        function SetTxIqDataPath( obj, Port, v )
            obj.Adrv9001SetParam([Port 'IqDataPath'], num2str(v));            
        end      
        
        function SetTxDisableDelay( obj, Port, v )
            obj.Adrv9001SetParam([Port 'DisableDly'], num2str(v));            
        end 
        
        function SetSsiEnableDelay( obj, Port, v )
            obj.Adrv9001SetParam([Port 'SsiEnableDly'], num2str(v));            
        end
        
        function SetSsiDisableDelay( obj, Port, v )
            obj.Adrv9001SetParam([Port 'SsiDisableDly'], num2str(v));            
        end  
        
    end
        
end


