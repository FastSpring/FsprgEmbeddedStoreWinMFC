#pragma once
#include <map>
#include <string>
#include <msxml6.h>
#include "plistdict.h"

using namespace std;

namespace FsprgEmbeddedStore
{
	namespace Model
	{

		class __declspec(dllexport) Plist :
			public PlistDict
		{
		public:
			Plist(void);
			~Plist(void);

			wstring GetOriginalDoc() { return _originalDoc; }

			static Plist* Parse(wstring plistXml);
			static Plist* Parse(CComPtr<IXMLDOMDocument>);

		private:
			wstring _originalDoc;

			Plist(wstring originalDoc, DictType*);
			static DictType* ParseDict(CComPtr<IXMLDOMNode>);
			static PlistDictType ParseElement(CComPtr<IXMLDOMNode>);
			static vector<PlistDictType>* ParseArray(CComPtr<IXMLDOMNode>);
		};

	};
};
