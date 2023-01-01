#pragma once
#include "CIwMenu.h"

namespace FFXI {
	namespace CTk {
		class CIwChfWin : public CIwMenu {
		public:
			virtual void OnInitialUpdatePrimitive() override final;
			virtual void OnDrawCalc(bool) override final;
			virtual void OnKeyDown(short, short) override final;
		};
	}
}