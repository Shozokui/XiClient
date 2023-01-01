#pragma once
#include "MemoryHeader.h"

namespace FFXI {
	namespace CYy {
		class CYyExMemory : public MemoryHeader {
		public:
			const static CYyClass CYyExMemoryClass;
			virtual const CYyClass* GetRuntimeClass() final;
		};
	}
}