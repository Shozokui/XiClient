#pragma once
#include "MemoryHeader.h"

namespace FFXI {
	namespace CYy {
		class CYyLoadMemory : public MemoryHeader {
		public:
			const static CYyClass CYyLoadMemoryClass;
			virtual const CYyClass* GetRuntimeClass() override final;
		};
	}
}