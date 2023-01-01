#include "Globals.h"
#include "Values.h"
#include "CNtTcpDLL.h"
#include "lpkt_work.h"
#include "CNtDLLInitStruct.h"
#include "CNtTcpQueue.h"
#include "md5.h"
#include <WinSock2.h>
using namespace FFXI::Network;

int GetMD5NextKey(int a1) {
	return a1 + 1;
}
int rapGetMD5NextKey(lpkt_work* a1, int a2) {
	if ((a1->field_134 & 1) == 0)
		return a2 + 1;

	return GetMD5NextKey(a2);
}

void rapMakeMD5(lpkt_work* a1, int key, char* salt, char* output) {
	if ((a1->field_134 & 1) != 0) {
		int length{ 16 };
		if (a1->field_A20 == 0)
			length = strlen(salt);

		char v5[24];
		memcpy(v5, salt, length);
		*(int*)(v5 + length) = key;

		FFXI::Util::md5 ctx{};
		ctx.init();
		ctx.update(v5, length + 4);
		ctx.final(output);
	}
	else {
		memcpy(output, salt, 16);
	}
}

bool ntLoginAnalyzePacket(lpkt_work* a1, char* buffer) {
	int* intdata = (int*)buffer;
	int opcode = intdata[2] & 0x7FFFFFFF;
	switch (opcode) {
	case 3:
		a1->field_138 = rapGetMD5NextKey(a1, a1->field_138);
		return true;
	case FFXI::Constants::Enums::LobbyIncomingOpcodes::LOBBY_IN_LOGIN_GOOD:
		a1->field_138 = rapGetMD5NextKey(a1, intdata[7]);
		a1->ServerExpansions = intdata[8];
		a1->field_1A24 = intdata[9];
		return true;
	case 11:
		a1->field_124 = intdata[14];
		a1->field_128 = intdata[15];
		a1->field_12C = intdata[16];
		a1->field_130 = intdata[17];
		a1->field_138 = rapGetMD5NextKey(a1, a1->field_138);
		return true;
	case FFXI::Constants::Enums::LobbyIncomingOpcodes::LOBBY_IN_CHARLIST:
		a1->ContentIDCount = intdata[7];
		
		for (int i = 0; i < a1->ContentIDCount; ++i) {
			ContentIDInfo* info = a1->ContentIDList + i;
			memcpy_s(info, sizeof(*info), buffer + 32 + sizeof(*info) * i, sizeof(*info));
		}
		a1->field_138 = rapGetMD5NextKey(a1, a1->field_138);
		return true;
	case FFXI::Constants::Enums::LobbyIncomingOpcodes::LOBBY_IN_WORLDLIST:
		a1->field_138 = rapGetMD5NextKey(a1, a1->field_138);
		a1->RealmListCount = intdata[7];
		for (int i = 0; i < a1->RealmListCount; ++i) {
			RealmInfo* info = a1->RealmList + i;
			memcpy_s(info, sizeof(*info), buffer + 32 + sizeof(*info) * i, sizeof(*info));
		}
		return true;
	default:
		return true;
	}
}
void ntLoginHashPacket(int* buffer) {
	if (buffer[0] > 0xFC0 || buffer[0] < 0x1C) return;

	buffer[3] = 0;
	buffer[4] = 0;
	buffer[5] = 0;
	buffer[6] = 0;

	FFXI::Util::md5 ctx{};
	ctx.init();
	ctx.update(buffer, *buffer);
	ctx.final(buffer + 3);
}

int LobbyPktIdentifierCheck(int* buffer) {
	unsigned int packetLength = (unsigned int)buffer[0];
	if (packetLength > 0xFC0 || packetLength < 0x1C)
		return -1;

	int sentHash[4], hash[4];
	memcpy(sentHash, buffer + 3, sizeof(sentHash));
	memset(buffer + 3, 0, sizeof(sentHash));

	FFXI::Util::md5 ctx{};
	ctx.init();
	ctx.update(buffer, packetLength);
	ctx.final(hash);

	memcpy(buffer + 3, sentHash, sizeof(sentHash));
	if (memcmp(sentHash, hash, sizeof(sentHash)) == 0)
		return 0;

	return -1;
}

int LobbyPktCommandGet(char* buffer) {
	int* intdata = (int*)buffer;
	if (LobbyPktIdentifierCheck(intdata) >= 0) 
		return intdata[2] & 0x7FFFFFFF;
	
	return 0;
}

void ntLoginMakePacket(char* buffer, int opcode, lpkt_work* a3) {
	int* intData = (int*)buffer;
	intData[1] = FFXI::Constants::Values::LOBBY_PROTOCOL_TAG;
	intData[2] = opcode & 0x7FFFFFFF;

	switch (intData[2]) {
	case FFXI::Constants::Enums::LobbyOutgoingOpcodes::LOBBY_OUT_REQUEST_WORLDLIST:
		//packet length is 44
		intData[0] = 0x2C;
		rapMakeMD5(a3, a3->field_138, a3->Password, buffer + 28);
		a3->field_138 = rapGetMD5NextKey(a3, a3->field_138);
		ntLoginHashPacket(intData);
		break;
	case FFXI::Constants::Enums::LobbyOutgoingOpcodes::LOBBY_OUT_REQUEST_CONTENTIDS:
		//packet length is 44
		intData[0] = 0x2C;
		rapMakeMD5(a3, a3->field_138, a3->Password, buffer + 28);
		a3->field_138 = rapGetMD5NextKey(a3, a3->field_138);
		ntLoginHashPacket(intData);
		break;
	case FFXI::Constants::Enums::LobbyOutgoingOpcodes::LOBBY_OUT_LOGIN:
		//packet length is 152
		intData[0] = 0x98;

		//copy debug name and null terminate it
		memcpy(intData + 7, a3->DebugName, sizeof(a3->DebugName));
		buffer[43] = 0;

		intData[11] = a3->field_A18;
		intData[12] = a3->field_A1C;

		memcpy(intData + 13, a3->Password, sizeof(a3->Password));

		//copy version string and null terminate it
		memcpy(intData + 29, a3->VersionString, sizeof(a3->VersionString));
		buffer[131] = 0;

		intData[33] = a3->ClientExpansions;
		intData[34] = 0;
		intData[35] = 0;
		intData[36] = 0;
		intData[37] = 0;

		//clear debug name
		//if (a3->field_A20)
		//	buffer[28] = 0;

		ntLoginHashPacket(intData);
		return;
	}
}

void ntTcpClientInit(FFXI::Network::CNtTcpList* a1, int* a2, char* a3, int a4, int a5, int a6, int a7)
{
	FFXI::Network::CNtTcpQueue queue{};
	memset(&queue, 0xCC, sizeof(queue));
	for (int i = 0; i < a4; ++i) {
		FFXI::Network::CNtTcpList* list = a1 + i;
		list->ntTcpListInit(0);
		queue.buffer = a3;
		queue.buflen = a5;
		queue.field_8 = 0;
		for (int j = 0; j < a6; ++j) {
			list->field_803C.ntTcpPushToQueue(&queue);
			a3 += a5;
			queue.buffer = a3;
		}
		for (int k = 0; k < a7; ++k) {
			list->field_4034.ntTcpPushToQueue(&queue);
			a3 += a5;
			queue.buffer = a3;
		}
		if (a2[1])
			list->ntTcpSetAddr((char*)a2[0], a2[1]);
		a2 += 2;
	}

}
FFXI::Network::CNtTcpDLL* CreateLobbyCLT(char* a1, int a2, char* DebugName, char*Password, int* a5, char* VersionString)
{
	FFXI::Network::CNtTcpDLL* clt = new FFXI::Network::CNtTcpDLL();
	FFXI::Network::lpkt_work* ss = new FFXI::Network::lpkt_work();

	memset(clt, 0xFF, sizeof(*clt));
	memset(ss, 0xFF, sizeof(*ss));

	clt->SubStruct = ss;
	clt->field_16D0[0] = 2;

	clt->field_3F12C.field_0 = &clt->field_2F0D4;
	clt->field_3F12C.field_4 = 1;
	clt->field_3F12C.field_8 = 0;
	clt->field_3F12C.field_C = (unsigned char*)&clt->field_16D0;
	clt->field_3F12C.field_10 = 0;
	clt->field_3F12C.field_1C = 0;
	clt->field_3F12C.threadId = 0;
	clt->field_3F12C.thread = nullptr;

	strcpy_s(ss->SomeIP, a1);
	memcpy(ss->DebugName, DebugName, 15);
	memcpy(ss->Password, Password, 16);
	memcpy(ss->VersionString, VersionString, 15);
	
	ss->DebugName[15] = 0;
	ss->VersionString[15] = 0;

	ss->field_A18 = a5[0];
	ss->field_A1C = a5[1];

	ss->field_120 = a2;
	ss->field_134 = 0;
	ss->field_A20 = 0;
	ss->field_13C = 0;
	ss->field_1A24 = 0;
	ss->field_1A28 = 0;
	ss->field_1A2C = 0;
	ss->field_1A30 = 0;

	clt->field_3F12C.field_20.field_10 = -1;
	if (clt->field_3F12C.Init() == false)
		return 0;

	int v10[2] = { 0, 0 };
	ntTcpClientInit(&clt->field_2F0D4, v10, clt->field_16D0 + 1, 1, 0x400, 16, 16);
	clt->field_3F12C.ntTcpThreadStart(0, 1);
	clt->field_4F1A8 = 1;
	return clt;
}
void FFXI::Network::CNtTcpDLL::ntTcpDLLRequestCreateObject(FFXI::Network::CNtTcpDLL** a1, FFXI::Network::ntDLLInitStruct* a2)
{
	WSAData v1{};
	memset(&v1, 0xCC, sizeof(v1));
	WSAStartup(0x202, &v1);

	*a1 = CreateLobbyCLT(a2->Ip, a2->field_C, a2->DebugName, a2->Password, &a2->field_10, a2->VersionString);
}

int FFXI::Network::CNtTcpDLL::ntTcpDLLGetError()
{
	if (NT_SYS::pGcMainSys->ntTcpDLL == nullptr)
		return 0;

	return NT_SYS::pGcMainSys->ntTcpDLL->SubStruct->field_13C;
}

bool FFXI::Network::CNtTcpDLL::ntTcpDLLRequestResetError()
{
	if (this->field_4F1A8) return false;

	if (this->field_2F0D4.field_24 == 6)
		this->field_2F0D4.ntTcpListResetError();
	this->SubStruct->field_13C = 0;
	this->field_4F1A8 = 9;
	return true;
}

int FFXI::Network::CNtTcpDLL::ntTcpDLLRequestOption(int a2)
{
	if (this->field_4F1A8)
		return 0;

	this->SubStruct->field_134 = a2;
	this->field_4F1A8 = 11;
	return 1;
}

bool FFXI::Network::CNtTcpDLL::ntTcpDLLRequestShutdown()
{
	if (this->field_4F1A8)
		return 0;

	if (this->field_2F0D4.ntTcpShutdown() < 0)
		return 0;
	this->field_4F1A8 = 8;
	return 1;
}

bool FFXI::Network::CNtTcpDLL::ntTcpDLLRequestConnection()
{
	if (this->field_4F1A8)
		return false;
	if (this->field_2F0D4.ntTcpSetAddr(this->SubStruct->SomeIP, this->SubStruct->field_120) < 0)
		return false;
	this->field_4F1A8 = 2;
	return true;
}

int FFXI::Network::CNtTcpDLL::ntTcpDLLCheckError()
{
	if (this->field_2F0D4.field_24)
		return this->field_2F0D4.field_24 != 6;

	return this->field_2F0D4.field_24;
}

void FFXI::Network::CNtTcpDLL::ntTcpDLLStatus(int* a2)
{
	if (this->field_4F1A8 == 0) {
		*a2 = 0;
		return;
	}

	CNtTcpQueue que{};
	//canrecvget doesn't do anything looks like
	switch (this->field_4F1A8) {
	case 1:
	case 9:
		this->field_4F1A8 = 0;
		*a2 = 0;
		break;
	case 2:
		if (this->field_2F0D4.field_24 == 6) {
			this->SubStruct->field_13C = 100;
			this->field_4F1A8 = 0;
			*a2 = -1;
		}
		else if (this->field_2F0D4.field_24 == 4) {
			this->field_4F1A8 = 0;
			*a2 = 0;
		}
		else {
			*a2 = 1;
		}
		break;
	case 3:
		if (this->field_2F0D4.ntTcpRecvGet(&que) < 0) {
			*a2 = 1;
			if (this->field_2F0D4.field_24 != 6)
				return;
			this->SubStruct->field_13C = 101;
			this->field_4F1A8 = 0;
			*a2 = -1;
		}
		else {
			int opcode = LobbyPktCommandGet(que.buffer);
			if (opcode != FFXI::Constants::Enums::LobbyIncomingOpcodes::LOBBY_IN_LOGIN_GOOD) {
				if (opcode == FFXI::Constants::Enums::LobbyIncomingOpcodes::LOBBY_IN_ERROR)
					this->SubStruct->field_13C = *(int*)(que.buffer + 32);
				else
					this->SubStruct->field_13C = 101;
				this->field_2F0D4.ntTcpRecvLose(&que);
				this->field_4F1A8 = 0;
				*a2 = -1;
			}
			else {
				if (ntLoginAnalyzePacket(this->SubStruct, que.buffer) == 0) {
					this->SubStruct->field_13C = 103;
					this->field_2F0D4.ntTcpRecvLose(&que);
					this->field_4F1A8 = 0;
					*a2 = -1;
				}

				this->field_2F0D4.ntTcpRecvLose(&que);
				this->field_4F1A8 = 0;
				*a2 = 0;
			}
		}
		break;
	case 4:
		if (this->field_2F0D4.ntTcpRecvGet(&que) < 0) {
			*a2 = 1;
			if (this->field_2F0D4.field_24 != 6)
				return;
			this->SubStruct->field_13C = 101;
			this->field_4F1A8 = 0;
			*a2 = -1;
		}
		else {
			int opcode = LobbyPktCommandGet(que.buffer);
			if (opcode != FFXI::Constants::Enums::LobbyIncomingOpcodes::LOBBY_IN_CHARLIST) {
				if (opcode == FFXI::Constants::Enums::LobbyIncomingOpcodes::LOBBY_IN_ERROR)
					this->SubStruct->field_13C = *(int*)(que.buffer + 32);
				else
					this->SubStruct->field_13C = 101;
				this->field_2F0D4.ntTcpRecvLose(&que);
				this->field_4F1A8 = 0;
				*a2 = -1;
			}
			else {
				if (ntLoginAnalyzePacket(this->SubStruct, que.buffer) == 0) {
					this->SubStruct->field_13C = 103;
					this->field_2F0D4.ntTcpRecvLose(&que);
					this->field_4F1A8 = 0;
					*a2 = -1;
				}

				this->field_2F0D4.ntTcpRecvLose(&que);
				this->field_4F1A8 = 0;
				*a2 = 0;
			}
		}
		break;
	case 8:
		if (this->field_2F0D4.field_24 == 6) {
			this->SubStruct->field_13C = 101;
			this->field_4F1A8 = 0;
			*a2 = -1;
		}
		else if (this->field_2F0D4.field_24 == 0) {
			this->field_4F1A8 = 0;
			*a2 = 0;
		}
		else {
			*a2 = 1;
		}
		break;
	case 0xB:
	case 0xC:
	case 0xD:
	case 0xE:
	case 0xF:
		this->field_4F1A8 = 0;
		*a2 = 0;
		return;
	case 0x12:
	case 0x14:
	case 0x15:
		break;
	case 0x13:
		if (this->field_2F0D4.ntTcpRecvGet(&que) < 0) {
			*a2 = 1;
			if (this->field_2F0D4.field_24 == 6) {
				if (this->field_2F0D4.field_28)
					this->SubStruct->field_13C = this->field_2F0D4.field_28;
				else
					this->SubStruct->field_13C = 101;
				this->field_4F1A8 = 0;
				*a2 = -1;
			}
		}
		else {
			int opcode = LobbyPktCommandGet(que.buffer);
			if (opcode != FFXI::Constants::Enums::LobbyIncomingOpcodes::LOBBY_IN_WORLDLIST) {
				if (opcode == FFXI::Constants::Enums::LobbyIncomingOpcodes::LOBBY_IN_ERROR) {
					this->SubStruct->field_13C = *(int*)(que.buffer + 32);
					this->SubStruct->field_138 = rapGetMD5NextKey(this->SubStruct, this->SubStruct->field_138);
					this->field_2F0D4.ntTcpRecvLose(&que);
				}
				else {
					this->field_13C = 101;
					this->field_2F0D4.ntTcpRecvLose(&que);
				}
			}
			else {
				if (ntLoginAnalyzePacket(this->SubStruct, que.buffer) == 0) {
					this->SubStruct->field_13C = 103;
					this->field_2F0D4.ntTcpRecvLose(&que);
					this->field_4F1A8 = 0;
					*a2 = -1;
				}
				else {
					this->field_2F0D4.ntTcpRecvLose(&que);
					this->field_4F1A8 = 0;
					*a2 = 0;
				}
			}
		}
	default:
		return;
	}
}

bool FFXI::Network::CNtTcpDLL::ntTcpDLLRequestLobbyLogin()
{
	if (this->field_4F1A8) return false;
	if (this->field_2F0D4.ntTcpCanSendGet() < 0) return false;

	CNtTcpQueue v2{};
	this->field_2F0D4.ntTcpSendGet(&v2);
	ntLoginMakePacket(v2.buffer, FFXI::Constants::Enums::LobbyOutgoingOpcodes::LOBBY_OUT_LOGIN, this->SubStruct);
	this->field_2F0D4.ntTcpSendLose(&v2);
	this->field_4F1A8 = 3;
	return 1;
}

bool FFXI::Network::CNtTcpDLL::ntTcpDLLRequestGetChr()
{
	if (this->field_4F1A8) return false;
	if (this->field_2F0D4.ntTcpCanSendGet() < 0) return false;

	CNtTcpQueue v2{};
	this->field_2F0D4.ntTcpSendGet(&v2);
	ntLoginMakePacket(v2.buffer, FFXI::Constants::Enums::LobbyOutgoingOpcodes::LOBBY_OUT_REQUEST_CONTENTIDS, this->SubStruct);
	this->field_2F0D4.ntTcpSendLose(&v2);
	this->field_4F1A8 = 4;
	return true;
}

bool FFXI::Network::CNtTcpDLL::ntTcpDLLRequestQueryWorldList()
{
	if (this->field_4F1A8) return false;
	if (this->field_2F0D4.ntTcpCanSendGet() < 0) return false;

	CNtTcpQueue v2{};
	this->field_2F0D4.ntTcpSendGet(&v2);
	ntLoginMakePacket(v2.buffer, FFXI::Constants::Enums::LobbyOutgoingOpcodes::LOBBY_OUT_REQUEST_WORLDLIST, this->SubStruct);
	this->field_2F0D4.ntTcpSendLose(&v2);
	this->field_4F1A8 = 19;
	return true;
}

void FFXI::Network::CNtTcpDLL::ntTcpDLLSetExCodeClient(int a2)
{
	this->SubStruct->ClientExpansions = a2;
}

void FFXI::Network::CNtTcpDLL::ntTcpDLLGetExCodeServer(int* a2)
{
	*a2 = this->SubStruct->ServerExpansions;
}

bool FFXI::Network::CNtTcpDLL::ntTcpDLLRequestGetWork(lpkt_work* a2)
{
	if (this->field_4F1A8) return false;
	*a2 = *this->SubStruct;
	this->field_4F1A8 = 14;
	return true;
}

void FFXI::Network::CNtTcpDLL::ntTcpDLLSetAthCode(const void* a2)
{
	this->SubStruct->field_A20 = 1;
	memcpy(this->SubStruct->AuthCode, a2, sizeof(this->SubStruct->AuthCode));
}

void FFXI::Network::CNtTcpDLL::ntTcpDLLSetPasswd(const void* a2)
{
	memcpy(this->SubStruct->Password, a2, sizeof(this->SubStruct->Password));
}

void FFXI::Network::CNtTcpDLL::ntTcpDLLSetClientCode(int* a2)
{
	this->SubStruct->field_A18 = a2[0];
	this->SubStruct->field_A1C = a2[1];
}
