function struct2tree( parent, s )

names = fieldnames(s);

for i = 1:length(names)
    child = getfield(s,names{i});
    
    if( isnumeric( child ))
        node = uitreenode( parent, "Text", [names{i} '=' num2str(child)] ); 
    elseif( islogical( child ))
        node = uitreenode( parent, "Text", [names{i} '=' num2str(child)] ); 
    elseif( isenum( child ))
        node = uitreenode( parent, "Text", [names{i} '=' char(child)] );  
    elseif( isa(child,'AnalogDevices.EvalClient.Adrv9001ProfileTypes.adrv9001_RxChannelCfg_t[]') )
        node = uitreenode(parent,"Text",names{i});        
        for j = 1: child.Length
            struct2tree( node, child(j) );
        end  
    elseif( isa(child,'AnalogDevices.EvalClient.Adrv9001ProfileTypes.adrv9001_TxProfile_t[]') )
        node = uitreenode(parent,"Text",names{i});        
        for j = 1: child.Length
            struct2tree( node, child(j) );
        end   
    elseif( isa(child,'AnalogDevices.EvalClient.Adrv9001ProfileTypes.adrv9001_PfirPulseBuffer_t[]') )
        node = uitreenode(parent,"Text",names{i});        
        for j = 1: child.Length
            struct2tree( node, child(j) );
        end  
    elseif( isa(child,'AnalogDevices.EvalClient.Adrv9001ProfileTypes.adrv9001_PfirMag21Buffer_t[]') )
        node = uitreenode(parent,"Text",names{i});        
        for j = 1: child.Length
            struct2tree( node, child(j) );
        end  
    elseif( isa(child,'AnalogDevices.EvalClient.Adrv9001ProfileTypes.adrv9001_PfirMag13Buffer_t[]') )
        node = uitreenode(parent,"Text",names{i});        
        for j = 1: child.Length
            struct2tree( node, child(j) );
        end          
    elseif(isa(child,'System.Int16[]') || isa(child,'System.UInt16[]') || isa(child,'System.UInt32[]') || isa(child,'System.Int32[]'))
        
        str = [names{i} ' = '];
        for j = 1:child.Length
           str = [str num2str(child(j)) ', '];
        end
        node = uitreenode(parent,"Text",str);        
               
    elseif( ~isempty(fieldnames(child)) )
        node = uitreenode(parent,"Text",names{i});
        struct2tree( node, child );
    end
end

end