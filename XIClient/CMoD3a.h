#pragma once
#include "CMoResource.h"

namespace FFXI {
	namespace CYy {
		class CMoD3a : public CMoResource {
		public:
			CMoD3a() = default;
			virtual ~CMoD3a() = default;
			virtual void Open() override final;
			virtual void Close() override final;
		};
	}
}