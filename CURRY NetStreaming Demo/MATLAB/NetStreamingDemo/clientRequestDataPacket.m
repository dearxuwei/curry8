function [status, data] = clientRequestDataPacket(con, basicInfo, infoList, handles, startStop, init)
% [status, data] = ClientRequestDataPacket(con, handles, startStop)
% Can receive eeg data, events and impedance values.
%
% startStop = 1 % start
% startStop = 0 % stop

% Event variables definitions
% C++ definition of CURRY event struct:
% struct NetStreamingEvent
% {                                 % Offsets in bytes
% 	long	nEventType;             % 1:4
% 	long	nEventLatency;          % 5:8
% 	long	nEventStart;            % 9:12
% 	long	nEventEnd;              % 13:16
% 	wchar_t	wcEventAnnotation[260]; % 17:536
% };

% Offsets for struct variables
offsetEventType         = 1;
offsetEventLatency      = offsetEventType       + 4;
offsetEventStart        = offsetEventLatency    + 4;
offsetEventEnd          = offsetEventStart      + 4;
offsetEventAnnotation   = offsetEventEnd        + 4;

eventStructLength = (offsetEventAnnotation + 520) - 1;  % Raw length
eventStructLength = round(eventStructLength/8)*8 ;      % Length of CURRY event struct in bytes, consider padding

% Protocol variable definitions
dataTypes   = [ dataType('Data_Eeg'),...
                dataType('Data_Events'),...
                dataType('Data_Impedance')];
    
blockTypes  = [ blockType('DataTypeFloat32bit'),...
                blockType('DataTypeEventList')];

% Build header
if startStop
    % start
    header = initHeader('CTRL',...
                         controlCode('CTRL_FromClient'),...
                         requestType('RequestStreamingStart'),...
                         0,0,0); 
else
    % stop
    header = initHeader('CTRL',...
                         controlCode('CTRL_FromClient'),...
                         requestType('RequestStreamingStop'),...
                         0,0,0);    
end

[data,status,message] = clientProcessRequest(con, handles, header, 'ClientRequestDataPacket', dataTypes, blockTypes, init);

if ~startStop
    return
end
    
if status
    if message.code == 2        % eeg data
        receivedSamples = length(data)/ (basicInfo.dataSize*basicInfo.eegChan); 
        addLogMsg({[' Received ' num2str(length(data)/1000) ' kBytes, EEG, '...
            num2str(receivedSamples) ' samples, Start sample = ' num2str(message.startSample)]}, handles.com_log_text,1);       
        plotPacket(handles, basicInfo, infoList, data);
    elseif message.code == 3    % event data
        if mod(message.packetSize, eventStructLength) == 0
            numEvents = message.packetSize/eventStructLength;
            addLogMsg({[' Received ' num2str(length(data)) ' Bytes, Event ']}, handles.com_log_text,1);
            if numEvents > 0
                for i = 1 : numEvents
                    eventOffset     = eventStructLength * (i-1);
                    eventType       = typecast(data(eventOffset + offsetEventType       : eventOffset + offsetEventLatency-1),  'int32');
                    eventLatency    = typecast(data(eventOffset + offsetEventLatency    : eventOffset + offsetEventStart -1),   'int32');
                    %eventStart      = typecast(data(eventOffset + offsetEventStart      : eventOffset + offsetEventEnd -1),     'int32');
                    %eventEnd        = typecast(data(eventOffset + offsetEventEnd        : eventOffset + offsetEventEnd -1),     'int32');
                    eventAnnotation = typecast(data(eventOffset + offsetEventAnnotation  : eventOffset + eventStructLength),    'uint16');
                    addLogMsg({[' Event type ' num2str(eventType)...
                                ', Latency: ' num2str(eventLatency)...
                                ', Annotation: ' char(eventAnnotation)]}, handles.events_log_text,1);
                end
            end
        else
            status = 0;
            addLogMsg({'ClientRequestDataPacket failed: unmatching event structure size'}, handles.com_log_text,1);
        end
    elseif message.code == 4    % impedances
        addLogMsg({[' Received ' num2str(length(data)) ' Bytes, Impedances']}, handles.com_log_text,1);
        impedances = (reshape(typecast(data, 'single'), basicInfo.eegChan, 1));
        tempStr = ' ';
        for i = 1:basicInfo.eegChan
            tempStr = [tempStr infoList(i).chanLabel '= ' num2str(impedances(i)/1000) ', '];
        end
        addLogMsg({[' Impedances (kOhm): ' tempStr]} , handles.config_log_text,1);
    end
    
    addLogMsg({' ClientRequestDataPacket successful'}, handles.com_log_text,1);
end
