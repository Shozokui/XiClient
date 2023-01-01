#pragma once
#include "CTkObject.h"

namespace FFXI {
	namespace CTk {
		class CTkMenuFrame;
		class CTkMenuCursol;
		class CTkMenuButton;
		class CTkMenuData : public CTkObject {
		public:
			CTkMenuData();
			~CTkMenuData();
			void Store(void**);
			CTkMenuFrame* GetFrame();
			CTkMenuButton* GetButton(int);
			CTkMenuCursol* GetCursol();
			void SetLastCursol(short, short);
			void GetLastCursol(short*, short*);
			short GetCommand(short, short);
			CTkMenuButton* MenuButtons;
			CTkMenuFrame* MenuFrame;
			CTkMenuCursol* MenuCursol;
			short field_10;
			short field_12;
			short field_14;
			short field_16;
			unsigned short ButtonCount;
			short field_1A;
			short field_1C;
			char field_1E[32];
			short field_3E;
			int field_40;
			short field_44;
			char field_46[16];
			short field_56;
			int field_58;
			int field_5C;
			int field_60;
			int field_64;
		};
	}
}