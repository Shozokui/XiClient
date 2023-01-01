#pragma once
#include "CNtTcpQueue.h"
namespace FFXI {
	namespace Network {
		class CNtTcpQueueList {
		public:
			int ntTcpPushToQueue(CNtTcpQueue*);
			int ntTcpPopFromQueue(CNtTcpQueue*);
			int ntTcpCanRecvGet();
			int ntTcpPeekBottom(CNtTcpQueue**);
			CNtTcpQueue field_0[0x400];
			int StackTop;
			int StackBottom;
		};
	}
}