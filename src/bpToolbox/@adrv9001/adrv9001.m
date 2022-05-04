classdef adrv9001 < handle
    
    properties
        ipAddress = '192.168.1.10';
        portNumber = '55557';
        username = 'root';
        password = 'analog';
        
        adrv9001_init;
        
        INSTALL_DIRECTORY = 'C:/Program Files/Analog Devices/ADRV9001 Transceiver Evaluation Software/';
        CLIENT_DIRECTORY = 'C:/Program Files/Analog Devices/ADRV9001 Transceiver Evaluation Software/';
        serverLocation = '/home/analog/adrv9001_server/resources/Adi.Adrv9001.Server/';
        serverName = 'Adrv9001Server_C0';
                
        CHANNEL_1;
        CHANNEL_2;
        
        PORT_RX;
        PORT_TX;
        PRIMED;
        CALIBRATED;
        STANDBY;
        ENABLED;
    end
    
    properties (SetObservable = true)
       status_string; 
    end
    
    properties (SetAccess = 'private')
        Peitho;
        EvalClient;
        CommonApi;
        Adrv9001;
        Fpga9001;

        
        adrv9001Device;
        fpga9001Device;
        
        iqTx1;
        iqTx2;
        iqRx1;
        iqRx2;
    end

    methods
        
        function obj = adrv9001(varargin)            
            SetProperties(obj,varargin);
            
            addlistener(obj,'status_string','PostSet',@obj.PrintfCallback); % Add obj to argument list
        end
        
        function s = Adrv9001_Connect( obj, varargin )
            SetProperties(obj,varargin);
            
            obj.Peitho = NET.addAssembly([obj.INSTALL_DIRECTORY 'AnalogDevices.Peitho.dll']);
            obj.EvalClient = NET.addAssembly([obj.INSTALL_DIRECTORY 'AnalogDevices.EvalClient.dll']);
            NET.addAssembly([obj.INSTALL_DIRECTORY 'AnalogDevices.EvalClient.Installers.dll']);
            NET.addAssembly([obj.CLIENT_DIRECTORY 'AnalogDevices.Adrv9001.ProfileTypes.dll']);
            obj.CommonApi = NET.addAssembly([obj.CLIENT_DIRECTORY 'AnalogDevices.CommonApi.dll']);
            obj.Adrv9001 = NET.addAssembly([obj.CLIENT_DIRECTORY 'AnalogDevices.EvalClient.Adrv9001.Device.dll']);
            obj.Fpga9001 = NET.addAssembly([obj.CLIENT_DIRECTORY 'AnalogDevices.EvalClient.Fpga9001.Device.dll']);
            NET.addAssembly([obj.CLIENT_DIRECTORY 'AnalogDevices.Peitho.dll']);
            import AnalogDevices.Peitho.*;
            import AnalogDevices.EvalClient.*;
            import AnalogDevices.EvalClient.Installers.*;
            import AnalogDevices.EvalClient.Adrv9001.*
            import AnalogDevices.EvalClient.Adrv9001.Device.*
            import AnalogDevices.EvalClient.CommonApi.*
            import AnalogDevices.EvalClient.Fpga9001.*
            import AnalogDevices.EvalClient.Fpga9001.Device.*
            import AnalogDevices.Peitho.*
            import AnalogDevices.EvalClient.Adrv9001ProfileTypes.*
            NET.addAssembly([obj.CLIENT_DIRECTORY 'AnalogDevices.Adrv9001.ProfileTypes.dll']);
            import AnalogDevices.EvalClient.Adrv9001ProfileTypes.*
            
            serverManager = ServerManager.CreateServerManager(obj.ipAddress, obj.username, obj.password, obj.serverLocation, obj.serverName);
            serverManager.Start();
            connectionAttempts = 0;
            while (~serverManager.IsRunning && connectionAttempts < 5)
                pause(0.2);
                connectionAttempts = connectionAttempts + 1;
            end
            EvalClientManager.Instance.Initialize(obj.CLIENT_DIRECTORY);
            transport = Transports.CreateDefaultTcpTransport([obj.ipAddress ':' obj.portNumber]);
            context = ExecutionContext(transport);
            context.ErrorRetriever = LabviewErrorRetriever();
            context.ErrorHandler = ServerExceptionThrower();
            platform = EvalClientManager.Instance.PlatformBuilder.CreatePlatform(System.String.Empty, context);
            platform.Timeout = 60000;
            boardArray = ToArray(GetRange(platform.Boards, 0, 1));
            board = boardArray(1);
            obj.adrv9001Device = NET.invokeGenericMethod(board, 'DeviceGet', { 'AnalogDevices.EvalClient.Adrv9001.Device.Adrv9001Device' }, 0);
            obj.fpga9001Device = NET.invokeGenericMethod(board, 'DeviceGet', { 'AnalogDevices.EvalClient.Fpga9001.Device.Fpga9001Device' }, 0);
            
            obj.CHANNEL_1 = common_ChannelNumber_e.CHANNEL_1;
            obj.CHANNEL_2 = common_ChannelNumber_e.CHANNEL_2;
            
            obj.PORT_TX = common_Port_e.TX;
            obj.PORT_RX = common_Port_e.RX;
            obj.STANDBY = AnalogDevices.EvalClient.Adrv9001.adrv9001_ChannelState_e.ADRV9001_CHANNEL_STANDBY;
            obj.CALIBRATED = AnalogDevices.EvalClient.Adrv9001.adrv9001_ChannelState_e.ADRV9001_CHANNEL_CALIBRATED;
            obj.PRIMED = AnalogDevices.EvalClient.Adrv9001.adrv9001_ChannelState_e.ADRV9001_CHANNEL_PRIMED;
            obj.ENABLED = AnalogDevices.EvalClient.Adrv9001.adrv9001_ChannelState_e.ADRV9001_CHANNEL_RF_ENABLED;
                        
            s = 0;
            
        end
        
        function s = Adrv9001_LoadProfile( ~, profileDir, varargin )
            
            workingPath = fileparts(mfilename('fullpath'));
            
            delete([workingPath '\*.bin']);
            delete([workingPath '\Adrv9001_*.m']);
            copyfile([profileDir '\*.bin'],workingPath);
            
            mfiles = dir(fullfile(profileDir,'*.m'));
            
            for k = 1:length(mfiles)
                fid  = fopen([profileDir '\' mfiles(k).name],'r');
                f=fread(fid,'*char')';
                fclose(fid);
                
                f0 = mfiles(k).name(1:end-2);
                f1 = ['Adrv9001_' upper(f0(1)) f0(2:end)];
                
                % update function name
                f = regexprep(f,['function ' f0 '('], ...
                    ['function ' f1 '( obj ) % ']);
                
                % replace fprintf with function call
                %f = regexprep(f,'\tfprintf(','\tobj.Adrv9001_PrintCallback(');
                f = regexprep(f,'\tfprintf(','\tobj.status_string = sprintf(');
                
                % remove user input
                f = regexprep(f,'\tinput(','%\tinput(');
                
                % replace device with obj device
                f = regexprep(f,'adrv9001Device_0','obj.adrv9001Device');
                f = regexprep(f,'fpga9001Device_0','obj.fpga9001Device');
                
                f = insertAfter(f,'sprintf("Initialization successful.\n");',[newline '   obj.adrv9001_init = init_7;']);
                
                % replace contents of file
                fid  = fopen([workingPath '\' f1 '.m'],'w');
                fprintf(fid,'%s',f);
                fclose(fid);
            end
            
            s = 0;
         end
%         
%         function iqdata = Adrv9001_DataCapture( obj, length, channel )
%             
%             dmaCfg = AnalogDevices.EvalClient.Fpga9001.fpga9001_DmaCfg_t;
%             dmaCfg.length = length*4;
%             dmaCfg.continuous = false;
%             dmaCfg.timeout_ms = 300000;
%             dmaCfg.trigger = AnalogDevices.EvalClient.Fpga9001.fpga9001_DmaTrigger_e.FPGA9001_DMA_TRIGGER_IMMEDIATE;
%             dmaCfg.captureType = AnalogDevices.EvalClient.Fpga9001.fpga9001_CaptureType_e.FPGA9001_CAPTURE_TYPE_RX_DATA;
%             dmaCfg.dmaEnableSync = false;
%             
%             obj.fpga9001Device.dataChain.PerformTransfer(obj.PORT_RX, channel, dmaCfg);
%             
%             iData = NET.createArray('System.Int16', length);
%             qData = NET.createArray('System.Int16', length);
%             obj.fpga9001Device.dataChain.Data_Get_16I16QSigned(obj.PORT_RX, channel, iData, qData, length);
%             
%             iqdata = double(iData) + 1i*double(qData);
%             
%             iqdata = iqdata / 2^15;
%         end
        
        function info = Adrv9001_GetVerInfo( obj )
            
            apiVersion_0 = AnalogDevices.EvalClient.CommonApi.common_ApiVersion_t(0, 0, 0);
            obj.adrv9001Device.core.ApiVersion_Get(apiVersion_0);
            
            siliconVersion_0 = AnalogDevices.EvalClient.Adrv9001.adrv9001_SiliconVersion_t(0,0);
            obj.adrv9001Device.core.SiliconVersion_Get(siliconVersion_0);
            
            fpgaVersion_1 = AnalogDevices.EvalClient.Fpga9001.fpga9001_Version_t(0, 0, 0);
            obj.fpga9001Device.core.VersionGet(fpgaVersion_1);
            
            partnumber = AnalogDevices.EvalClient.Adrv9001.adrv9001_PartNumber_e;
            obj.adrv9001Device.core.PartNumber_Get(partnumber);
            partnumber = strsplit(char(partnumber), '_');
            partnumber = partnumber(end);
            
            info = struct(...
                'BoardName','BytePipe_x9002',...
                'BoardRevision','2',...
                'HDLName','TES',...
                'TransceiverName',partnumber{:},...
                'SiliconRevision',[dec2hex(siliconVersion_0.major) dec2hex(siliconVersion_0.minor)],...
                'FPGAVersion',[num2str(fpgaVersion_1.major), '.', num2str(fpgaVersion_1.minor), '.',num2str(fpgaVersion_1.patch)],...
                'APIVersion',[num2str(apiVersion_0.major), '.', num2str(apiVersion_0.minor), '.', num2str(apiVersion_0.patch)],...
                'FirmwareVersion', '');
            
        end
        
        function state = Adrv9001_GetRadioState( obj, port, channel )
            channelState = AnalogDevices.EvalClient.Adrv9001.adrv9001_ChannelState_e;
            [~, channelState] = obj.adrv9001Device.radio.Channel_State_Get(port, channel, channelState);
            state = char(channelState);
        end
        
        function s = Adrv9001_SetRadioState( obj, port, channel, state )
            s = obj.adrv9001Device.radio.Channel_ToState(port, channel, state);
        end
               
        function v = Adrv9001_GetSampleRate( obj, port, channel )
            
            if( isempty(obj.adrv9001_init)) 
                v = 0;
                return;
            end
                
            if( port == obj.PORT_RX )
                if( channel == obj.CHANNEL_1 )
                    v = obj.adrv9001_init.rx.rxChannelCfg(1).profile.rxInterfaceSampleRate_Hz;
                else
                    v = obj.adrv9001_init.rx.rxChannelCfg(2).profile.rxInterfaceSampleRate_Hz;
                end
            else
                if( channel == obj.CHANNEL_1 )
                    v = obj.adrv9001_init.tx.txProfile(1).txInterfaceSampleRate_Hz;
                else
                    v = obj.adrv9001_init.tx.txProfile(2).txInterfaceSampleRate_Hz;
                end                    
            end            
            
            v = double(v);
        end

        function attn = Adrv9001_GetTxAttn( obj, channel )
            v = uint16(0);            
            [~,v] = obj.adrv9001Device.tx.Attenuation_Get(channel,v); 
            attn = double(v) /1e3;            
        end
        
        function Adrv9001_SetTxAttn( obj, channel, attn )
            v = uint16((attn * 1e3));            
            obj.adrv9001Device.tx.Attenuation_Set(channel,v);           
        end
        
        function v = Adrv9001_GetTxBoost( obj, channel )
            v = logical(true);
            [~,v] = obj.adrv9001Device.tx.OutputPowerBoost_Get(channel,v);            
        end
        
        function v = Adrv9001_SetTxBoost( obj, channel, value )
            v = logical(value);
            obj.adrv9001Device.tx.OutputPowerBoost_Set(channel,v);            
        end        

        function Adrv9001_SetIfFrequency( obj, port, channel, freq )
            carrier = AnalogDevices.EvalClient.Adrv9001.adrv9001_Carrier_t;
            obj.adrv9001Device.radio.Carrier_Inspect(port, channel, carrier);
            carrier.intermediateFrequency_Hz = int32(freq);
            obj.adrv9001Device.radio.Carrier_Configure(port, channel, carrier);
        end
        
        function Adrv9001_SetCarrierFrequency( obj, port, channel, freq )
            carrier = AnalogDevices.EvalClient.Adrv9001.adrv9001_Carrier_t;
            obj.adrv9001Device.radio.Carrier_Inspect(port, channel, carrier);
            carrier.carrierFrequency_Hz = uint64(freq);
            obj.adrv9001Device.radio.Carrier_Configure(port, channel, carrier);         
        end
                       
        function f = Adrv9001_GetCarrierFrequency( obj, port, channel )
            carrier = AnalogDevices.EvalClient.Adrv9001.adrv9001_Carrier_t;
            obj.adrv9001Device.radio.Carrier_Inspect(port, channel, carrier);
            f = double(carrier.carrierFrequency_Hz);            
        end
               
        function f = Adrv9001_GetIfFrequency( obj, port, channel )
            carrier = AnalogDevices.EvalClient.Adrv9001.adrv9001_Carrier_t;
            obj.adrv9001Device.radio.Carrier_Inspect(port, channel, carrier);
            f = double(carrier.intermediateFrequency_Hz);            
        end

        function Adrv9001_SetPFirCoefficients( obj, port, id, coeff )
            
            h = zeros(128,1);
            h(1:length(coeff)) = coeff;
            coeff = h' * 2^23;
            
            coeff = NET.convertArray(coeff, 'System.Int32');

            
            if( port == obj.PORT_RX )                
                if( strcmp(id,'A') )
                    obj.adrv9001_init.pfirBuffer.pfirRxWbNbChFilterCoeff_A.coefficients = coeff;
                elseif(strcmp(id,'B'))
                    obj.adrv9001_init.pfirBuffer.pfirRxWbNbChFilterCoeff_B.coefficients = coeff;                    
                elseif(strcmp(id,'C'))
                    obj.adrv9001_init.pfirBuffer.pfirRxWbNbChFilterCoeff_C.coefficients = coeff;
                else
                    obj.adrv9001_init.pfirBuffer.pfirRxWbNbChFilterCoeff_D.coefficients = coeff;
                end                
            else
                if( strcmp(id,'A') )
                    obj.adrv9001_init.pfirBuffer.pfirTxWbNbPulShpCoeff_A.coefficients = coeff;
                elseif(strcmp(id,'B'))
                    obj.adrv9001_init.pfirBuffer.pfirTxWbNbPulShpCoeff_B.coefficients = coeff;            
                elseif(strcmp(id,'C'))
                    obj.adrv9001_init.pfirBuffer.pfirTxWbNbPulShpCoeff_C.coefficients = coeff;
                else
                    obj.adrv9001_init.pfirBuffer.pfirTxWbNbPulShpCoeff_D.coefficients = coeff;
                end                           
            end
            
        end
        
        function coeff = Adrv9001_GetPFirCoefficients( obj, port, id )
            
            if( port == obj.PORT_RX )                
                if( strcmp(id,'A') )
                    coeff = obj.adrv9001_init.pfirBuffer.pfirRxWbNbChFilterCoeff_A.coefficients.double;
                elseif(strcmp(id,'B'))
                    coeff = obj.adrv9001_init.pfirBuffer.pfirRxWbNbChFilterCoeff_B.coefficients.double;                    
                elseif(strcmp(id,'C'))
                    coeff = obj.adrv9001_init.pfirBuffer.pfirRxWbNbChFilterCoeff_C.coefficients.double;
                else
                    coeff = obj.adrv9001_init.pfirBuffer.pfirRxWbNbChFilterCoeff_D.coefficients.double;
                end                
            else
                if( strcmp(id,'A') )
                    coeff = obj.adrv9001_init.pfirBuffer.pfirTxWbNbPulShpCoeff_A.coefficients.double;
                elseif(strcmp(id,'B'))
                    coeff = obj.adrv9001_init.pfirBuffer.pfirTxWbNbPulShpCoeff_B.coefficients.double;                    
                elseif(strcmp(id,'C'))
                    coeff = obj.adrv9001_init.pfirBuffer.pfirTxWbNbPulShpCoeff_C.coefficients.double;
                else
                    coeff = obj.adrv9001_init.pfirBuffer.pfirTxWbNbPulShpCoeff_D.coefficients.double;
                end                           
            end
            
        end
        
        function [x,y] = Adrv9001_GetPFirResponse( obj, port, channel, id )
                       
            coeff = obj.Adrv9001_GetPFirCoefficients( port, id );            
            fs = obj.Adrv9001_GetSampleRate( port, channel );
            h = freqz(coeff);
            h = 20*log10(abs(h));
            y = h - 20*log10(2^23);
            x = [0:fs/2/length(h):fs/2-1];
            x = double(x);
            
        end
        
        
%         function Adrv9001_BeginReceiving( obj )
%             obj.username = 1;
%         end
%         
%         function Adrv9001_BeginTransmitting( obj )
%             obj.username = 1;
%         end
%         
%         function Adrv9001_Calibrate( obj )
%             obj.username = 1;
%         end
%         
%         function Adrv9001_Configure( obj )
%             obj.username = 1;
%         end
%         
%         function Adrv9001_Initialize( obj )
%             obj.username = 1;
%         end
%                 
%         function Adrv9001_Main( obj )
%             obj.username = 1;
%         end
%         
%         function Adrv9001_Prime( obj )
%             obj.username = 1;
%         end
%         
%         function Adrv9001_StopReceiving( obj )
%             obj.username = 1;
%         end
%         
%         function Adrv9001_StopTransmitting( obj )
%             obj.username = 1;
%         end
%         
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
        
        function PrintfCallback( obj, ~, ~ )
            
            disp(obj.status_string);
            
        end
        
    end
    
end
