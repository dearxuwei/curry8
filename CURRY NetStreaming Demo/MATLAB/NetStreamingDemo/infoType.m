function  i = infoType(type)

i = -1;

if      strcmp('InfoType_Version',      type)
    i = 1;
elseif  strcmp('InfoType_BasicInfo',    type)
    i = 2;
elseif  strcmp('InfoType_ChannelInfo',  type)
    i = 4;
elseif  strcmp('InfoType_StatusAmp',    type)
    i = 7;
elseif  strcmp('InfoType_Time',         type)
    i = 9;
end