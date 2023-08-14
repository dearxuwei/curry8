function varargout = NetStreamingDemo(varargin)
% NETSTREAMINGDEMO MATLAB code for NetStreamingDemo.fig
%      NETSTREAMINGDEMO, by itself, creates a new NETSTREAMINGDEMO or raises the existing
%      singleton*.
%
%      H = NETSTREAMINGDEMO returns the handle to a new NETSTREAMINGDEMO or the handle to
%      the existing singleton*.
%
%      NETSTREAMINGDEMO('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in NETSTREAMINGDEMO.M with the given input arguments.
%
%      NETSTREAMINGDEMO('Property','Value',...) creates a new NETSTREAMINGDEMO or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before NetStreamingDemo_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to NetStreamingDemo_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help NetStreamingDemo

% Last Modified by GUIDE v2.5 15-Feb-2019 15:18:20

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @NetStreamingDemo_OpeningFcn, ...
                   'gui_OutputFcn',  @NetStreamingDemo_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT

% --- Executes just before NetStreamingDemo is made visible.
function NetStreamingDemo_OpeningFcn(hObject, ~, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to NetStreamingDemo (see VARARGIN)

% set path
mfn = mfilename('fullpath');
[mpath] = fileparts(mfn);
msubfolder = [mpath,'\tcp_udp_ip_2.0.6\tcp_udp_ip'];
path ( mpath, path );
path ( msubfolder, path );

% Choose default command line output for NetStreamingDemo
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

if strcmp(get(hObject,'Visible'),'off')
    set(gca,'visible','off');
end

% UIWAIT makes NetStreamingDemo wait for user response (see UIRESUME)
% uiwait(handles.mainWindow);


% --- Outputs from this function are returned to the command line.
function varargout = NetStreamingDemo_OutputFcn(~, ~, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;

pnet('closeall')


function port_edit_Callback(~, ~, ~)
% hObject    handle to port_edit (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of port_edit as text
%        str2double(get(hObject,'String')) returns contents of port_edit as a double


% --- Executes during object creation, after setting all properties.
function port_edit_CreateFcn(hObject, ~, ~)
% hObject    handle to port_edit (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function hostname_edit_Callback(~, ~, ~)
% hObject    handle to hostname_edit (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of hostname_edit as text
%        str2double(get(hObject,'String')) returns contents of hostname_edit as a double


% --- Executes during object creation, after setting all properties.
function hostname_edit_CreateFcn(hObject, ~, ~)
% hObject    handle to hostname_edit (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in connect_button.
function connect_button_Callback(hObject, ~, handles)
% hObject    handle to connect_button (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of connect_button

if get(hObject,'Value')
    init = 0;
    cont = 0;
    timeout = 10;
    port = str2double(handles.port_edit.String);
    hostname = char(handles.hostname_edit.String);
    
    set(hObject,'String','Sending request...'); pause(0.1)
    con = pnet('tcpconnect',hostname, port);
    [ip,~] = pnet(con,'gethost');
    
    if con == -1
        msgbox('Invalid Hostname or Port', 'Help','help');
        set(hObject,'String','Connect');
        addLogMsg({' Connection failed: Invalid Hostname or Port'}, handles.com_log_text,1);
        return
    else
        addLogMsg({strcat(' Connected to server(IP):',num2str(ip))}, handles.com_log_text,1);
        set(handles.start_stop_amplifier_button,'Enable', 'on');
        hObject.UserData = con; % save reference to pnet's con object
    end
    
    try
        set(hObject,'String','Connecting...');
        [status, ~] = clientCheckNetStreamingVersion(con, handles);
        if status
            [status, basicInfo] = clientGetBasicInfo(con, handles);
            if status
                [status, infoList] = clientGetChannelInfoList(con, basicInfo.eegChan, handles);
                if status
                    set(hObject,'String','Disconnect');
                    while(get(hObject,'Value') && cont < timeout)
                       [status, ~] = clientRequestDataPacket(con, basicInfo, infoList, handles, 1, init);                      
                       if status
                           cont = 0;
                           init = 1;
                       else
                           cont = cont+1;
                       end 
                    end
                end
            end
        end
    catch ME
        addLogMsg({[' Error in ' ME.stack(1).name]}, handles.com_log_text,1);
        addLogMsg({[' Error details: ' ME.message]}, handles.com_log_text,1);
    end
    
    % close client
    set(hObject,'String','Connect');
    pnet('closeall')
    hObject.UserData = []; % delete reference to pnet's con object
    addLogMsg({' Client closed'}, handles.com_log_text,1);
    
    % disable button
    set(handles.start_stop_amplifier_button,'Enable','off');
    
    % handle when user hits application-window's X button
    if ~isempty(get(handles.mainWindow, 'UserData'))
        delete(handles.mainWindow);
    end
    
    if cont == timeout
      addLogMsg({' TIMEOUT: try connecting again or restart communication on server side'}, handles.com_log_text,1);  
    end
end

% --- Executes during object creation, after setting all properties.
function com_log_text_CreateFcn(~, ~, ~)
% hObject    handle to com_log_text (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% --- Executes when user attempts to close mainWindow.
function mainWindow_CloseRequestFcn(hObject, ~, handles)
% hObject    handle to mainWindow (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: delete(hObject) closes the figure

% Clean exit if user hits main window's X button
if ~isempty(handles) && get(handles.connect_button,'Value') % if communication is running
    set(handles.connect_button, 'Value',0); % disconnect first
    set(handles.mainWindow, 'UserData',1);
else
    delete(hObject);
end

% --- Executes on button press in start_stop_amplifier_button.
function start_stop_amplifier_button_Callback(hObject, eventdata, handles)
% hObject    handle to start_stop_amplifier_button (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

if ~isempty(hObject.UserData) && hObject.UserData   
    % allow only if there is an active connection (stored in UserData)
    if ~isempty(handles.connect_button.UserData)
        con = handles.connect_button.UserData;      
        if strcmp('Disconnect', handles.connect_button.String)
            clientStartStopAmplifier(con, 'RequestAmpDisconnect', handles);
        end
    end
else
    uiwait(msgbox('The server does not allow the client to control the amplifier. In CURRY, enable "Allow client to control amplifier" option.', 'Info','help','modal'));
end

% Hint: get(hObject,'Value') returns toggle state of start_stop_amplifier_button
