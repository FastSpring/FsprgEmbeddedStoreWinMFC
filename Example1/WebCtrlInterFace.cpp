#include "StdAfx.h"
#include "WebCtrlInterFace.h"


CWebCtrlInterFace::CWebCtrlInterFace(void)
{
	m_defaultClientSite			= NULL;
	m_defaultDocHostUIHandler	= NULL;
	m_cRef						= 0;
}


CWebCtrlInterFace::~CWebCtrlInterFace(void)
{
	//We release our default interfaces
	this->SetDefaultClientSite(NULL);
}

VOID CWebCtrlInterFace::SetDefaultClientSite(IOleClientSite *pClientSite)
{
	if (pClientSite != NULL)
	{
		pClientSite->AddRef();

		m_defaultClientSite = pClientSite;
		m_defaultClientSite->QueryInterface(IID_IDocHostUIHandler, (VOID **)&m_defaultDocHostUIHandler);
	}
	else
	{
		if (m_defaultClientSite != NULL)
		{
			m_defaultClientSite->Release();
			m_defaultClientSite = NULL;
		}

		if (m_defaultDocHostUIHandler != NULL)
		{
			m_defaultDocHostUIHandler->Release();
			m_defaultDocHostUIHandler = NULL;
		}
	}
}

// *** IUnknown ***

HRESULT STDMETHODCALLTYPE CWebCtrlInterFace::QueryInterface(REFIID riid, LPVOID *ppv) 
{
HRESULT result = S_OK;

	// Always set out parameter to NULL, validating it first 
	if (IsBadWritePtr(ppv, sizeof(LPVOID))) 
		result = E_INVALIDARG;  

	if (result == S_OK)
	{
		*ppv = NULL; 
 
		if ( IsEqualIID( riid, IID_IUnknown ) )
			*ppv = this;
		else if ( IsEqualIID( riid, IID_IOleClientSite ) )
			*ppv = (IOleClientSite *) this;
		else if ( IsEqualIID( riid, IID_IDocHostUIHandler ) )
			*ppv = (IDocHostUIHandler *) this;
		else
			result = E_NOINTERFACE;
	}

    if (result == S_OK)
        this->AddRef(); 
 
    return result; 
}

ULONG STDMETHODCALLTYPE CWebCtrlInterFace::AddRef() 
{    
    InterlockedIncrement(&m_cRef); 
    return m_cRef; 
} 
 
ULONG STDMETHODCALLTYPE CWebCtrlInterFace::Release() 
{ 
    // Decrement the object's internal counter 
    ULONG ulRefCount = InterlockedDecrement(&m_cRef); 
 
    if (0 == m_cRef) 
    {
        delete this; 
    }
 
    return ulRefCount; 
} 

// *** IOleClientSite ***

HRESULT STDMETHODCALLTYPE CWebCtrlInterFace::SaveObject()
{
	if (m_defaultClientSite != NULL)
		return m_defaultClientSite->SaveObject();
	else
		return E_FAIL;
}

HRESULT STDMETHODCALLTYPE CWebCtrlInterFace::GetMoniker(DWORD dwAssign, 
										DWORD dwWhichMoniker, LPMONIKER *ppmk)
{
	if (m_defaultClientSite != NULL)
		return m_defaultClientSite->GetMoniker(dwAssign, dwWhichMoniker, ppmk);
	else
	{
		if (! IsBadWritePtr(ppmk, sizeof(*ppmk)))
			*ppmk = NULL;

		return E_NOTIMPL;
	}
}

HRESULT STDMETHODCALLTYPE CWebCtrlInterFace::GetContainer(LPOLECONTAINER *ppContainer)
{	
	if (m_defaultClientSite != NULL)
		return m_defaultClientSite->GetContainer(ppContainer);
	else
	{
		if (! IsBadWritePtr(ppContainer, sizeof(*ppContainer)))
			*ppContainer = NULL;

		return E_NOINTERFACE;
	}
}

HRESULT STDMETHODCALLTYPE CWebCtrlInterFace::ShowObject()
{
	if (m_defaultClientSite != NULL)
		return m_defaultClientSite->ShowObject();
	else
		return S_OK;
}

HRESULT STDMETHODCALLTYPE CWebCtrlInterFace::OnShowWindow(BOOL fShow)
{
	if (m_defaultClientSite != NULL)
		return m_defaultClientSite->OnShowWindow(fShow);
	else
		return S_OK;
}

HRESULT STDMETHODCALLTYPE CWebCtrlInterFace::RequestNewObjectLayout()
{
	if (m_defaultClientSite != NULL)
		return m_defaultClientSite->RequestNewObjectLayout();
	else
		return E_NOTIMPL;
}

// *** IDocHostUIHandler ***

HRESULT STDMETHODCALLTYPE CWebCtrlInterFace::ShowContextMenu(DWORD dwID, POINT *ppt, 
					IUnknown *pcmdtReserved, IDispatch *pdispReserved)
{
	if (m_defaultDocHostUIHandler != NULL)
		return m_defaultDocHostUIHandler->ShowContextMenu(dwID, ppt, pcmdtReserved, pdispReserved);
	else
		return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CWebCtrlInterFace::GetHostInfo(DOCHOSTUIINFO *pInfo)
{
	if (m_defaultDocHostUIHandler != NULL)
	{
		HRESULT hr = m_defaultDocHostUIHandler->GetHostInfo(pInfo);
		pInfo->dwFlags = DOCHOSTUIFLAG_NO3DBORDER;
		return S_OK;
	}
	else
	{
		pInfo->dwFlags = DOCHOSTUIFLAG_NO3DBORDER;
		return S_OK;
	}
}

HRESULT STDMETHODCALLTYPE CWebCtrlInterFace::ShowUI(DWORD dwID, 
			IOleInPlaceActiveObject *pActiveObject, IOleCommandTarget *pCommandTarget,
				IOleInPlaceFrame *pFrame, IOleInPlaceUIWindow *pDoc)
{
	if (m_defaultDocHostUIHandler != NULL)
		return m_defaultDocHostUIHandler->ShowUI(dwID, pActiveObject, 
						pCommandTarget, pFrame, pDoc);
	else
		return S_FALSE;
}

HRESULT STDMETHODCALLTYPE CWebCtrlInterFace::HideUI()
{
	if (m_defaultDocHostUIHandler != NULL)
		return m_defaultDocHostUIHandler->HideUI();
	else
		return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CWebCtrlInterFace::UpdateUI()
{
	if (m_defaultDocHostUIHandler != NULL)
		return m_defaultDocHostUIHandler->UpdateUI();
	else
		return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CWebCtrlInterFace::EnableModeless(BOOL fEnable)
{
	if (m_defaultDocHostUIHandler != NULL)
		return m_defaultDocHostUIHandler->EnableModeless(fEnable);
	else
		return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CWebCtrlInterFace::OnDocWindowActivate(BOOL fActivate)
{
	if (m_defaultDocHostUIHandler != NULL)
		return m_defaultDocHostUIHandler->OnDocWindowActivate(fActivate);
	else
		return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CWebCtrlInterFace::OnFrameWindowActivate(BOOL fActivate)
{
	if (m_defaultDocHostUIHandler != NULL)
		return m_defaultDocHostUIHandler->OnFrameWindowActivate(fActivate);
	else
		return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CWebCtrlInterFace::ResizeBorder(LPCRECT prcBorder,
			IOleInPlaceUIWindow __RPC_FAR *pUIWindow, BOOL fRameWindow)
{
	if (m_defaultDocHostUIHandler != NULL)
		return m_defaultDocHostUIHandler->ResizeBorder(prcBorder, pUIWindow, fRameWindow);
	else
		return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CWebCtrlInterFace::TranslateAccelerator(LPMSG lpMsg,
			const GUID *pguidCmdGroup, DWORD nCmdID)
{
	if (m_defaultDocHostUIHandler != NULL)
		return m_defaultDocHostUIHandler->TranslateAccelerator(lpMsg, pguidCmdGroup, nCmdID);
	else
		return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CWebCtrlInterFace::GetOptionKeyPath( 
						LPOLESTR *pchKey, DWORD dw)
{
	if (m_defaultDocHostUIHandler != NULL)
		return m_defaultDocHostUIHandler->GetOptionKeyPath(pchKey, dw);
	else
	{
		if (! IsBadWritePtr(pchKey, sizeof(*pchKey))) 
			*pchKey = NULL;

		return E_NOTIMPL;
	}
}

HRESULT STDMETHODCALLTYPE CWebCtrlInterFace::GetDropTarget( 
			IDropTarget *pDropTarget, IDropTarget **ppDropTarget)
{
	if (m_defaultDocHostUIHandler != NULL)
	{
		HRESULT result = m_defaultDocHostUIHandler->GetDropTarget(pDropTarget, ppDropTarget);

		//Returning S_FALSE seems to disable DragNDrop, while DragNDrop is by default on.
		//Changing return code to E_FAIL seems to fix things. 
		if (result == S_FALSE)
			result = E_FAIL;

		return result;
	}
	else
	{
		if (! IsBadWritePtr(ppDropTarget, sizeof(*ppDropTarget))) 
			*ppDropTarget = NULL;

		return E_NOTIMPL;
	}
}

HRESULT STDMETHODCALLTYPE CWebCtrlInterFace::GetExternal(IDispatch **ppDispatch)
{
	if (m_defaultDocHostUIHandler != NULL)
		return m_defaultDocHostUIHandler->GetExternal(ppDispatch);
	else
	{
		if (! IsBadWritePtr(ppDispatch, sizeof(*ppDispatch))) 
			*ppDispatch = NULL;

		return E_NOTIMPL;
	}
}

HRESULT STDMETHODCALLTYPE CWebCtrlInterFace::TranslateUrl( 
			DWORD dwTranslate, OLECHAR *pchURLIn, OLECHAR **ppchURLOut)
{
	if (m_defaultDocHostUIHandler != NULL)
		return m_defaultDocHostUIHandler->TranslateUrl(dwTranslate, pchURLIn, ppchURLOut);
	else
	{
		if (! IsBadWritePtr(ppchURLOut, sizeof(*ppchURLOut))) 
			*ppchURLOut = NULL;

		return S_FALSE;
	}
}

HRESULT STDMETHODCALLTYPE CWebCtrlInterFace::FilterDataObject( 
			IDataObject *pDO, IDataObject  **ppDORet)
{
	if (m_defaultDocHostUIHandler != NULL)
		return m_defaultDocHostUIHandler->FilterDataObject(pDO, ppDORet);
	else
	{
		if (! IsBadWritePtr(ppDORet, sizeof(*ppDORet))) 
			*ppDORet = NULL;

		return S_FALSE;
	}
}
