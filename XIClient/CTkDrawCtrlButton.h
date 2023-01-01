#pragma once
#include "CTkObject.h"
#include <wtypes.h>
namespace FFXI {
	namespace CTk {
		class CTkMenuCtrlData;
		class CTkMenuButton;
		class _49SubList;
		class CTkDrawCtrlButton :public CTkObject{
		public:
			CTkDrawCtrlButton(CTkMenuCtrlData*);
			~CTkDrawCtrlButton();
			void SetMenuCtrlID(int);
			void SetResourceData(CTkMenuButton*);
			void LocateCalc(short, short, bool);
			void ShapeCalc(bool);
			bool PointInButton(const tagPOINT&);
			_49SubList* field_4;
			_49SubList* field_8;
			CTkMenuButton* MenuButton;
			int field_10;
			int field_14;
			int field_18;
			float field_1C;
			float field_20;
			char field_24;
			char field_25;
			short field_26;
			short field_28;
			short ButtonLeft;
			short ButtonTop;
			short ButtonWidth;
			short ButtonHeight;
			unsigned short field_32;
			unsigned short field_34;
			unsigned short field_36;
			unsigned short field_38;
			unsigned short field_3A;
			unsigned char field_3C;
			char field_3D;
			short field_3E;
			char* field_40;
			char* field_44;
			char field_48;
			char field_49;
			char field_4A;
			char field_4B;
			char* field_4C;
			char* field_50;
			int field_54;
			CTkObject* field_58;
			CTkMenuCtrlData* field_5C;
		};
	}
}