
#pragma once

//****************************************************************************
// This is the dialog class
//****************************************************************************

#include "NetStreamingReceiver.h"
#include "CtrlWaveforms.h"

class CCURRYNetStreamingDlg : public	CDialogEx, 
										IAddLogLine
{
public:
	CCURRYNetStreamingDlg(CWnd* pParent = NULL);	// standard constructor

	enum { IDD = IDD_CURRYNETSTREAMING_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL OnInitDialog();

	HRESULT ConnectToServer();

	//IAddLogLine implementation
	void AddLogLine(CString sLogLine, long nLogID = 0);

	HICON					m_hIcon;
	CEdit					m_EditServerIP;
	CEdit					m_EditServerPort;
	CEdit					m_EditLogCommunication;
	CEdit					m_EditLogEvents;
	CEdit					m_EditLogConfig;
	CButton					m_btnScroll;
	CButton					m_btnConnect;
	CButton					m_btnStartStopAmp;
	CStatic					m_staticDelay;
	CCtrlWaveforms			m_ctrlWaveforms;
	CNetStreamingReceiver	m_NetStreaming;

	BasicInfoAcq			m_basicInfo;

	// Generated message map functions
	afx_msg void	OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void	OnPaint();
	afx_msg HCURSOR	OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButtonConnect();
	afx_msg void OnBnClickedButtonStopAmp();
	afx_msg void OnBnClickedButtonStartAmp();
	afx_msg void OnBnClickedButtonStartRec();
	afx_msg void OnBnClickedButtonStoptRec();
	afx_msg void OnBnClickedButtonStartImp();
	afx_msg void OnBnClickedButtonStoptImp();
	afx_msg void OnBnClickedButtonSetRecPath();
};
