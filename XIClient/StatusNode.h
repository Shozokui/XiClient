#pragma once
#include <wtypes.h>
namespace FFXI {
	namespace CYy {
		class StatusNode {
		public:
			StatusNode();
			~StatusNode();
			void ApplyRS(DWORD*);
			void ApplyTSS(DWORD*);
			void ApplyRSandTSS(DWORD*, DWORD*);
			void Link();
			DWORD token;
			StatusNode* field_4;
			StatusNode* field_8;
		};
	}
}