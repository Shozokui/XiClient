#pragma once
#include "CIwMenu.h"

namespace FFXI {
	namespace CTk {
		class CIwLicenceMenu : public CIwMenu {
		public:
			CIwLicenceMenu();
			virtual void OnInitialUpdatePrimitive() override final;
			virtual void OnDrawPrimitive() override final;
			virtual void OnUpdatePrimitive() override final;
			virtual void OnKeyDown(short, short) override final;
			int field_14;
			int field_18;
			int field_1C;
		};
	}
}