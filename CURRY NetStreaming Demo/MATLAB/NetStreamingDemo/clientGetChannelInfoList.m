function [status, infoList] = clientGetChannelInfoList(con, numChannels, handles)
% [status, data] = clientGetChannelInfoList(handles)

% Offsets in CURRY struct (in bytes)
offset_channelId    =                     1;
offset_chanLabel    = offset_channelId  + 4;
offset_chanType     = offset_chanLabel  + 80;  
offset_deviceType   = offset_chanType   + 4;
offset_eegGroup     = offset_deviceType + 4;
offset_posX         = offset_eegGroup   + 4;
offset_posY         = offset_posX       + 8;
offset_posZ         = offset_posY       + 8;
offset_posStatus    = offset_posZ       + 8;
offset_bipolarRef   = offset_posStatus  + 4;
offset_addScale     = offset_bipolarRef + 4;
offset_isDropDown   = offset_addScale   + 4;
offset_isNoFilter   = offset_isDropDown + 4;

chanInfoLen = (offset_isNoFilter + 4) - 1;  % Raw length
chanInfoLen = round(chanInfoLen/8)*8 ;      % Length of CURRY channel info struct in bytes, consider padding

infoList = struct( 'id',           [],...
                   'chanLabel',    [],...
                   'chanType',     [],...
                   'deviceType',   [],...
                   'eegGroup',     [],...
                   'posX',         [],...
                   'posY',         [],...
                   'posZ',         [],...
                   'posStatus',    [],...
                   'bipolarRef',   [],...
                   'addScale',     [],...
                   'isDropDown',   [],...
				   'isNoFilter',   []);

% Build header
header = initHeader('CTRL',...
                     controlCode('CTRL_FromClient'),...
                     requestType('RequestChannelInfo'),...
                     0,0,0); 

[infoListRaw,status] = clientProcessRequest(con, handles, header,...
                                            'ClientGetChannelInfoList',...
                                            dataType('Data_Info'),...
                                            infoType('InfoType_ChannelInfo'),0);
if status
    try
        % Fill struct
        for i = 1 : numChannels
            j = chanInfoLen*(i-1); % block index
            infoList(i).id          = typecast( infoListRaw( j+offset_channelId  :j+(offset_chanLabel-1)),  'uint32');
            infoList(i).chanLabel   = typecast( infoListRaw( j+offset_chanLabel  :j+(offset_chanType-1)),   'uint16');
            infoList(i).chanType    = typecast( infoListRaw( j+offset_chanType   :j+(offset_deviceType-1)), 'uint32');
            infoList(i).deviceType  = typecast( infoListRaw( j+offset_deviceType :j+(offset_eegGroup-1)),   'uint32');
            infoList(i).eegGroup    = typecast( infoListRaw( j+offset_eegGroup   :j+(offset_posX-1)),       'uint32');
            infoList(i).posX        = typecast( infoListRaw( j+offset_posX       :j+(offset_posY-1)),       'double');
            infoList(i).posY        = typecast( infoListRaw( j+offset_posY       :j+(offset_posZ-1)),       'double');
            infoList(i).posZ        = typecast( infoListRaw( j+offset_posZ       :j+(offset_posStatus-1)),  'double');
            infoList(i).posStatus   = typecast( infoListRaw( j+offset_posStatus  :j+(offset_bipolarRef-1)), 'uint32');
            infoList(i).bipolarRef  = typecast( infoListRaw( j+offset_bipolarRef :j+(offset_addScale-1)),   'uint32');
            infoList(i).addScale    = typecast( infoListRaw( j+offset_addScale   :j+(offset_isDropDown-1)), 'single');
            infoList(i).isDropDown  = typecast( infoListRaw( j+offset_isDropDown :j+(offset_isNoFilter-1)), 'uint32');
			infoList(i).isNoFilter  = typecast( infoListRaw( j+offset_isNoFilter :chanInfoLen*i),           'uint32');
        end
        
        % Fix labels
        for i = 1 : numChannels
            infoList(i).chanLabel = char(infoList(i).chanLabel (1:strfind(infoList(i).chanLabel, 0) - 1));
            infoList(i).chanLabel = [infoList(i).chanLabel ' '];
        end
        
        addLogMsg({[' Labels: ' [infoList.chanLabel]]}, handles.config_log_text,1);
        addLogMsg({' ClientGetChannelInfoList successful'}, handles.com_log_text,1);
        
    catch
        addLogMsg({' ClientGetChannelInfoList failed: data error'}, handles.com_log_text,1);
        status = 0;
        return
    end
else
    return
end
