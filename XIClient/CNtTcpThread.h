#pragma once
#define WIN32_LEAN_AND_MEAN
#include "CNtTcpList.h"
#include <wtypes.h>
namespace FFXI {
	namespace Network {
		class CNtTcpThread {
		public:
			static LPTHREAD_START_ROUTINE TcpThread;
			bool Init();
			bool ntTcpThreadStart(int, bool);
			CNtTcpList* field_0;
			int field_4;
			int field_8;
			unsigned char* field_C;
			int field_10;
			DWORD threadId;
			HANDLE thread;
			int field_1C;
			CNtTcpList field_20;
		};
	}
}