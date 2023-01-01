#pragma once
#include "CIwMenu.h"

namespace FFXI {
	namespace CTk {
		class CIwNetWin : public CIwMenu {
		public:
			CIwNetWin();
			virtual void OnInitialUpdatePrimitive() override final;
			virtual void OnDrawPrimitive() override final;
			char HelpText[0x200];
		};
	}
}