#pragma once

namespace FFXI {
	namespace CTk {
		class CTkResourceID;
		class CTkMenuCursol {
		public:
			CTkMenuCursol();
			~CTkMenuCursol();
			void SetCursolID(CTkResourceID*, unsigned short, unsigned short);
			char field_0[16];
			char field_10;
			char field_11;
			char field_12;
			char field_13;
			int field_14;
			int field_18;
			int field_1C;
			short field_20;
			unsigned short field_22;
			unsigned short field_24;
		};
	}
}