// Preview.cpp : implementation file
//

#include "stdafx.h"
#include <string>
#include <map>
#include <fstream>
#include "TestApp.h"
#include "Preview.h"
#include "afxdialogex.h"
#include "StoreParameters.h"

// CPreview dialog

IMPLEMENT_DYNAMIC(CPreview, CPropertyPage)

CPreview::CPreview(StoreParameters &params)
	: CPropertyPage(CPreview::IDD)
	, m_previewPageType(_T(""))
	, m_UrlField(_T(""))
	, _controller(NULL)
	, _params(params)
	, _receivedOrder(NULL)
{
	
}

CPreview::~CPreview()
{
	if (_controller != NULL)
	{
		__unhook(&Controller::DidLoadStoreEventHandler, _controller, &CPreview::DidLoadStore);
		__unhook(&Controller::DidLoadPageEventHandler, _controller, &CPreview::DidLoadPage);
		__unhook(&Controller::DidReceiveOrderEventHandler, _controller, &CPreview::DidReceiveOrder);
		__unhook(&Controller::PropertyChangedEventHandler, _controller, &CPreview::PropertyChanged);

		delete _controller;
	}

	if (_receivedOrder != NULL)
	{
		delete _receivedOrder;
	}
}

void CPreview::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PREVIEW_PAGE_TYPE, m_previewPageType);
	DDX_Text(pDX, IDC_URL_FIELD, m_UrlField);
	DDX_Control(pDX, IDC_SAVEAS_BTN, m_saveAsBtn);
	DDX_Control(pDX, IDC_PROGRESS1, m_browserProgress);
}


BEGIN_MESSAGE_MAP(CPreview, CPropertyPage)
	ON_BN_CLICKED(IDC_SAVEAS_BTN, &CPreview::OnBnClickedSaveasBtn)
	ON_BN_CLICKED(IDC_RELOAD_BTN, &CPreview::OnBnClickedReloadBtn)
	ON_WM_SIZE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CPreview message handlers


void CPreview::OnBnClickedSaveasBtn()
{
	CFileDialog saveFileDialog(FALSE, L".xml", NULL, 0, L"XML files (*.xml)|*.xml||", this);

	if (saveFileDialog.DoModal() == IDOK)
	{
		wstring orderXml = _receivedOrder->GetRaw()->GetOriginalDoc();
		
		wofstream outfile;
		outfile.open(saveFileDialog.GetPathName());
		outfile<<orderXml;
		outfile.close();
	}
}


void CPreview::OnBnClickedReloadBtn()
{
	_controller->LoadWithParameters(_params);
}


BOOL CPreview::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	CWnd* pControl = (CWnd*)GetDlgItem(IDC_EXPLORER1);
	LPUNKNOWN pUnk = pControl->GetControlUnknown();
	LPDISPATCH pDisp = NULL;
	pUnk->QueryInterface(IID_IDispatch, (LPVOID*)&pDisp);
	CComQIPtr<IWebBrowser2> spTempWebBrowser = pDisp;

	// Create client site in order to customize the web browser.  We
	// are going to remove the 3D border
	m_iClientSite = new CWebCtrlInterFace();
	
	if (m_iClientSite != NULL)
	{
		m_iClientSite->AddRef();

		IOleObject *pIOleObj = NULL;
		if (spTempWebBrowser != NULL)
			spTempWebBrowser->QueryInterface(IID_IOleObject, (void**)&pIOleObj);

		if (spTempWebBrowser != NULL)
		{
			IOleClientSite *oldClientSite = NULL;

			if (pIOleObj->GetClientSite(&oldClientSite) == S_OK)
			{
				m_iClientSite->SetDefaultClientSite(oldClientSite);
				oldClientSite->Release();
			}

			pIOleObj->SetClientSite(m_iClientSite);
		}
	}

	_controller = new Controller();
	_controller->SetWebView(spTempWebBrowser);
	__hook(&Controller::DidLoadStoreEventHandler, _controller, &CPreview::DidLoadStore);
	__hook(&Controller::DidLoadPageEventHandler, _controller, &CPreview::DidLoadPage);
	__hook(&Controller::DidReceiveOrderEventHandler, _controller, &CPreview::DidReceiveOrder);
	__hook(&Controller::PropertyChangedEventHandler, _controller, &CPreview::PropertyChanged);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CPreview::DidLoadStore(const DidLoadStoreEventArgs &eventArgs)
{
	_receivedOrder = NULL;
	m_saveAsBtn.EnableWindow(FALSE);
	wstring type = L"@";
	type += Controller::PageTypeToString(FS);
	m_previewPageType = type.c_str();
	UpdateData(FALSE);
}

void CPreview::DidLoadPage(const DidLoadPageEventArgs &eventArgs)
{
	wstring type = L"@";
	type += Controller::PageTypeToString(eventArgs.GetPageType());
	m_previewPageType = type.c_str();
	UpdateData(FALSE);
}

void CPreview::DidReceiveOrder(const DidReceiveOrderEventArgs &eventArgs)
{
	_receivedOrder = eventArgs.GetOrder();
	m_saveAsBtn.EnableWindow(TRUE);
}

void CPreview::PropertyChanged(const PropertyChangedEventArgs &eventArgs)
{
	if (eventArgs.GetPropertyName() == L"IsLoading")
	{
		CWnd *pWnd = GetDlgItem(IDC_PROGRESS1);
		if (pWnd != NULL)
		{
			if (_controller->IsLoading())
			{
				pWnd->ShowWindow(SW_SHOW);
			}
			else
			{
				pWnd->ShowWindow(SW_HIDE);
			}
		}
	}
	else if (eventArgs.GetPropertyName() == L"IsSecure")
	{
		CWnd *pWnd = GetDlgItem(IDC_LOCK);
		if (pWnd != NULL)
		{
			if (_controller->IsSecure())
			{
				pWnd->ShowWindow(SW_SHOW);
			}
			else
			{
				pWnd->ShowWindow(SW_HIDE);
			}
		}
	}
}

void CPreview::OnSize(UINT nType, int cx, int cy)
{
	CPropertyPage::OnSize(nType, cx, cy);

	CWnd* pControl = (CWnd*)GetDlgItem(IDC_EXPLORER1);
	if (pControl != NULL)
	{
		// Move Save As button
		CWnd* pSaveAs = GetDlgItem(IDC_SAVEAS_BTN);
		CRect saveAsRect;
		pSaveAs->GetWindowRect(saveAsRect);
		ScreenToClient(saveAsRect);

		saveAsRect.MoveToXY(cx - saveAsRect.Width() - 1, cy - saveAsRect.Height() - 5);
		pSaveAs->MoveWindow(saveAsRect);

		// Move secure image
		CWnd* pSecure = GetDlgItem(IDC_LOCK);
		CRect secureRect;
		pSecure->GetWindowRect(secureRect);
		ScreenToClient(secureRect);

		secureRect.MoveToY(cy - 25);
		pSecure->MoveWindow(secureRect);

		// Move progress bar
		CWnd* pProgress = GetDlgItem(IDC_PROGRESS1);
		CRect progressRect;
		pProgress->GetWindowRect(progressRect);
		ScreenToClient(progressRect);

		progressRect.MoveToY(cy - 22);
		pProgress->MoveWindow(progressRect);

		// Move preview page type
		CWnd* pProductId = GetDlgItem(IDC_PREVIEW_PAGE_TYPE);
		CRect prdRect;
		pProductId->GetWindowRect(prdRect);
		ScreenToClient(prdRect);

		prdRect.MoveToXY((cx / 2) - (prdRect.Width() / 2), saveAsRect.top);
		pProductId->MoveWindow(prdRect);

		// Move WebBrowser control
		CRect wbRect;
		pControl->GetWindowRect(wbRect);
		ScreenToClient(wbRect);

		int wbOrigHeight = wbRect.Height();
		wbRect.right = cx - wbRect.left;
		wbRect.bottom = prdRect.top - 10;
		if (wbOrigHeight != wbRect.Height())
		{
			// Notify that the height has changed
			_controller->GetWebView()->put_Height(wbRect.Height());
		}

		pControl->MoveWindow(wbRect);

		// Resize the address bar
		CWnd* pUrl = GetDlgItem(IDC_URL_FIELD);
		CRect urlRect;
		pUrl->GetWindowRect(urlRect);
		ScreenToClient(urlRect);

		urlRect.right = wbRect.right;
		pUrl->MoveWindow(urlRect);
	}
}


BOOL CPreview::OnSetActive()
{
	m_UrlField = _params.ToURL().c_str();

	UpdateData(FALSE);

	_controller->LoadWithParameters(_params);

	return CPropertyPage::OnSetActive();
}

BEGIN_EVENTSINK_MAP(CPreview, CPropertyPage)
	ON_EVENT(CPreview, IDC_EXPLORER1, 108, CPreview::ProgressChangeExplorer1, VTS_I4 VTS_I4)
END_EVENTSINK_MAP()


void CPreview::ProgressChangeExplorer1(long Progress, long ProgressMax)
{
	if (Progress >= 0 && ProgressMax > 0)
	{
		m_browserProgress.SetRange(0, (short)ProgressMax);
		m_browserProgress.SetPos(Progress);
	}
}


void CPreview::OnDestroy()
{
	if (m_iClientSite != NULL)
	{
		m_iClientSite->Release();
		m_iClientSite = NULL;
	}

	CPropertyPage::OnDestroy();
}
