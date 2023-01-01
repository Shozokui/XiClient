#include "CNtTcpList.h"
#include "CNtTcpQueue.h"
#include <string>
using namespace FFXI::Network;

void FFXI::Network::CNtTcpList::ntTcpListInit(int a2)
{
	memset(this, 0, 0x10u);
	this->field_10 = -1;
	this->field_20 = a2;
	this->field_2C.StackTop = 0;
	this->field_2C.StackBottom = 0;
	this->field_4034.StackTop = 0;
	this->field_4034.StackBottom = 0;
	this->field_803C.StackTop = 0;
	this->field_803C.StackBottom = 0;
	this->field_C044.StackTop = 0;
	this->field_C044.StackBottom = 0;
	this->field_24 = 0;
	this->field_28 = 0;
	this->field_1004C = 0;
	this->WinsockEvents = 0;
	this->WinsockErrors = 0;
}

void FFXI::Network::CNtTcpList::ntTcpListResetError()
{
	CNtTcpQueue v1{};
	memset(&v1, 0xCC, sizeof(v1));

	while (this->field_C044.ntTcpPopFromQueue(&v1))
		this->field_803C.ntTcpPushToQueue(&v1);
	while (this->field_2C.ntTcpPopFromQueue(&v1))
		this->field_4034.ntTcpPushToQueue(&v1);

	this->field_24 = 0;
	this->field_28 = 0;
}

int FFXI::Network::CNtTcpList::ntTcpCloseSocket()
{
	if (closesocket(this->field_10) == SOCKET_ERROR)
		return -1;

	return 0;
}

bool FFXI::Network::CNtTcpList::ntTcpCreateEvent()
{
	this->field_1004C = WSACreateEvent();
	return this->field_1004C != WSA_INVALID_EVENT;
}

int FFXI::Network::CNtTcpList::ntTcpSelectEvent(int a2)
{
	return WSAEventSelect(this->field_10, this->field_1004C, a2);
}

bool FFXI::Network::CNtTcpList::ntTcpCloseEvent()
{
	return WSACloseEvent(this->field_1004C);
}

int FFXI::Network::CNtTcpList::ntTcpCloseList()
{
	int retval = 0;
	if (this->field_10 != -1) {
		if (this->ntTcpCloseSocket() == -1) {
			retval = -1;
		}
		this->field_10 = -1;
	}

	if (this->field_1004C) {
		if (this->ntTcpCloseEvent() == false)
			retval = -1;

		this->field_1004C = 0;
	}
		
	return retval;
}

int FFXI::Network::CNtTcpList::ntTcpShutdown()
{
	CNtTcpQueue v2{};
	memset(&v2, 0xCC, sizeof(v2));
	if (this->field_4034.ntTcpPopFromQueue(&v2) < 0)
		return -1;

	v2.field_8 = 5;
	this->field_2C.ntTcpPushToQueue(&v2);
	return 0;
}

int FFXI::Network::CNtTcpList::ntTcpCanSendGet()
{
	return this->field_4034.ntTcpCanRecvGet();
}

int FFXI::Network::CNtTcpList::ntTcpSendGet(CNtTcpQueue* a2)
{
	return this->field_4034.ntTcpPopFromQueue(a2);
}

int FFXI::Network::CNtTcpList::ntTcpSendLose(CNtTcpQueue* a2)
{
	return this->field_2C.ntTcpPushToQueue(a2);
}

int FFXI::Network::CNtTcpList::ntTcpRecvGet(CNtTcpQueue* a2)
{
	return this->field_C044.ntTcpPopFromQueue(a2);
}

int FFXI::Network::CNtTcpList::ntTcpRecvLose(CNtTcpQueue* a2)
{
	return this->field_803C.ntTcpPushToQueue(a2);
}

int FFXI::Network::CNtTcpList::ntTcpSetAddr(char* a2, int a3)
{
	CNtTcpQueue v2{};
	memset(&v2, 0xCC, sizeof(v2));
	hostent* v5 = gethostbyname(a2);
	if (v5) {
		if (this->field_4034.ntTcpPopFromQueue(&v2) < 0) {
			return -1;
		}
		else {
			sockaddr_in v88{};
			v88.sin_family = AF_INET;
			v88.sin_port = htons(a3);
			memcpy(&v88.sin_addr, *v5->h_addr_list, v5->h_length);
			memcpy(v2.buffer, &v88, sizeof(v88));
			v2.field_8 = 1;
			this->field_2C.ntTcpPushToQueue(&v2);
			return 0;
		}
	}
	else {
		OutputDebugStringA("No such host\x0A");
		return -1;
	}
}

int FFXI::Network::CNtTcpList::GetWinsockEvents()
{
	if (this->WinsockErrors)
		return 0;

	return this->WinsockEvents;
}
