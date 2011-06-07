#pragma once
#include "PlistDict.h"

namespace FsprgEmbeddedStore
{
	namespace Model
	{

		class __declspec(dllexport) FileDownload
		{
		public:
			FileDownload(PlistDict*);
			~FileDownload(void);

			PlistDict* GetRaw();
			wstring GetFileURL();

		private:
			PlistDict *_raw;
		};

	};
};
