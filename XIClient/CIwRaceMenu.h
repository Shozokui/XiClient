#pragma once
#include "CIwMenu.h"

namespace FFXI {
	namespace CTk {
		class CIwRaceMenu : public CIwMenu {
		public:
			CIwRaceMenu();
			virtual void OnInitialUpdatePrimitive() override final;
			virtual void OnDrawPrimitive() override final;
			virtual void OnUpdatePrimitive() override final;
			virtual void OnKeyDown(short, short) override final;
			void DispRace(int);
			int field_14;
			int field_18;
			int field_1C;
			int field_20;
		};
	}
}