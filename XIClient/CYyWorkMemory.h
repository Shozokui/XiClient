#pragma once
#include "MemoryHeader.h"

namespace FFXI {
	namespace CYy {
		class CYyWorkMemory : public MemoryHeader {
		public:
			const static CYyClass CYyWorkMemoryClass;
			virtual const CYyClass* GetRuntimeClass() final;
		};
	}
}