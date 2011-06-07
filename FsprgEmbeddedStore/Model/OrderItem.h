#pragma once
#include "PlistDict.h"
#include "Fulfillment.h"
#include "FsprgLicense.h"
#include "FileDownload.h"

namespace FsprgEmbeddedStore
{
	namespace Model
	{

		class __declspec(dllexport) OrderItem
		{
		public:
			OrderItem(PlistDict *dict);
			~OrderItem(void);

			wstring GetProductName();
			wstring GetProductDisplay();
			float GetQuantity();
			float GetItemTotal();
			float GetItemTotalUSD();

			// This reference can be used to make calls to FastSpring's Subscription API.
			// See https://support.fastspring.com/entries/236487-api-subscriptions
			wstring GetSubscriptionReference();

			Fulfillment* GetFulfullment();

			// Shortcut for (FsprgLicense*)(*GetFulfullment())[wstring(L"license")]
			FsprgLicense* GetLicense();

			// Shortcut for (FileDownload*)(*GetFulfullment())[wstring(L"download")]
			FileDownload* GetFileDownload();

		private:
			PlistDict *_raw;
		};

	};
};
