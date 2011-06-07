#pragma once
#include "PlistDict.h"

namespace FsprgEmbeddedStore
{
	namespace Model
	{

		class __declspec(dllexport) FsprgLicense
		{
		public:
			FsprgLicense(PlistDict*);
			~FsprgLicense(void);

			PlistDict* GetRaw();
			wstring GetLicenseName();
			wstring GetLicenseEmail();
			wstring GetLicenseCompany();
			wstring GetFirstLicenseCode();
			vector<wstring> GetLicenseCodes();
			PlistDict* GetLicensePropertyList();
			wstring GetLicenseURL();

		private:
			PlistDict *_raw;
		};

	};
};