#pragma once
#include "MemoryHeader.h"

namespace FFXI {
	namespace CYy {
		class CYyMenuMemory : public MemoryHeader {
		public:
			const static CYyClass CYyMenuMemoryClass;
			virtual const CYyClass* GetRuntimeClass() final;
		};
	}
}