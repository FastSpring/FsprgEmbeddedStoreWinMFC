#pragma once

#include "Settings.h"
#include "Preview.h"
#include "StoreParameters.h"

// CTestAppSheet

class CTestAppSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CTestAppSheet)

public:
	CTestAppSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CTestAppSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CTestAppSheet();

	CSettings m_settingsPage;
	CPreview m_previewPage;
	StoreParameters _params;

protected:
   BOOL   m_bNeedInit;
   CRect  m_rCrt;
   int    m_nMinCX;
   int    m_nMinCY;

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
};


