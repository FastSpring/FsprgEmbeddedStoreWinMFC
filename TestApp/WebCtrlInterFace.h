#pragma once
#include "oleidl.h"
#include <mshtmhst.h>

class CWebCtrlInterFace :
	public IOleClientSite, public IDocHostUIHandler
{
public:
	CWebCtrlInterFace(void);
	virtual ~CWebCtrlInterFace(void);

	VOID SetDefaultClientSite(IOleClientSite *pClientSite);
	IOleClientSite *GetDefaultClientSite()
				{ return m_defaultClientSite; }
	IDocHostUIHandler *GetDefaultDocHostUIHandler()
				{ return m_defaultDocHostUIHandler; }

	VOID SetContextMenuMode(ULONG inMode);

// *** IUnknown ***
	STDMETHOD(QueryInterface)(
		/* [in] */ REFIID riid,
		/* [iid_is][out] */ VOID **ppvObject);

	STDMETHOD_(ULONG, AddRef)();
	STDMETHOD_(ULONG, Release)();

// *** IOleClientSite ***        
	STDMETHOD(SaveObject)();
	STDMETHOD(GetMoniker)(
		/* [in] */ DWORD dwAssign,
		/* [in] */ DWORD dwWhichMoniker,
		/* [out] */ IMoniker **ppmk);

	STDMETHOD(GetContainer)(
		/* [out] */ IOleContainer **ppContainer);

	STDMETHOD(ShowObject)();
	STDMETHOD(OnShowWindow)(
		/* [in] */ BOOL fShow);

	STDMETHOD(RequestNewObjectLayout)();

	// *** IDocHostUIHandler ***

	STDMETHOD(ShowContextMenu)( 
		/* [in] */ DWORD dwID,
		/* [in] */ POINT *ppt,
		/* [in] */ IUnknown *pcmdtReserved,
		/* [in] */ IDispatch *pdispReserved);
	STDMETHOD(GetHostInfo)( 
		/* [out][in] */ DOCHOSTUIINFO *pInfo);
	STDMETHOD(ShowUI)( 
		/* [in] */ DWORD dwID,
		/* [in] */ IOleInPlaceActiveObject *pActiveObject,
		/* [in] */ IOleCommandTarget *pCommandTarget,
		/* [in] */ IOleInPlaceFrame *pFrame,
		/* [in] */ IOleInPlaceUIWindow *pDoc);
	STDMETHOD(HideUI)();
	STDMETHOD(UpdateUI)();
	STDMETHOD(EnableModeless)( 
		/* [in] */ BOOL fEnable);
	STDMETHOD(OnDocWindowActivate)( 
		/* [in] */ BOOL fActivate);
	STDMETHOD(OnFrameWindowActivate)( 
		/* [in] */ BOOL fActivate);
	STDMETHOD(ResizeBorder)( 
		/* [in] */ LPCRECT prcBorder,
		/* [in] */ IOleInPlaceUIWindow *pUIWindow,
		/* [in] */ BOOL fRameWindow);
	STDMETHOD(TranslateAccelerator)( 
		/* [in] */ LPMSG lpMsg,
		/* [in] */ const GUID *pguidCmdGroup,
		/* [in] */ DWORD nCmdID);
	STDMETHOD(GetOptionKeyPath)( 
		/* [out] */ LPOLESTR *pchKey,
		/* [in] */ DWORD dw);
	STDMETHOD(GetDropTarget)( 
		/* [in] */ IDropTarget *pDropTarget,
		/* [out] */ IDropTarget **ppDropTarget);
	STDMETHOD(GetExternal)( 
		/* [out] */ IDispatch **ppDispatch);
	STDMETHOD(TranslateUrl)( 
		/* [in] */ DWORD dwTranslate,
		/* [in] */ OLECHAR *pchURLIn,
		/* [out] */ OLECHAR **ppchURLOut);
	STDMETHOD(FilterDataObject)( 
		/* [in] */ IDataObject *pDO,
		/* [out] */ IDataObject **ppDORet);

private:
	LONG				m_cRef;
	IOleClientSite		*m_defaultClientSite;
	IDocHostUIHandler	*m_defaultDocHostUIHandler;
};

