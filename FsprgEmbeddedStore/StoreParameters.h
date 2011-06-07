#pragma once
#include <map>
#include <vector>
#include <string>

#ifdef _EXPORTING
   #define CLASS_DECLSPEC    __declspec(dllexport)
#else
   #define CLASS_DECLSPEC    __declspec(dllimport)
#endif

using namespace std;

namespace FsprgEmbeddedStore
{

	class OrderProcessType;
	class Mode;

	class __declspec(dllexport) PropertyChangedEventArgs
	{
	public:
		PropertyChangedEventArgs(wstring propertyName) : _propertyName(propertyName) {}

		wstring GetPropertyName() const { return _propertyName; }
	private:
		wstring _propertyName;
	};

	[event_source(native)]
	class CLASS_DECLSPEC StoreParameters
	{
	public:
		StoreParameters(void);
		~StoreParameters(void);

		map<wstring, wstring> GetRaw();
	
		wstring GetLanguage() const;
		void SetLanguage(wstring value);

		const OrderProcessType& GetOrderProcessType() const;
		void SetOrderProcessType(const OrderProcessType& value);

		wstring GetStoreId() const;
		void SetStoreId(wstring value);

		wstring GetProductId() const;
		void SetProductId(wstring value);

		const Mode& GetMode() const;
		void SetMode(const Mode& value);

		wstring GetCampaign() const;
		void SetCampaign(wstring value);

		wstring GetOption() const;
		void SetOption(wstring value);

		wstring GetReferrer() const;
		void SetReferrer(wstring value);

		wstring GetSource() const;
		void SetSource(wstring value);

		wstring GetCoupon() const;
		void SetCoupon(wstring value);

		bool HasContactDefaults() const;

		wstring GetContactFname() const;
		void SetContactFname(wstring value);

		wstring GetContactLname() const;
		void SetContactLname(wstring value);

		wstring GetContactEmail() const;
		void SetContactEmail(wstring value);

		wstring GetContactCompany() const;
		void SetContactCompany(wstring value);

		wstring GetContactPhone() const;
		void SetContactPhone(wstring value);

		wstring ToURL() const;

		__event void PropertyChangedEventHandler(PropertyChangedEventArgs&);

		static const wstring LANGUAGE;
		static const wstring ORDER_PROCESS_TYPE;
		static const wstring STORE_ID;
		static const wstring PRODUCT_ID;
		static const wstring MODE;
		static const wstring CAMPAIGN;
		static const wstring OPTION;
		static const wstring REFERRER;
		static const wstring SOURCE;
		static const wstring COUPON;
		static const wstring CONTACT_FNAME;
		static const wstring CONTACT_LNAME;
		static const wstring CONTACT_EMAIL;
		static const wstring CONTACT_COMPANY;
		static const wstring CONTACT_PHONE;

	protected:
		wstring GetValue(wstring key) const;
		void SetValue(wstring key, wstring value);

		void ToURLChanged();

	private:
		map<wstring, wstring> _raw;
	};

	class CLASS_DECLSPEC OrderProcessType
	{
	public:
		~OrderProcessType(void);
		
		static const OrderProcessType& Parse(wstring);
		static vector<OrderProcessType> GetAll();
		wstring ToString() const;

		bool operator==(const OrderProcessType& aType) const;
		bool operator!=(const OrderProcessType& aType) const;

		static const OrderProcessType Detail;
		static const OrderProcessType Instant;
		static const OrderProcessType Unknown;
	private:
		OrderProcessType(wstring);
		OrderProcessType(void);

		static map<wstring, const OrderProcessType> Initialize();
		static map<wstring, const OrderProcessType> _byValue;
		wstring _value;
	};

	class CLASS_DECLSPEC Mode
	{
	public:
		~Mode(void);

		static const Mode& Parse(wstring);
		static vector<Mode> GetAll();
		wstring ToString() const;

		bool operator==(const Mode& aMode) const;
		bool operator!=(const Mode& aMode) const;

		static const Mode Active;
		static const Mode ActiveTest;
		static const Mode Test;
		static const Mode Unknown;
	private:
		Mode(void);
		Mode(wstring);
		
		static map<wstring, const Mode> Initialize();
		static map<wstring, const Mode> _byValue;
		wstring _value;
	};

};