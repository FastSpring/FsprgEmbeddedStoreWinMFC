#include "StdAfx.h"
#include "StoreParameters.h"
#include "Utilities\WebBrowserHelper.h"
#include <msxml6.h>

using namespace FsprgEmbeddedStore;
using namespace Utilities::WebBrowser;

const wstring StoreParameters::LANGUAGE(L"language");
const wstring StoreParameters::ORDER_PROCESS_TYPE(L"orderProcessType");
const wstring StoreParameters::STORE_ID(L"storeId");
const wstring StoreParameters::PRODUCT_ID(L"productId");
const wstring StoreParameters::MODE(L"mode");
const wstring StoreParameters::CAMPAIGN(L"campaign");
const wstring StoreParameters::OPTION(L"option");
const wstring StoreParameters::REFERRER(L"referrer");
const wstring StoreParameters::SOURCE(L"source");
const wstring StoreParameters::COUPON(L"coupon");
const wstring StoreParameters::CONTACT_FNAME(L"contact_fname");
const wstring StoreParameters::CONTACT_LNAME(L"contact_lname");
const wstring StoreParameters::CONTACT_EMAIL(L"contact_email");
const wstring StoreParameters::CONTACT_COMPANY(L"contact_company");
const wstring StoreParameters::CONTACT_PHONE(L"contact_phone");

StoreParameters::StoreParameters(void)
{
	
}


StoreParameters::~StoreParameters(void)
{
}


map<wstring, wstring> StoreParameters::GetRaw() 
{
	return _raw;
}

wstring StoreParameters::GetLanguage() const
{
	return GetValue(LANGUAGE);
}

void StoreParameters::SetLanguage(wstring value) 
{
	SetValue(LANGUAGE, value);
}

const OrderProcessType& StoreParameters::GetOrderProcessType() const
{
	return OrderProcessType::Parse(GetValue(ORDER_PROCESS_TYPE));
}

void StoreParameters::SetOrderProcessType(const OrderProcessType& value) 
{
	SetValue(ORDER_PROCESS_TYPE, value.ToString());
}

wstring StoreParameters::GetStoreId() const
{
	return GetValue(STORE_ID);
}

void StoreParameters::SetStoreId(wstring value) 
{
	SetValue(STORE_ID, value);
}

wstring StoreParameters::GetProductId() const
{
	return GetValue(PRODUCT_ID);
}

void StoreParameters::SetProductId(wstring value)
{
	SetValue(PRODUCT_ID, value);
}

const Mode& StoreParameters::GetMode() const
{
	return Mode::Parse(GetValue(MODE));
}

void StoreParameters::SetMode(const Mode& value) 
{
	if (value != Mode::Unknown)
	{
		SetValue(MODE, value.ToString());
	}
}

wstring StoreParameters::GetCampaign() const
{
	return GetValue(CAMPAIGN);
}

void StoreParameters::SetCampaign(wstring value)
{
	SetValue(CAMPAIGN, value);
}

wstring StoreParameters::GetOption() const
{
	return GetValue(OPTION);
}

void StoreParameters::SetOption(wstring value)
{
	SetValue(OPTION, value);
}

wstring StoreParameters::GetReferrer() const
{
	return GetValue(REFERRER);
}

void StoreParameters::SetReferrer(wstring value)
{
	SetValue(REFERRER, value);
}

wstring StoreParameters::GetSource() const
{
	return GetValue(SOURCE);
}

void StoreParameters::SetSource(wstring value)
{
	SetValue(SOURCE, value);
}

wstring StoreParameters::GetCoupon() const
{
	return GetValue(COUPON);
}

void StoreParameters::SetCoupon(wstring value)
{
	SetValue(COUPON, value);
}

bool StoreParameters::HasContactDefaults() const
{
	map<wstring, wstring>::const_iterator iter = _raw.find(CONTACT_FNAME);
	if (iter != _raw.end())
	{
		return true;
	}
	iter = _raw.find(CONTACT_LNAME);
	if (iter != _raw.end())
	{
		return true;
	}
	iter = _raw.find(CONTACT_EMAIL);
	if (iter != _raw.end())
	{
		return true;
	}
	iter = _raw.find(CONTACT_COMPANY);
	if (iter != _raw.end())
	{
		return true;
	}
	iter = _raw.find(CONTACT_PHONE);
	if (iter != _raw.end())
	{
		return true;
	}

	return false;
}

wstring StoreParameters::GetContactFname() const
{
	return GetValue(CONTACT_FNAME);
}

void StoreParameters::SetContactFname(wstring value)
{
	SetValue(CONTACT_FNAME, value);
}

wstring StoreParameters::GetContactLname() const
{
	return GetValue(CONTACT_LNAME);
}

void StoreParameters::SetContactLname(wstring value)
{
	SetValue(CONTACT_LNAME, value);
}

wstring StoreParameters::GetContactEmail() const
{
	return GetValue(CONTACT_EMAIL);
}

void StoreParameters::SetContactEmail(wstring value)
{
	SetValue(CONTACT_EMAIL, value);
}

wstring StoreParameters::GetContactCompany() const
{
	return GetValue(CONTACT_COMPANY);
}

void StoreParameters::SetContactCompany(wstring value)
{
	SetValue(CONTACT_COMPANY, value);
}

wstring StoreParameters::GetContactPhone() const
{
	return GetValue(CONTACT_PHONE);
}

void StoreParameters::SetContactPhone(wstring value)
{
	SetValue(CONTACT_PHONE, value);
}

wstring StoreParameters::ToURL() const
{
	wstring storeIdEncoded = GetStoreId();
	if (storeIdEncoded.length() > 0)
	{
		storeIdEncoded = WebBrowserHelper::hexencode(storeIdEncoded);
	}
	wstring productIdEncoded = GetProductId();
	if (productIdEncoded.length() > 0)
	{
		productIdEncoded = WebBrowserHelper::hexencode(productIdEncoded);
	}

	wstring urlAsStr;
	const OrderProcessType &type = GetOrderProcessType();
	if (type == OrderProcessType::Detail) 
	{
		wstring scheme = L"http";
		if (HasContactDefaults())
		{
			scheme = L"https";
		}
		wchar_t url[1024]; 
		wsprintf(url, L"%s://sites.fastspring.com/%s/product/%s", scheme.c_str(), storeIdEncoded.c_str(), productIdEncoded.c_str());
		urlAsStr = url;
	}
	else if (type == OrderProcessType::Instant) 
	{
		wchar_t url[1024];
		wsprintf(url, L"https://sites.fastspring.com/%s/instant/%s", storeIdEncoded.c_str(), productIdEncoded.c_str());
		urlAsStr = url;
	}

	wstring queryStr;
	map<wstring, wstring>::const_iterator iter;
	for (iter = _raw.begin(); iter != _raw.end(); ++iter)
	{
		if (iter->first == ORDER_PROCESS_TYPE
			|| iter->first == STORE_ID
			|| iter->first == PRODUCT_ID)
		{
			continue;
		}

		if (iter->second.length() > 0)
		{
			wchar_t param[256];
			wsprintf(param, L"&%s=%s", iter->first.c_str(), WebBrowserHelper::hexencode(iter->second).c_str());
			queryStr += param;
		}
	}

	if (queryStr.length() > 0)
	{
		queryStr[0] = '?';
		urlAsStr += queryStr;
	}

	return urlAsStr;
}

wstring StoreParameters::GetValue(wstring key) const
{
	map<wstring, wstring>::const_iterator iter = _raw.find(key);
	if (iter != _raw.end()) 
	{
		return iter->second;
	}
	else
	{
		return L"";
	}
}

void StoreParameters::SetValue(wstring key, wstring value)
{
	map<wstring, wstring>::iterator iter = _raw.find(key);
	if (iter != _raw.end()) 
	{
		_raw.erase(iter);
	}
	
	if (value.length() > 0)
	{
		_raw.insert(pair<wstring, wstring>(key, value));
	}
	
	ToURLChanged();
}

void StoreParameters::ToURLChanged()
{
	__raise PropertyChangedEventHandler(PropertyChangedEventArgs(L"ToURL"));
}


const OrderProcessType OrderProcessType::Detail(L"detail");
const OrderProcessType OrderProcessType::Instant(L"instant");
const OrderProcessType OrderProcessType::Unknown(L"unknown");
map<wstring, const OrderProcessType> OrderProcessType::_byValue = Initialize();

map<wstring, const OrderProcessType> OrderProcessType::Initialize()
{
	map<wstring, const OrderProcessType> init;
	init.insert(pair<wstring, OrderProcessType>(Detail.ToString(), Detail));
	init.insert(pair<wstring, OrderProcessType>(Instant.ToString(), Instant));

	return init;
}

OrderProcessType::OrderProcessType(void)
{

}

OrderProcessType::OrderProcessType(wstring value) : _value(value)
{

}

OrderProcessType::~OrderProcessType(void)
{

}

const OrderProcessType& OrderProcessType::Parse(wstring value)
{
	if (value.length() == 0)
	{
		return Unknown;
	}

	map<wstring, const OrderProcessType>::iterator iter = _byValue.find(value);
	if (iter != _byValue.end())
	{
		return iter->second;
	}

	return Unknown;
}

vector<OrderProcessType> OrderProcessType::GetAll()
{
	vector<OrderProcessType> all;
	
	map<wstring, const OrderProcessType>::iterator it;
	for (it = _byValue.begin(); it != _byValue.end(); it++)
	{
		all.push_back(it->second);
	}

	return all;
}

wstring OrderProcessType::ToString() const
{
	return _value;
}

bool OrderProcessType::operator==(const OrderProcessType& aType) const
{
	return _value == aType._value;
}

bool OrderProcessType::operator!=(const OrderProcessType& aType) const
{
	return _value != aType._value;
}

const Mode Mode::Active(L"active");
const Mode Mode::ActiveTest(L"active.test");
const Mode Mode::Test(L"test");
const Mode Mode::Unknown(L"unknown");
map<wstring, const Mode> Mode::_byValue = Initialize();

map<wstring, const Mode> Mode::Initialize()
{
	map<wstring, const Mode> init;
	init.insert(pair<wstring, Mode>(Active.ToString(), Active));
	init.insert(pair<wstring, Mode>(ActiveTest.ToString(), ActiveTest));
	init.insert(pair<wstring, Mode>(Test.ToString(), Test));

	return init;
}

Mode::Mode(void)
{

}

Mode::Mode(wstring value) : _value(value)
{

}

Mode::~Mode(void)
{

}

const Mode& Mode::Parse(wstring value)
{
	if (value.length() == 0)
	{
		return Unknown;
	}

	map<wstring, const Mode>::iterator iter = _byValue.find(value);
	if (iter != _byValue.end())
	{
		return iter->second;
	}

	return Unknown;
}

vector<Mode> Mode::GetAll()
{
	vector<Mode> all;
	
	map<wstring, const Mode>::iterator it;
	for (it = _byValue.begin(); it != _byValue.end(); it++)
	{
		all.push_back(it->second);
	}

	return all;
}

wstring Mode::ToString() const
{
	return _value;
}

bool Mode::operator==(const Mode& aMode) const
{
	return _value == aMode._value;
}

bool Mode::operator!=(const Mode& aMode) const
{
	return _value != aMode._value;
}
