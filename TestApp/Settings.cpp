// Settings.cpp : implementation file
//

#include "stdafx.h"
#include "afxdialogex.h"
#include "TestApp.h"
#include "Settings.h"
#include "StoreParameters.h"

// CSettings dialog

IMPLEMENT_DYNAMIC(CSettings, CPropertyPage)

CSettings::CSettings(StoreParameters &params)
	: CPropertyPage(CSettings::IDD)
	, _params(params)
	, m_storeUrl(_T(""))
	, m_storeId(_T(""))
	, m_orderProcess(_T(""))
	, m_mode(_T(""))
	, m_productId(_T(""))
	, m_language(_T(""))
	, m_campaign(_T(""))
	, m_referrer(_T(""))
	, m_option(_T(""))
	, m_source(_T(""))
	, m_coupon(_T(""))
	, m_firstname(_T(""))
	, m_lastname(_T(""))
	, m_email(_T(""))
	, m_phone(_T(""))
	, m_company(_T(""))
{

}

CSettings::~CSettings()
{
}

void CSettings::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STORE_URL, m_storeUrl);
	DDX_Text(pDX, IDC_STOREID, m_storeId);
	DDX_CBString(pDX, IDC_ORDER_PROCESS, m_orderProcess);
	DDX_CBString(pDX, IDC_MODE, m_mode);
	DDX_Text(pDX, IDC_PRODUCTID, m_productId);
	DDX_Text(pDX, IDC_LANGUAGE, m_language);
	DDX_Text(pDX, IDC_CAMPAIGN, m_campaign);
	DDX_Text(pDX, IDC_REFERRER, m_referrer);
	DDX_Text(pDX, IDC_OPTION, m_option);
	DDX_Text(pDX, IDC_SOURCE, m_source);
	DDX_Text(pDX, IDC_COUPON, m_coupon);
	DDX_Text(pDX, IDC_FIRSTNAME, m_firstname);
	DDX_Text(pDX, IDC_LASTNAME, m_lastname);
	DDX_Text(pDX, IDC_EMAIL, m_email);
	DDX_Text(pDX, IDC_PHONE, m_phone);
	DDX_Text(pDX, IDC_COMPANY, m_company);
	DDX_Control(pDX, IDC_STORE_URL, m_storeUrlCntr);
	DDX_Control(pDX, IDC_URL_GRP, m_UrlGrp);
	DDX_Control(pDX, IDC_ORDER_PROCESS, m_orderProcessCtrl);
	DDX_Control(pDX, IDC_MODE, m_modeCtrl);
}


BEGIN_MESSAGE_MAP(CSettings, CPropertyPage)
	ON_CBN_SELCHANGE(IDC_ORDER_PROCESS, &CSettings::OnValueChanged)
	ON_EN_KILLFOCUS(IDC_STOREID, &CSettings::OnValueChanged)
	ON_CBN_SELCHANGE(IDC_MODE, &CSettings::OnValueChanged)
	ON_EN_KILLFOCUS(IDC_PRODUCTID, &CSettings::OnValueChanged)
	ON_EN_KILLFOCUS(IDC_LANGUAGE, &CSettings::OnValueChanged)
	ON_EN_KILLFOCUS(IDC_CAMPAIGN, &CSettings::OnValueChanged)
	ON_EN_KILLFOCUS(IDC_REFERRER, &CSettings::OnValueChanged)
	ON_EN_KILLFOCUS(IDC_OPTION, &CSettings::OnValueChanged)
	ON_EN_KILLFOCUS(IDC_SOURCE, &CSettings::OnValueChanged)
	ON_EN_KILLFOCUS(IDC_COUPON, &CSettings::OnValueChanged)
	ON_EN_KILLFOCUS(IDC_FIRSTNAME, &CSettings::OnValueChanged)
	ON_EN_KILLFOCUS(IDC_LASTNAME, &CSettings::OnValueChanged)
	ON_EN_KILLFOCUS(IDC_EMAIL, &CSettings::OnValueChanged)
	ON_EN_KILLFOCUS(IDC_PHONE, &CSettings::OnValueChanged)
	ON_EN_KILLFOCUS(IDC_COMPANY, &CSettings::OnValueChanged)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CSettings message handlers


void CSettings::OnValueChanged()
{
	UpdateData(TRUE);

	OrderProcessType type = OrderProcessType::Parse(m_orderProcess.GetString());
	if (type != OrderProcessType::Unknown)
	{
		_params.SetOrderProcessType(type);
	}
	_params.SetStoreId(m_storeId.GetString());
	const Mode &mode = Mode::Parse(m_mode.GetString());
	if (mode != Mode::Unknown)
	{
		_params.SetMode(mode);
	}
	_params.SetProductId(m_productId.GetString());
	_params.SetLanguage(m_language.GetString());
	_params.SetCampaign(m_campaign.GetString());
	_params.SetReferrer(m_referrer.GetString());
	_params.SetOption(m_option.GetString());
	_params.SetSource(m_source.GetString());
	_params.SetCoupon(m_coupon.GetString());
	_params.SetContactFname(m_firstname.GetString());
	_params.SetContactLname(m_lastname.GetString());
	_params.SetContactEmail(m_email.GetString());
	_params.SetContactPhone(m_phone.GetString());
	_params.SetContactCompany(m_company.GetString());

	UpdateStoreUrl();
}


void CSettings::UpdateStoreUrl()
{
	_url = _params.ToURL();
	
	WrapStoreUrl();
}


BOOL CSettings::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	vector<OrderProcessType> orderProcessTypes = OrderProcessType::GetAll();
	vector<OrderProcessType>::iterator opIter;
	for (opIter = orderProcessTypes.begin(); opIter != orderProcessTypes.end(); opIter++)
	{
		OrderProcessType type = *opIter;
		m_orderProcessCtrl.AddString(type.ToString().c_str());
	}
	
	vector<Mode> modes = Mode::GetAll();
	vector<Mode>::iterator mIter;
	for (mIter = modes.begin(); mIter != modes.end(); mIter++)
	{
		Mode mode = *mIter;
		m_modeCtrl.AddString(mode.ToString().c_str());
	}

	OrderProcessType type = _params.GetOrderProcessType();
	if (type != OrderProcessType::Unknown)
	{
		m_orderProcess = type.ToString().c_str();
	}
	m_storeId = _params.GetStoreId().c_str();
	const Mode mode = _params.GetMode();
	if (mode != Mode::Unknown)
	{
		m_mode = mode.ToString().c_str();
	}
	m_productId = _params.GetProductId().c_str();
	m_language = _params.GetLanguage().c_str();
	m_campaign = _params.GetCampaign().c_str();
	m_option = _params.GetOption().c_str();
	m_coupon = _params.GetCoupon().c_str();
	m_referrer = _params.GetReferrer().c_str();
	m_source = _params.GetSource().c_str();
	m_firstname = _params.GetContactFname().c_str();
	m_lastname = _params.GetContactLname().c_str();
	m_email = _params.GetContactEmail().c_str();
	m_company = _params.GetContactCompany().c_str();
	m_phone = _params.GetContactPhone().c_str();
	UpdateStoreUrl();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CSettings::OnSize(UINT nType, int cx, int cy)
{
	CPropertyPage::OnSize(nType, cx, cy);

	CWnd *pGrp = GetDlgItem(IDC_URL_GRP);
	if (pGrp != NULL)
	{
		CRect grpRect;
		pGrp->GetWindowRect(grpRect);
		ScreenToClient(grpRect);

		grpRect.right = cx - 7;
		grpRect.bottom = cy - 7;
		pGrp->MoveWindow(grpRect);

		CWnd *pUrl = GetDlgItem(IDC_STORE_URL);
		if (pUrl != NULL)
		{
			CRect urlRect;
			
			urlRect.left = grpRect.left + 10;
			urlRect.top = grpRect.top + 18;
			urlRect.right = grpRect.right - 10;
			urlRect.bottom = grpRect.bottom - 10;

			pUrl->MoveWindow(urlRect);

			WrapStoreUrl();
		}
	}
}

void CSettings::WrapStoreUrl()
{
	wstring formattedUrl = _url;
	
	CRect rect;
	m_storeUrlCntr.GetWindowRect(rect);
	ScreenToClient(rect);
	int cntrHeight = rect.Height();
	int cntrWidth = rect.Width();
	
	LPCWSTR pFUrl = formattedUrl.c_str();
	int istart = 0, iend = formattedUrl.length();
	wstring wrappedText;
	CClientDC dc(&m_storeUrlCntr);
	CFont *font = dc.SelectObject(m_storeUrlCntr.GetFont());
	CSize size = dc.GetTextExtent(pFUrl, formattedUrl.length());
	if (size.cx > cntrWidth)
	{
		for (int i = 0; i < formattedUrl.length(); i++)
		{
			size = dc.GetTextExtent(pFUrl + istart, i - istart + 1);
			if (size.cx >= cntrWidth - 5)
			{
				iend = i;
				wrappedText += formattedUrl.substr(istart, iend - istart);
				wrappedText += L"\r\n";
				istart = i;
			}
		}
	}
	dc.SelectObject(font);

	wrappedText += formattedUrl.substr(istart, iend);

	m_storeUrl = wrappedText.c_str();
	
	UpdateData(FALSE);
}
