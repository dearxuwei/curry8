function  d = blockType(type)

d = -1;

if      strcmp('DataTypeFloat32bit',    type)
    d = 1;
elseif  strcmp('DataTypeFloat32bitZIP', type) % Not supported in this demo
    d = 2;
elseif  strcmp('DataTypeEventList',     type)
    d = 3;
end