#pragma once
#include "KzCib.h"
#include "CMoResource.h"

namespace FFXI {
	namespace CYy {
		class CMoCib : public CMoResource {
		public:
			virtual void Open() override final;
			virtual void Close() override final;
			CMoCib();
			virtual ~CMoCib() = default;
			KzCib info;
		};
	}
}