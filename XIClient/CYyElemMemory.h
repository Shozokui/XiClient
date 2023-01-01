#pragma once
#include "MemoryHeader.h"

namespace FFXI {
	namespace CYy {
		class CYyElemMemory : public MemoryHeader {
		public:
			const static CYyClass CYyElemMemoryClass;
			virtual const CYyClass* GetRuntimeClass() override final;
		};
	}
}