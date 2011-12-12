#include "StdAfx.h"
#include "FileDownload.h"

using namespace FsprgEmbeddedStore::Model;

FileDownload::FileDownload(PlistDict *dict)
{
	_raw = new PlistDict(dict);
}


FileDownload::~FileDownload(void)
{
	if (_raw != NULL)
	{
		delete _raw;
	}
}


PlistDict *FileDownload::GetRaw()
{
	return _raw;
}

wstring FileDownload::GetFileURL()
{
	return _raw->GetString(L"FileURL", L"");
}
