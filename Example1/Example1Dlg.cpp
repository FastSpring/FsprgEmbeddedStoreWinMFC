
// Example1Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "Example1.h"
#include "Example1Dlg.h"
#include "afxdialogex.h"
#include "StoreParameters.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CExample1Dlg dialog


CExample1Dlg::CExample1Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CExample1Dlg::IDD, pParent)
	, _receivedOrder(NULL)
	, m_confirmationLabel1(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CExample1Dlg::~CExample1Dlg(void)
{
	__unhook(&Controller::DidLoadStoreEventHandler, &_controller, &CExample1Dlg::DidLoadStore);
	__unhook(&Controller::DidReceiveOrderEventHandler, &_controller, &CExample1Dlg::DidReceiveOrder);
	__unhook(&Controller::PropertyChangedEventHandler, &_controller, &CExample1Dlg::PropertyChanged);

	if (_receivedOrder != NULL)
	{
		delete _receivedOrder;
	}
}

void CExample1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_browserProgress);
	DDX_Text(pDX, IDC_CONFIRMATION_LABEL1, m_confirmationLabel1);
}

void CExample1Dlg::DidLoadStore(const DidLoadStoreEventArgs &eventArgs)
{
	_receivedOrder = NULL;

	ShowBrowser();
}

void CExample1Dlg::DidReceiveOrder(const DidReceiveOrderEventArgs &eventArgs)
{
	_receivedOrder = eventArgs.GetOrder();

	m_confirmationLabel1 = "Hi ";
	m_confirmationLabel1 += _receivedOrder->GetCustomerFirstName().c_str();

	UpdateData(FALSE);

	ShowConfirmation();
}

void CExample1Dlg::PropertyChanged(const PropertyChangedEventArgs &eventArgs)
{
	if (eventArgs.GetPropertyName() == L"IsLoading")
	{
		CWnd *pWnd = GetDlgItem(IDC_PROGRESS1);
		if (pWnd != NULL)
		{
			if (_controller.IsLoading())
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
			if (_controller.IsSecure())
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


BEGIN_MESSAGE_MAP(CExample1Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_LICENSE_BTN, &CExample1Dlg::OnBnClickedLicenseBtn)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_RELOAD_BTN, &CExample1Dlg::OnBnClickedReloadBtn)
	ON_WM_DESTROY()
	ON_STN_CLICKED(IDC_OPEN_BROWSER, &CExample1Dlg::OnStnClickedOpenBrowser)
	ON_WM_DRAWITEM()
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CExample1Dlg, CDialogEx)
	ON_EVENT(CExample1Dlg, IDC_EXPLORER1, 108, CExample1Dlg::ProgressChangeExplorer1, VTS_I4 VTS_I4)
END_EVENTSINK_MAP()

// CExample1Dlg message handlers

BOOL CExample1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	_parameters.SetOrderProcessType(OrderProcessType::Detail);
	_parameters.SetStoreId(L"brumple");
	_parameters.SetProductId(L"prd1");
	_parameters.SetMode(Mode::Test);
	// Get the current locale
	wchar_t lang[LOCALE_NAME_MAX_LENGTH];
	GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SISO639LANGNAME, lang, LOCALE_NAME_MAX_LENGTH);
	wstring isoLang = lang;
	isoLang += L"-";
	GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SISO3166CTRYNAME, lang, LOCALE_NAME_MAX_LENGTH);
	isoLang += lang;
	_parameters.SetLanguage(isoLang);

	// Get access to the IWebBrowser2 interface of the Web Browser control
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

	// configure controller
    _controller.SetWebView(spTempWebBrowser);
	__hook(&Controller::DidLoadStoreEventHandler, &_controller, &CExample1Dlg::DidLoadStore);
	__hook(&Controller::DidReceiveOrderEventHandler, &_controller, &CExample1Dlg::DidReceiveOrder);
	__hook(&Controller::PropertyChangedEventHandler, &_controller, &CExample1Dlg::PropertyChanged);

	// load store. The browser control must be visible for the DocumentComplete event
	// to be called which is required for the FsprgEmbeddedStore SDK.
	_controller.LoadWithParameters(_parameters);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CExample1Dlg::OnPaint()
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

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CExample1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CExample1Dlg::OnBnClickedLicenseBtn()
{
	wchar_t buffer[1024];
	swprintf(buffer, 1024, L"Name: %s\nEmail: %s", _receivedOrder->GetCustomerLastName().c_str(), _receivedOrder->GetCustomerEmail().c_str());
	MessageBox(buffer, L"Your License");
}


void CExample1Dlg::ProgressChangeExplorer1(long Progress, long ProgressMax)
{
	if (Progress >= 0 && ProgressMax > 0)
	{
		m_browserProgress.SetRange(0, ProgressMax);
		m_browserProgress.SetPos(Progress);
	}
}


void CExample1Dlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	CWnd* pControl = (CWnd*)GetDlgItem(IDC_EXPLORER1);
	if (pControl != NULL)
	{
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

		// Move WebBrowser control
		CRect wbRect;
		pControl->GetWindowRect(wbRect);
		ScreenToClient(wbRect);

		int wbOrigHeight = wbRect.Height();
		wbRect.right = cx - wbRect.left;
		wbRect.bottom = secureRect.top - 1;
		if (wbOrigHeight != wbRect.Height())
		{
			// Notify that the height has changed
			_controller.GetWebView()->put_Height(wbRect.Height());
		}

		pControl->MoveWindow(wbRect);
		GetDlgItem(IDC_RELOAD_COVER)->MoveWindow(wbRect);

		CWnd* pReload = GetDlgItem(IDC_RELOAD_BTN);
		CRect reloadRect;
		pReload->GetWindowRect(reloadRect);
		ScreenToClient(reloadRect);

		reloadRect.MoveToX(cx - reloadRect.Width() - 7);
		pReload->MoveWindow(reloadRect);

		CWnd* pLink = GetDlgItem(IDC_OPEN_BROWSER);
		CRect linkRect;
		pLink->GetWindowRect(linkRect);
		ScreenToClient(linkRect);

		linkRect.MoveToX(reloadRect.left - linkRect.Width() - 5);
		pLink->MoveWindow(linkRect);

		CWnd *pWnd = GetDlgItem(IDC_CONFIRMATION_LABEL1);
		CRect tmpRect;
		pWnd->GetWindowRect(tmpRect);
		ScreenToClient(tmpRect);

		tmpRect.MoveToX(cx / 2 - tmpRect.Width() / 2);
		pWnd->MoveWindow(tmpRect);

		pWnd = GetDlgItem(IDC_CONFIRMATION_LABEL2);
		pWnd->GetWindowRect(tmpRect);
		ScreenToClient(tmpRect);

		tmpRect.MoveToX(cx / 2 - tmpRect.Width() / 2);
		pWnd->MoveWindow(tmpRect);

		pWnd = GetDlgItem(IDC_LICENSE_BTN);
		pWnd->GetWindowRect(tmpRect);
		ScreenToClient(tmpRect);

		tmpRect.MoveToX(cx / 2 - tmpRect.Width() / 2);
		pWnd->MoveWindow(tmpRect);
	}
}

void CExample1Dlg::ShowBrowser()
{
	CWnd *pTmp = GetDlgItem(IDC_RELOAD_COVER);
	if (pTmp) pTmp->ShowWindow(SW_HIDE);
	pTmp = GetDlgItem(IDC_CONFIRMATION_LABEL1);
	if (pTmp) pTmp->ShowWindow(SW_HIDE);
	pTmp = GetDlgItem(IDC_CONFIRMATION_LABEL2);
	if (pTmp) pTmp->ShowWindow(SW_HIDE);
	pTmp = GetDlgItem(IDC_LICENSE_BTN);
	if (pTmp) pTmp->ShowWindow(SW_HIDE);
}

void CExample1Dlg::ShowConfirmation()
{
	BlankPage();

	CWnd *pCover = GetDlgItem(IDC_RELOAD_COVER);
	if (pCover != NULL)
	{
		CWnd *pTmp = GetDlgItem(IDC_CONFIRMATION_LABEL1);
		if (pTmp)
			pTmp->SetWindowPos(pCover, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
		pTmp = GetDlgItem(IDC_CONFIRMATION_LABEL2);
		if (pTmp)
			pTmp->SetWindowPos(pCover, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
		pTmp = GetDlgItem(IDC_LICENSE_BTN);
		if (pTmp)
			pTmp->SetWindowPos(pCover, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
	}
}

// The browser control must be visible for the DocumentComplete event
// to be called which is required for the FsprgEmbeddedStore SDK 
// (http://support.microsoft.com/kb/259935/en-us) This
// method will put a static control on top of all controls in the dialog.
void CExample1Dlg::BlankPage()
{
	CWnd *pControl = GetDlgItem(IDC_EXPLORER1);
	CWnd *pTmp = GetDlgItem(IDC_RELOAD_COVER);
	if (pControl && pTmp != NULL)
	{
		CRect wbRect;
		pControl->GetWindowRect(wbRect);
		ScreenToClient(wbRect);

		pTmp->SetWindowPos(&CWnd::wndTop, wbRect.left, wbRect.top, 
			wbRect.Width(), wbRect.Height(), SWP_SHOWWINDOW);
	}
}

void CExample1Dlg::OnBnClickedReloadBtn()
{
	// The browser control must be visible for the DocumentComplete event
	// to be called which is required for the FsprgEmbeddedStore SDK.
	BlankPage();
	
	_controller.LoadWithParameters(_parameters);
}


void CExample1Dlg::OnDestroy()
{
	if (m_iClientSite != NULL)
	{
		m_iClientSite->Release();
		m_iClientSite = NULL;
	}

	CDialogEx::OnDestroy();
}


BOOL CExample1Dlg::PreTranslateMessage(MSG* pMsg)
{
	// The browser control needs special message processing to
	// allow accelerator keys to be handled when hosted in a dialog
	if (pMsg->message == WM_KEYDOWN
		&& (pMsg->wParam == VK_TAB || pMsg->wParam == VK_DELETE || pMsg->wParam == VK_RETURN))
	{
		CComPtr<IWebBrowser2> spBrowser = _controller.GetWebView();

		if (spBrowser != NULL)
		{
			IOleInPlaceActiveObject* pIOIPAO;
			HRESULT hr = spBrowser->QueryInterface(IID_IOleInPlaceActiveObject, (void**)&pIOIPAO);
			if (SUCCEEDED(hr))
			{
				hr = pIOIPAO->TranslateAccelerator(pMsg);
				pIOIPAO->Release();
				return TRUE;
			}
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

// Need to create our own hyperlink that responds to a click to open the url in the browser
// and will draw itself as a hyperlink and set the cursor to the hand
void CExample1Dlg::OnStnClickedOpenBrowser()
{
	ShellExecuteW(NULL, L"open", _parameters.ToURL().c_str(), NULL, NULL, SW_SHOWNORMAL);
}

void CExample1Dlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if (nIDCtl == IDC_OPEN_BROWSER)
	{
		COLORREF blue = RGB(19, 100, 196);

		CDC dc;
		dc.Attach(lpDrawItemStruct->hDC);
		dc.SetTextColor(blue);

		CFont *font = dc.GetCurrentFont();
		
		LOGFONT lf;
		font->GetLogFont(&lf);
		lf.lfUnderline = 1;

		CFont underlineFont;
		underlineFont.CreateFontIndirectW(&lf);
		dc.SelectObject(&underlineFont);

		dc.TabbedTextOutW(0, 0, L"Open In Browser", 0, NULL, 0);
	}

	CDialogEx::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

BOOL CExample1Dlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (pWnd->GetDlgCtrlID() == IDC_OPEN_BROWSER)
	{
		HCURSOR newcursor = AfxGetApp()->LoadStandardCursor(IDC_HAND);
		SetCursor(newcursor);
		return TRUE;
	}

	return CDialogEx::OnSetCursor(pWnd, nHitTest, message);
}
