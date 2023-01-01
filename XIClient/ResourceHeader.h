#pragma once
#include <string>
namespace FFXI {
	namespace CYy {
		class CMoResource;
		class ResourceHeader {
		public:
			inline void Parse(char* p_buf) {
				char* me = reinterpret_cast<char*>(this);
				memcpy(me, p_buf, sizeof(ResourceHeader));
			}
			int FourCC;
			unsigned int Type_Size;
			CMoResource** RPLHead;
			CMoResource** LastResourceCreated;
		};
	}
}