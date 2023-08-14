function  i = controlCode(type)

i = -1;

if      strcmp('CTRL_FromServer',     type)
    i = 1;
elseif  strcmp('CTRL_FromClient',    type)
    i = 2;
end