#include "StdAfx.h"
#include "Fulfillment.h"
#include "FsprgLicense.h"
#include "FileDownload.h"

using namespace FsprgEmbeddedStore::Model;

Fulfillment::Fulfillment(PlistDict *dict)
{
	_raw = new PlistDict(dict);
}


Fulfillment::~Fulfillment(void)
{
	if (_raw != NULL)
	{
		delete _raw;
	}
}

PlistDict* Fulfillment::GetRaw()
{
	return _raw;
}

void* Fulfillment::operator[](wstring key)
{
	PlistDict *item = _raw->GetDict(key);

	if (item != NULL)
	{
		wstring type = item->GetString(L"FulfillmentType", L"");
		if (type == L"License")
		{
			return new FsprgLicense(item);
		}
		if (type == L"File")
		{
			return new FileDownload(item);
		}
	}

	return item;
}