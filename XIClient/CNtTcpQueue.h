#pragma once
namespace FFXI {
	namespace Network {
		class CNtTcpQueue {
		public :
			char* buffer;
			int buflen;
			int field_8;
			int bufOffset;
		};
	}
}