#include "StdAfx.h"
#include "FsprgLicense.h"

using namespace FsprgEmbeddedStore::Model;

FsprgLicense::FsprgLicense(PlistDict *dict) : _raw(dict)
{
}


FsprgLicense::~FsprgLicense(void)
{
	if (_raw != NULL)
	{
		delete _raw;
	}
}


PlistDict* FsprgLicense::GetRaw()
{
	return _raw;
}

wstring FsprgLicense::GetLicenseName()
{
	return _raw->GetString(L"LicenseName", L"");
}

wstring FsprgLicense::GetLicenseEmail()
{
	return _raw->GetString(L"LicenseEmail", L"");
}

wstring FsprgLicense::GetLicenseCompany()
{
	return _raw->GetString(L"LicenseCompany", L"");
}

wstring FsprgLicense::GetFirstLicenseCode()
{
	vector<PlistDictType> list = _raw->GetArray(L"LicenseCodes");
	if (list.size() > 0 && list[0].GetType() == PlistDict::STRING)
	{
		return *(wstring*)list[0].GetItem();
	}

	return L"";
}

vector<wstring> FsprgLicense::GetLicenseCodes()
{
	vector<PlistDictType> list = _raw->GetArray(L"LicenseCodes");
	vector<wstring> licenseCodes;
	for (int i = 0; i < list.size(); i++)
	{
		if (list[i].GetType() == PlistDict::STRING)
		{
			licenseCodes.push_back(*(wstring*)list[i].GetItem());
		}
	}
	return licenseCodes;
}

PlistDict* FsprgLicense::GetLicensePropertyList()
{
	return _raw->GetDict(L"LicensePropertyList");
}

wstring FsprgLicense::GetLicenseURL()
{
	return _raw->GetString(L"LicenseURL", L"");
}
