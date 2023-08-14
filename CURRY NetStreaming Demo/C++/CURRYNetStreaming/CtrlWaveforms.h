#pragma once

//****************************************************************************
// Control that draws a block of waveform 
//****************************************************************************

#include "Packets.h"

class CCtrlWaveforms :	public CWnd, 
						public ICtrlWaveforms
{
	DECLARE_DYNAMIC(CCtrlWaveforms)

public:
	CCtrlWaveforms();
	virtual ~CCtrlWaveforms();

	void SetChannelInfo(NetStreamingChannelInfo* pChannelInfoList, long nNumChannels);

	//ICtrlWaveforms
	void NewDataBlock(float* pfData, long nNumChannels, long nNumSamples);

	afx_msg void	OnPaint();
	DECLARE_MESSAGE_MAP()

private:
	CArray<CString>		m_aLabels;
	CArray<float>		m_aCurrentDataBlock;
	CRITICAL_SECTION	m_CritSecData;
};