#include "StdAfx.h"
#include "WebBrowserHelper.h"
#include <sstream>
#include <iomanip>
#include <WinInet.h>

using namespace Utilities::WebBrowser;

WebBrowserHelper::WebBrowserHelper(void)
{
}


WebBrowserHelper::~WebBrowserHelper(void)
{
}

int WebBrowserHelper::ClearCache()
{
	// Pointer to a GROUPID variable
	GROUPID groupId = 0;
	
	// Local variables
	DWORD cacheEntryInfoBufferSizeInitial = 0;
    DWORD cacheEntryInfoBufferSize = 0;
    int *cacheEntryInfoBuffer = 0;
    INTERNET_CACHE_ENTRY_INFO *internetCacheEntry;
    HANDLE enumHandle = NULL;
    BOOL returnValue = false;

	// Delete the groups first.
    // Groups may not always exist on the system.
    // For more information, visit the following Microsoft Web site:
    // http://msdn2.microsoft.com/en-us/library/ms909365.aspx			
    // By default, a URL does not belong to any group. Therefore, that cache may become
    // empty even when the CacheGroup APIs are not used because the existing URL does not belong to any group.			
    enumHandle = FindFirstUrlCacheGroup(0, CACHEGROUP_SEARCH_ALL, 0, 0, &groupId, 0);

	// If there are no items in the Cache, you are finished.
	if (enumHandle != NULL && ERROR_NO_MORE_ITEMS == GetLastError())
		return 0;
	
	// Loop through Cache Group, and then delete entries.
	if (enumHandle != NULL)
	{
		while (1)
		{
			// Delete a particular Cache Group.
			returnValue = DeleteUrlCacheGroup(groupId, CACHEGROUP_FLAG_FLUSHURL_ONDELETE, 0);
		
			returnValue = FindNextUrlCacheGroup(enumHandle, &groupId, 0);
			
			if (!returnValue)
			{
				break;
			}
		}
	}
	
	 // Start to delete URLs that do not belong to any group.
	enumHandle = FindFirstUrlCacheEntry(NULL, 0, &cacheEntryInfoBufferSizeInitial);
	if (enumHandle == NULL && ERROR_NO_MORE_ITEMS == GetLastError())
		return 0;
	
	cacheEntryInfoBufferSize = cacheEntryInfoBufferSizeInitial;
	internetCacheEntry =  (INTERNET_CACHE_ENTRY_INFO*)malloc(cacheEntryInfoBufferSize);
	enumHandle = FindFirstUrlCacheEntry(NULL, internetCacheEntry, &cacheEntryInfoBufferSizeInitial);
	if (enumHandle != NULL)
	{
		while (1)
		{
			cacheEntryInfoBufferSizeInitial = cacheEntryInfoBufferSize;		
			returnValue = DeleteUrlCacheEntry(internetCacheEntry->lpszSourceUrlName);				
		
			// Allows try to get the next entry
			returnValue = FindNextUrlCacheEntry(enumHandle, internetCacheEntry, &cacheEntryInfoBufferSizeInitial);
			
			DWORD dwError = GetLastError();
			if (!returnValue && ERROR_NO_MORE_ITEMS == dwError)
			{
				break;
			}			
		
			if (!returnValue && cacheEntryInfoBufferSizeInitial > cacheEntryInfoBufferSize)
			{
				cacheEntryInfoBufferSize = cacheEntryInfoBufferSizeInitial;
				internetCacheEntry =  (INTERNET_CACHE_ENTRY_INFO*)realloc(internetCacheEntry, cacheEntryInfoBufferSize);
				returnValue = FindNextUrlCacheEntry(enumHandle, internetCacheEntry, &cacheEntryInfoBufferSizeInitial);					
			}
		}
	}
	
	free(internetCacheEntry);
	return 0;
}

std::wstring WebBrowserHelper::hexencode(const std::wstring& input)
{
	std::wostringstream ssOut;
    ssOut << std::setbase(16);
    for(std::wstring::const_iterator i = input.begin(); i != input.end(); ++i)
    {
        if(isalnum(*i))
            ssOut << *i;
        else
            ssOut << '%' << std::setw(2) << ((unsigned int)(unsigned char)*i);
    }
    return ssOut.str();
}