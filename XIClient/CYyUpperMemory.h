#pragma once
#include "MemoryHeader.h"

namespace FFXI {
	namespace CYy {
		class CYyUpperMemory : public MemoryHeader {
		public :
			const static CYyClass CYyUpperMemoryClass;
			virtual const CYyClass* GetRuntimeClass() override final;
		};
	}
}