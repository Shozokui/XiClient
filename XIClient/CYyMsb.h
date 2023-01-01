#pragma once
#include "CMoResource.h"
namespace FFXI {
	namespace CYy {
		class CYyMsb : public CMoResource {
		public:
			virtual void Open() override final;
			virtual void Close() override final;
			CYyMsb() = default;
			virtual ~CYyMsb() = default;
		};
	}
}