#pragma once
#include "CTkObList.h"

namespace FFXI {
	namespace CTk {
		class CTkMsbData;
		class _49SubList;
		class CTkMsbDataList : public CTkObList {
		public:
			~CTkMsbDataList();
			void Init();
			_49SubList*** GetSubLists(const char*);
			void Remove(void** Data);
			CTkMsbData* ContainsName(char*);
			int field_1C;
			int field_20;
			int field_24;
			int field_28;
			int field_2C;
			int field_30;
			int field_34;
			int field_38;
			int field_3C;
			int field_40;
		};
	}
}