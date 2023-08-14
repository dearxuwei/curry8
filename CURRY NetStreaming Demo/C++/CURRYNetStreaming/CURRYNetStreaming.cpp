
//****************************************************************************
// CURRYNetStreaming.cpp : Defines the class behaviors for the application.
//****************************************************************************

#include "stdafx.h"
#include "CURRYNetStreaming.h"
#include "CURRYNetStreamingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCURRYNetStreamingApp

BEGIN_MESSAGE_MAP(CCURRYNetStreamingApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


CCURRYNetStreamingApp::CCURRYNetStreamingApp()
{
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
}


CCURRYNetStreamingApp theApp;


BOOL CCURRYNetStreamingApp::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	CShellManager *pShellManager = new CShellManager;

	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	CCURRYNetStreamingDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}
	
	return FALSE;
}

