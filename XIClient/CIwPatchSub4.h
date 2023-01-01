#pragma once
#include "CIwMenu.h"

namespace FFXI {
	namespace CTk {
		class CIwPatchSub4 : public CIwMenu {
		public:
			void Init();
			virtual void OnInitialUpdatePrimitive() override final;
			virtual void OnDrawPrimitive() override final;
			int field_14;
			int field_18;
			int field_1C;
			int field_20;
			int field_24;
			int field_28;
			int field_2C;
			int field_30[64];
			int field_130;
			int field_134;
		};
	}
}