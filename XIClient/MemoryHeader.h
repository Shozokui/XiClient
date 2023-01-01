#pragma once
#include "CYyObject.h"
namespace FFXI {
	namespace CYy {
		class MemoryHeader : public CYyObject{
		public:
			bool occupied { false };
			MemoryHeader* NextEntry{ nullptr };
			MemoryHeader* PreviousEntry{ nullptr };
			MemoryHeader* field_10{ nullptr };
			MemoryHeader* field_14{ nullptr };
			int field_18{ 0 };
			int* field_1C{ nullptr };
			int GetSize();
			int GetFreeSize();
			MemoryHeader* Delete();
		};
	}
}