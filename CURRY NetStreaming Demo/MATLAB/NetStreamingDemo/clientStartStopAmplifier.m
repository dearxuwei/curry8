function status = clientStartStopAmplifier(con, request, handles)
% status = clientStartStopAmplifier(con, request, handles)

status = 1;

% Build header
header = initHeader('CTRL',...
                     controlCode('CTRL_FromClient'),...
                     requestType(request),...
                     0,0,0); 

% Send request to start/stop
pnet(con,'printf','%s',char(header));

if strcmp('RequestAmpConnect', request)
    addLogMsg({' Sending request to START amplifier' }, handles.config_log_text,1);
elseif strcmp('RequestAmpDisconnect', request)
    addLogMsg({' Sending request to STOP amplifier' }, handles.config_log_text,1);
else
    status = 0;
end