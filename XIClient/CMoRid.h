#pragma once
#include "CMoResource.h"

namespace FFXI {
	namespace CYy {
		class CMoRid : public CMoResource {
		public:
			CMoRid();
			virtual ~CMoRid() = default;
			virtual void Open() override final;
		};
	}
}