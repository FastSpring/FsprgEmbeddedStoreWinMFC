#pragma once
#include <string>

namespace Utilities
{
	namespace WebBrowser
	{

		class WebBrowserHelper
		{
		public:
			WebBrowserHelper(void);
			~WebBrowserHelper(void);

			static void ClearCache();
			static std::wstring hexencode(const std::wstring&);
		};

	};
};