#pragma once
#include "CTkObject.h"

namespace FFXI {
	namespace CTk {
		class CTkMenuFrame;
		class KaWindow;
		class _49SubList;
		class CTkDrawCtrlFrame : public CTkObject {
		public:
			CTkDrawCtrlFrame();
			~CTkDrawCtrlFrame();
			static int AnimOnValue;
			void SetResourceData(CTkMenuFrame*);
			void OffAnimFrame();
			void OnAnimFrame();
			void ShapeCalc(char);
			int LocateCalc(short, short, char);
			KaWindow* Window;
			//TODO:: _49SubList may be called CShape
			_49SubList* field_8;
			float field_C;
			float field_10;
			float field_14;
			short field_18;
			short field_1A;
			short field_1C;
			short field_1E;
			short field_20;
			short field_22;
			short field_24;
			short field_26;
			char* field_28;
			char* field_2C;
			CTkMenuFrame* MenuFrame;
			int field_34;
			int field_38;
			char* field_3C;
			char* field_40;
			char field_44;
			char field_45;
			char field_46;
			char field_47;
			int field_48;
			char field_4C;
			char field_4D;
			char field_4E;
			char field_4F;
		};
	}
}