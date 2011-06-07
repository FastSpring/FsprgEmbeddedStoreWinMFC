
#pragma once
#include "StoreParameters.h"
#include "Model\Order.h"


namespace FsprgEmbeddedStore {
	using namespace Model;

	enum PageType {
		FS,
		PayPal,
		Unknown
	};

		
	class __declspec(dllexport) DidLoadStoreEventArgs
	{
	public:
		DidLoadStoreEventArgs(wstring uri) : _uri(uri) {}
		wstring GetUri() const { return _uri; }

	private:
		wstring _uri;
	};

	class __declspec(dllexport) DidLoadPageEventArgs
	{
	public:
		DidLoadPageEventArgs(wstring uri, PageType pageType) : _uri(uri), _pageType(pageType) {}

		wstring GetUri() const { return _uri; }
		PageType GetPageType() const { return _pageType; }

	private:
		wstring _uri;
		PageType _pageType;
	};

	class __declspec(dllexport) DidReceiveOrderEventArgs
	{
	public:
		DidReceiveOrderEventArgs(Order *order) : _order(order) {}

		Order* GetOrder() const { return _order; }
	private:
		Order *_order;
	};

	// Controller command target
	[event_source(native)]
	class __declspec(dllexport) Controller : public CCmdTarget
	{
		DECLARE_DYNAMIC(Controller)

	public:
		Controller();
		virtual ~Controller();

		CComPtr<IWebBrowser2> GetWebView();
		void SetWebView(CComPtr<IWebBrowser2>);
		void SetWebView(LPDISPATCH);

		void LoadWithParameters(const StoreParameters&);
		void LoadWithContentsOfFile(wstring);
		bool IsLoading();
		bool IsSecure();
		wstring GetStoreHost();

		__event void DidLoadStoreEventHandler(const DidLoadStoreEventArgs&);
		__event void DidLoadPageEventHandler(const DidLoadPageEventArgs&);
		__event void DidReceiveOrderEventHandler(const DidReceiveOrderEventArgs&);
		__event void PropertyChangedEventHandler(const PropertyChangedEventArgs&);

		static wstring PageTypeToString(PageType);

	private:
		DWORD m_dwCookie;
		CComPtr<IWebBrowser2> m_spWebBrowser;
		wstring _storeHost;
		bool _isLoading;
	
		void SetIsLoading(bool);
		void ExpandUserAgent(string);
		void ResetCookies();
		void AdjustResizableContent(long);

	// methods
	protected:

		// Fires before a navigation occurs in the given object 
		afx_msg void BeforeNavigate2(LPDISPATCH pDisp, VARIANT FAR *url, VARIANT FAR *Flags, VARIANT FAR *TargetFrameName, VARIANT FAR *PostData, VARIANT FAR *Headers, VARIANT_BOOL* Cancel);
		// Fires when the document that is being navigated to reaches the READYSTATE_COMPLETE state
		afx_msg void DocumentComplete(IDispatch *pDisp,VARIANT *URL);
		afx_msg void WindowSetHeight(long Height);

	protected:
		DECLARE_MESSAGE_MAP()
		DECLARE_DISPATCH_MAP()
	};


}