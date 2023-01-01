#pragma once
namespace FFXI {
	namespace CYy {
		class RidStruct;
		class RidListNode {
		public:
			RidListNode* Next{ nullptr };
			char* data{ nullptr };
			int field_8{ 0 };
			RidStruct* field_C{ 0 };
			int field_10{ 0 };
		};
	}
}