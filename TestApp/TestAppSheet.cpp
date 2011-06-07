// TestAppSheet.cpp : implementation file
//

#include "stdafx.h"
#include "TestApp.h"
#include "TestAppSheet.h"
#include "Controller.h"

// CTestAppSheet

IMPLEMENT_DYNAMIC(CTestAppSheet, CPropertySheet)

CTestAppSheet::CTestAppSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage), m_settingsPage(_params), m_previewPage(_params)
	 , m_bNeedInit(TRUE)
	 , m_nMinCX(0)
	 , m_nMinCY(0)
{
	AddPage(&m_settingsPage);
	AddPage(&m_previewPage);
}

CTestAppSheet::CTestAppSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage), m_settingsPage(_params), m_previewPage(_params)
	 , m_bNeedInit(TRUE)
	 , m_nMinCX(0)
	 , m_nMinCY(0)
{
	m_psh.dwFlags &= ~PSH_HASHELP;
	m_settingsPage.m_psp.dwFlags &= ~PSP_HASHELP;
	m_previewPage.m_psp.dwFlags &= ~PSP_HASHELP;

	AddPage(&m_settingsPage);
	AddPage(&m_previewPage);
}

CTestAppSheet::~CTestAppSheet()
{
}


BEGIN_MESSAGE_MAP(CTestAppSheet, CPropertySheet)
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
END_MESSAGE_MAP()


// CTestAppSheet message handlers


BOOL CTestAppSheet::OnInitDialog()
{
	BOOL bResult = CPropertySheet::OnInitDialog();

	CRect r;  GetWindowRect(&r);
	r.bottom += GetSystemMetrics(SM_CYMENU);
	MoveWindow(r);

	m_nMinCX = r.Width();
	m_nMinCY = r.Height();
   
	// After this point we allow resize code to kick in
	m_bNeedInit = FALSE;
	GetClientRect(&m_rCrt);

	return bResult;
}


void CTestAppSheet::OnSize(UINT nType, int cx, int cy)
{
	CRect r1; 
   CPropertySheet::OnSize(nType, cx, cy);

   if (m_bNeedInit)
      return;

   CTabCtrl *pTab = GetTabControl();
   ASSERT(NULL != pTab && IsWindow(pTab->m_hWnd));
    
   int dx = cx - m_rCrt.Width();
   int dy = cy - m_rCrt.Height();
   GetClientRect(&m_rCrt);

   HDWP hDWP = ::BeginDeferWindowPos(5);

   pTab->GetClientRect(&r1); 
   r1.right += dx; r1.bottom += dy;
   ::DeferWindowPos(hDWP, pTab->m_hWnd, NULL,
                    0, 0, r1.Width(), r1.Height(),
                    SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOZORDER);

   // Move all buttons with the lower right sides
   for (CWnd *pChild = GetWindow(GW_CHILD);
        pChild != NULL;
        pChild = pChild->GetWindow(GW_HWNDNEXT))
   {
      if (pChild->SendMessage(WM_GETDLGCODE) & DLGC_BUTTON)
      {
         pChild->GetWindowRect(&r1); ScreenToClient(&r1); 
         r1.top += dy; r1.bottom += dy; r1.left+= dx; r1.right += dx;
         ::DeferWindowPos(hDWP, pChild->m_hWnd, NULL,
                          r1.left, r1.top, 0, 0,
                          SWP_NOACTIVATE|SWP_NOSIZE|SWP_NOZORDER);
      }
      // Resize everything else...
      else
      {
         pChild->GetClientRect(&r1); 
	 r1.right += dx; r1.bottom += dy;
	 ::DeferWindowPos(hDWP, pChild->m_hWnd, NULL, 0, 0, r1.Width(), r1.Height(),SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOZORDER);
      }

   }

   ::EndDeferWindowPos(hDWP);
}


void CTestAppSheet::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: Add your message handler code here and/or call default

	CPropertySheet::OnGetMinMaxInfo(lpMMI);

	lpMMI->ptMinTrackSize.x = m_nMinCX;
	lpMMI->ptMinTrackSize.y = m_nMinCY;
}
