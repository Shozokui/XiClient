#pragma once
#include "CMoElem.h"

namespace FFXI {
	namespace CYy {
		class CMoLfdElem : public CMoElem {
		public:
			static const CYyClass CMoLfdElemClass;
			const CYyClass* GetRuntimeClass() override final;
			CMoLfdElem();
			virtual ~CMoLfdElem() = default;
			virtual void VirtOt1() override final;
			virtual bool VirtElem1(FFXI::Constants::Enums::ElemType) override final;
			float field_194;
			float field_198;
			int field_19C;
		};
	}
}