#pragma once

namespace FFXI {
	namespace CTk {
		class CTkMenuPrimitive;
		class PrimTableEntry {
		public:
			const char* MenuName;
			CTkMenuPrimitive** PrimMenuPtr;
			int SomeInt;
			char SomeByte1;
			char SomeByte2;
			short SomeWord;
		};
	}
}