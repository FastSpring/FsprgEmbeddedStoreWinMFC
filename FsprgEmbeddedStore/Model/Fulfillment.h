#pragma once
#include "PlistDict.h"

namespace FsprgEmbeddedStore
{
	namespace Model
	{

		class __declspec(dllexport) Fulfillment
		{
		public:
			Fulfillment(PlistDict*);
			~Fulfillment(void);

			PlistDict* GetRaw();
			void* operator[](wstring key);

		private:
			PlistDict *_raw;
		};

	};
};
