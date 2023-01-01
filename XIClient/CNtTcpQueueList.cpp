#include "Globals.h"
#include "CNtTcpQueueList.h"
using namespace FFXI::Network;

int FFXI::Network::CNtTcpQueueList::ntTcpPushToQueue(CNtTcpQueue* a2)
{
	int nextIndex = (this->StackTop + 1) & 0x3FF;
	if (nextIndex == this->StackBottom) {
		OutputDebugString(L"cannot push to que\x0A");
		return -1;
	}
	else {
		a2->bufOffset = 0;
		this->field_0[this->StackTop] = *a2;
		this->StackTop = nextIndex;
		return 0;
	}
}

int FFXI::Network::CNtTcpQueueList::ntTcpPopFromQueue(CNtTcpQueue* a2)
{
	if (this->StackBottom == this->StackTop) return -1;
	*a2 = this->field_0[this->StackBottom];
	this->StackBottom += 1;
	this->StackBottom &= 0x3FF;
	return 0;
}

int FFXI::Network::CNtTcpQueueList::ntTcpCanRecvGet()
{
	if (this->StackBottom == this->StackTop)
		return -1;

	return this->field_0[this->StackBottom].field_8;
}

int FFXI::Network::CNtTcpQueueList::ntTcpPeekBottom(CNtTcpQueue** a2)
{
	if (this->StackTop == this->StackBottom)
		return -1;
	*a2 = this->field_0 + this->StackBottom;
	return 0;
}
