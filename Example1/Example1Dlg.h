
// Example1Dlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "afxlinkctrl.h"
#include "Controller.h"
#include "Model\Order.h"
#include "WebCtrlInterFace.h"

using namespace FsprgEmbeddedStore;

// CExample1Dlg dialog
class CExample1Dlg : public CDialogEx
{
// Construction
public:
	CExample1Dlg(CWnd* pParent = NULL);	// standard constructor
	~CExample1Dlg(void);

	void DidLoadStore(const DidLoadStoreEventArgs&);
	void DidReceiveOrder(const DidReceiveOrderEventArgs&);
	void PropertyChanged(const PropertyChangedEventArgs&);
	
// Dialog Data
	enum { IDD = IDD_EXAMPLE1_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

private:
	CWebCtrlInterFace *m_iClientSite;
	Controller _controller;
	StoreParameters _parameters;
	Order *_receivedOrder;

	void ShowBrowser();
	void ShowConfirmation();
	void BlankPage();

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	DECLARE_EVENTSINK_MAP()
	void ProgressChangeExplorer1(long Progress, long ProgressMax);
	
	afx_msg void OnBnClickedLicenseBtn();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedReloadBtn();
	
	CProgressCtrl m_browserProgress;
	CString m_confirmationLabel1;
	CMFCLinkCtrl m_OpenBrowserLink;
	afx_msg void OnDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnStnClickedOpenBrowser();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
};
