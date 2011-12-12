#include "StdAfx.h"
#include "PlistDict.h"

using namespace FsprgEmbeddedStore::Model;

const wstring PlistDict::PLIST(L"plist");
const wstring PlistDict::DICT(L"dict");
const wstring PlistDict::KEY(L"key");
const wstring PlistDict::ARRAY(L"array");
const wstring PlistDict::REAL(L"real");
const wstring PlistDict::INTEGER(L"integer");
const wstring PlistDict::BTRUE(L"true");
const wstring PlistDict::BFALSE(L"false");
const wstring PlistDict::DATE(L"date");
const wstring PlistDict::STRING(L"string");
const wstring PlistDict::DATA(L"data");

PlistDict::PlistDict(void)
{
}

PlistDict::PlistDict(PlistDict *list)
{
	_dict = new DictType();

	if (list->_dict != NULL)
	{
		DictTypeIter it;
		for (it = list->_dict->begin(); it != list->_dict->end(); it++)
		{
			_dict->insert(DictTypePair(((*it).first), ((*it).second)));
		}
	}
}

PlistDict::PlistDict(DictType *dict) : _dict(dict)
{

}

PlistDict::~PlistDict(void)
{
	if (_dict != NULL)
	{
		DictTypeIter it;
		for (it = _dict->begin(); it != _dict->end(); it++)
		{
			recursiveDelete((*it).second);
		}

		delete _dict;
	}
}

void PlistDict::recursiveDelete(PlistDictType type)
{
	void *ptr = type.GetItem();
	if (ptr != NULL)
	{
		if (type.GetType() == STRING)
		{
			delete (wstring*)type.GetItem();
		}
		else if (type.GetType() == ARRAY)
		{
			vector<PlistDictType> *list = (vector<PlistDictType>*)type.GetItem();
			if (list != NULL)
			{
				vector<PlistDictType>::iterator arIt;
				for (arIt = list->begin(); arIt != list->end(); arIt++)
				{
					recursiveDelete(*arIt);
				}
			}

			delete list;
		}
		else if (type.GetType() == DICT)
		{
			delete (PlistDict*)type.GetItem();
		}
		else
		{
			delete ptr;
		}
	}
}

wstring PlistDict::GetString(wstring key, wstring defaultStr)
{
	if (_dict != NULL)
	{
		DictTypeIter iter = _dict->find(key);
		if (iter != _dict->end())
		{
			if (iter->second.GetType() == STRING)
			{
				return *(wstring*)iter->second.GetItem();
			}
		}
	}

	return defaultStr;
}

float PlistDict::GetDecimal(wstring key, float defaultDecimal)
{
	if (_dict != NULL)
	{
		DictTypeIter iter = _dict->find(key);
		if (iter != _dict->end())
		{
			if (iter->second.GetType() == REAL)
			{
				return *(float*)iter->second.GetItem();
			}
		}
	}

	return defaultDecimal;
}

bool PlistDict::GetBool(wstring key, bool defaultBool)
{
	if (_dict != NULL)
	{
		DictTypeIter iter = _dict->find(key);
		if (iter != _dict->end())
		{
			if (iter->second.GetType() == BTRUE || iter->second.GetType() == BFALSE)
			{
				return *(bool*)iter->second.GetItem();
			}
		}
	}

	return defaultBool;
}

vector<PlistDictType> PlistDict::GetArray(wstring key)
{
	if (_dict != NULL)
	{
		DictTypeIter iter = _dict->find(key);
		if (iter != _dict->end())
		{
			if (iter->second.GetType() == ARRAY)
			{
				return *(vector<PlistDictType>*)iter->second.GetItem();
			}
		}
	}

	return vector<PlistDictType>();
}

PlistDict* PlistDict::GetDict(wstring key)
{
	if (_dict != NULL)
	{
		DictTypeIter iter = _dict->find(key);
		if (iter != _dict->end())
		{
			if (iter->second.GetType() == DICT)
			{
				return new PlistDict((PlistDict*)iter->second.GetItem());
			}
		}
	}

	return NULL;
}