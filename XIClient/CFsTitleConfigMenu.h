#pragma once
#include "CFsGameConfigMenu.h"

namespace FFXI {
	namespace CTk {
		class CFsTitleConfigMenu : public CFsGameConfigMenu {
		public:
			CFsTitleConfigMenu();
			virtual void OnInitialUpdatePrimitive() override final;
			int field_20;
			int field_24;
			int field_28;
			int field_2C;
			int field_30;
			int field_34;
			int field_38;
			int field_3C;
			int field_40;
			int field_44;
			int field_48;
		};
	}
}