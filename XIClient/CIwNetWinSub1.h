#pragma once
#include "CIwMenu.h"

namespace FFXI {
	namespace CTk {
		class CIwNetWinSub1 : public CIwMenu {
		public:
			virtual void OnInitialUpdatePrimitive() override final;
			virtual void OnDrawPrimitive() override final;
			float progress;
		};
	}
}