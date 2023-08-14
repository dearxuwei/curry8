
//****************************************************************************
// CURRYNetStreaming.h : main header file for the CURRY NetStreaming demo application
//****************************************************************************

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

class CCURRYNetStreamingApp : public CWinApp
{
public:
	CCURRYNetStreamingApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CCURRYNetStreamingApp theApp;
