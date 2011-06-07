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

void WebBrowserHelper::ClearCache()
{
	GROUPID groupId;
	// Local variables
    DWORD cacheEntryInfoBufferSizeInitial = 0;
    DWORD cacheEntryInfoBufferSize = 0;
	INTERNET_CACHE_ENTRY_INFO *cacheEntryInfoBuffer;
	INTERNET_CACHE_ENTRY_INFO internetCacheEntry;
	HANDLE enumHandle = NULL;
    BOOL returnValue = FALSE;

    // Delete the groups first.
    // Groups may not always exist on the system.
    // For more information, visit the following Microsoft Web site:
    // http://msdn.microsoft.com/library/?url=/workshop/networking/wininet/overview/cache.asp   
    // By default, a URL does not belong to any group. Therefore, that cache may become
    // empty even when the CacheGroup APIs are not used because the existing URL does not belong to any group.   
    enumHandle = FindFirstUrlCacheGroup(0, CACHEGROUP_SEARCH_ALL, NULL, 0, &groupId, NULL);
    // If there are no items in the Cache, you are finished.
    if (enumHandle != NULL && ERROR_NO_MORE_ITEMS == GetLastError())
        return;
    // Loop through Cache Group, and then delete entries.
    while (true) {
        if (ERROR_NO_MORE_ITEMS == GetLastError() || ERROR_FILE_NOT_FOUND == GetLastError()) { break; }
        // Delete a particular Cache Group.
        returnValue = DeleteUrlCacheGroup(groupId, CACHEGROUP_FLAG_FLUSHURL_ONDELETE, NULL);
        if (!returnValue && ERROR_FILE_NOT_FOUND == GetLastError()) {
            returnValue = FindNextUrlCacheGroup(enumHandle, &groupId, NULL);
        }
        if (!returnValue && (ERROR_NO_MORE_ITEMS == GetLastError() || ERROR_FILE_NOT_FOUND == GetLastError()))
            break;
    }
    // Start to delete URLs that do not belong to any group.
    enumHandle = FindFirstUrlCacheEntry(NULL, NULL, &cacheEntryInfoBufferSizeInitial);
    if (enumHandle != NULL && ERROR_NO_MORE_ITEMS == GetLastError())
        return;
    cacheEntryInfoBufferSize = cacheEntryInfoBufferSizeInitial;
    cacheEntryInfoBuffer = (INTERNET_CACHE_ENTRY_INFO*)new TCHAR[cacheEntryInfoBufferSize];
    enumHandle = FindFirstUrlCacheEntry(NULL, cacheEntryInfoBuffer, &cacheEntryInfoBufferSizeInitial);
    while (true) {
        internetCacheEntry = *cacheEntryInfoBuffer;
        if (ERROR_NO_MORE_ITEMS == GetLastError()) { break; }
        cacheEntryInfoBufferSizeInitial = cacheEntryInfoBufferSize;
        returnValue = DeleteUrlCacheEntry(internetCacheEntry.lpszSourceUrlName);
        if (!returnValue) {
            returnValue = FindNextUrlCacheEntry(enumHandle, cacheEntryInfoBuffer, &cacheEntryInfoBufferSizeInitial);
        }
        if (!returnValue && ERROR_NO_MORE_ITEMS == GetLastError()) {
            break;
        }
        if (!returnValue && cacheEntryInfoBufferSizeInitial > cacheEntryInfoBufferSize) {
            cacheEntryInfoBufferSize = cacheEntryInfoBufferSizeInitial;
			delete [] cacheEntryInfoBuffer;
            cacheEntryInfoBuffer = (INTERNET_CACHE_ENTRY_INFO*)new TCHAR[cacheEntryInfoBufferSize];
            returnValue = FindNextUrlCacheEntry(enumHandle, cacheEntryInfoBuffer, &cacheEntryInfoBufferSizeInitial);
        }
    }
    delete [] cacheEntryInfoBuffer;
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