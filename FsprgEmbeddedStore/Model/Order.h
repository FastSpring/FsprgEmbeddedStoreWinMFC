#pragma once
#include <vector>
#include "Plist.h"
#include "OrderItem.h"

using namespace std;

namespace FsprgEmbeddedStore
{
	namespace Model
	{

		class __declspec(dllexport) Order
		{
		public:
			~Order(void);

			static Order* Parse(wstring plistXml);

			Plist* GetRaw();
			bool GetOrderIsTest();
			wstring GetOrderReference();
			wstring GetOrderLanguage();
			wstring GetOrderCurrency();
			float GetOrderTotal();
			float GetOrderTotalUSD();
			wstring GetCustomerFirstName();
			wstring GetCustomerLastName();
			wstring GetCustomerCompany();
			wstring GetCustomerEmail();

			OrderItem* GetFirstOrderItem();
			vector<OrderItem*> GetOrderItems();

		private:
			Plist *_raw;

			Order(Plist*);
		};

	};
};
