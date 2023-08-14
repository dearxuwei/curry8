function [dataOut, status, message] = clientProcessRequest(con, handles, header, method, code, request, init)
% [data,status,message_code] = clientProcessRequest(con, handles, header, method, code, request)

% NOTE : Byte order in header needs to be swapped

w = whos('header');
headerSize = w.bytes;

if headerSize ~= 20
    addLogMsg({[' Error in ' method ' Header']}, handles.com_log_text,1);
    status = 0;
    return
end
    
% Send header if data-streaming has not started
if (~init)
    pnet(con,'printf','%s',char(header));
end

% Receive header
[data, status] = requestPacket(con, method, 20, handles);

tempPacketSize = 0;
count = 0;
timeout = 10;
synchPackets = 5;
dataOut = [];
message = struct( 'code',           [],...
                  'request',        [],...
                  'startSample',    [],...
                  'packetSize',     []);
              
if status
    message.code        = typecast(fliplr(data(5:6)),'uint16');
    message.request     = typecast(fliplr(data(7:8)),'uint16');
    message.startSample = typecast(fliplr(data(9:12)),'uint32');
    message.packetSize  = typecast(fliplr(data(13:16)),'uint32');
    % Receive body
    if ismember(message.code,code) && ismember(message.request,request)
        % in case packet comes in separate chunks
        while (tempPacketSize < message.packetSize && count < timeout)
            [data, status] = requestPacket(con, method, message.packetSize, handles);
            tempPacketSize = tempPacketSize + length(data);
            dataOut = [dataOut data];
            count = count+1;
        end
    else
        if ~ismember(message.request,request) && strcmp('ClientRequestDataPacket',method)
            addLogMsg({' WARNING: Verify data format! This demo only supports uncompressed data.'}, handles.com_log_text,1);
        end
        addLogMsg({[' ' method ' failed: code or request']}, handles.com_log_text,1);
        addLogMsg({' Attempting synchronization...'}, handles.com_log_text,1);
        count = 0;
        % receive dummy packets to allow synchonization
        while (count < synchPackets)
            requestPacket(con,...
                          ['SynchPacket (',num2str(count+1),' of ', num2str(synchPackets),')'],...
                          message.packetSize, handles);
            count = count+1;
        end
        status = 0;
    end
else
    return
end




