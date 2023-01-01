#pragma once
#include "CXiActor.h"
namespace FFXI {
	namespace CYy {
		class CXiDollActor : public CXiActor {
		public:
			static const CYyClass CXiDollActorClass;
			virtual const CYyClass* GetRuntimeClass() override final;
		};
	}
}