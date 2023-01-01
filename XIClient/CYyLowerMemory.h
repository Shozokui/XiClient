#pragma once
#include "MemoryHeader.h"

namespace FFXI {
	namespace CYy {
		class CYyLowerMemory : public MemoryHeader {
		public:
			const static CYyClass CYyLowerMemoryClass;
			virtual const CYyClass* GetRuntimeClass() override final;
		};
	}
}