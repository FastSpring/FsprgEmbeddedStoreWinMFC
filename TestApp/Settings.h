#pragma once
#include "StoreParameters.h"
#include "afxwin.h"

using namespace FsprgEmbeddedStore;

// CSettings dialog

class CSettings : public CPropertyPage
{
	DECLARE_DYNAMIC(CSettings)

public:
	CSettings(StoreParameters &params);
	virtual ~CSettings();

// Dialog Data
	enum { IDD = IDD_PROPPAGE_SETTINGS };

private:
	StoreParameters &_params;
	wstring _url;
	void UpdateStoreUrl();
	void WrapStoreUrl();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnValueChanged();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	
	CString m_storeUrl;
	CString m_storeId;
	CString m_orderProcess;
	CString m_mode;
	CString m_productId;
	CString m_language;
	CString m_campaign;
	CString m_referrer;
	CString m_option;
	CString m_source;
	CString m_coupon;
	CString m_firstname;
	CString m_lastname;
	CString m_email;
	CString m_phone;
	CString m_company;
	CString m_url;
	CStatic m_storeUrlCntr;
	CStatic m_UrlGrp;
	CComboBox m_orderProcessCtrl;
	CComboBox m_modeCtrl;
};
