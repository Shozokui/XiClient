#pragma once

namespace FFXI {
	namespace CTk {
		class CTkObject;
		class CTkNode {
		public :
			CTkNode();
			CTkObject* GetTkObject();
			CTkNode* field_0;
			CTkNode* field_4;
			CTkNode* field_8;
			CTkNode* field_C;
			CTkObject* field_10;
			bool field_14;
		};
	}
}