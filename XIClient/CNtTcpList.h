#pragma once
#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS //allow use of gethostbyname and inet_addr
#include "CNtTcpQueue.h"
#include "CNtTcpQueueList.h"
#include <WinSock2.h>
namespace FFXI {
	namespace Network {
		class CNtTcpList {
		public:
			void ntTcpListInit(int);
			void ntTcpListResetError();
			int ntTcpCloseSocket();
			bool ntTcpCreateEvent();
			int ntTcpSelectEvent(int);
			bool ntTcpCloseEvent();
			int ntTcpCloseList();
			int ntTcpShutdown();
			int ntTcpCanSendGet();
			int ntTcpSendGet(CNtTcpQueue*);
			int ntTcpSendLose(CNtTcpQueue*);
			int ntTcpRecvGet(CNtTcpQueue*);
			int ntTcpRecvLose(CNtTcpQueue*);
			int ntTcpSetAddr(char*, int);
			int GetWinsockEvents();
			CNtTcpQueue field_0;
			SOCKET field_10;
			int field_14;
			int field_18;
			int field_1C;
			int field_20;
			int field_24;
			int field_28;
			CNtTcpQueueList field_2C;
			CNtTcpQueueList field_4034;
			CNtTcpQueueList field_803C;
			CNtTcpQueueList field_C044;
			HANDLE field_1004C;
			int WinsockEvents;
			int WinsockErrors;
		};
	}
}