#pragma once
#include "CMoResource.h"

namespace FFXI {
	namespace CYy {
		class CMoMmb : public CMoResource {
		public:
			CMoMmb();
			virtual ~CMoMmb() = default;
			virtual void Open() override final;
			int field_30;
		};
	}
}