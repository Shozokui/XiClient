#pragma once
#include "UdpSubStruct.h"

namespace FFXI {
	namespace Network {
		class CNtUdpSys {
		public:
			UdpSubStruct subs[4];
			int MyIp;
			unsigned char field_1AFC4;
			char field_1AFC5;
			char field_1AFC6;
			char field_1AFC7;
		};
	}
}