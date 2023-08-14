#include "stdafx.h"

//****************************************************************************
// Control that draws a block of waveform 
//****************************************************************************

#include "CtrlWaveforms.h"
#include <limits>

BEGIN_MESSAGE_MAP(CCtrlWaveforms, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()

IMPLEMENT_DYNAMIC(CCtrlWaveforms, CWnd)

CCtrlWaveforms::CCtrlWaveforms():CWnd()
{
	::InitializeCriticalSection(&m_CritSecData);	
}

CCtrlWaveforms::~CCtrlWaveforms()
{
	::DeleteCriticalSection(&m_CritSecData);
}

long RINT(double d)
{
	return (long)(d + (d > 0 ? 0.5 : -0.5));
}

void CCtrlWaveforms::OnPaint()
{
	CRect		rect;
	CPaintDC	dc(this);	// device context for painting
	CDC*		pDC				= &dc;
	long		nNumChannels	= m_aLabels.GetCount();

	if (!pDC)
	{
		return;
	}

	GetClientRect(&rect);

	pDC->FillSolidRect(rect, 0xffffff);	//clear pane

	if (nNumChannels <= 0)
	{
		return;	//nothing to draw
	}
	
	float	fSample;
	long	nSamples;
	CSize	size;
	double	dDeltaX;
	double	dDeltaY			= (double)rect.Height() / (double)(nNumChannels);
	long	nMaxLabelLength	= 0;

	//get fitting font, draw labels and find longest label
	HFONT hFont = CreateFont((long)(min(50, dDeltaY + 0.5)),0,0,0,FW_DONTCARE,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_OUTLINE_PRECIS,CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY, VARIABLE_PITCH,TEXT("Arial"));
	pDC->SelectObject(hFont);
	for (long c = 0; c < nNumChannels; ++c)
	{
		pDC->TextOutW(0, (long)(c * dDeltaY + 0.5), m_aLabels[c]);
		size			= pDC->GetTextExtent(m_aLabels[c]);
		nMaxLabelLength	= max (nMaxLabelLength, size.cx);
	}

	nMaxLabelLength += 5; //leave a few empty pixels between the labels and the waveforms

	//draw waveforms of last received block
	::EnterCriticalSection(&m_CritSecData);
		
		if (m_aCurrentDataBlock.GetCount() <= 0)
		{
			::LeaveCriticalSection(&m_CritSecData);
			return; //nothing to draw
		}

		nSamples	= m_aCurrentDataBlock.GetCount() / nNumChannels;
		dDeltaX		= (double)(rect.Width() - nMaxLabelLength) / (double)nSamples;

		//find min/max and compute average per channel
		CArray<float> aMin;
		CArray<float> aMax;
		CArray<float> aAvg;

		aMin.SetSize(nNumChannels);
		aMax.SetSize(nNumChannels);
		aAvg.SetSize(nNumChannels);

		for (long c = 0; c < nNumChannels; ++c)
		{
			aAvg[c] = 0.0f;
			
			for (long n = 0; n < nSamples; ++n)
			{
				fSample = m_aCurrentDataBlock[(n * nNumChannels) + c];
				
				if (n == 0)
				{
					aMin[c] = fSample;
					aMax[c] = fSample;
				}
				else
				{
					if (fSample < aMin[c]) {aMin[c] = fSample;}
					if (fSample > aMax[c]) {aMax[c] = fSample;}
				}
				
				aAvg[c] += fSample;
			}
			aAvg[c] /= nSamples;
		}

		//draw all channel waveforms autoscaled and baseline-corrected
		CArray<POINT> aPoints;
		aPoints.SetSize(nSamples);
		for (long c = 0; c < nNumChannels; ++c)
		{
			double	dAutoscale	= dDeltaY / (aMax[c] - aMin[c]) * 1.25;
			long	nYOffset	= (long)((c + 0.5) * dDeltaY + 0.5);

			for (long n = 0; n < nSamples; ++n)
			{
				fSample			= m_aCurrentDataBlock[(n * nNumChannels) + c] - aAvg[c];
				aPoints[n].x	= RINT(nMaxLabelLength + (n * dDeltaX));
				aPoints[n].y	= RINT(min(max(0, nYOffset + (fSample * dAutoscale)), rect.Height() - 1));
			}

			pDC->Polyline(aPoints.GetData(), aPoints.GetCount());
		}
	::LeaveCriticalSection(&m_CritSecData);
}

void CCtrlWaveforms::SetChannelInfo(NetStreamingChannelInfo* pChannelInfoList, long nNumChannels)
{
	m_aLabels.RemoveAll();

	if (!pChannelInfoList || nNumChannels <= 0)
	{
		return;
	}

	//save labels in list
	for (long n = 0; n < nNumChannels; ++n)
	{
		m_aLabels.Add(pChannelInfoList[n].wcLabel);
	}
	
	Invalidate();
}

//ICtrlWaveforms
void CCtrlWaveforms::NewDataBlock(float* pfData, long nNumChannels, long nNumSamples)
{
	if (!pfData || nNumChannels * nNumSamples <= 0)
	{
		return;
	}

	::EnterCriticalSection(&m_CritSecData);
		m_aCurrentDataBlock.RemoveAll();
		m_aCurrentDataBlock.SetSize(nNumChannels * nNumSamples);
		::memcpy_s(m_aCurrentDataBlock.GetData(), m_aCurrentDataBlock.GetCount() * SIZE_F, pfData, nNumChannels * nNumSamples * SIZE_F);
	::LeaveCriticalSection(&m_CritSecData);

	Invalidate();
}