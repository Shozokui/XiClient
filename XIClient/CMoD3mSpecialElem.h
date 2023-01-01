#pragma once
#include "CMoD3mElem.h"

namespace FFXI {
	namespace CYy {
		class CMoD3mSpecialElem : public CMoElem {
		public:
			CMoD3mSpecialElem();
			virtual ~CMoD3mSpecialElem() = default;
			virtual void VirtElem3(unsigned short) override final;
			virtual int VirtElem4() override final;
			int field_198;
		};
	}
}