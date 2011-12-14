#include "StdAfx.h"
#include "Plist.h"
#include <atlenc.h>

using namespace FsprgEmbeddedStore::Model;

Plist::Plist(void)
{
}

Plist::Plist(wstring originalDoc, DictType *dict) : PlistDict(dict), _originalDoc(originalDoc)
{

}

Plist::~Plist(void)
{
}

Plist* Plist::Parse(wstring plistXml)
{
	CComPtr<IXMLDOMDocument> spXMLDom;
	HRESULT hr = spXMLDom.CoCreateInstance(__uuidof(DOMDocument60));
	if (SUCCEEDED(hr) && spXMLDom)
	{
		VARIANT_BOOL success;
		hr = spXMLDom->loadXML(CComBSTR(plistXml.c_str()), &success);

		if (success)
		{
			return Parse(spXMLDom);
		}
	}

	return NULL;
}

Plist* Plist::Parse(CComPtr<IXMLDOMDocument> spListDoc)
{
	if (spListDoc)
	{
		CComBSTR bstrOrderXml;
		spListDoc->get_xml(&bstrOrderXml);
		wstring orderXml = bstrOrderXml;
		
		CComPtr<IXMLDOMElement> plistE;
		HRESULT hr = spListDoc->get_documentElement(&plistE);
		if (FAILED(hr) || !plistE)
		{
			return NULL;
		}

		CComBSTR bstrLocalname;
		hr = plistE->get_tagName(&bstrLocalname);

		wstring localname = bstrLocalname;

		if (localname != Plist::PLIST)
		{
			return NULL;
		}

		CComPtr<IXMLDOMNodeList> spNodeList;
		hr = plistE->get_childNodes(&spNodeList);
		if (SUCCEEDED(hr) && spNodeList)
		{
			long childCount;
			spNodeList->get_length(&childCount);
			if (childCount != 1)
			{
				return NULL;
			}

			CComPtr<IXMLDOMNode> spDictE;
			hr = plistE->get_firstChild(&spDictE);
			if (SUCCEEDED(hr) && spDictE)
			{
				CComBSTR bstrNodeName;
				hr = spDictE->get_nodeName(&bstrNodeName);
				if (SUCCEEDED(hr))
				{
					wstring nodeName = bstrNodeName;
					if (nodeName != DICT)
					{
						return NULL;
					}

					return new Plist(orderXml, ParseDict(spDictE));
				}
			}
		}
	}

	return NULL;
}

DictType* Plist::ParseDict(CComPtr<IXMLDOMNode> spDictE)
{
	if (spDictE)
	{
		CComPtr<IXMLDOMNodeList> spNodeList;
		HRESULT hr = spDictE->get_childNodes(&spNodeList);
		if (SUCCEEDED(hr) && spNodeList)
		{
			DictType* dictionary = new DictType();

			CComPtr<IXMLDOMNode> spKeyE;
			hr = spNodeList->nextNode(&spKeyE);
			while (SUCCEEDED(hr) && spKeyE)
			{
				CComBSTR bstrNodeName;
				hr = spKeyE->get_nodeName(&bstrNodeName);
				wstring nodeName = bstrNodeName;
				if (nodeName != KEY)
				{
					return NULL;
				}
				CComPtr<IXMLDOMNode> spValueE;
				hr = spNodeList->nextNode(&spValueE);
				if (SUCCEEDED(hr) && spValueE)
				{
					CComBSTR bstrNodeText;
					hr = spKeyE->get_text(&bstrNodeText);
					wstring key = bstrNodeText;

					dictionary->insert(DictTypePair(key, ParseElement(spValueE)));
				}
				
				spKeyE = NULL;
				hr = spNodeList->nextNode(&spKeyE);
			}

			return dictionary;
		}
	}

	return NULL;
}

vector<PlistDictType>* Plist::ParseArray(CComPtr<IXMLDOMNode> spElement)
{
	if (spElement)
	{
		CComPtr<IXMLDOMNodeList> spNodeList;
		HRESULT hr = spElement->get_childNodes(&spNodeList);
		if (SUCCEEDED(hr) && spNodeList)
		{
			long childCount;
			spNodeList->get_length(&childCount);

			vector<PlistDictType> *list = new vector<PlistDictType>();

			int i = 0;
			CComPtr<IXMLDOMNode> spChildNode;
			hr = spNodeList->nextNode(&spChildNode);
			while (SUCCEEDED(hr) && spChildNode)
			{
				list->push_back(ParseElement(spChildNode));
				i++;
				spChildNode = NULL;
				hr = spNodeList->nextNode(&spChildNode);
			}

			return list;
		}
	}

	return NULL;
}

PlistDictType Plist::ParseElement(CComPtr<IXMLDOMNode> spElement)
{
	if (spElement)
	{
		CComBSTR bstrNodeName;
		HRESULT hr = spElement->get_nodeName(&bstrNodeName);
		if (SUCCEEDED(hr))
		{
			wstring type = bstrNodeName;
			CComBSTR bstrNodeText;
			hr = spElement->get_text(&bstrNodeText);
			if (SUCCEEDED(hr))
			{
				wstring text = bstrNodeText;

				if (type == INTEGER)
				{
					int *pInt = new int;
					*pInt = _wtoi(text.c_str());
					return PlistDictType(INTEGER, pInt);
				}
				else if (type == REAL)
				{
					float *pFloat = new float;
					*pFloat = _wtof(text.c_str());
					return PlistDictType(REAL, pFloat);
				}
				else if (type == STRING)
				{
					wstring *pStr = new wstring(text);
					return PlistDictType(STRING, pStr);
				}
				else if (type == DATE)
				{
					COleDateTime date;
					date.ParseDateTime(text.c_str());
					SYSTEMTIME *pSysTime = new SYSTEMTIME;
					date.GetAsSystemTime(*pSysTime);
					return PlistDictType(DATE, pSysTime);
				}
				else if (type == DATA)
				{
					int length = Base64DecodeGetRequiredLength(text.length());
					BYTE *dest = new BYTE[length];
					CW2A src(text.c_str());
					int reqLen = text.length();
					Base64Decode(src, text.length(), dest, &reqLen);

					FSDATA *fsdata = new FSDATA();
					fsdata->length = reqLen;
					fsdata->data = new BYTE[reqLen];
					memcpy(fsdata->data, dest, reqLen);

					delete [] dest;

					return PlistDictType(DATA, fsdata);
				}
				else if (type == ARRAY)
				{
					return PlistDictType(ARRAY, ParseArray(spElement));
				}
				else if (type == BTRUE)
				{
					bool *pBool = new bool;
					*pBool = true;
					return PlistDictType(BTRUE, pBool);
				}
				else if (type == BFALSE)
				{
					bool *pBool = new bool;
					*pBool = false;
					return PlistDictType(BFALSE, pBool);
				}
				else if (type == DICT)
				{
					return PlistDictType(DICT, new PlistDict(ParseDict(spElement)));
				}
			}
		}
	}

	return PlistDictType(L"", NULL);
}