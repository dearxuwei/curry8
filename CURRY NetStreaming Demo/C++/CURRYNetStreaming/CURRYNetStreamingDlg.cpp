
//****************************************************************************
// This is the dialog class
//****************************************************************************

#include "stdafx.h"
#include "CURRYNetStreaming.h"
#include "CURRYNetStreamingDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//****************************************************************************
// CAboutDlg dialog used for App About
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()
//****************************************************************************



//****************************************************************************
// CCURRYNetStreamingDlg dialog
CCURRYNetStreamingDlg::CCURRYNetStreamingDlg(CWnd* pParent /*=NULL*/) :	CDialogEx(CCURRYNetStreamingDlg::IDD, pParent),
																	m_NetStreaming(this, &m_ctrlWaveforms)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCURRYNetStreamingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control (pDX, IDC_EDIT_SERVER_IP,		m_EditServerIP);
	DDX_Control (pDX, IDC_EDIT_SERVER_PORT,		m_EditServerPort);
	DDX_Control (pDX, IDC_EDIT_LOG,				m_EditLogCommunication);
	DDX_Control (pDX, IDC_EDIT_LOG_EVENTS,		m_EditLogEvents);
	DDX_Control (pDX, IDC_EDIT_LOG_CONFIG,		m_EditLogConfig);
	DDX_Control (pDX, IDC_CHECK_SCROLL,			m_btnScroll);	
	DDX_Control (pDX, IDC_CTR_WAVEFORMS,		m_ctrlWaveforms);
	DDX_Control (pDX, IDC_BUTTON_CONNECT,		m_btnConnect);
	DDX_Control (pDX, IDC_BUTTON_STOP_AMP,		m_btnStartStopAmp);
	DDX_Control (pDX, IDC_STATIC_DELAY,			m_staticDelay);

	m_btnScroll.SetCheck(BST_CHECKED);
	m_EditServerIP.SetWindowTextW(L"localhost");
	m_EditServerPort.SetWindowTextW(L"4455");
	m_btnStartStopAmp.EnableWindow(false);
	AddLogLine(L"Type in the IP (or name) and port of your running CURRY NetStreaming Server and press \'Connect\'");
}

BEGIN_MESSAGE_MAP(CCURRYNetStreamingDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CCURRYNetStreamingDlg::OnBnClickedButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON_STOP_AMP, &CCURRYNetStreamingDlg::OnBnClickedButtonStopAmp)
	ON_BN_CLICKED(IDC_BUTTON_START_AMP, &CCURRYNetStreamingDlg::OnBnClickedButtonStartAmp)
	ON_BN_CLICKED(IDC_BUTTON_START_REC, &CCURRYNetStreamingDlg::OnBnClickedButtonStartRec)
	ON_BN_CLICKED(IDC_BUTTON_STOPT_REC, &CCURRYNetStreamingDlg::OnBnClickedButtonStoptRec)
	ON_BN_CLICKED(IDC_BUTTON_START_IMP, &CCURRYNetStreamingDlg::OnBnClickedButtonStartImp)
	ON_BN_CLICKED(IDC_BUTTON_STOPT_IMP, &CCURRYNetStreamingDlg::OnBnClickedButtonStoptImp)
	ON_BN_CLICKED(IDC_BUTTON_SET_REC_PATH, &CCURRYNetStreamingDlg::OnBnClickedButtonSetRecPath)
END_MESSAGE_MAP()


// CCURRYNetStreamingDlg message handlers
BOOL CCURRYNetStreamingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCURRYNetStreamingDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

void CCURRYNetStreamingDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR CCURRYNetStreamingDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// connect to a CURRY netstreaming server
void CCURRYNetStreamingDlg::OnBnClickedButtonConnect()
{
	HRESULT hr = S_OK;
	CString sIP, sPort, sTmp;
	
	m_EditServerIP.GetWindowTextW(sIP);
	m_EditServerPort.GetWindowTextW(sPort);

	if (m_NetStreaming.IsConnected())
	{
		//stop netstreaming client
		m_NetStreaming.SetClient(false);
		m_NetStreaming.ClientRequestStreamData(false);
		m_NetStreaming.CloseClient();
		m_EditServerIP.EnableWindow(true);
		m_EditServerPort.EnableWindow(true);
		m_btnStartStopAmp.EnableWindow(false);
		m_btnConnect.SetWindowTextW(L"Connect");
	}
	else
	{
		//start netstreaming client

		//clear logs
		m_EditLogCommunication.SetWindowTextW(L"");
		m_EditLogConfig.SetWindowTextW(L"");
		m_EditLogEvents.SetWindowTextW(L"");
		
		//change some UI elements
		m_EditServerIP.EnableWindow(false);
		m_EditServerPort.EnableWindow(false);
		m_btnStartStopAmp.EnableWindow(true);
		m_btnConnect.SetWindowTextW(L"Disconnect");

		//start netstreaming client
		m_NetStreaming.SetClient(true, sIP, StrToInt(sPort));

		while (1)
		{
			if (FAILED(hr = m_NetStreaming.OpenClient(&sTmp)) ||				//open socket
				FAILED(hr = m_NetStreaming.ClientCheckNetStreamingVersion()))	//get netstreaming version from server
			{
				break;
			}
			
			//Optional: check communication delay 10 times and compute an average and standard deviation
			double dDelay[10]	= {0,};
			double dDelayAvg	= 0.0;
			double dDelayStdDev	= 0.0;
			for (long n = 0; n < _countof(dDelay); ++n)
			{
				m_NetStreaming.ClientGetDelay(&dDelay[n]);
				dDelayAvg += dDelay[n];
			}
			dDelayAvg /= _countof(dDelay);
			
			for (long n = 0; n < _countof(dDelay); ++n)
			{
				dDelayStdDev += (dDelay[n] - dDelayAvg) * (dDelay[n] - dDelayAvg);
			}
			dDelayStdDev /= _countof(dDelay);
			dDelayStdDev = dDelayStdDev > 0 ? sqrt(dDelayStdDev) : 0;
			sTmp.Format(L"Average Communication Delay: %.2f ms (+/- %.2f ms)", dDelayAvg, dDelayStdDev);
			m_staticDelay.SetWindowText(sTmp);
			
			//Optional: check if the amplifier on the server is currently running
			long nAmpStatus = -1;
			if (FAILED(m_NetStreaming.ClientGetServerAmpStatus(&nAmpStatus)))
			{
				AddLogLine(L"ERROR: ClientGetServerAmpStatus failed.");
			}
			
			sTmp.Format(L"Current Amplifier Status: %d", nAmpStatus);
			AddLogLine(sTmp);

			if (FAILED(hr = m_NetStreaming.ClientGetBasicInfo(&m_basicInfo)))
			{
				AddLogLine(L"ERROR: ClientGetBasicInfo failed.");
				break;
			}

			//Optional: if the amplifier is not running currently, try to start it
			if (nAmpStatus != 1 && m_basicInfo.bAllowClientToControlAmp)
			{
				//Optional: send a request to connect the amplifier (the server may not allow this)
				if (SUCCEEDED(m_NetStreaming.ClientRequestControlAmp(true)))
				{
					AddLogLine(L"Waiting for server to connect to amplifier...");
					//wait for a while and check if server has connected to amplifier
					long nWaitSeconds = 10;
					while (nAmpStatus != 1 && nWaitSeconds >= 0)
					{
						Sleep(1000);
						if (FAILED(m_NetStreaming.ClientGetServerAmpStatus(&nAmpStatus)))
						{
							AddLogLine(L"ERROR: ClientGetServerAmpStatus failed.");
							break;
						}
						nWaitSeconds--;
					}

					if (nAmpStatus != 1)
					{
						AddLogLine(L"ERROR: Timeout: Server does not allow starting the amplfier or did not start amplifier in time.");
						break;
					}
				}
				else
				{
					AddLogLine(L"ERROR: ClientRequestControlAmp failed.");
				}
			}

			//request basic information (number of channels, sampling rate, etc...) if current information are not valid
			if ((m_basicInfo.nEegChan <= 0 || m_basicInfo.nRate <= 0) &&
				FAILED(hr = m_NetStreaming.ClientGetBasicInfo(&m_basicInfo)))
			{
				AddLogLine(L"ERROR: ClientGetBasicInfo failed.");
				break;
			}

			if (m_basicInfo.nEegChan <= 0 || m_basicInfo.nRate <= 0 || nAmpStatus != 1)
			{
				AddLogLine(L"ERROR: Amplifier is not running on Server.");
				hr = E_FAIL;
				break;
			}

			sTmp.Format(L"%d channels, %dHz", m_basicInfo.nEegChan, m_basicInfo.nRate);
			AddLogLine(sTmp, 1);

			//get channel properties (label, group, 3D position, etc)
			NetStreamingChannelInfo*	pNetStreamingChannelInfoList		= NULL;
			long						nNetStreamingChannelInfoListSize	= m_basicInfo.nEegChan * sizeof(NetStreamingChannelInfo);
			
			pNetStreamingChannelInfoList = (NetStreamingChannelInfo*)malloc(nNetStreamingChannelInfoListSize);
			if (!pNetStreamingChannelInfoList)
			{
				hr = E_OUTOFMEMORY;
				break;
			}

			if (FAILED(hr = m_NetStreaming.ClientGetChannelInfoList(pNetStreamingChannelInfoList, nNetStreamingChannelInfoListSize)))
			{
				break;
			}

			sTmp = L"\r\n";
			for (long n = 0; n < m_basicInfo.nEegChan; ++n)
			{
				CString s;
				s.Format(L"%s\ttype %d\tx:%.1f\ty:%.1f\tz:%.1f\r\n", 
					pNetStreamingChannelInfoList[n].wcLabel, pNetStreamingChannelInfoList[n].eType, pNetStreamingChannelInfoList[n].dPosX, pNetStreamingChannelInfoList[n].dPosY, pNetStreamingChannelInfoList[n].dPosZ);
				sTmp.Append(s);
			}
			AddLogLine(sTmp, 1);

			m_ctrlWaveforms.SetChannelInfo(pNetStreamingChannelInfoList, m_basicInfo.nEegChan);

			free(pNetStreamingChannelInfoList);

			//notify server to start sending data
			if (FAILED(hr = m_NetStreaming.ClientRequestStreamData(true)))
			{
				break;
			}

			hr = m_NetStreaming.StartClientThread() ? S_OK : E_FAIL;
			break;
		}

		if (SUCCEEDED(hr))
		{
			AddLogLine(L"Succesfully connected to server.");
		}
		else
		{
			m_NetStreaming.SetClient(false);
			AddLogLine(L"ERROR connecting to server.");
			m_EditServerIP.EnableWindow(true);
			m_EditServerPort.EnableWindow(true);
			m_btnStartStopAmp.EnableWindow(false);
			m_btnConnect.SetWindowTextW(L"Connect");
		}
	}
}

#include <shobjidl_core.h>

//set recording path (must be done before amplifier is connected)
void CCURRYNetStreamingDlg::OnBnClickedButtonSetRecPath()
{
	HRESULT			hr;
	long			nAmpStatus = 1;

	if (!m_NetStreaming.ClientIsInited())
	{
		AddLogLine(L"ERROR: Client is not initialized (press \"Connect\").");
		return;
	}

	//check if server allows us to start/stop the amplifier
	if (!m_basicInfo.bAllowClientToControlRec)
	{
		MessageBox(L"The server does not allow the client to control the recording.\nIn CURRY, set option \"Allow Client to control recording\".", L"ERROR");
		return;
	}

	CFileDialog FileDlg(FALSE, L"", L"", 6UL, L"CURRY Recording (*.cdt)|*.cdt|"); 

    if( FileDlg.DoModal() == IDOK )
    {
		wchar_t wPath[MAX_PATH];
		
		_wmakepath_s(wPath, L"", FileDlg.GetFolderPath().GetString(), FileDlg.GetFileName().GetString(), L"");

		//send request to set path for next recording
		if (FAILED(hr = m_NetStreaming.ClientRequestSetRecPath(CString(wPath))))	//path should look like L"C:\\Users\\Tech\\Desktop\\Test.cdt"
		{
			AddLogLine(L"ERROR: ClientRequestSetRecPath failed.");
			return;
		}

		AddLogLine(L"Sent request to set recording path.");
	}
}


//Start the amplifier on the server, if allowed by the server
void CCURRYNetStreamingDlg::OnBnClickedButtonStartAmp()
{
	HRESULT			hr;
	long			nAmpStatus = 1;

	if (!m_NetStreaming.ClientIsInited())
	{
		AddLogLine(L"ERROR: Client is not initialized (press \"Connect\").");
		return;
	}

	//check if server allows us to start/stop the amplifier
	if (!m_basicInfo.bAllowClientToControlAmp)
	{
		MessageBox(L"The server does not allow the client to control the amplifier.\nIn CURRY, set option \"Allow Client to control amplifier\".", L"ERROR");
		return;
	}
	
	//send request to stop the amplifier
	if (FAILED(hr = m_NetStreaming.ClientRequestControlAmp(true)))
	{
		AddLogLine(L"ERROR: ClientRequestControlAmp failed.");
		return;
	}

	AddLogLine(L"Sent request to start the amplifier.");
}


//Stop the amplifier on the server, if allowed by the server
void CCURRYNetStreamingDlg::OnBnClickedButtonStopAmp()
{
	HRESULT			hr;
	long			nAmpStatus = 1;

	if (!m_NetStreaming.ClientIsInited())
	{
		AddLogLine(L"ERROR: Client is not initialized (press \"Connect\").");
		return;
	}

	//check if server allows us to start/stop the amplifier
	if (!m_basicInfo.bAllowClientToControlAmp)
	{
		MessageBox(L"The server does not allow the client to control the amplifier.\nIn CURRY, set option \"Allow Client to control amplifier\".", L"ERROR");
		return;
	}
	
	//send request to stop the amplifier
	if (FAILED(hr = m_NetStreaming.ClientRequestControlAmp(false)))
	{
		AddLogLine(L"ERROR: ClientRequestControlAmp failed.");
		return;
	}

	AddLogLine(L"Sent request to stop the amplifier.");
}

//Start Recording on the server, if allowed by server
void CCURRYNetStreamingDlg::OnBnClickedButtonStartRec()
{
	HRESULT			hr;
	long			nAmpStatus = 1;

	if (!m_NetStreaming.ClientIsInited())
	{
		AddLogLine(L"ERROR: Client is not initialized (press \"Connect\").");
		return;
	}

	//check if server allows us to start/stop recording
	if (!m_basicInfo.bAllowClientToControlRec)
	{
		MessageBox(L"The server does not allow the client to control the recording.\nIn CURRY, set option \"Allow Client to control recording\".", L"ERROR");
		return;
	}
	
	//send request to stop the amplifier
	if (FAILED(hr = m_NetStreaming.ClientRequestControlRecording(true)))
	{
		AddLogLine(L"ERROR: ClientRequestControlRec failed.");
		return;
	}

	AddLogLine(L"Sent request to start recording.");
}

void CCURRYNetStreamingDlg::OnBnClickedButtonStoptRec()
{
	HRESULT			hr;
	long			nAmpStatus = 1;

	if (!m_NetStreaming.ClientIsInited())
	{
		AddLogLine(L"ERROR: Client is not initialized (press \"Connect\").");
		return;
	}

	//check if server allows us to start/stop recording
	if (!m_basicInfo.bAllowClientToControlRec)
	{
		MessageBox(L"The server does not allow the client to control the recording.\nIn CURRY, set option \"Allow Client to control recording\".", L"ERROR");
		return;
	}
	
	//send request to stop the amplifier
	if (FAILED(hr = m_NetStreaming.ClientRequestControlRecording(false)))
	{
		AddLogLine(L"ERROR: ClientRequestControlRec failed.");
		return;
	}

	AddLogLine(L"Sent request to stop recording.");
}


void CCURRYNetStreamingDlg::OnBnClickedButtonStartImp()
{
	HRESULT			hr;
	long			nAmpStatus = 1;

	if (!m_NetStreaming.ClientIsInited())
	{
		AddLogLine(L"ERROR: Client is not initialized (press \"Connect\").");
		return;
	}

	//check if server allows us to start/stop recording
	if (!m_basicInfo.bAllowClientToControlAmp)
	{
		MessageBox(L"The server does not allow the client to control the impedance test.\nIn CURRY, set option \"Allow Client to control amplifier\".", L"ERROR");
		return;
	}
	
	//send request to stop the amplifier
	if (FAILED(hr = m_NetStreaming.ClientRequestControlImpedanceTest(true)))
	{
		AddLogLine(L"ERROR: ClientRequestControlImpedanceTest failed.");
		return;
	}

	AddLogLine(L"Sent request to start an impedance test.");
}


void CCURRYNetStreamingDlg::OnBnClickedButtonStoptImp()
{
	HRESULT			hr;
	long			nAmpStatus = 1;

	if (!m_NetStreaming.ClientIsInited())
	{
		AddLogLine(L"ERROR: Client is not initialized (press \"Connect\").");
		return;
	}

	//check if server allows us to start/stop recording
	if (!m_basicInfo.bAllowClientToControlAmp)
	{
		MessageBox(L"The server does not allow the client to control the impedance test.\nIn CURRY, set option \"Allow Client to control amplifier\".", L"ERROR");
		return;
	}
	
	//send request to stop the amplifier
	if (FAILED(hr = m_NetStreaming.ClientRequestControlImpedanceTest(false)))
	{
		AddLogLine(L"ERROR: ClientRequestControlImpedanceTest failed.");
		return;
	}

	AddLogLine(L"Sent request to stop an impedance test.");
}


//write a line into a log window
void CCURRYNetStreamingDlg::AddLogLine(CString sLogLine, long nLogID)
{
	CString sTmp;
	CEdit*	pLog	= nLogID == 1 ? &m_EditLogConfig
					: nLogID == 2 ? &m_EditLogEvents
					: &m_EditLogCommunication;

	if (sLogLine.IsEmpty() || !IsWindow(pLog->m_hWnd))
	{
		return;
	}

	//get date string
	time_t		rawtime = {0,};
	struct tm	timeinfo = {0,};
	char		cTime[80];
	time(&rawtime);
	localtime_s(&timeinfo, &rawtime);
	strftime(cTime,80,"%I:%M:%S",&timeinfo);
	
	pLog->SetRedraw(false);

	UINT uiLimit	= pLog->GetLimitText();
	long nLineCount = pLog->GetLineCount();

	//limit lines
	if (nLineCount >= 1200)	//remove 200 lines if more than 1200 lines
	{
		const CString sLineEndToken = L"\r\n";
		CString sTmpText;
		pLog->SetSel(-1);
		pLog->GetWindowTextW(sTmpText);
				
		for (long nLine = 0; nLine < nLineCount - 200; ++nLine)	//remove 200 lines
		{
			sTmpText = sTmpText.Right(sTmpText.GetLength() - sTmpText.Find(sLineEndToken) - sLineEndToken.GetLength());
		}

		pLog->SetWindowTextW(sTmpText);
		nLineCount = pLog->GetLineCount();
	} 

	//finalize logline
	sTmp = CString(cTime) + L": " + sLogLine + L"\r\n";

	//append text to end
	pLog->SetSel(-1);
	pLog->ReplaceSel(sTmp);

	//scroll to bottom, if desired
	if (m_btnScroll.GetCheck() == BST_CHECKED)
	{
		SCROLLINFO si = { sizeof(si) };
		pLog->GetScrollInfo(SB_VERT, &si, SIF_ALL);
		nLineCount = pLog->GetLineCount();
		if (nLineCount > 1 && si.nPos + si.nPage < (unsigned long)nLineCount) 
		{
			pLog->LineScroll(nLineCount - (si.nPos + si.nPage));	
		}
	}

	pLog->SetRedraw(true);
	pLog->Invalidate();
}