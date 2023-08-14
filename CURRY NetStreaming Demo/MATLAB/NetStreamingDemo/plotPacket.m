function plotPacket(handles, basicInfo, infoList, data)

try   
    axes(handles.eegplot);
    cla;
    
    numSamples = length(data)/ (basicInfo.dataSize*basicInfo.eegChan);
    
    if basicInfo.dataSize == 2 % int
        packet = flipud(reshape(typecast(data, 'int16'), basicInfo.eegChan, numSamples));
    elseif basicInfo.dataSize == 4 % short
        packet = flipud(reshape(typecast(data, 'single'), basicInfo.eegChan, numSamples));
    else
        addLogMsg({' Error in plotPacket: Unknown datatype '}, handles.com_log_text,1);
        return
    end
    
    t = linspace(0,double(numSamples)/double(basicInfo.sampleRate),numSamples);
    
    % remove baseline
    packet = packet - repmat(median(packet,2),1,numSamples);
    
    % calculate shift for each channel in plot
    minmax = [min(min(packet)) max(max(packet))];
    oneshift = mean(abs(minmax));
    shift = repmat(oneshift, basicInfo.eegChan, 1);
    shift = repmat(cumsum(shift),1,numSamples);
    
    % plot 'eeg' data
    plot(t, packet+shift)
    
    % add labels, making sure we have a ploting range
    if (minmax(1) ~= minmax(2))
        set(gca,'ytick',shift(:,1),'yticklabel',...
            flip({infoList.chanLabel}), 'fontsize', 8)
            ylim([-oneshift/2 max(max(shift+packet))+oneshift/2])
    end
    
    % edit axes
    xlabel('Time [s]')
    title('Incomming Packet (autoscaled, baseline corrected)','fontsize', 8),grid on
    
catch ME
    addLogMsg({[' Error details: ' ME.message]}, handles.com_log_text,1);
    addLogMsg({' Error: plotPacket failed' }, handles.com_log_text,1);
end