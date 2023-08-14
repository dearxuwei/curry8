
#pragma once

//****************************************************************************
// This file contains some defines needed for this demo program
//****************************************************************************

#define SIZE_C	(sizeof(char))
#define SIZE_F	(sizeof(float))

//interface to add a line to the log window
class IAddLogLine	
{
	public:
		virtual ~IAddLogLine() {}
		virtual void AddLogLine(CString sLogLine, long nLogID) = 0;
};

//interface to pass data to the waveform control
class ICtrlWaveforms
{
	public:
		virtual ~ICtrlWaveforms() {}
		virtual void NewDataBlock(float* pfData, long nNumChannels, long nNumSamples) = 0;
};