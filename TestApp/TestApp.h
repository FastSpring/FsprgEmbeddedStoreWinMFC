
// TestApp.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "TestAppSheet.h"

// CTestAppApp:
// See TestApp.cpp for the implementation of this class
//

class CTestAppApp : public CWinApp
{
public:
	CTestAppApp();
	~CTestAppApp();

private:
	void ManageAppConfig(StoreParameters &params, bool update = false);
	CTestAppSheet *_sheet;

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CTestAppApp theApp;