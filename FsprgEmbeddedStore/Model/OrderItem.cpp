#include "StdAfx.h"
#include "OrderItem.h"

using namespace FsprgEmbeddedStore::Model;

OrderItem::OrderItem(PlistDict *dict)
{
	_raw = new PlistDict(dict);
}

OrderItem::~OrderItem(void)
{
	if (_raw != NULL)
	{
		delete _raw;
	}
}

wstring OrderItem::GetProductName()
{
	return _raw->GetString(L"ProductName", L"");
}

wstring OrderItem::GetProductDisplay()
{
	return _raw->GetString(L"ProductDisplay", L"");
}

float OrderItem::GetQuantity()
{
	return _raw->GetDecimal(L"Quantity", 0);
}

float OrderItem::GetItemTotal()
{
	return _raw->GetDecimal(L"ItemTotal", 0);
}

float OrderItem::GetItemTotalUSD()
{
	return _raw->GetDecimal(L"ItemTotalUSD", 0);
}

wstring OrderItem::GetSubscriptionReference()
{
	return _raw->GetString(L"SubscriptionReference", L"");
}

Fulfillment* OrderItem::GetFulfullment()
{
	return new Fulfillment(_raw->GetDict(L"Fulfillment"));
}

FsprgLicense* OrderItem::GetLicense()
{
	Fulfillment *p = GetFulfullment();
	FsprgLicense *l = (FsprgLicense*)(*p)[wstring(L"license")];
	delete p;
	return l;
}

FileDownload* OrderItem::GetFileDownload()
{
	Fulfillment *p = GetFulfullment();
	FileDownload *d = (FileDownload*)(*p)[wstring(L"download")];
	delete p;
	return d;
}
