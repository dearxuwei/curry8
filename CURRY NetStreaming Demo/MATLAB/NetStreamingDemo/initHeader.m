function header = initHeader(chanID,code,request,samples,sizeBody,sizeUn)
% [header, headerSize] = initHeaderNetView(chanID,code,request,samples,sizeBody,sizeUn)

% NOTE : Byte order in header needs to be swapped

% Set up header
c_chID      = uint8    (chanID);
w_Code      = typecast (swapbytes (uint16(code)),    'uint8');
w_Request   = typecast (swapbytes (uint16(request)), 'uint8');
un_Sample   = typecast (swapbytes (uint32(samples)), 'uint8');
un_Size     = typecast (swapbytes (uint32(sizeBody)),'uint8');
un_SizeUn   = typecast (swapbytes (uint32(sizeUn)),  'uint8');

header = [c_chID w_Code w_Request un_Sample un_Size un_SizeUn];

