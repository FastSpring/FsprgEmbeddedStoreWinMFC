#pragma once
#include "Controller.h"
#include "Model\Order.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "WebCtrlInterFace.h"

using namespace FsprgEmbeddedStore;

// CPreview dialog
[event_receiver(native)]
class CPreview : public CPropertyPage
{
	DECLARE_DYNAMIC(CPreview)

public:
	CPreview(StoreParameters&);
	virtual ~CPreview();

	void DidLoadStore(const DidLoadStoreEventArgs&);
	void DidLoadPage(const DidLoadPageEventArgs&);
	void DidReceiveOrder(const DidReceiveOrderEventArgs&);
	void PropertyChanged(const PropertyChangedEventArgs&);

// Dialog Data
	enum { IDD = IDD_PROPPAGE_PREVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_previewPageType;
	CString m_UrlField;
	afx_msg void OnBnClickedSaveasBtn();
	afx_msg void OnBnClickedReloadBtn();
	virtual BOOL OnInitDialog();

private:
	CWebCtrlInterFace *m_iClientSite;
	Controller *_controller;
	StoreParameters &_params;
	Order *_receivedOrder;
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnSetActive();
	CButton m_saveAsBtn;
	DECLARE_EVENTSINK_MAP()
	void ProgressChangeExplorer1(long Progress, long ProgressMax);
	CProgressCtrl m_browserProgress;
	afx_msg void OnDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
