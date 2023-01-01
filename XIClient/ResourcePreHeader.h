#pragma once

namespace FFXI {
	namespace CYy {
		class CMoResource;
		class ResourcePreHeader {
		public:
			void Init();
			void SetNextRPL(CMoResource*);
			void SetPrevRPL(CMoResource*);
			CMoResource** PreviousRPL;
			CMoResource** NextRPL;
			CMoResource** WeirdPointer1;
			CMoResource** LastResourceCreatedMine;
			CMoResource** PointerToThisInRPL;
			unsigned short field_14;
			unsigned short field_16;
			int field_18;
		};
	}
}