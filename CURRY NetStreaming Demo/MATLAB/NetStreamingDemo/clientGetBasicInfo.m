function [status, basicInfo] = clientGetBasicInfo(con, handles)
% [status, data] = clientGetBasicInfo(handles)
basicInfo = [];
maxChans = 300;

% Build header
header = initHeader('CTRL',...
                     controlCode('CTRL_FromClient'),...
                     requestType('RequestBasicInfoAcq'),...
                     0,0,0); 

[basicInfoRaw,status] = clientProcessRequest(   con, handles, header,...
                                                'ClientGetBasicInfo',...
                                                dataType('Data_Info'),...
                                                infoType('InfoType_BasicInfo'),0);

if status    
    basicInfo = struct( 'size',                     typecast(uint8(basicInfoRaw(1:4)),  'uint32'),...
                        'eegChan',                  typecast(uint8(basicInfoRaw(5:8)),  'uint32'),...
                        'sampleRate',               typecast(uint8(basicInfoRaw(9:12)), 'uint32'),...
                        'dataSize',                 typecast(uint8(basicInfoRaw(13:16)),'uint32'),...
                        'allowClientToControlAmp',  typecast(uint8(basicInfoRaw(17:20)),'uint32'),...   % allow client to control amp on server (connect/disconnect/impedance)
                        'allowClientToControlRec',  typecast(uint8(basicInfoRaw(21:24)),'uint32'));     % allow client to control recording on server (start/pause)

    if ( basicInfo.eegChan > 0 ...
            && basicInfo.eegChan < maxChans ...
            && basicInfo.sampleRate > 0 ...
            && ( basicInfo.dataSize == 2 || basicInfo.dataSize == 4 )) % 2 = short, 4 = int (number of bytes)
        addLogMsg({[ ' ' num2str(basicInfo.eegChan) ' Channels']}, handles.config_log_text,1);
        addLogMsg({[ ' ' num2str(basicInfo.sampleRate) ' Hz Sample Rate' ]}, handles.config_log_text,1);
        if basicInfo.allowClientToControlAmp
            addLogMsg({' Client is allowed to control amplifier'}, handles.config_log_text,1);
            handles.start_stop_amplifier_button.UserData = 1;
        else
            addLogMsg({' Client is not allowed to control amplifier'}, handles.config_log_text,1);
            handles.start_stop_amplifier_button.UserData = 0;
        end
        addLogMsg({' ClientGetBasicInfo successful'}, handles.com_log_text,1);
    else
        addLogMsg({' Error in ClientGetBasicInfo data'}, handles.com_log_text,1);
        status = 0;
        return
    end  
end
