#pragma once
#include <string>
#include <map>
#include <vector>

using namespace std;

namespace FsprgEmbeddedStore
{
	namespace Model
	{

		class __declspec(dllexport) PlistDictType
		{
		public:
			PlistDictType(wstring type, void *item) : _type(type), _item(item) {}
			~PlistDictType(void) {};

			wstring GetType() { return _type; }
			void* GetItem() { return _item; }
		private:
			wstring _type;
			void *_item;
		};

		typedef std::map<wstring, PlistDictType> DictType;
		typedef std::pair<wstring, PlistDictType> DictTypePair;
		typedef std::map<std::wstring, PlistDictType>::iterator DictTypeIter;

		class __declspec(dllexport) PlistDict
		{
		public:
			PlistDict(void);
			PlistDict(PlistDict*);
			PlistDict(DictType*);
			~PlistDict(void);

			wstring GetString(wstring, wstring);
			float GetDecimal(wstring, float);
			bool GetBool(wstring, bool);
			vector<PlistDictType> GetArray(wstring);
			PlistDict* GetDict(wstring);

			static const wstring PLIST;
			static const wstring DICT;
			static const wstring KEY;
			static const wstring ARRAY;
			static const wstring REAL;
			static const wstring INTEGER;
			static const wstring BTRUE;
			static const wstring BFALSE;
			static const wstring DATE;
			static const wstring STRING;
			static const wstring DATA;

		private:
			DictType *_dict;

			void recursiveDelete(PlistDictType);
		};

	};
};
