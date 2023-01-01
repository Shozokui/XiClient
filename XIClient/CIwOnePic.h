#pragma once
#include "CIwMenu.h"

namespace FFXI {
	namespace CTk {
		class CIwOnePic : public CIwMenu {
		public:
			CIwOnePic();
			virtual void OnInitialUpdatePrimitive() override final;
			virtual void OnDrawPrimitive() override final;
			virtual void OnDrawCalc(bool) override final;
			void init(int);
			int field_14;
			int field_18;
			int field_1C;
			int field_20;
			int field_24;
			int field_28;
			int field_2C;
			int field_30;
			int field_34;
		};
	}
}