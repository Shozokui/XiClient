#pragma once

namespace FFXI {
	namespace CTk {
		class CTkResourceID;
		class CTkMenuButton {
		public:
			CTkMenuButton();
			~CTkMenuButton();
			void Store(void**);
			const unsigned char* Buffer;
			CTkResourceID* ResourceIDs;
			char* field_8;
			char* field_C;
			int field_10;
		};
	}
}