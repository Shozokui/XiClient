#pragma once
#include "CMoResource.h"

namespace FFXI {
	namespace CYy {
		class CYyMeb : public CMoResource {
		public:
			CYyMeb() = default;
			virtual ~CYyMeb() = default;
			virtual void Open() override final;
			virtual void Close() override final;
		};
	}
}