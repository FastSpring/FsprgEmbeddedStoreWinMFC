// FsprgEmbeddedStore.h : main header file for the FsprgEmbeddedStore DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CFsprgEmbeddedStoreApp
// See FsprgEmbeddedStore.cpp for the implementation of this class
//

class CFsprgEmbeddedStoreApp : public CWinApp
{
public:
	CFsprgEmbeddedStoreApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
