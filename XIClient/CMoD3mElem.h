#pragma once
#include "CMoElem.h"

namespace FFXI {
	namespace CYy {
		class CMoD3mElem : public CMoElem {
		public:
			CMoD3mElem() = default;
			virtual ~CMoD3mElem() = default;
			virtual double VirtElem6() override final;
			virtual void OnDraw() override final;
			virtual void VirtElem9() override final;
			void DoMMBDraw();
			int field_194;
		};
	}
}