#pragma once
namespace FFXI {
	namespace Network {
		class ntDLLInitStruct {
		public:
			char* DebugName;
			char* Password;
			char* Ip;
			int field_C;
			int field_10;
			int field_14;
			char VersionString[16];
		};
	}
}