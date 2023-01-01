#pragma once
#include "CTkObject.h"

namespace FFXI {
	namespace CTk {
		class _49SubList;
		class CTkMenuCursol;
		class CTkDrawCtrlButton;
		class CTkDrawCtrlCursol : public CTkObject {
		public:
			CTkDrawCtrlCursol();
			~CTkDrawCtrlCursol();
			void SetResourceData(CTkMenuCursol*);
			void LocateCalc(CTkDrawCtrlButton*);
			void ShapeInit();
			void ShapeCalc();
			_49SubList*** field_4;
			int field_8;
			_49SubList* field_C;
			_49SubList* field_10;
			CTkMenuCursol* MenuCursol;
			int field_18;
			short field_1C;
			short field_1E;
			short field_20;
			short field_22;
			float field_24;
			short field_28;
			short field_2A;
			short field_2C;
			short field_2E;
			int field_30;
		};
	}
}