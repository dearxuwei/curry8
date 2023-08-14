function [data, status] = requestPacket(con, method, packetSize, handles)
%[status, data] = requestPacket(con, method, packetSize, handles)

count = 0;
timeout = 20;

while 1    
   data = pnet(con,'read', packetSize, 'uint8');
   if ~isempty(data) || count == timeout
   break
   end
   count = count+1;
   pause(0.2)
end

if count == timeout
   addLogMsg({[' ' method ' failed: timeout']}, handles.com_log_text,1);
   status = 0;
   return
end

status = 1;