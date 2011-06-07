
// TestApp.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "TestApp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTestAppApp

BEGIN_MESSAGE_MAP(CTestAppApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CTestAppApp construction

CTestAppApp::CTestAppApp()
{
}

CTestAppApp::~CTestAppApp()
{
	delete _sheet;
}


// The one and only CTestAppApp object

CTestAppApp theApp;


// CTestAppApp initialization

BOOL CTestAppApp::InitInstance()
{
	CoInitialize(NULL);

	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// Create the shell manager, in case the dialog contains
	// any shell tree view or shell list view controls.
	//CShellManager *pShellManager = new CShellManager;

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	_sheet = new CTestAppSheet(L"FsprgEmbeddedStore TestApp");
	ManageAppConfig(_sheet->_params);
	_sheet->Create(NULL, WS_THICKFRAME | WS_POPUP | WS_VISIBLE | WS_CAPTION | WS_SYSMENU);
	_sheet->ShowWindow(SW_SHOW);
	m_pMainWnd = _sheet;

	return TRUE;
}



int CTestAppApp::ExitInstance()
{
	ManageAppConfig(_sheet->_params, true);
	
	return CWinApp::ExitInstance();
}


void CTestAppApp::ManageAppConfig(StoreParameters &params, bool update)
{
	CComPtr<IXMLDOMDocument> spXMLDom;
	HRESULT hr = spXMLDom.CoCreateInstance(__uuidof(DOMDocument60));
	if (SUCCEEDED(hr) && spXMLDom)
	{
		VARIANT_BOOL success;
		CComVariant xmlFile = L"app.config";
		hr = spXMLDom->load(xmlFile, &success);
		if (SUCCEEDED(hr))
		{
			CComPtr<IXMLDOMNodeList> spSettingNodes;
			hr = spXMLDom->getElementsByTagName(L"setting", &spSettingNodes);
			if (SUCCEEDED(hr) && spSettingNodes)
			{
				long count;
				spSettingNodes->get_length(&count);
				CComPtr<IXMLDOMNode> spSettingNode;
				while (SUCCEEDED(spSettingNodes->nextNode(&spSettingNode)) && spSettingNode)
				{
					wstring paramName;
					CComPtr<IXMLDOMNamedNodeMap> attributes;
					if (SUCCEEDED(spSettingNode->get_attributes(&attributes)))
					{
						CComPtr<IXMLDOMNode> attrNode;
						attributes->getNamedItem(CComBSTR(L"name"), &attrNode);
						CComVariant nodeValue;
						attrNode->get_nodeValue(&nodeValue);
						paramName = nodeValue.bstrVal;
					}
					CComPtr<IXMLDOMNode> valueNode;
					CComBSTR bstrNodeName;
					hr = spSettingNode->get_firstChild(&valueNode);
					if (SUCCEEDED(hr) && SUCCEEDED(valueNode->get_nodeName(&bstrNodeName)))
					{
						wstring nodeName = bstrNodeName;
						if (nodeName == L"value")
						{
							wstring nodeValue;
							CComBSTR bstrText;
							valueNode->get_text(&bstrText);
							nodeValue = bstrText;
							if (StoreParameters::LANGUAGE == paramName)
							{
								if (!update)
								{
									params.SetLanguage(nodeValue);
								}
								else
								{
									valueNode->put_text(CComBSTR(params.GetLanguage().c_str()));
								}
							}
							else if (StoreParameters::ORDER_PROCESS_TYPE == paramName)
							{
								if (!update)
								{
									OrderProcessType type = OrderProcessType::Parse(nodeValue);
									params.SetOrderProcessType(type);
								}
								else
								{
									OrderProcessType type = params.GetOrderProcessType();
									if (type != OrderProcessType::Unknown)
									{
										valueNode->put_text(CComBSTR(type.ToString().c_str()));
									}
									else
									{
										valueNode->put_text(CComBSTR(L""));
									}
								}
							}
							else if (StoreParameters::STORE_ID == paramName)
							{
								if (!update)
								{
									params.SetStoreId(nodeValue);
								}
								else
								{
									valueNode->put_text(CComBSTR(params.GetStoreId().c_str()));
								}
							}
							else if (StoreParameters::PRODUCT_ID == paramName)
							{
								if (!update)
								{
									params.SetProductId(nodeValue);
								}
								else
								{
									valueNode->put_text(CComBSTR(params.GetProductId().c_str()));
								}
							}
							else if (StoreParameters::MODE == paramName)
							{
								if (!update)
								{
									const Mode &mode = Mode::Parse(nodeValue);
									params.SetMode(mode);
								}
								else
								{
									const Mode &mode = params.GetMode();
									if (mode != Mode::Unknown)
									{
										valueNode->put_text(CComBSTR(mode.ToString().c_str()));
									}
									else
									{
										valueNode->put_text(CComBSTR(L""));
									}
								}
							}
							else if (StoreParameters::CAMPAIGN == paramName)
							{
								if (!update)
								{
									params.SetCampaign(nodeValue);
								}
								else
								{
									valueNode->put_text(CComBSTR(params.GetCampaign().c_str()));
								}
							}
							else if (StoreParameters::OPTION == paramName)
							{
								if (!update)
								{
									params.SetOption(nodeValue);
								}
								else
								{
									valueNode->put_text(CComBSTR(params.GetOption().c_str()));
								}
							}
							else if (StoreParameters::REFERRER == paramName)
							{
								if (!update)
								{
									params.SetReferrer(nodeValue);
								}
								else
								{
									valueNode->put_text(CComBSTR(params.GetReferrer().c_str()));
								}
							}
							else if (StoreParameters::SOURCE == paramName)
							{
								if (!update)
								{
									params.SetSource(nodeValue);
								}
								else
								{
									valueNode->put_text(CComBSTR(params.GetSource().c_str()));
								}
							}
							else if (StoreParameters::CONTACT_FNAME == paramName)
							{
								if (!update)
								{
									params.SetContactFname(nodeValue);
								}
								else
								{
									valueNode->put_text(CComBSTR(params.GetContactFname().c_str()));
								}
							}
							else if (StoreParameters::CONTACT_LNAME == paramName)
							{
								if (!update)
								{
									params.SetContactLname(nodeValue);
								}
								else
								{
									valueNode->put_text(CComBSTR(params.GetContactLname().c_str()));
								}
							}
							else if (StoreParameters::CONTACT_EMAIL == paramName)
							{
								if (!update)
								{
									params.SetContactEmail(nodeValue);
								}
								else
								{
									valueNode->put_text(CComBSTR(params.GetContactEmail().c_str()));
								}
							}
							else if (StoreParameters::CONTACT_COMPANY == paramName)
							{
								if (!update)
								{
									params.SetContactCompany(nodeValue);
								}
								else
								{
									valueNode->put_text(CComBSTR(params.GetContactCompany().c_str()));
								}
							}
							else if (StoreParameters::CONTACT_PHONE == paramName)
							{
								if (!update)
								{
									params.SetContactPhone(nodeValue);
								}
								else
								{
									valueNode->put_text(CComBSTR(params.GetContactPhone().c_str()));
								}
							}
						}
					}

					spSettingNode = NULL;
				}
			}
		}
		
		if (update)
		{
			spXMLDom->save(xmlFile);
		}
	}
}
