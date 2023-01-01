#pragma once
#include "Enums.h"
#include <wtypes.h>

namespace FFXI {
	class GlobalStruct {
	public:
		static GlobalStruct g_GlobalStruct;
		int field_0{ 0 };
		HKEY phkResult{ 0 };
		int RegionCode{ 0 };
		int field_C{ 0 };
		int ClientExpansions{ 0 };
		int ServerExpansions{ 0 };
		int field_18{ 0 };

		int GetRegionCode();

		bool Check10Flag(int);
		bool Check14Flag(int);
	};
}