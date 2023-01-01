#pragma once
#include "CMoElem.h"

namespace FFXI {
	namespace CYy {
		class CMoD3bElem : public CMoElem {
		public:
			CMoD3bElem();
			virtual ~CMoD3bElem();
			virtual bool VirtElem1(FFXI::Constants::Enums::ElemType) override final;
			virtual void OnDraw() override final;
			virtual void VirtElem9() override final;
			float field_194[5];
			float field_1A8[5];
			float* field_1BC;
		};
	}
}