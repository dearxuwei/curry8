function  d = dataType(type)

d = -1;

if      strcmp('Data_Info',    type)
    d = 1;
elseif  strcmp('Data_Eeg',      type)
    d = 2;
elseif  strcmp('Data_Events',    type)
    d = 3;
elseif  strcmp('Data_Impedance',type)
    d = 4;
end