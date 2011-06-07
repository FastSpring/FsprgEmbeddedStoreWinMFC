// Controller.cpp : implementation file
//

#include "stdafx.h"
#include <Afxctl.h>
#include <afxinet.h>
#include <exdispid.h>
#include <mshtml.h>
#include "FsprgEmbeddedStore.h"
#include "Controller.h"
#include "Model\Order.h"
#include "Utilities\WebBrowserHelper.h"

using namespace FsprgEmbeddedStore;
using namespace Utilities::WebBrowser;

// Controller

IMPLEMENT_DYNAMIC(Controller, CCmdTarget)


Controller::Controller()
{
	EnableAutomation();
	CoInitialize(NULL);
}

Controller::~Controller()
{
	if (m_spWebBrowser) 
	{
		LPUNKNOWN pUnkSink = GetIDispatch(FALSE);
		AfxConnectionUnadvise(m_spWebBrowser, DIID_DWebBrowserEvents2, pUnkSink, FALSE, m_dwCookie);
	}
}

void Controller::LoadWithParameters(const StoreParameters &parameters)
{
	if (m_spWebBrowser)
	{
		_storeHost.empty();
		SetIsLoading(true);

		ExpandUserAgent("FSEmbeddedStore/1.0");
		ResetCookies();
		WebBrowserHelper::ClearCache();

		COleVariant url(parameters.ToURL().c_str());
		m_spWebBrowser->Navigate2(url, NULL, NULL, NULL, NULL);
	}
}

void Controller::LoadWithContentsOfFile(wstring path)
{
	if (m_spWebBrowser)
	{
		wstring tmpUrl = L"file://";
		tmpUrl += path;
		COleVariant url(tmpUrl.c_str());
		m_spWebBrowser->Navigate2(url, NULL, NULL, NULL, NULL);
	}
}

bool Controller::IsLoading()
{
	return _isLoading;
}

void Controller::SetIsLoading(bool isLoading)
{
	_isLoading = isLoading;
	__raise PropertyChangedEventHandler(PropertyChangedEventArgs(L"IsLoading"));
	__raise PropertyChangedEventHandler(PropertyChangedEventArgs(L"IsSecure"));
}

bool Controller::IsSecure()
{
	if (!IsLoading() && m_spWebBrowser)
	{
		CComBSTR url;
		HRESULT hr = m_spWebBrowser->get_LocationURL(&url);
		if (SUCCEEDED(hr) && url.Length() > 0
			&& wcsncmp(url, L"https", 5) == 0)
		{
			return true;
		}
	}

	return false;
}

wstring Controller::GetStoreHost()
{
	return _storeHost;
}

CComPtr<IWebBrowser2> Controller::GetWebView()
{
	return m_spWebBrowser;
}

void Controller::SetWebView(CComPtr<IWebBrowser2> webView) 
{
	LPUNKNOWN pUnkSink = GetIDispatch(FALSE);
	if (m_spWebBrowser) 
	{
		AfxConnectionUnadvise(m_spWebBrowser, DIID_DWebBrowserEvents2, pUnkSink, FALSE, m_dwCookie);
	}
	m_spWebBrowser = webView;
	// advise the browser to send events here
	AfxConnectionAdvise(m_spWebBrowser, DIID_DWebBrowserEvents2, pUnkSink, FALSE, &m_dwCookie); 
}

void Controller::SetWebView(LPDISPATCH pDisp)
{
	CComQIPtr<IWebBrowser2> spTempWebBrowser = pDisp;
	SetWebView(spTempWebBrowser);
}


BEGIN_MESSAGE_MAP(Controller, CCmdTarget)
END_MESSAGE_MAP()

// these are the events we want to monitor. 
// The events are explained below above their function implementations.
BEGIN_DISPATCH_MAP(Controller, CCmdTarget)
	DISP_FUNCTION_ID(Controller, "BeforeNavigate2",DISPID_BEFORENAVIGATE2,BeforeNavigate2,
					 VT_EMPTY, VTS_DISPATCH VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PBOOL)
	DISP_FUNCTION_ID(Controller, "DocumentComplete",DISPID_DOCUMENTCOMPLETE,DocumentComplete,
					 VT_EMPTY, VTS_DISPATCH VTS_PVARIANT)
	DISP_FUNCTION_ID(Controller, "WindowSetHeight", DISPID_WINDOWSETHEIGHT, WindowSetHeight,
					 VT_EMPTY, VTS_I4)
END_DISPATCH_MAP()

// Controller message handlers
// Fires before a navigation occurs in the given object 
void Controller::BeforeNavigate2(LPDISPATCH pDisp, VARIANT FAR *url, VARIANT FAR *Flags, 
			VARIANT FAR *TargetFrameName, VARIANT FAR *PostData, VARIANT FAR *Headers, VARIANT_BOOL* Cancel)
{
	SetIsLoading(true);
}

void Controller::WindowSetHeight(long height) 
{
	AdjustResizableContent(height);
}

// Fires when the document that is being navigated to reaches the READYSTATE_COMPLETE state
void Controller::DocumentComplete(IDispatch *pDisp, VARIANT *URL)
{
	HRESULT hr = S_OK;

    // Query for the IWebBrowser2 interface.
    CComQIPtr<IWebBrowser2> spTempWebBrowser = pDisp;

	// Is this event associated with the top-level browser?
	if (URL->bstrVal != NULL && spTempWebBrowser && m_spWebBrowser 
		&& m_spWebBrowser.IsEqualObject(spTempWebBrowser))
    {
		SetIsLoading(false);

		wstring newStoreHost;
		long height;
		hr = m_spWebBrowser->get_Height(&height);
		if (SUCCEEDED(hr))
		{
			AdjustResizableContent(height);
		}

		wstring newUri = URL->bstrVal;
		if (_wcsnicmp(URL->bstrVal, L"file", 4) == 0)
		{
			newStoreHost = L"file";
		}
		else
		{
			BOOL bDidItParse;
			DWORD dwServiceType;
			CString strServer;
			CString strObject;
			INTERNET_PORT nPort;
			CString strUserid;
			CString strPasswd;
			// This is the critical difference here: flags tell it how to convert weird characters.
			DWORD dwFlags = ICU_BROWSER_MODE; 

			CString url(*URL);
			bDidItParse = AfxParseURLEx(url, dwServiceType, strServer, strObject,
				nPort, strUserid, strPasswd, dwFlags);

			if (bDidItParse)
			{
				newStoreHost = strServer;
			}
			else
			{
				return;
			}
		}

		if (_storeHost.length() == 0) 
		{
			_storeHost = newStoreHost;
			__raise DidLoadStoreEventHandler(DidLoadStoreEventArgs(newUri));
		}
		else
		{
			PageType newPageType;
			if (_wcsicmp(newStoreHost.c_str(), _storeHost.c_str()) == 0) 
			{
				newPageType = FS;
			}
			else
			{
				size_t pos = newStoreHost.rfind(L"paypal.com");
				if (pos != wstring::npos)
				{
					newPageType = PayPal;
				}
				else
				{
					newPageType = Unknown;
				}
			}
			__raise DidLoadPageEventHandler(DidLoadPageEventArgs(newUri, newPageType));
		}

		// Get the current document object from browser...
        CComPtr<IDispatch> spDispDoc;
        hr = m_spWebBrowser->get_Document(&spDispDoc);
        if (SUCCEEDED(hr) && spDispDoc)
        {
			CComQIPtr<IHTMLDocument3> spHTMLDoc3 = spDispDoc;
			if (spHTMLDoc3)
			{
				CComPtr<IHTMLElement> spDocElem;
				hr = spHTMLDoc3->get_documentElement(&spDocElem);
				if (SUCCEEDED(hr) && spDocElem)
				{
					CComBSTR bstrInnerText;
					hr = spDocElem->get_innerText(&bstrInnerText);
					if (SUCCEEDED(hr)) 
					{
						wstring innerText = bstrInnerText;
						wstring match = L"<!DOCTYPE plist (View Source for full doctype...)>";
						size_t n = innerText.find(match);
						if (n != wstring::npos)
						{
							innerText.replace(n, match.length(), L"");
						}
						n = 0;
						while ((n = innerText.find(L"\r\n-", n)) != wstring::npos)
						{
							innerText.replace(n, 3, L"");
							n -= 3;
						}
						n = innerText.find(L"<?xml version=");
						if (n != wstring::npos)
						{
							innerText = innerText.substr(n);
							Order *order = Order::Parse(innerText);
							if (order != NULL)
							{
								__raise DidReceiveOrderEventHandler(DidReceiveOrderEventArgs(order));
							}
						}
					}
				}
			}
		}
    }
}

void Controller::ExpandUserAgent(string newSuffix)
{
	char userAgent[512];
	DWORD returnLength;
	HRESULT hr = UrlMkGetSessionOption(URLMON_OPTION_USERAGENT, userAgent, 512, &returnLength, 0);
	if (returnLength < 512)
	{
		string newUserAgent = userAgent;
		size_t n = newUserAgent.find(newSuffix);
		if (n == string::npos)
		{
			newUserAgent += " ";
			newUserAgent += newSuffix;

			UrlMkSetSessionOption(URLMON_OPTION_USERAGENT, (LPVOID)newUserAgent.c_str(), newUserAgent.length(), 0);
		}
	}
}

void Controller::ResetCookies()
{
	CComPtr<IDispatch> spDispDoc;
    HRESULT hr = m_spWebBrowser->get_Document(&spDispDoc);
    if (SUCCEEDED(hr) && spDispDoc != NULL)
    {
		CComQIPtr<IHTMLDocument2> spHTMLDoc2 = spDispDoc;
		if (spHTMLDoc2 != NULL)
		{
			CComBSTR bstrCookie;
			hr = spHTMLDoc2->get_cookie(&bstrCookie);
			if (SUCCEEDED(hr) && bstrCookie != NULL)
			{
				wstring hostname;
				wstring pathname;

				CComPtr<IHTMLLocation> spLocation;
				hr = spHTMLDoc2->get_location(&spLocation);
				if (SUCCEEDED(hr) && spLocation != NULL)
				{
					CComBSTR bstrHostname;
					hr = spLocation->get_hostname(&bstrHostname);
					if (SUCCEEDED(hr))
					{
						hostname = bstrHostname;
					}

					CComBSTR bstrPathname;
					hr = spLocation->get_pathname(&bstrPathname);
					if (SUCCEEDED(hr))
					{
						pathname = bstrPathname;
					}
				}

				wstring cookie = bstrCookie;
				wchar_t newCookie[4096];
				wchar_t *c = new wchar_t[cookie.length() + 1];
				wcscpy_s(c, cookie.length() + 1, cookie.c_str());
				wchar_t *pch, *next_token;
				pch = wcstok_s(c, L";", &next_token);
				// Go through each cookie
				while (pch != NULL)
				{
					wstring domain = hostname;
					
					while (domain.length() > 0)
					{
						wstring path = pathname;
						while (path.length() > 0)
						{
							swprintf(newCookie, 4096, L"%s; domain=%s; path=%s; expires=Thu, 2 Aug 2001 20:47:11 UTC", pch, domain.c_str(), path.c_str());
							spHTMLDoc2->put_cookie(CComBSTR(newCookie));

							path = path.substr(0, path.rfind('/'));
						}

						size_t dothIdx = domain.find('.');
						domain = dothIdx == wstring::npos ? L"" : domain.substr(dothIdx + 1);
					}
					
					pch = wcstok_s(NULL, L";", &next_token);
				}
				delete [] c;
			}
		}
	}
}

void Controller::AdjustResizableContent(long browserWindowHeightPx)
{
	CComPtr<IDispatch> spDispDoc;
    HRESULT hr = m_spWebBrowser->get_Document(&spDispDoc);
    if (!SUCCEEDED(hr) || !spDispDoc)
    {
		return;
	}

	CComQIPtr<IHTMLDocument3> spHTMLDoc3 = spDispDoc;
	if (!spHTMLDoc3)
	{
		return;
	}

	CComPtr<IHTMLElement> resizableContentE;
	hr = spHTMLDoc3->getElementById(CComBSTR(L"FsprgResizableContent"), &resizableContentE);
	if (!SUCCEEDED(hr) || !resizableContentE)
	{
		return;
	}

	CComPtr<IHTMLElement> storePageNavigationE;
	CComPtr<IHTMLElementCollection> divEs;
	hr = spHTMLDoc3->getElementsByTagName(CComBSTR(L"div"), &divEs);
	if (SUCCEEDED(hr) && divEs != NULL)
	{
		long length;
		divEs->get_length(&length);
		int i;
		for (i = 0; i < length; i++)
		{
			IDispatch *pDisp;
			hr = divEs->item(CComVariant(), CComVariant(i), &pDisp);
			if (SUCCEEDED(hr) && pDisp != NULL)
			{
				CComQIPtr<IHTMLElement> divE = pDisp;
				if (divE != NULL)
				{
					CComBSTR bstrClassname;
					hr = divE->get_className(&bstrClassname);
					if (SUCCEEDED(hr) && bstrClassname != NULL)
					{
						wstring classname = bstrClassname;
						if (classname == L"store-page-navigation")
						{
							storePageNavigationE = divE;
							break;
						}
					}
				}
			}
		}
	}

	if (!storePageNavigationE)
	{
		return;
	}

	int paddingTopPx, paddingBottomPx;
	CComVariant attr;
	hr = resizableContentE->getAttribute(CComBSTR(L"currentStyle"), 0, &attr);
	if (SUCCEEDED(hr) && attr.pdispVal != NULL) 
	{
		CComQIPtr<IHTMLCurrentStyle> spHTMLStyle = attr.pdispVal;
		if (spHTMLStyle != NULL)
		{
			CComVariant variant;
			hr = spHTMLStyle->get_paddingTop(&variant);
			if (SUCCEEDED(hr) && variant.bstrVal != NULL) 
			{
				wstring paddingTop = variant.bstrVal;
				paddingTopPx = _wtoi(paddingTop.c_str());
			}

			hr = spHTMLStyle->get_paddingBottom(&variant);
			if (SUCCEEDED(hr) && variant.bstrVal != NULL)
			{
				wstring paddingBottom = variant.bstrVal;
				paddingBottomPx = _wtoi(paddingBottom.c_str());
			}
		}
	}

	int storePageNavigationHeightPx = 0;
	hr = storePageNavigationE->getAttribute(CComBSTR(L"clientHeight"), 0, &attr);
	if (SUCCEEDED(hr)) 
	{
		storePageNavigationHeightPx = attr.intVal;
	}

	int newHeight = browserWindowHeightPx - paddingTopPx - paddingBottomPx - storePageNavigationHeightPx;
    if (newHeight < 0) 
	{
        newHeight = 0;
    }

	CComPtr<IHTMLStyle> style;
	hr = storePageNavigationE->get_style(&style);
	if (SUCCEEDED(hr) && style != NULL) 
	{
		style->put_height(CComVariant(newHeight));
	}
	
}

wstring Controller::PageTypeToString(PageType type)
{
	switch (type)
	{
	case FS: return L"FS";
	case PayPal: return L"PayPal";
	case Unknown: return L"Unknown";
	}

	return L"";
}