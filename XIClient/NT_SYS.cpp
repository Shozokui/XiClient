#include "NT_SYS.h"
#include "Globals.h"
#include "CNtTcpDLL.h"
#include "StringTables.h"

using namespace FFXI::Network;

int NT_SYS::timer{ 0 };
int NT_SYS::dllsw{ 0 };
char NT_SYS::InfoMationString[0x200] = { "" };
int NT_SYS::SomeValue{ 0 };
char NT_SYS::versionstring[16]{};
bool NT_SYS::CliZoneFadeOutFlag{ false };
bool NT_SYS::CliZoneFadeInFlag{ false };
int NT_SYS::CliZoneFadeOutCount{ 0 };
int NT_SYS::CliZoneFadeInCount{ 0 };

void NT_SYS::SetVersionString(const char* a1)
{
	//gcLoginSetVersionString
	memcpy(versionstring, a1, sizeof(versionstring) - 1);
	versionstring[sizeof(versionstring) - 1] = 0;
}

int NT_SYS::getTotalModePhase(int a1)
{
	switch (a1)
	{
	case 1:
		return 21;
	case 2:
	case 17:
	case 19:
		return 4;
	case 3:
		return 22;
	case 4:
	case 20:
	case 22:
		return 10;
	case 5:
	case 18:
	case 23:
		return 2;
	default:
		return 1;
	}
}

void NT_SYS::CliLocalTask()
{
//sub //TODO
}

void NT_SYS::ResetTimer()
{
    timer = 0;
}

bool NT_SYS::UpdateTimer()
{
    ++timer;
    return (timer >= 3600);
}

char* NT_SYS::getChrName(int a2)
{
    return g_GcMainSys.IwWorkPacket.ContentIDList[a2].Name;
}

char* FFXI::Network::NT_SYS::getWorldName(int a2)
{
    return g_GcMainSys.IwWorkPacket.ContentIDList[a2].WorldName;
}

int FFXI::Network::NT_SYS::getWorld(int a2)
{
    return g_GcMainSys.IwWorkPacket.ContentIDList[a2].World;
}

int FFXI::Network::NT_SYS::getArea(int a2)
{
    int base = g_GcMainSys.IwWorkPacket.ContentIDList[a2].Zone;
    int top = g_GcMainSys.IwWorkPacket.ContentIDList[a2].field_4F & 1;
    return (top << 8) + base;
}

short* FFXI::Network::NT_SYS::getEquip(int a2)
{
    return g_GcMainSys.IwWorkPacket.ContentIDList[a2].Equip;
}

int FFXI::Network::NT_SYS::getRaceGender(int a2)
{
    return g_GcMainSys.IwWorkPacket.ContentIDList[a2].RaceGender;
}

int FFXI::Network::NT_SYS::getJob(int a2)
{
    return g_GcMainSys.IwWorkPacket.ContentIDList[a2].MainJob;
}

int FFXI::Network::NT_SYS::getFaceNo(int a2)
{
    return g_GcMainSys.IwWorkPacket.ContentIDList[a2].Face;
}

int FFXI::Network::NT_SYS::getTownNo(int a2)
{
    return g_GcMainSys.IwWorkPacket.ContentIDList[a2].Town;
}

int FFXI::Network::NT_SYS::getHair(int a2)
{
    return g_GcMainSys.IwWorkPacket.ContentIDList[a2].Hair;
}

int FFXI::Network::NT_SYS::getSize(int a2)
{
    return g_GcMainSys.IwWorkPacket.ContentIDList[a2].Size;
}

int FFXI::Network::NT_SYS::getLv(int a2)
{
    return g_GcMainSys.IwWorkPacket.ContentIDList[a2].MainJobLevel;
}

int FFXI::Network::NT_SYS::gcLoginGetXiError()
{
    if (NT_SYS::pGcMainSys == nullptr)
        return 101;

    int err = CNtTcpDLL::ntTcpDLLGetError();
    if (err)
        return err;

    err = NT_SYS::pGcMainSys->field_136AC;
    if (err)
        return err;

    return 113;
}

int FFXI::Network::NT_SYS::getFFXIID(int a2)
{
    return g_GcMainSys.IwWorkPacket.ContentIDList[a2].field_0;
}

int FFXI::Network::NT_SYS::getTotalCharacters()
{
    return g_GcMainSys.IwWorkPacket.ContentIDCount;
}

int FFXI::Network::NT_SYS::getChrStatus(int a2)
{
    return g_GcMainSys.IwWorkPacket.ContentIDList[a2].Status;
}

bool NT_SYS::Init()
{
	if (NT_SYS_BASE::Init() != 1)
		return false;

	this->field_8 = 0;
	this->field_C = 0;
	this->field_80 = 0;
	this->field_90 = 101;
	this->field_8C = -1;
	this->field_94 = 200;
	this->field_14 = 0;

    this->ClientMode = pGame ? pGame->CliMode : 0;
    this->ZoneMode = 0;
    this->POLCON = pGame ? pGame->Polcon : 0;

	this->field_A0 = 0;
	this->field_B4 = 0;
	if (this->ClientMode == 3)
		this->field_88 = 0;
	else
		this->field_88 = 3;
	this->field_18 = 0;
	this->field_B8 = 0;
	this->field_A8 = 2;

	return true;
}

void NT_SYS::Proc()
{
    switch (this->field_8) {
    case 0:
        this->ProcBase();
        if (this->field_14 == 12 || this->field_14 == 21
            || this->field_14 == 11 || this->field_14 == 14)
            return;
        ++this->field_8;
        CliZoneFadeOutFlag = true;
        return;
    case 1:
        this->ProcBase();
        if (!CliZoneFadeOutFlag)
            ++this->field_8;
        CliLocalTask();
        return;
    case 2:
        this->ProcBase();
        CliLocalTask();

    }
}

void NT_SYS::ProcBase()
{
    if (this->field_8C >= 0)
        this->field_8C = -1;

    if (this->ClientMode) {
        int v3 = this->field_88;
        if (v3 != 12 && v3 != 21 && v3 != 202 && v3 != 203 && v3 != 201)
        {
            //sub //TODO
        }
        ntGameProc2();
        switch (this->field_88) {
        case 0: 
            //XIInfo start
            //sub //TODO
            if (this->field_90 == 100)
                this->field_8C = this->field_88;
            this->field_90 = 101;
            break;
        case 1:
            if (this->field_90 == 100) {
                int v8 = gcLoginProc(&this->field_C);
                if (v8 == -1) {
                    if (this->field_88 == 1) {
                        this->field_94 = 203;
                        this->modeSet(203, 0);
                    }
                }
                else if (v8 == 1) {
                    this->field_90 = 101;
                    this->field_8C = this->field_88;
                }
            }
            break;
        case 2:
            if (this->field_90 == 100) {
                int v13 = 0;
                if (v3 == -1) {
                    if (this->field_88 == 2) {
                        this->field_94 = 203;
                        this->modeSet(203, 0);
                    } 
                }
                else if (v13 == 1) {
                    this->field_90 = 101;
                    this->field_8C = this->field_88;
                }
            }
            break;
        case 3:
            if (this->field_90 == 100) {

            }
            break;
        case 201:
        case 202:
        case 203:
            this->field_88 = 0;
            break;
        default:
            break;
        }
        switch (this->field_88) {
        case 201:
        case 202:
        case 203:
            this->field_8C = this->field_88;
            this->field_90 = 101;
            break;
        default:
            break;
        }
    }
    else if (this->field_90 == 100) {
        this->field_90 = 101;
        this->field_8C = this->field_88;
    }
}

int NT_SYS::gcLoginProc(int* a2)
{
    if (!pGcMainSys) return -1;

    if (*a2 == 0) {
        //some pol stuff here
        ResetTimer();
    }

    int result{};
    char* string;
    switch (*a2) {
    case 0:
        string = FFXI::Text::XiStrGet(8, 0);
        gcLoginSetInfoMationString(string);
        //sub //todo field_136b0 is supposed to be set here to something from pol
        if (dllsw || pGcMainSys->field_136B0 >= 0) {
            *a2 += 1;
        }
        else {
            pGcMainSys->field_136AC = pGcMainSys->field_136B0;
            return -1;
        }
    //FALLTHROUGH
    case 1:
        if (dllsw) {
            *a2 += 2;
        }
        else {
            int v5 = 1; //this gets something from pol
            if (v5 == 0)
                return 0;
            if (v5 >> 31 < 0) {
                pGcMainSys->field_136AC = v5;
                return -1;
            }

            pGcMainSys->DLLInitStruct.DebugName = pGcMainSys->DebugName;
            pGcMainSys->DLLInitStruct.Password = pGcMainSys->Password;
            in_addr v88{};
            //sub //TODO remove
            pGcMainSys->WorldIP = 0x7F000001;
            v88.S_un.S_addr = htonl(pGcMainSys->WorldIP);
            pGcMainSys->DLLInitStruct.Ip = inet_ntoa(v88);
            pGcMainSys->DLLInitStruct.field_C = htons(pGcMainSys->field_10);
            memcpy(pGcMainSys->DLLInitStruct.VersionString, versionstring, sizeof(versionstring));
            pGcMainSys->DLLInitStruct.VersionString[15] = 0;
            CNtTcpDLL::ntTcpDLLRequestCreateObject(&pGcMainSys->ntTcpDLL, &pGcMainSys->DLLInitStruct);
            dllsw = 2;
            ++* a2;
        }
        result = 0;
        break;
    case 2:
    case 6:
    case 8:
    case 10:
    case 14:
    case 16:
    case 18:
    case 20:
        pGcMainSys->ntTcpDLL->ntTcpDLLStatus(&pGcMainSys->field_13698);
        if (pGcMainSys->field_13698 == 0)
            ++*a2;
        if (pGcMainSys->field_13698 == -1 || *a2 >= 14 && UpdateTimer())
            return -1;
        return 0;
    case 3:
        string = Text::XiStrGet(8, 1);
        gcLoginSetInfoMationString(string);
        pGcMainSys->ntTcpDLL->ntTcpDLLRequestResetError();
        ++* a2;
        return 0;
    case 4:
        pGcMainSys->ntTcpDLL->ntTcpDLLStatus(&pGcMainSys->field_13698);
        ++ * a2;
        return 0;
    case 5:
        if (pGcMainSys->field_136AC == 0 && pGcMainSys->ntTcpDLL) {
            if (pGcMainSys->ntTcpDLL->ntTcpDLLCheckError() == 1) {
                *a2 = 21;
                return 0;
            }
        }
        pGcMainSys->field_136AC = 0;
        pGcMainSys->ntTcpDLL->ntTcpDLLRequestOption(1);
        ++* a2;
        return 0;
    case 7:
        string = Text::XiStrGet(8, 2);
        gcLoginSetInfoMationString(string);
        pGcMainSys->ntTcpDLL->ntTcpDLLRequestShutdown();
        ++* a2;
        return 0;
    case 9:
        string = Text::XiStrGet(8, 3);
        gcLoginSetInfoMationString(string);
        pGcMainSys->ntTcpDLL->ntTcpDLLRequestConnection();
        ++* a2;
        return 0;
    case 11:
        string = Text::XiStrGet(8, 4);
        gcLoginSetInfoMationString(string);
        pGcMainSys->field_136A8 = 0; //something from POL
        if (pGcMainSys->field_136A8 < 0) {
            pGcMainSys->field_136AC = pGcMainSys->field_136A8;
            return -1;
        }
        ++* a2;
        return 0;
    case 12: {
        int v12 = 1; //something from pol
        if (v12 == 0)
            return 0;
        if (v12 >= 0) {
            ++* a2;
            return 0;
        }
        else {
            pGcMainSys->field_136AC = v12;
            return -1;
        }
    }
    case 13:
        string = Text::XiStrGet(8, 5);
        gcLoginSetInfoMationString(string);
        //pol message stuff
        {
            //Some of this is altered in order to work with a modified LSB server
            char v27[0x40];
            int v14 = 0; //get authcode
            if (v14 >= 0) {
                pGcMainSys->ntTcpDLL->ntTcpDLLSetAthCode(v27);
                //get random value from pol 
                //char v26[16];
                //pGcMainSys->ntTcpDLL->ntTcpDLLSetPasswd(v26);
                int v25[] = { 1, 0 };
                pGcMainSys->ntTcpDLL->ntTcpDLLSetClientCode(v25);
                this->ClientExpansions = 1 | FFXI::GlobalStruct::g_GlobalStruct.ClientExpansions;
                pGcMainSys->ntTcpDLL->ntTcpDLLSetExCodeClient(this->ClientExpansions);
                pGcMainSys->ntTcpDLL->ntTcpDLLRequestLobbyLogin();
                ++* a2;
                return 0;
            }
            else {
                pGcMainSys->field_136AC = v14;
                return -1;
            }
        }
        return 0;
    case 15:
        string = FFXI::Text::XiStrGet(8, 6);
        gcLoginSetInfoMationString(string);
        pGcMainSys->ntTcpDLL->ntTcpDLLGetExCodeServer(&this->ServerExpansions);
        FFXI::GlobalStruct::g_GlobalStruct.ServerExpansions = 1 | this->ServerExpansions;
        pGcMainSys->ntTcpDLL->ntTcpDLLRequestGetChr();
        ++* a2;
        return 0;
    case 17:
        string = FFXI::Text::XiStrGet(8, 7);
        gcLoginSetInfoMationString(string);
        pGcMainSys->ntTcpDLL->ntTcpDLLRequestQueryWorldList();
        ++* a2;
        return 0;
    case 19:
        pGcMainSys->ntTcpDLL->ntTcpDLLRequestGetWork(&pGcMainSys->IwWorkPacket);
        ++ * a2;
        return 0;
    case 21:
        return 1;
    default: 
        return 0;
    }
}

void NT_SYS::gcLoginSetInfoMationString(char* a1)
{
    strcpy_s(InfoMationString, a1);
}

const char* NT_SYS::gcLoginGetInfoMationString()
{
    return InfoMationString;
}

bool NT_SYS::IsConnect()
{
	//sub //TODO
	return true;
}

int NT_SYS::modeGet()
{
	if (this->field_90 == 100)
		return 100;

	if (this->field_8C < 0)
		return 101;

	this->field_14 = this->field_8C;

	return this->field_14;
}

void NT_SYS::modeSet(int a2, int a3)
{
    int v5 = this->field_88;
    if (this->ClientMode)
    {
        switch (a2)
        {
        case 0:
            this->field_88 = 0;
            this->field_C = 0;
            this->field_90 = 100;
            return;
        case 1:
            this->field_C = 0;
            this->field_88 = 1;
            this->field_90 = 100;
            return;
        case 2:
            if (v5 != 1 && v5 != 19 && v5 != 3 && v5 != 4 && v5 != 20 && v5 != 22 && v5 != 23 && v5 != 5) {
                this->field_94 = 201;
                this->modeSet(201, 0);
                return;
            }
            this->field_C = 0;
            this->field_88 = 2;
            this->field_90 = 100;
            return;
        case 3:
            if (v5 != 2 && v5 != 1) {
                this->field_94 = 201;
                this->modeSet(201, 0);
                return;
            }
            this->field_C = 0;
            this->field_80 = a3;
            this->field_88 = 3;
            this->field_90 = 100;
            return;
        case 4:
            if (v5 != 1 && v5 != 20) {
                this->field_94 = 201;
                this->modeSet(201, 0);
                return;
            }
            this->field_C = 0;
            this->field_80 = a3;
            this->field_88 = 4;
            this->field_90 = 100;
            return;
        case 5:
            if (v5 != 2 && v5 != 1) {
                this->field_94 = 201;
                this->modeSet(201, 0);
                return;
            }
            this->field_C = 0;
            this->field_80 = a3;
            this->field_88 = 5;
            this->field_90 = 100;
            return;
        case 6:
            if (this->field_10)
                ;// POLSTAT_Reset();
            if (v5 != 10 && v5 != 3 && v5 != 11 && v5 != 14 && v5 != 12 && v5 != 21) {
                this->field_94 = 201;
                this->modeSet(201, 0);
                return;
            }

            this->field_88 = 6;
            this->field_90 = 100;
            if (this->ClientMode == 3) {
                this->field_C = 0;
                return;
            }
            this->field_C = 15;
            break;
        case 7:
            if (this->field_88 != 6) {
                this->field_94 = 201;
                this->modeSet(201, 0);
                return;
            }

            this->field_C = 0;
            this->field_88 = 7;
            this->field_90 = 100;
            return;
        case 8:
            if (this->field_88 != 6) {
                this->field_94 = 201;
                this->modeSet(201, 0);
                return;
            }
            this->field_C = 0;
            this->field_88 = 8;
            this->field_90 = 100;
            return;
        case 9:
            if (v5 != 7 && v5 != 8) {
                this->field_94 = 201;
                this->modeSet(201, 0);
                return;
            }
            this->field_8C = 9;
            this->field_88 = 9;
            this->field_90 = 100;
            this->field_C = 0;
            return;
        case 10:
            if (this->field_88 != 9) {
                this->field_94 = 201;
                this->modeSet(201, 0);
                return;
            }
            this->field_C = 0;
            this->field_88 = 10;
            this->field_90 = 100;
            return;
        case 11:
        case 14:
            if (this->field_88 == 9) {
                this->field_88 = a2;
                this->field_90 = 100;
                this->field_C = 0;
                return;
            }
            this->field_94 = 201;
            this->modeSet(201, 0);
            return;
        case 12:
        case 21:
        case 201:
        case 202:
        case 203:
            this->field_88 = a2;
            this->field_90 = 100;
            this->field_C = 0;
            return;
        case 17:
            this->field_C = 0;
            this->field_84 = a3;
            this->field_88 = 17;
            this->field_90 = 100;
            return;
        case 18:
            this->field_C = 0;
            this->field_84 = a3;
            this->field_88 = 18;
            this->field_90 = 100;
            return;
        case 19:
            this->field_C = 0;
            this->field_88 = 19;
            this->field_90 = 100;
            return;
        case 20:
            if (v5 != 2 && v5 != 1 && v5 != 20) {
                this->field_94 = 201;
                this->modeSet(201, 0);
                return;
            }
            this->field_C = 0;
            this->field_80 = a3;
            this->field_88 = 20;
            this->field_90 = 100;
            return;
        case 22:
            if (v5 != 2 && v5 != 1 && v5 != 22) {
                this->field_94 = 201;
                this->modeSet(201, 0);
                return;
            }

            this->field_C = 0;
            this->field_80 = a3;
            this->field_88 = 22;
            this->field_90 = 100;
            return;
        case 23:
            if (v5 != 2 && v5 != 1 && v5 != 23) {
                this->field_94 = 201;
                this->modeSet(201, 0);
                return;
            }
            this->field_C = 0;
            this->field_80 = a3;
            this->field_88 = 23;
            this->field_90 = 100;
            return;
        default:
            this->field_94 = 201;
            this->modeSet(201, 0);
            return;
        }
    }

    this->field_88 = a2;
    this->field_90 = 100;
    this->field_C = 0;
}

int FFXI::Network::NT_SYS::getLastError()
{
    return NT_SYS::gcLoginGetXiError();
}
