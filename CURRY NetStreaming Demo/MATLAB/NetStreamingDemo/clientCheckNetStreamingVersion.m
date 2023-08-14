function [status, version] = clientCheckNetStreamingVersion(con, handles)
% [status, data] = clientCheckNetViewVersion(handles)

NetStreamingVersion = 803;

% Build header
header = initHeader( 'CTRL',...
                      controlCode('CTRL_FromClient'),...
                      requestType('RequestVersion'),...
                      0,0,0); 

[version,status] = clientProcessRequest(    con, handles, header,...
                                            'NetStreamingVersion',... 
                                            dataType('Data_Info'),...
                                            infoType('InfoType_Version'),0);

if status
    if typecast(uint8(version),'uint32') == NetStreamingVersion
        addLogMsg({[' NetStreaming version ' num2str(NetStreamingVersion)]}, handles.config_log_text,1);
        addLogMsg({' CheckNetStreamingVersion successful'}, handles.com_log_text,1);
    else
        addLogMsg({[' CheckNetStreamingVersion failed: incompatible client version ' num2str(NetStreamingVersion)]}, handles.com_log_text,1);
        status = 0;
    end
end

