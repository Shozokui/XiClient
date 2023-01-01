#pragma once
#include "CIwMenu.h"

namespace FFXI {
	namespace CTk {
		class CIwPtcBgWin : public CIwMenu {
		public:
			virtual void OnDrawPrimitive() override final;
		};
	}
}