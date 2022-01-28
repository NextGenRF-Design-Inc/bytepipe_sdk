%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
classdef bytepipe_rflan < handle
   
%Read/Write Properties  
properties

end

% Read Only Properties 
properties (GetAccess = 'public', SetAccess = 'private')    
    AvailablePorts = [];
    ActivePort = []; 
    Tx1 = 'Tx1';
    Tx2 = 'Tx2';
    Rx1 = 'Rx1';   
    Rx2 = 'Rx2';   
    
    
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

% Private Properties 
properties (GetAccess = 'private', SetAccess = 'private') 
       
s;    

end

    
% Public Functions
methods
    
    function obj = bytepipe_rflan(varargin)
        
        %Set Optional Properties
        SetProperties(obj,varargin);    
        
        %Get COM Ports
        obj.AvailablePorts = GetAvailablePorts();
        
    end
     
    function Open(obj,Port)
        
        obj.ActivePort = Port;
        
        Close(obj);
               
        obj.s = serialport(obj.ActivePort,115200);        
        configureTerminator(obj.s,"CR/LF"); 
        configureCallback(obj.s,"terminator",@obj.cliCallbackFcn);
        set(obj.s, 'Timeout', 1);
    end
       
    function Close(obj)
               
        p = instrfind('Port',obj.ActivePort);
        if(~isempty(p))
            fclose(p);
            delete(p);
        end
        
    end    

    function s = Status2String( ~,v )
        if( v == 0)
            s = 'Passed';
        else
            s = 'Failed';
        end
    end
    
    function status = CalStatus(obj, CalMask0, CalMask1)
        
        status = 1;
        
        if( obj.PhyAdrv9001Init() == 0)            
            if( obj.Adrv9001InitProfile() == 0)
                status = obj.Adrv9001InitCalsRun(CalMask0, CalMask1, 0);                
            end
        end   
    end
    
    function t = SweepCalStatus(obj, path)
        
        CalMask = convertCharsToStrings(fieldnames(obj.adi_adrv9001_init_cal));
        Status = strings(length(CalMask),1);
        
        for i = 1:length(CalMask)
            if( obj.PhyAdrv9001Init() ~= 0)
                Status = nan;
                break;
            end            
            
            if( obj.Adrv9001InitProfile() ~= 0)
                Status = nan;
                break;
            end
            
            if(path == 0)
                Status(i) = obj.Status2String(obj.Adrv9001InitCalsRun(getfield(obj.adi_adrv9001_init_cal,CalMask(i)), 0, 0));
            elseif(path == 3)
                Status(i) = obj.Status2String(obj.Adrv9001InitCalsRun(getfield(obj.adi_adrv9001_init_cal,CalMask(i)), getfield(obj.adi_adrv9001_init_cal,CalMask(i)), 0));
            else
                Status(i) = obj.Status2String(obj.Adrv9001InitCalsRun(0, getfield(obj.adi_adrv9001_init_cal,CalMask(i)), 0));
            end
            
            disp(strjoin([CalMask{i} repmat(' ',1,max(strlength(CalMask))-length(CalMask{i})) Status(i)]));
            
        end
        
        t = table(CalMask,Status);
        
    end        
        
end

% Application CLI Interface
methods
    
    function Reboot(obj)
        writeline(obj.s,'Reboot');   
    end  
    
    function ls(obj)
        owriteline(obj.s,'ls');   
    end     
    
    function help(obj,varargin)
        writeline(obj.s,['help ' varargin{:}]);   
    end      
end

% Private CLI Interface
methods
    
    function GetSsiDelay(obj, Port)        
        obj.Write(['GetSsiDelay ' Port]);   
    end 

    function status = SetSsiDelay(obj, Port, ClkDelay, DataDelay)
        status = obj.ReadStatus(['SetSsiDelay ' Port, ' ',...
            num2str(ClkDelay), ' ',...
            num2str(0), ' ',...
            num2str(DataDelay), ' ',...
            num2str(DataDelay), ' ',...
            num2str(DataDelay)]);
    end

    function GetSsiCfg(obj, Port)        
        obj.Write(['GetSsiCfg ' Port]);   
    end   
    
    function status = LoadDmaConstant(obj, Port, Value)        
        status = obj.ReadStatus(['LoadDmaConstant ' Port ' ' num2str(Value)]);   
    end       

    function status = LoadDmaRamp(obj, Port)        
        status = obj.ReadStatus(['LoadDmaRamp ' Port]);   
    end
    
    function status = LoadDmaPrbs7(obj, Port)        
        status = obj.ReadStatus(['LoadDmaPrbs7 ' Port]);   
    end 
    
    function status = LoadDmaPrbs15(obj, Port)        
        status = obj.ReadStatus(['LoadDmaPrbs15 ' Port]);   
    end
    
    function GetTestModeStatus(obj, Port)        
        obj.Write(['GetTestModeStatus ' Port]);   
    end 
    
    function status = PerformSsiSweepTest(obj, Port)        
        status = obj.ReadStatus(['PerformSsiSweepTest ' Port]);   
    end
    
    function status = PerformSsiTestMode(obj, Port)        
        status = obj.ReadStatus(['PerformSsiTestMode ' Port]);   
    end  
    
    function status = Adrv9001InitCalsRun(obj, Mask0, Mask1, CalMode)        
        set(obj.s, 'Timeout', 130);        
        status = obj.ReadStatus(['Adrv9001InitCalsRun ' num2str(Mask0) ' ' num2str(Mask1) ' ' num2str(CalMode)]);        
        set(obj.s, 'Timeout', 1);
    end  
    
    function status = Adrv9001InitProfile(obj)  
        set(obj.s, 'Timeout', 10);
        status = obj.ReadStatus('Adrv9001InitProfile');  
        set(obj.s, 'Timeout', 1);
    end      
end

% ADRV9001 CLI Interface
methods
    
    function status = Adrv9001SetRadioState(obj, Port, State)        
        status = obj.ReadStatus(['Adrv9001SetRadioState ' Port, ' ', num2str(State)]);   
    end   
    
    function Adrv9001GetRadioState(obj, Port)        
        writeline(obj.s,['Adrv9001GetRadioState ' Port]);   
    end       
    
    function v = Adrv9001GetCarrierFrequency(obj, Port)        
        v = obj.ReadValue(['Adrv9001GetCarrierFrequency ' Port],'%d');   
    end   

    function status = Adrv9001SetCarrierFrequency(obj, Port, Freq)        
        status = obj.ReadStatus(['Adrv9001SetCarrierFrequency ' Port, ' ', num2str(Freq)]);   
    end
    
    function v = Adrv9001GetSampleRate(obj, Port)        
        v = obj.ReadValue(['Adrv9001GetSampleRate ' Port], '%d');   
    end       
    
    function v = Adrv9001GetTxAttn(obj, Port)        
        v = obj.ReadValue(['Adrv9001GetTxAttn ' Port], '%d');   
    end   

    function status = Adrv9001SetTxAttn(obj, Port, Attn)        
        status = obj.ReadStatus(['Adrv9001SetTxAttn ' Port, ' ', num2str(Attn)]);   
    end
    
    function v = Adrv9001GetTxBoost(obj, Port)        
        v = obj.ReadValue(['Adrv9001GetTxBoost ' Port],'%d');   
    end   

    function status = Adrv9001SetTxBoost(obj, Port, Enabled)        
        status = obj.ReadStatus(['Adrv9001SetTxBoost ' Port, ' ', num2str(Enabled)]);   
    end       
    
    function status = Adrv9001ToRfPrimed(obj, Port)        
        status = obj.ReadStatus(['Adrv9001ToRfPrimed ' Port]);   
    end
    
    function status = Adrv9001ToRfCalibrated(obj, Port)        
        status = obj.ReadStatus(['Adrv9001ToRfCalibrated ' Port]);   
    end 
    
    function status = Adrv9001ToRfEnabled(obj, Port)        
        status = obj.ReadStatus(['Adrv9001ToRfEnabled ' Port]);   
    end       
    
    function v = Adrv9001GetRssi(obj, Port)        
        v = obj.ReadValue(['Adrv9001GetRssi ' Port],'%d');   
    end   

    function v = Adrv9001GetTemp(obj)                    
        v = obj.ReadValue('Adrv9001GetTemp', '%d');        
    end    

    function v = Adrv9001GetVerInfo(obj)
        
        v = struct('Silicon',nan,'Firmware',nan,'API',nan);
        
        v = obj.ReadStruct('Adrv9001GetVerInfo',v);                        
        
    end  
    
    function status = Adrv9001LoadProfile(obj)        
        status = obj.ReadStatus('Adrv9001LoadProfile');   
    end     
    
	function status = Adrv9001SetLoopBack(obj, Port, Enabled)      
        
        status = obj.ReadStatus(['Adrv9001SetLoopBack ' Port, ' ', num2str(Enabled)]);
         
    end  
    
    function Adrv9001ReadDma(obj, Port, Length)        
        obj.Write(['Adrv9001ReadDma ' Port, ' ', num2str(Length)]);   
    end  
    
    function status = Adrv9001EnableAuxDac(obj, Id, Enable)        
        status = obj.ReadStatus(['Adrv9001EnableAuxDac ' num2str(Id), ' ', num2str(Enable)]);   
    end    
    
    function status = Adrv9001SetAuxDac(obj, Id, Voltage)        
        status = obj.ReadStatus(['Adrv9001SetAuxDac ' num2str(Id), ' ', num2str(Voltage)]);   
    end   

	function v = Adrv9001GetAuxDac(obj, Id)        
        v = obj.ReadValue(['Adrv9001GetAuxDac ' num2str(Id)],'%2.1f');   
    end   
    
end

% PHY CLI Interface
methods
    
    function status = PhyIqFileStreamEnable(obj, Port, Filename, SampleCnt)        
        status = obj.ReadStatus(['PhyIqFileStreamEnable ' Port, ' ', Filename, ' ', num2str(SampleCnt)]);   
    end  
    
    function status = PhyIqFileStreamDisable(obj, Port)        
        status = obj.ReadStatus(['PhyIqFileStreamDisable ' Port]);   
    end   

    function v = PhyIqFileSize(obj, Filename)        
        v = obj.ReadValue(['PhyIqFileSize ', Filename],'%d');   
    end   

    function status = PhyUpdateProfile(obj)        
        status = obj.ReadStatus('PhyUpdateProfile');   
    end
    
    function status = PhyAdrv9001Init(obj)        
        status = obj.ReadStatus('PhyAdrv9001Init');   
    end   
    
    function status = PhyAdrv9001LoadProfile(obj)        
        status = obj.ReadStatus('PhyAdrv9001LoadProfile');   
    end    
          
end
    
%Private Functions
methods(Access = 'private')     
  
    function v = ReadValue(obj,s,format)        
                    
        v = [];
        configureCallback(obj.s, "off");
        flush(obj.s);
        
        writeline(obj.s,s);
        
        for i = 1:5
            line = readline(obj.s);
            
            if(isempty(line))
                break;
            end
            
            v = sscanf(line,format);
            
            if(~isempty(v))
                break;
            end
        end
        
        configureCallback(obj.s,"terminator",@obj.cliCallbackFcn);
                   
    end
    
    function status = ReadStatus(obj,s)
            
        status = 1;
        configureCallback(obj.s, "off");
        flush(obj.s);
        
        writeline(obj.s,s);
        
        for i = 1:5
            line = deblank(readline(obj.s));
            
            if(isempty(line))
                break;
            end
            
            if(contains(line,'Success'))
                status = 0;
                break;
            elseif(contains(line,'Failed'))
                status = 1;
                break;
            end            
        end
        
        configureCallback(obj.s,"terminator",@obj.cliCallbackFcn);        
    end
        
    function s = ReadStruct(obj,str,s)
        
        configureCallback(obj.s, "off");
        flush(obj.s);
        
        writeline(obj.s,str);
        
        f = fieldnames(s);
        for i = 1:length(f)+5
            
            line = cell2mat(deblank(readline(obj.s)));
            
            if(isempty(line))
                break;
            end
            
            for fidx = 1:length(f)
                
                if(contains(line,f(fidx)))
                    s = setfield(s,f{fidx},line(strfind(line,':')+2:end));
                end
            end
        end
        
        configureCallback(obj.s,"terminator",@obj.cliCallbackFcn);
        
    end
        
    
    function cliCallbackFcn(~,ser,~)
        data = readline(ser); 
        data = erase(data,newline);
        data = erase(data,'\r\n');
        disp(strtrim(data));     
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
            
end
    
end


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Gets Available COM Ports
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 
function ports = GetAvailablePorts()

   devices = winqueryreg('name', 'HKEY_LOCAL_MACHINE', 'HARDWARE\DEVICEMAP\SERIALCOMM');

   %Still need to filter out for Suter USB Descriptor
   if(isempty(devices))
       ports = [];
   else
      ports = cell(1,length(devices));

      for i = 1:length(devices)
          ports{i} = winqueryreg('HKEY_LOCAL_MACHINE', 'HARDWARE\DEVICEMAP\SERIALCOMM', devices{i});
      end

   end
end
    