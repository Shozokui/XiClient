#pragma once
#include "CMoElem.h"

namespace FFXI {
	namespace CYy {
		class CMoDistElem : public CMoElem {
		public:
			static const CYyClass CMoDistElemClass;
			const CYyClass* GetRuntimeClass() override final;
			CMoDistElem();
			virtual ~CMoDistElem() = default;
			virtual bool VirtElem1(FFXI::Constants::Enums::ElemType) override final;
			virtual void VirtElem2(float) override final;
			virtual void OnDraw() override final;
			float field_194;
		};
	}
}