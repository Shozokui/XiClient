#pragma once
#include "CMoElem.h"

namespace FFXI {
	namespace CYy {
		class CMoD3aElem : public CMoElem {
		public:
			static const CYyClass CMoD3aElemClass;
			const CYyClass* GetRuntimeClass() override final;
			CMoD3aElem() = default;
			virtual ~CMoD3aElem() = default;
			void PrepD3aTSS();
			virtual void OnDraw() override final;
			virtual void VirtElem9() override final;
		};
	}
}