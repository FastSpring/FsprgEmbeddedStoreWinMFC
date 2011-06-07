#include "StdAfx.h"
#include "Order.h"

using namespace FsprgEmbeddedStore::Model;

Order::Order(Plist *plist) : _raw(plist)
{
}


Order::~Order(void)
{
	if (_raw != NULL)
	{
		delete _raw;
	}
}


Order* Order::Parse(wstring plistXml)
{
	return new Order(Plist::Parse(plistXml));
}

Plist* Order::GetRaw()
{
	return _raw;
}

bool Order::GetOrderIsTest()
{
	return _raw->GetBool(L"OrderIsTest", false);
}

wstring Order::GetOrderReference()
{
	return _raw->GetString(L"OrderReference", L"");
}

wstring Order::GetOrderLanguage()
{
	return _raw->GetString(L"OrderLanguage", L"");
}

wstring Order::GetOrderCurrency()
{
	return _raw->GetString(L"OrderCurrency", L"");
}

float Order::GetOrderTotal()
{
	return _raw->GetDecimal(L"OrderTotal", 0);
}

float Order::GetOrderTotalUSD()
{
	return _raw->GetDecimal(L"OrderTotalUSD", 0);
}

wstring Order::GetCustomerFirstName()
{
	return _raw->GetString(L"CustomerFirstName", L"");
}

wstring Order::GetCustomerLastName()
{
	return _raw->GetString(L"CustomerLastName", L"");
}

wstring Order::GetCustomerCompany()
{
	return _raw->GetString(L"CustomerCompany", L"");
}

wstring Order::GetCustomerEmail()
{
	return _raw->GetString(L"CustomerEmail", L"");
}

OrderItem* Order::GetFirstOrderItem()
{
	vector<PlistDictType> list = _raw->GetArray(L"OrderItems");
	if (list.size() > 0 && list[0].GetType() == PlistDict::DICT)
	{
		return new OrderItem((PlistDict*)list[0].GetItem());
	}

	return NULL;
}

vector<OrderItem*> Order::GetOrderItems()
{
	vector<PlistDictType> list = _raw->GetArray(L"OrderItems");
	vector<OrderItem*> items;

	for (int i = 0; i < list.size(); i++)
	{
		if (list[i].GetType() == PlistDict::DICT)
		{
			items.push_back(new OrderItem((PlistDict*)list[i].GetItem()));
		}
	}

	return items;
}