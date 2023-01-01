#pragma once

namespace FFXI {
	namespace CTk {
		class CTkObject {
		public:
			virtual ~CTkObject() = default;
			CTkObject();
			short DrawType{ 0 };
			short field_2{ 0 };
		};
	}
}