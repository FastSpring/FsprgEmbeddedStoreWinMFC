// FsprgEmbeddedStore.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "FsprgEmbeddedStore.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: If this DLL is dynamically linked against the MFC DLLs,
//		any functions exported from this DLL which call into
//		MFC must have the AFX_MANAGE_STATE macro added at the
//		very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

// CFsprgEmbeddedStoreApp

BEGIN_MESSAGE_MAP(CFsprgEmbeddedStoreApp, CWinApp)
END_MESSAGE_MAP()


// CFsprgEmbeddedStoreApp construction

CFsprgEmbeddedStoreApp::CFsprgEmbeddedStoreApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CFsprgEmbeddedStoreApp object

CFsprgEmbeddedStoreApp theApp;

const GUID CDECL _tlid = { 0x4BCF0A54, 0x6E0, 0x4BDA, { 0xAD, 0x5D, 0xA2, 0x42, 0x88, 0x31, 0x22, 0xFD } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;


// CFsprgEmbeddedStoreApp initialization

BOOL CFsprgEmbeddedStoreApp::InitInstance()
{
	CWinApp::InitInstance();

	// Register all OLE server (factories) as running.  This enables the
	//  OLE libraries to create objects from other applications.
	COleObjectFactory::RegisterAll();

	return TRUE;
}

// DllGetClassObject - Returns class factory

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AfxDllGetClassObject(rclsid, riid, ppv);
}


// DllCanUnloadNow - Allows COM to unload DLL

STDAPI DllCanUnloadNow(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AfxDllCanUnloadNow();
}


// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return SELFREG_E_TYPELIB;

	if (!COleObjectFactory::UpdateRegistryAll())
		return SELFREG_E_CLASS;

	return S_OK;
}


// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return SELFREG_E_TYPELIB;

	if (!COleObjectFactory::UpdateRegistryAll(FALSE))
		return SELFREG_E_CLASS;

	return S_OK;
}
