#pragma once

namespace FFXI {
	namespace CTk {
		class CTkResourceID;
		class CTkMenuFrame {
		public:
			CTkMenuFrame();
			~CTkMenuFrame();
			void Store(void**);
			CTkResourceID* GetResourceID(int);
			const unsigned char* Buffer;
			CTkResourceID* ResourceIDs;
			char* field_8;
			char* field_C;
		};
	}
}