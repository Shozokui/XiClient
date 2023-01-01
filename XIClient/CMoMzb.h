#pragma once
#include "CMoResource.h"

namespace FFXI {
	namespace CYy {
		class CMoMzb : public CMoResource {
		public:
			CMoMzb();
			virtual ~CMoMzb() = default;
			virtual void Open() override final;
		};
	}
}