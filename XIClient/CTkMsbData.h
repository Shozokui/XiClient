#pragma once
#include "CTkObject.h"
#include "CTkObList.h"
namespace FFXI {
	namespace CTk {
		class _49SubList;
		class CTkMsbData : public CTkObject {
		public:
			~CTkMsbData();
			CTkObList ObList;
			_49SubList** SubLists;
			_49SubList*** GetSubLists();
			void DoSomething();
			char ObjName[16];
			int field_34;
			int field_38;
			unsigned short field_3C;
			unsigned short field_3E;
		};
	}
}