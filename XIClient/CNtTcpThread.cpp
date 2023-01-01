#include "CNtTcpThread.h"
#include <WinSock2.h>
#include <math.h>
#include "Values.h"
using namespace FFXI::Network;

int WaitEvent(DWORD* a1, int* a2, FFXI::Network::CNtTcpThread* a3) {
	WSANETWORKEVENTS events{};
	if (a1[0] == 0) return -1;
	DWORD v10 = WSAWaitForMultipleEvents(a1[0], (const HANDLE*)a1 + 1, false, a2[1], false);
	a3->field_1C = 0;
	if (v10 == WAIT_FAILED)
		return -1;
	for (unsigned int i = 0; i < a1[0]; ++i) {
		FFXI::Network::CNtTcpList* list = (FFXI::Network::CNtTcpList*)a1[301 + i];
		list->WinsockErrors = 0;
		if (list->field_10 == -1) {
			WSAResetEvent(list->field_1004C);
		}
		else {
			DWORD v10 = WSAEnumNetworkEvents(list->field_10, list->field_1004C, &events);
			if (v10 == SOCKET_ERROR) {
				list->WinsockEvents = 0;
				list->WinsockErrors = 0;
				return -1;
			}

			list->WinsockEvents = events.lNetworkEvents;
			for (int j = 0; j < 10; ++j) {
				if (((1 << j) & list->WinsockEvents) != 0) {
					list->WinsockErrors = events.iErrorCode[j];
					break;
				}
			}
			if ((list->WinsockEvents & 0x20) != 0)
				list->WinsockEvents |= 1;

			int optlen{ 4 };
			DWORD optval{};
			if (getsockopt(list->field_10, SOL_SOCKET, SO_ERROR, (char*)&optval, &optlen) < 0) {
				list->WinsockErrors |= 1;
			}
			if (optval)
				OutputDebugStringA("error state %d\x0A");
			list->WinsockErrors |= optval;
		}
	}
	return 1;
}

void IncThing(FFXI::Network::CNtTcpList* a1, DWORD* a2) {
	a2[*a2 + 1] = (DWORD)a1->field_1004C;
	a2[(*a2)++ + 301] = (DWORD)a1;
}

DWORD __stdcall HandleTcp(LPVOID param) {
	FFXI::Network::CNtTcpThread* thread = (FFXI::Network::CNtTcpThread*)param;
	OutputDebugStringA("client start\x0A");

	for (int i = 0; i < thread->field_4; ++i)
		(thread->field_0 + i)->field_10 = -1;

	DWORD v7[1396];
	int v16[2] = { 0, 100 };
	int v5{};
	LABEL_RESTART:
	while (true) {
		v7[0] = 0;
		for (int i = 0; i < thread->field_4; ++i) {
			if ((thread->field_0 + i)->field_24 >= 2 && (thread->field_0 + i)->field_24 < 6) {
				IncThing(thread->field_0 + i, v7);
			}
		}
		IncThing(&thread->field_20, v7);
		v5 = WaitEvent(v7, v16, thread);
		if (v5 != -1) break;
		OutputDebugStringA("client select error\x0A");
		Sleep(1000);
	}

	CNtTcpQueue v2{};
	DWORD argp = 1;
	DWORD optval = 0x10000;
	for (int i=0; ; ++i) {
		if (i >= thread->field_4)
			goto LABEL_RESTART;
		if (thread->field_C[0] != 2)
			break;
		int v4 = GetTickCount() / 1000;
		CNtTcpList* list = thread->field_0 + i;
		int v13 = list->field_2C.ntTcpCanRecvGet();
		if (v13 == 1) {
			list->field_24 = 1;
			list->field_1C = v4;
			list->field_2C.ntTcpPopFromQueue(&v2);
			list->field_0 = v2;
			v2.field_8 = 0;
			list->field_4034.ntTcpPushToQueue(&v2);
		}
		else if (v13 == 5) {
			list->ntTcpCloseList();
			OutputDebugStringA("client shutdown\x0A");
			list->field_2C.ntTcpPopFromQueue(&v2);
			list->field_24 = 0;
			v2.field_8 = 0;
			list->field_4034.ntTcpPushToQueue(&v2);
			continue;
		}
		if (list->field_24 == 1) {
			list->field_1C = v4;
			list->field_10 = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			if (list->field_10 == -1) {
				OutputDebugStringA("client socket\x0A");
				Sleep(1000);
				continue;
			}
			if (ioctlsocket(list->field_10, FIONBIO, &argp) == SOCKET_ERROR) {
				OutputDebugStringA("ioctrl\x0A");
				Sleep(1000);
				continue;
			}
			if (setsockopt(list->field_10, SOL_SOCKET, SO_DEBUG, (const char*)&argp, sizeof(argp)) == SOCKET_ERROR)
			{
				list->ntTcpCloseSocket();
OutputDebugStringA("client setsockopt\0xA");
Sleep(0x3E8u);
continue;
			}
			if (setsockopt(list->field_10, SOL_SOCKET, SO_REUSEADDR, (const char*)&argp, sizeof(argp)) == SOCKET_ERROR)
			{
				list->ntTcpCloseSocket();
				OutputDebugStringA("client setsockopt\0xA");
				Sleep(0x3E8u);
				continue;
			}

			optval = 0x10000;
			if (setsockopt(list->field_10, SOL_SOCKET, SO_RCVBUF, (const char*)&optval, sizeof(optval)) == SOCKET_ERROR)
			{
				list->ntTcpCloseSocket();
				OutputDebugStringA("client setsockopt\0xA");
				Sleep(0x3E8u);
				continue;
			}

			optval = 0x10000;
			if (setsockopt(list->field_10, SOL_SOCKET, SO_SNDBUF, (const char*)&optval, sizeof(optval)) == SOCKET_ERROR)
			{
				list->ntTcpCloseSocket();
				OutputDebugStringA("client setsockopt\0xA");
				Sleep(0x3E8u);
				continue;
			}
			if (list->ntTcpCreateEvent() == 0) {
				OutputDebugStringA("create event\0xA");
				list->ntTcpCloseList();
				Sleep(0x3E8u);
				continue;
			}
			if (list->ntTcpSelectEvent(FD_CONNECT) == SOCKET_ERROR) {
				OutputDebugStringA("event select\0xA");
				list->ntTcpCloseList();
				Sleep(0x3E8u);
				continue;
			}
			list->field_24 = 2;
		}
		if (list->field_24 == 2) {
			OutputDebugStringA("trying connecting\x0A");
			const sockaddr* sock = (const sockaddr*)list->field_0.buffer;
			connect(list->field_10, sock, sizeof(*sock));
			list->field_24 = 3;
			continue;
		}

		if (list->field_24 == 3) {
			if (v5 > 0 && (list->GetWinsockEvents() & FD_CONNECT)) {
				if (list->WinsockErrors) {
					OutputDebugStringA("connection refused\x0A");
					list->ntTcpShutdown();
					list->field_24 = 7;
					continue;
				}
				if (list->ntTcpSelectEvent(NULL) == -1) {
					OutputDebugStringA("event select\x0A");
					list->ntTcpShutdown();
					Sleep(1000);
					continue;
				}
				if (list->ntTcpSelectEvent(FD_CLOSE_BIT | FD_READ) == -1) {
					OutputDebugStringA("event select\x0A");
					list->ntTcpShutdown();
					Sleep(1000);
					continue;
				}
				OutputDebugStringA("connection succeed\x0A");
				list->field_24 = 4;
			}

			if (abs(list->field_1C - v4) > 10) {
				OutputDebugStringA("client connection timeout\x0A");
				list->ntTcpShutdown();
				list->field_24 = 6;
				continue;
			}
		}

		if (list->field_24 == 7 && abs(list->field_1C - v4) > 1) {
			OutputDebugStringA("client error wait\x0A");
			list->field_24 = 6;
		}
		else if (list->field_24 == 4) {
			if (list->WinsockErrors) {
				OutputDebugStringA("Client Link Dead 1\x0A");
				list->ntTcpShutdown();
				list->field_24 = 7;
			}
			else if (list->field_20 && abs(v4 - list->field_1C) < list->field_20) {
				list->ntTcpShutdown();
				list->field_24 = 6;
			}
			else if (v5 > 0) {
				if ((list->GetWinsockEvents() & FD_READ) != 0) {
					CNtTcpQueue* q{ nullptr };
					while (list->field_803C.ntTcpPeekBottom(&q) >= 0) {
						int messageLength { 0 };
						int messageOffset { 0 };
						int bytesRead{ 0 };
						if (q->bufOffset) {
							messageLength = *(int*)q->buffer - q->bufOffset;
							messageOffset = q->bufOffset;
						}
						else {

							bytesRead = recv(list->field_10, (char*)&messageLength, 4, MSG_PEEK);
							if (bytesRead == 0) {
								OutputDebugStringA("Client Link Dead 2\x0A");
								list->ntTcpShutdown();
								list->field_24 = 6;
								break;
							}
							if (bytesRead < 4)
								break;
							if (messageLength < 8 || messageLength > 0xB68) {
								OutputDebugStringA("client illigal pktsize error\x0A");
								list->ntTcpShutdown();
								list->field_24 = 6;
								break;
							}
							*(int*)q->buffer = messageLength;
						}

						if (ioctlsocket(list->field_10, FIONREAD, &optval) == SOCKET_ERROR) {
							OutputDebugStringA("client FIONREAD error\x0A");
							list->ntTcpShutdown();
							list->field_24 = 6;
							break;
						}

						do {
							bytesRead = recv(list->field_10, q->buffer + messageOffset, messageLength, NULL);
							if (bytesRead <= 0)
								break;
							messageOffset += bytesRead;
							messageLength -= bytesRead;
							optval -= 12;
							if (messageLength == 0)
								break;
						} while (optval > 0);

						if (bytesRead <= 0) {
							OutputDebugStringA("Recv Client error!\x0A");
							list->field_803C.ntTcpPopFromQueue(&v2);
							list->field_C044.ntTcpPushToQueue(&v2);
							list->ntTcpShutdown();
							list->field_24 = 6;
							break;
						}

						if (messageOffset != *(int*)q->buffer) {
							OutputDebugStringA("Recv Client fragment %d\x0A");
							q->bufOffset = messageOffset;
							break;
						}

						if (*(int*)(q->buffer + 4) != FFXI::Constants::Values::LOBBY_PROTOCOL_TAG) {
							OutputDebugStringA("client illigal pkt error\x0A");
							list->ntTcpShutdown();
							list->field_24 = 6;
							break;
						}
						OutputDebugStringA("Recv Client %d\x0A");
						list->field_1C = v4;
						list->field_803C.ntTcpPopFromQueue(&v2);
						list->field_C044.ntTcpPushToQueue(&v2);
					}
				}
				while (true) {
					int v13 = list->field_2C.ntTcpCanRecvGet();
					CNtTcpQueue* q{ nullptr };
					if (v13 < 0 || v13 || list->field_24 == 6 || list->field_2C.ntTcpPeekBottom(&q) < 0)
						break;

					int bufferOffset = q->bufOffset;
					int sendLength = *(int*)q->buffer - bufferOffset;
					int sentCount{ 0 };
					do {
						sentCount = send(list->field_10, q->buffer + bufferOffset, sendLength, NULL);
						if (sentCount <= 0)
							break;
						sendLength -= sentCount;
						bufferOffset += sentCount;
					} while (sendLength);

					if (sentCount <= 0) {
						if (WSAGetLastError() == WSAEWOULDBLOCK) {
							OutputDebugStringA("client send buffer over flow %d\x0A");
							q->bufOffset = bufferOffset;
						}
						else {
							OutputDebugStringA("client send error\x0A");
							list->field_2C.ntTcpPopFromQueue(&v2);
							list->field_4034.ntTcpPushToQueue(&v2);
							list->ntTcpShutdown();
							list->field_24 = 6;
						}
						break;
					}

					list->field_1C = v4;
					list->field_2C.ntTcpPopFromQueue(&v2);
					list->field_4034.ntTcpPushToQueue(&v2);
				}
			}
		}
	}

	for (int i = 0; i < thread->field_4; ++i) {
		CNtTcpList* list = thread->field_0 + i;
		ioctlsocket(list->field_10, FIONBIO, &argp);
		list->ntTcpShutdown();
	}
	thread->field_20.ntTcpCloseEvent();
	OutputDebugStringA(" END OF TCP CLIENT THREAD\x0A");
	thread->field_10 = 1;
	return 0;
}

bool FFXI::Network::CNtTcpThread::Init()
{
	this->field_20.field_10 = -1;
	return this->field_20.ntTcpCreateEvent();
}

bool FFXI::Network::CNtTcpThread::ntTcpThreadStart(int a2, bool a3)
{
	if (this == nullptr) return false;

	if (a2)
		this->field_8 = a2;

	DWORD threadID = 0xCCCCCCCC;
	HANDLE v4{ nullptr };
	if (a3)
		v4 = CreateThread(0, 0, CNtTcpThread::TcpThread, this, 0, &threadID);
	else
		throw "not implemented";

	if (v4) {
		this->threadId = threadID;
		this->thread = v4;
		this->field_10 = 0;
		return threadID != NULL;
	}
	else {
		OutputDebugString(L"thread creattion failed\x0A");
		return false;
	}
}

LPTHREAD_START_ROUTINE CNtTcpThread::TcpThread = HandleTcp;