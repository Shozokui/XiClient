#include "NT_SYS_BASE.h"
#include "CommandLineConfig.h"
#include "Globals.h"
#include "CApp.h"
#include "Strings.h"
#define _WINSOCK_DEPRECATED_NO_WARNINGS //allow use of gethostbyname and inet_addr
#include <WinSock2.h>
#include <timeapi.h>
using namespace FFXI::Network;

CNtGame NT_SYS_BASE::g_Game{};
CNtGame* NT_SYS_BASE::pGame{ nullptr };
CNtDebug NT_SYS_BASE::g_Debug{};
CNtDebug* NT_SYS_BASE::pDebug{ nullptr };
CNtTimeSys NT_SYS_BASE::g_TimeSys{};
CNtTimeSys* NT_SYS_BASE::pTimeSys{ nullptr };
CNtThreadSys NT_SYS_BASE::g_ThreadSys{};
CNtThreadSys* NT_SYS_BASE::pThreadSys{ nullptr };
CNtUdpSys NT_SYS_BASE::g_UdpSys{};
CNtUdpSys* NT_SYS_BASE::pUdpSys{};
CGcMainSys NT_SYS_BASE::g_GcMainSys{};
CGcMainSys* NT_SYS_BASE::pGcMainSys{};

FFXI::Network::NT_SYS_BASE::NTGameInitStruct::NTGameInitStruct() {
	memset(this, 0, sizeof(NTGameInitStruct));
	this->field_0 = 21100;
	this->ZoneNumber = 0;
	this->WorldNumber = 0;
	this->CliMode = 1;
	this->field_10 = 0;
	this->field_14 = 0;
	this->field_18 = 0;
	this->WorldLocallyHosted = 0;
	this->WorldPort = htons(18396u);
	this->CenterPort = htons(18333u);
	this->CenterIP = 0;
	this->WorldIP = 0;
	this->MyIP = 0;
	this->pcnt = 400;
	this->ClientPort = htons(54100u);
	this->Polcon = 0;
	this->DnsMode = 0;
}
void ParseParam(char* a1, char* a2) {
	int v5{};
	bool foundArg{ false };
	char v6{ 0 };
	while (true) {
		v6 = *a2;
		bool isSpec = v6 == ' ' || !v6 || v6 == '\t';
		
		if (foundArg) {
			if (isSpec) {
				*a1 = 0;
				return;
			}
			else {
				*a1++ = v6;
				++v5;
				++a2;
			}
		}
		else {
			if (isSpec) {
				++v5;
				++a2;
			}
			else {
				*a1++ = v6;
				++v5;
				foundArg = true;
				++a2;
			}
		}
	}
}
bool NT_SYS_BASE::Init()
{
	if (this->field_0)
		return false;

	FFXI::CYy::CApp::g_pNT_SYS = &FFXI::CYy::CApp::g_NT_SYS;

	this->field_0 = 1;

	if (!ntGameInit())
		return false;

	++this->field_2;
	return true;
}

bool NT_SYS_BASE::ntGameInit()
{

	char Buffer[128] = { 0 };
	NTGameInitStruct v15{};

	NT_SYS_BASE::pGame = &NT_SYS_BASE::g_Game;
	memset(pGame, 0, sizeof(g_Game));

	NT_SYS_BASE::pDebug = &NT_SYS_BASE::g_Debug;
	memset(pDebug, 0, sizeof(g_Debug));

	pGame->NetTimeout1 = 30000;
	pGame->NetTimeout2 = 40000;
	pGame->NetTimeout3 = 4 * pGame->NetTimeout1;
	pGame->field_18 = 300000;

	v15.CenterIP = StrToIp(Constants::Strings::NET_SomeIP2);
	v15.WorldIP = StrToIp(Constants::Strings::NET_SomeIP);

	pGame->field_440 = 3;

	if (FFXI::Config::CommandLineConfig::instance.ArgGetParam(Constants::Strings::CLARG_ver, Buffer, sizeof(Buffer), 0)) {
		//sub //TODO
		MessageBoxA(NULL, "VERSION STRING HERE", "", NULL);
		exit(-1);
	}

	ntTimeInit();
	ntThreadInit();

	char* Param = FFXI::Config::CommandLineConfig::instance.ArgGetParam(Constants::Strings::CLARG_net, nullptr, 0, 0);
	if (Param) {
		ParseParam(Buffer, Param);
		switch (atoi(Buffer)) {
		case 0:
			v15.CliMode = 0;
			memset(&v15.field_10, 0, 12);
			break;
		case 1:
			v15.CliMode = 1;
			memset(&v15.field_10, 0, 12);
			pDebug->field_90 = 1;
			break;
		case 2:
			v15.CliMode = 2;
			memset(&v15.field_10, 0, 12);
			pDebug->field_90 = 1;
			break;
		case 3:
			v15.CliMode = 3;
			memset(&v15.field_10, 0, 12);
			pDebug->field_90 = 1;
			break;
		case 4:
			v15.CliMode = 4;
			memset(&v15.field_10, 0, 12);
			pDebug->field_90 = 1;
			break;
		case 10:
			v15.CliMode = 1;
			v15.field_10 = 1;
			v15.field_14 = 0;
			v15.field_18 = 0;
			v15.WorldLocallyHosted = 1;
			pDebug->field_4 = 1;
			pDebug->field_90 = 1;
			break;
		case 11:
			v15.CliMode = 0;
			v15.field_10 = 1;
			v15.field_14 = 0;
			v15.field_18 = 0;
			v15.WorldLocallyHosted = 1;
			pDebug->field_4 = 1;
			break;
		case 12:
			v15.CliMode = 0;
			v15.field_10 = 2;
			v15.field_14 = 0;
			v15.field_18 = 0;
			pDebug->field_4 = 2;
			break;
		case 20:
			v15.CliMode = 2;
			v15.field_10 = 2;
			v15.field_14 = 1;
			v15.field_18 = 0;
			v15.WorldLocallyHosted = 1;
			pDebug->field_4 = 2;
			pDebug->field_90 = 1;
			break;
		case 21:
			v15.CliMode = 0;
			v15.field_10 = 0;
			v15.field_14 = 1;
			v15.field_18 = 0;
			v15.WorldLocallyHosted = 1;
			break;
		case 22:
			v15.CliMode = 0;
			v15.field_10 = 0;
			v15.field_14 = 2;
			v15.field_18 = 0;
			v15.WorldLocallyHosted = 1;
			break;
		case 30:
			v15.CliMode = 3;
			v15.field_10 = 2;
			v15.field_14 = 2;
			v15.field_18 = 1;
			v15.WorldLocallyHosted = 1;
			pDebug->field_4 = 3;
			pDebug->field_90 = 1;
			break;
		case 31:
			v15.CliMode = 0;
			v15.field_10 = 0;
			v15.field_14 = 0;
			v15.field_18 = 1;
			v15.WorldLocallyHosted = 1;
			break;
		case 32:
			v15.CliMode = 0;
			v15.field_10 = 0;
			v15.field_14 = 0;
			v15.field_18 = 2;
			v15.WorldLocallyHosted = 1;
			break;
		case 40:
			v15.CliMode = 4;
			v15.field_10 = 2;
			v15.field_14 = 2;
			v15.field_18 = 2;
			v15.WorldLocallyHosted = 1;
			pDebug->field_4 = 4;
			pDebug->field_90 = 1;
			break;
		case 255:
			v15.CliMode = 255;
			memset(&v15.field_10, 0, 12);
			pDebug->field_90 = 1;
			break;
		}

	}

	if (FFXI::Config::CommandLineConfig::instance.ArgGetParam(Constants::Strings::CLARG_zoneno, Buffer, sizeof(Buffer), 0))
		v15.ZoneNumber = atoi(Buffer);

	if (FFXI::Config::CommandLineConfig::instance.ArgGetParam(Constants::Strings::CLARG_worldno, Buffer, sizeof(Buffer), 0))
		v15.WorldNumber = atoi(Buffer);

	if (FFXI::Config::CommandLineConfig::instance.ArgGetParam(Constants::Strings::CLARG_processid, Buffer, sizeof(Buffer), 0)) {
		int v2 = atoi(Buffer);
		v15.field_0 = v2;
		v15.ZoneNumber = v2 % 1000;
		v15.WorldNumber = v2 / 1000;
	}

	if (FFXI::Config::CommandLineConfig::instance.ArgGetParam(Constants::Strings::CLARG_debug, Buffer, sizeof(Buffer), 0))
		pDebug->field_154 = atoi(Buffer);

	if (FFXI::Config::CommandLineConfig::instance.ArgGetParam(Constants::Strings::CLARG_myip, Buffer, sizeof(Buffer), 0))
		v15.MyIP = StrToIp(Buffer);

	if (FFXI::Config::CommandLineConfig::instance.ArgGetParam(Constants::Strings::CLARG_ip, Buffer, sizeof(Buffer), 0))
		v15.WorldIP = StrToIp(Buffer);

	if (FFXI::Config::CommandLineConfig::instance.ArgGetParam(Constants::Strings::CLARG_port, Buffer, sizeof(Buffer), 0)) {
		v15.WorldPort = htons(atoi(Buffer));
	}
	else if (v15.CliMode == 1) {
		v15.WorldPort = v15.ClientPort;
	}

	if (FFXI::Config::CommandLineConfig::instance.ArgGetParam(Constants::Strings::CLARG_name, Buffer, sizeof(Buffer), 0)) {
		strncpy_s(pDebug->Name, Buffer, sizeof(pDebug->Name));
		strncpy_s(v15.Name, Buffer, sizeof(v15.Name));
	}
	else {
		sprintf_s(pDebug->Name, Constants::Strings::SpaceString);
	}

	if (FFXI::Config::CommandLineConfig::instance.ArgGetParam(Constants::Strings::CLARG_accunt, Buffer, sizeof(Buffer), 0))
		strncpy_s(v15.Name, Buffer, sizeof(v15.Name));
	if (FFXI::Config::CommandLineConfig::instance.ArgGetParam(Constants::Strings::CLARG_pass, Buffer, sizeof(Buffer), 0))
		strncpy_s(v15.Password, Buffer, sizeof(v15.Password));
	if (FFXI::Config::CommandLineConfig::instance.ArgGetParam(Constants::Strings::CLARG_pcnt, Buffer, sizeof(Buffer), 0))
		v15.pcnt = atoi(Buffer);
	if (FFXI::Config::CommandLineConfig::instance.ArgGetParam(Constants::Strings::CLARG_cliport, Buffer, sizeof(Buffer), 0))
		v15.ClientPort = htons(atoi(Buffer));
	if (FFXI::Config::CommandLineConfig::instance.ArgGetParam(Constants::Strings::CLARG_worldip, Buffer, sizeof(Buffer), 0))
		v15.WorldIP = StrToIp(Buffer);
	if (FFXI::Config::CommandLineConfig::instance.ArgGetParam(Constants::Strings::CLARG_worldport, Buffer, sizeof(Buffer), 0))
		v15.WorldPort = htons(atoi(Buffer));
	if (FFXI::Config::CommandLineConfig::instance.ArgGetParam(Constants::Strings::CLARG_centerip, Buffer, sizeof(Buffer), 0))
		v15.CenterIP = StrToIp(Buffer);
	if (FFXI::Config::CommandLineConfig::instance.ArgGetParam(Constants::Strings::CLARG_centerport, Buffer, sizeof(Buffer), 0))
		v15.CenterPort = htons(atoi(Buffer));

	pDebug->X = 0;
	pDebug->Y = 0;
	pDebug->Z = 0;
	pDebug->Dir = 0;

	if (FFXI::Config::CommandLineConfig::instance.ArgGetParam(Constants::Strings::CLARG_x, Buffer, sizeof(Buffer), 0))
		pDebug->X = atoi(Buffer);

	if (FFXI::Config::CommandLineConfig::instance.ArgGetParam(Constants::Strings::CLARG_y, Buffer, sizeof(Buffer), 0))
		pDebug->Y = atoi(Buffer);

	if (FFXI::Config::CommandLineConfig::instance.ArgGetParam(Constants::Strings::CLARG_z, Buffer, sizeof(Buffer), 0))
		pDebug->Z = atoi(Buffer);

	if (FFXI::Config::CommandLineConfig::instance.ArgGetParam(Constants::Strings::CLARG_dir, Buffer, sizeof(Buffer), 0))
		pDebug->Dir = atoi(Buffer);

	char* v7 = FFXI::Config::CommandLineConfig::instance.ArgGetParam(Constants::Strings::CLARG_gm, nullptr, sizeof(Buffer), 0);
	if (v7)
		ParseParam(pDebug->Gm, v7);

	if (FFXI::Config::CommandLineConfig::instance.ArgGetParam(Constants::Strings::CLARG_nohitck, nullptr, sizeof(Buffer), 0))
		pDebug->Nohitck = 0;
	else
		pDebug->Nohitck = 1;

	if (FFXI::Config::CommandLineConfig::instance.ArgGetParam(Constants::Strings::CLARG_timeout, Buffer, sizeof(Buffer), 0)) {
		pGame->NetTimeout1 = atoi(Buffer);
		pGame->NetTimeout2 = atoi(Buffer) + 3000;
		pGame->NetTimeout3 = 4 * pGame->NetTimeout1;
		if (pGame->NetTimeout3 > 0x493E0u)
		{
			if (pGame->NetTimeout2 >= 0x4BAF0u)
				pGame->NetTimeout3 = pGame->NetTimeout2 + 10000;
			else
				pGame->NetTimeout3 = 300000;
		}
	}
	else if (v15.field_10 == 1 || v15.CliMode == 1) {
		pGame->NetTimeout1 = 3600000;
		pGame->NetTimeout2 = 3660000;
		pGame->NetTimeout3 = 3720000;
	}

	if (FFXI::Config::CommandLineConfig::instance.ArgGetParam(Constants::Strings::CLARG_rtimeout, Buffer, sizeof(Buffer), 0))
		pGame->NetTimeout2 = atoi(Buffer);

	if (FFXI::Config::CommandLineConfig::instance.ArgGetParam(Constants::Strings::CLARG_uniqueno, Buffer, sizeof(Buffer), 0))
		pDebug->UniqueNo = atoi(Buffer);

	if (FFXI::Config::CommandLineConfig::instance.ArgGetParam(Constants::Strings::CLARG_globaluniqueno, Buffer, sizeof(Buffer), 0))
		pDebug->UniqueNo = atoi(Buffer);

	if (FFXI::Config::CommandLineConfig::instance.ArgGetParam(Constants::Strings::CLARG_mapno, Buffer, sizeof(Buffer), 0))
		pDebug->MapNo = atoi(Buffer);

	if (FFXI::Config::CommandLineConfig::instance.ArgGetParam(Constants::Strings::CLARG_mapblksize, Buffer, sizeof(Buffer), 0))
		pDebug->Mapblksize = atoi(Buffer);

	if (FFXI::Config::CommandLineConfig::instance.ArgGetParam(Constants::Strings::CLARG_stoptimer, Buffer, sizeof(Buffer), 0))
		pDebug->StopTimer = atoi(Buffer);

	if (FFXI::Config::CommandLineConfig::instance.ArgGetParam(Constants::Strings::CLARG_hp, Buffer, sizeof(Buffer), 0))
		pDebug->hp = atoi(Buffer);

	if (FFXI::Config::CommandLineConfig::instance.ArgGetParam(Constants::Strings::CLARG_mp, Buffer, sizeof(Buffer), 0))
		pDebug->mp = atoi(Buffer);

	if (FFXI::Config::CommandLineConfig::instance.ArgGetParam(Constants::Strings::CLARG_clinum, Buffer, sizeof(Buffer), 0))
		pDebug->CliNum = atoi(Buffer);
	else
		pDebug->CliNum = 1;

	if (FFXI::Config::CommandLineConfig::instance.ArgGetParam(Constants::Strings::CLARG_npcnum, Buffer, sizeof(Buffer), 0))
		pGame->NpcNum = atoi(Buffer);
	if (FFXI::Config::CommandLineConfig::instance.ArgGetParam(Constants::Strings::CLARG_pcnum, Buffer, sizeof(Buffer), 0))
		pGame->PcNum = atoi(Buffer);
	if (FFXI::Config::CommandLineConfig::instance.ArgGetParam(Constants::Strings::CLARG_petnum, Buffer, sizeof(Buffer), 0))
		pGame->PetNum = atoi(Buffer);

	if (FFXI::Config::CommandLineConfig::instance.ArgGetParam(Constants::Strings::CLARG_sendt, Buffer, sizeof(Buffer), 0))
		pDebug->Sendt = atoi(Buffer);
	else
		pDebug->Sendt = 0;

	if (FFXI::Config::CommandLineConfig::instance.ArgGetParam(Constants::Strings::CLARG_recvt, Buffer, sizeof(Buffer), 0))
		pDebug->Recvt = atoi(Buffer);
	else
		pDebug->Recvt = 0;

	if (FFXI::Config::CommandLineConfig::instance.ArgGetParam(Constants::Strings::CLARG_prnd, Buffer, sizeof(Buffer), 0))
		pDebug->Prnd = atoi(Buffer);
	else
		pDebug->Prnd = 0;

	if (FFXI::Config::CommandLineConfig::instance.ArgGetParam(Constants::Strings::CLARG_lost, Buffer, sizeof(Buffer), 0))
		pDebug->Lost = atoi(Buffer);
	else
		pDebug->Lost = 0;

	pDebug->field_188 = 0;
	pDebug->field_18A = 0;

	if (FFXI::Config::CommandLineConfig::instance.ArgGetParam(Constants::Strings::CLARG_polcon, Buffer, sizeof(Buffer), 0))
		v15.Polcon = v15.CliMode >= 3;
	if (FFXI::Config::CommandLineConfig::instance.ArgGetParam(Constants::Strings::CLARG_dnsmode, Buffer, sizeof(Buffer), 0))
		v15.DnsMode = atoi(Buffer);

	pGame->CliMode = v15.CliMode;
	pGame->field_0 = v15.field_10;

	if (!pGame->PcNum || pGame->PcNum > 768)
		pGame->PcNum = 768;

	if (!pGame->PetNum) {
		if (pGame->PcNum > 512)
			pGame->PetNum = 512;
		else
			pGame->PetNum = pGame->PcNum;
	}
	else if (pGame->PetNum > 512)
		pGame->PetNum = 512;

	if (pGame->PcNum >= 12) {
		pGame->field_30 = pGame->PcNum - 12;
	}

	pGame->field_2C = pGame->PetNum + 0x700;

	pGame->Polcon = v15.Polcon;
	pGame->DnsMode = v15.DnsMode;

	pTimeSys->ntTimeSet(0, 0);
	pTimeSys->ntGameTimeSet(0);
	if (ntIpInit())
		return false;

	if (!ntUdpInit())
		return false;

	if (v15.WorldLocallyHosted)
		v15.WorldIP = pUdpSys->MyIp;

	if (gcMainWorkInit(v15.CliMode, v15.WorldIP, v15.WorldPort, pDebug->Name, v15.Name, v15.Password, v15.pcnt) != true)
		return false;

	pDebug->field_79C = pTimeSys->ntGameTimeGet();

	pGame->VersionDateTime = Constants::Strings::VERSION_DateTime;
	pGame->field_46C = 1;
	return true;
}

bool NT_SYS_BASE::ntGameEnd() {
	//sub //TODO
	if (!pGame) return false;
	//sub

	WSACleanup();

	return true;
}

bool NT_SYS_BASE::gcMainWorkInit(int client_mode, int worldIP, short worldPort, char* debugname, char* name, char* password, int pcnt)
{
	if (!client_mode || client_mode == 255) return true;
	if (pGcMainSys || client_mode < 1 || client_mode > 3)
		return false;

	pGcMainSys = &g_GcMainSys;
	memset(pGcMainSys, 0, sizeof(g_GcMainSys));
	pGcMainSys->gcMainDivisionInit();
	strncpy_s(pGcMainSys->DebugName, debugname, sizeof(pGcMainSys->DebugName));
	strncpy_s(pGcMainSys->Name, name, sizeof(pGcMainSys->Name));
	strncpy_s(pGcMainSys->Password, password, sizeof(pGcMainSys->Password));
	pGcMainSys->field_12 = htons(54090u);
	pGcMainSys->WorldIP = worldIP;
	pGcMainSys->WorldPort = worldPort;
	pGcMainSys->field_8 = worldIP;
	pGcMainSys->field_10 = worldPort;
	pGcMainSys->pcnt = pcnt;
	pGcMainSys->field_120 = 1;
	//PolSys init
	//PolStat init
	//some init
	pGcMainSys->gcFriendInit(60);
	//FDtXiInit
	//gcPolMessageInit
	if (client_mode == 1) {
		pGcMainSys->field_11C = 7;
	}
	else if (client_mode == 2) {
		pGcMainSys->field_11C = 4;
	}
	else {
		pGcMainSys->field_11C = 1;
	}

	return true;
}

void NT_SYS_BASE::End()
{
	if (!this) return;

	this->field_0 = 0;
	if (ntGameEnd())
		FFXI::CYy::CApp::g_pCApp->g_pNT_SYS->ClientMode = 0;
}

unsigned int NT_SYS_BASE::StrToIp(const char* a1)
{
	unsigned int result = inet_addr(a1);
	if (result != INADDR_NONE && result != INADDR_ANY)
		return result;

	hostent* host = gethostbyname(a1);
	if (host)
		return **(unsigned int**)host->h_addr_list;

	return 0;
}

bool NT_SYS_BASE::ntIpgethostname(void* a1)
{
	char name[128];
	if (gethostname(name, 128)) return false;
	
	hostent* v2 = gethostbyname(name);
	if (!v2) return false;

	char* v5 = *v2->h_addr_list;
	int length = v2->h_length;
	int roundlength = 4 * (length >> 2);
	memcpy(a1, v5, roundlength);
	memcpy((char*)a1 + roundlength, v5 + roundlength, length & 3);
	return true;
}

bool NT_SYS_BASE::IsActive()
{
	return this->field_2 != 0;
}

void NT_SYS_BASE::ntThreadInit()
{
	pThreadSys = &g_ThreadSys;
	memset(pThreadSys, 0, sizeof(g_ThreadSys));
	pThreadSys->field_2EE1 = 2;
}

void NT_SYS_BASE::ntTimeInit()
{
	pTimeSys = &g_TimeSys;
	memset(pTimeSys, 0, sizeof(g_TimeSys));
	pTimeSys->field_C = time(0);
	pTimeSys->field_10 = 0;
	pTimeSys->field_0 = timeGetTime();
	pTimeSys->field_54 = 0;
	pTimeSys->field_4C = 0;
	pTimeSys->field_50 = field_0;
}

int NT_SYS_BASE::ntIpInit()
{
	static bool ntIpInitialized{ false };
	if (ntIpInitialized) return 0;

	WSADATA data{};
	if (WSAStartup(0x101u, &data) != 0) return -1;

	//todo //sub
	//Not sure if any of this stuff is used. Will come back to it if it is.

	ntIpInitialized = true;
	return 0;
}

int NT_SYS_BASE::ntUdpInit() {
	if (pUdpSys)
		ntUdpEnd();

	pUdpSys = &g_UdpSys;
	memset(pUdpSys, 0, sizeof(g_UdpSys));

	if (ntIpgethostname(&pUdpSys->MyIp))
		return 1;

	ntUdpEnd();
	return 0;
}

void NT_SYS_BASE::ntUdpEnd() {
	if (!pUdpSys) return;

	int v2 = (pUdpSys->field_1AFC4 + 1) % 4;
	while (v2 != pUdpSys->field_1AFC4) {
		UdpSubStruct* sub = pUdpSys->subs + v2;
		if (sub->field_4 == 2) {
			closesocket(sub->field_0);
			sub->field_4 = 0;
		}
		v2 = (v2 + 1) % 4;
	}

	pUdpSys = nullptr;
}
void NT_SYS_BASE::ntGameProc2()
{
	if (pGame && pTimeSys->ntTimeProc() && pThreadSys && pUdpSys) {

	}
}