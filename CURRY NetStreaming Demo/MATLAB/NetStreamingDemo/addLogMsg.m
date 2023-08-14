function addLogMsg(msg,handle,timeFlag)

oldmsgs = cellstr(get(handle,'String'));

if timeFlag
    set(handle,'String',[strcat(datestr(datetime('now'),13),' ',msg); oldmsgs]);
else
    set(handle,'String',[msg; oldmsgs]);
end