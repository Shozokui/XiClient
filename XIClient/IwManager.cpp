#include "IwManager.h"
#include "TaskBuff.h"
#include "Task.h"
#include "Globals.h"
#include "CMoResourceMng.h"
#include "CYyDb.h"
#include "PrimMng.h"
#include "CIwPatchSub4.h"
#include "CIwPtcBgWin.h"
#include "CIwTopMenu.h"
#include "CIwLobbyMenu.h"
#include "CIwLicenceMenu.h"
#include "CIwOkMenu.h"
#include "CIwRaceMenu.h"
#include "CIwFaceMenu.h"
#include "CIwHairMenu.h"
#include "CIwSizeMenu.h"
#include "CIwJobsMenu.h"
#include "CYyResourceFile.h"
#include "CApp.h"
#include "NT_SYS.h"
#include "StringTables.h"
#include "TkManager.h"
#include "Strings.h"
#include "CIwNetWin.h"
#include "CIwNetWinSub1.h"
#include "XIString.h"
#include "TextRenderer.h"
#include "CYyScheduler.h"
#include "CIwSelectMenu.h"
#include "CIwChfWin.h"

using namespace FFXI::CTk;
using namespace FFXI::CYy;
using namespace FFXI::Network;

char* IwManager::g_ErrMsg{ nullptr };
bool IwManager::patch_skip{ false };
int IwManager::lobby_result{ -1 };
int IwManager::polircstts{ 0 };
char IwManager::SomeByte1{0};
char IwManager::SomeByte2{0};
char IwManager::SomeByte3{0};

FFXI::CYy::TaskBuff* IwManager::task_buff{ nullptr };
FFXI::CYy::CYyResourceFile** IwManager::RES_1{ nullptr };
FFXI::CYy::CYyResourceFile** IwManager::title_demo{ nullptr };

extern DWORD WINAPI LobbyMainThread(LPVOID);

struct ErrorEntry {
public:
	int errorID;
	int callbackIndex;
	int PageIndex;
	int EntryIndex;
};

ErrorEntry ErrorTable[] = {
		{0x834, 0x0, -1, -1},
	{0x7D2, 0x0, 0x7, 0x3F},
	{0x7D1, 0x0, 0x7, 0x41},
	{0x7D3, 0x0, 0x1B, 0x1},
	{0x0FA1, 0x1, 0x7, 0x0AA},
	{0x0FA2, 0x1, 0x7, 0x0AB},
	{0x0FA3, 0x1, 0x7, 0x0AC},
	{0x0FA4, 0x1, 0x7, 0x0AD},
	{0x0FA5, 0x1, 0x7, 0x0AE},
	{0x0FA6, 0x1, 0x7, 0x0AF},
	{0x0BB9, 0x1, 0x7, 0x0A9},
	{0x0C1C, 0x2, 0x7, 0x2},
	{0x0C1D, 0x2, 0x7, 0x3},
	{0x0C1E, 0x2, 0x7, 0x4},
	{0x0C1F, 0x2, 0x7, 0x5},
	{0x0C25, 0x2, 0x7, 0x6},
	{0x0C26, 0x2, 0x7, 0x7},
	{0x0C27, 0x2, 0x7, 0x8},
	{0x0C28, 0x2, 0x7, 0x9},
	{0x0C29, 0x2, 0x7, 0x0A},
	{0x0C2A, 0x2, 0x7, 0x0B},
	{0x0C2B, 0x2, 0x7, 0x0C},
	{0x0C2C, 0x2, 0x7, 0x0D},
	{0x0C2D, 0x2, 0x1B, 0x0},
	{0x0C2E, 0x2, 0x7, 0x0E},
	{0x0C2F, 0x2, 0x7, 0x0F},
	{0x0C30, 0x2, 0x7, 0x10},
	{0x0C81, 0x2, 0x7, 0x11},
	{0x0C82, 0x2, 0x7, 0x12},
	{0x0C83, 0x2, 0x7, 0x13},
	{0x0C84, 0x2, 0x7, 0x14},
	{0x0C85, 0x2, 0x7, 0x15},
	{0x0C86, 0x2, 0x7, 0x17},
	{0x0C87, 0x2, 0x7, 0x13},
	{0x0C88, 0x2, 0x7, 0x16},
	{0x0C8E, 0x2, 0x7, 0x18},
	{0x0C8F, 0x2, 0x7, 0x19},
	{0x0CE4, 0x2, 0x7, 0x1A},
	{0x0CE5, 0x2, 0x7, 0x1B},
	{0x0CE6, 0x2, 0x7, 0x1C},
	{0x0CE7, 0x2, 0x7, 0x1D},
	{0x0CE8, 0x2, 0x7, 0x1E},
	{0x0CE9, 0x2, 0x7, 0x1F},
	{0x0CEA, 0x2, 0x7, 0x20},
	{0x0CEB, 0x2, 0x7, 0x21},
	{0x0CEC, 0x2, 0x7, 0x22},
	{0x0CED, 0x2, 0x7, 0x23},
	{0x0CEE, 0x2, 0x7, 0x24},
	{0x0CEF, 0x2, 0x7, 0x25},
	{0x0CF0, 0x2, 0x7, 0x26},
	{0x0CF1, 0x2, 0x7, 0x27},
	{0x0CF2, 0x2, 0x7, 0x28},
	{0x0CF3, 0x2, 0x7, 0x29},
	{0x0CF4, 0x2, 0x7, 0x2A},
	{0x0CF5, 0x2, 0x7, 0x2B},
	{0x0CF6, 0x2, 0x7, 0x2C},
	{0x0CF7, 0x2, 0x7, 0x2D},
	{0x0CF8, 0x2, 0x7, 0x2E},
	{0x0CF9, 0x2, 0x7, 0x2F},
	{0x0CFA, 0x2, 0x7, 0x30},
	{0x0CFC, 0x2, 0x7, 0x31},
	{0x0CFD, 0x2, 0x7, 0x32},
	{0x0CFE, 0x2, 0x7, 0x33},
	{0x0CFF, 0x2, 0x7, 0x34},
	{0x0D01, 0x2, 0x7, 0x35},
	{0x0D02, 0x2, 0x7, 0x36},
	{0x0D03, 0x2, 0x7, 0x37},
	{0x0D04, 0x2, 0x7, 0x38},
	{0x0D06, 0x2, 0x7, 0x39},
	{0x0D07, 0x2, 0x7, 0x3A},
	{0x0D08, 0x2, 0x7, 0x3B},
	{0x0D09, 0x2, 0x7, 0x3C},
	{0x0D0A, 0x2, 0x7, 0x3D},
	{0x0D0B, 0x2, 0x7, 0x0B4},
	{0x270F, 0x0, 0x0, 0x0}
};

void SetDefaultAccount() {
	FFXI::Network::lpkt_work* pkt = &FFXI::CYy::CApp::g_pCApp->g_pNT_SYS->g_GcMainSys.IwWorkPacket;
	pkt->ClientExpansions = 0x0FFF;
	FFXI::GlobalStruct::g_GlobalStruct.ServerExpansions = 1 | pkt->ClientExpansions;
	//Enable character selection
	pkt->ContentIDList[0].Status = 1;
	pkt->ContentIDList[0].Zone = FFXI::Constants::Enums::Zone::Southern_San_dOria;
	pkt->ContentIDList[0].MainJob = 1;
	pkt->ContentIDList[0].MainJobLevel = 75;
	pkt->ContentIDList[0].RaceGender = 1;
	pkt->ContentIDList[0].Face = 1;
	pkt->ContentIDList[0].Hair = 1;
	pkt->ContentIDList[0].Size = 1;

	strcpy_s(pkt->ContentIDList[0].Name, "Default");
	strcpy_s(pkt->ContentIDList[0].WorldName, "Offline");
	
	//Enable character creation
	strcpy_s(pkt->ContentIDList[1].Name, " ");
	pkt->ContentIDList[1].Status = 1;
	pkt->ContentIDCount = 2;
}

int WaitAndUpdateProgress(int a1, int a2, int a3, int a4) {
	IwManager::task_buff->task_wait(0);
	FFXI::CYy::CApp::g_pCApp->g_pNT_SYS->modeSet(a1, a2);
	if (IwManager::task_buff->task_wait(0) < 0)
		return -1;

	while (true) {
		strcpy_s(PrimMng::g_pIwNetWin->HelpText, FFXI::CYy::CApp::g_pCApp->g_pNT_SYS->gcLoginGetInfoMationString());
		PrimMng::g_pIwNetWinSub1->progress = ((float)a3 + FFXI::CYy::CApp::g_pCApp->g_pNT_SYS->field_C) / a4;
		int v5 = FFXI::CYy::CApp::g_pCApp->g_pNT_SYS->modeGet();
		if (v5 == a1)
			break;
		if (v5 > 200 || IwManager::task_buff->task_wait(0) < 0)
			return -1;
	}

	return 0;
}
void FFXI::CTk::IwManager::IwInit()
{
	int index = Globals::GetLanguageDependentDatIndex(FFXI::Constants::Enums::LanguageDependentDats::TEX_1);
	CYy::CYyResourceFile** Unused{ nullptr };
	IwManager::RES_1 = *CYyDb::g_pCYyDb->pCMoResourceMng->LoadNumFile(&Unused, index);
	CMoResource::DoTheThing(*IwManager::RES_1);
	//sub
	//TODO
	PrimMng::g_pIwOkMenu = new CIwOkMenu();
	PrimMng::g_pIwPtcBgWin = new CIwPtcBgWin();
	PrimMng::g_pIwTopMenu = new CIwTopMenu();
	PrimMng::g_pIwLobbyMenu = new CIwLobbyMenu();
	PrimMng::g_pIwRaceMenu = new CIwRaceMenu();
	PrimMng::g_pIwFaceMenu = new CIwFaceMenu();
	PrimMng::g_pIwHairMenu = new CIwHairMenu();
	PrimMng::g_pIwSizeMenu = new CIwSizeMenu();
	PrimMng::g_pIwJobsMenu = new CIwJobsMenu();
	PrimMng::g_pIwSelectMenu = new CIwSelectMenu();
	PrimMng::g_pIwChfWin = new CIwChfWin();
	//sub //TODO
	PrimMng::g_pIwPatchSub4 = new CIwPatchSub4();
	if (PrimMng::g_pIwPatchSub4)
		PrimMng::g_pIwPatchSub4->Init();

	//sub //TODO
	PrimMng::g_pIwNetWin = new CIwNetWin();
	PrimMng::g_pIwNetWinSub1 = new CIwNetWinSub1();
	// 
	PrimMng::g_pIwLicenceMenu = new CIwLicenceMenu();
	//sub //TODO
}

void FFXI::CTk::IwManager::IwLobbyInit(bool PatchingComplete)
{
	char* mem = StorageLists::instance->Get(sizeof(FFXI::CYy::TaskBuff), Constants::Enums::MEM_MODE::Ex);
	if (mem)
		IwManager::task_buff = new (mem) FFXI::CYy::TaskBuff();
	IwManager::task_buff->AddTask(LobbyMainThread, NULL, 0);
	lobby_result = 0;
	patch_skip = PatchingComplete;
}

void FFXI::CTk::IwManager::IwPutMes(char** a1, TKRECT* a2)
{
	if (!a1) return;
	Text::XIString v8{};

	char** v3 = a1;
	while (*v3) {
		v8.AppendStr(*v3);
		if (v3[1])
			v8.AppendStr(Constants::Strings::NewLineStr);
		++v3;
	}

	char* v6 = (char*)v8.c_str();
	Text::TextRenderer::SjisFontDrawInRectHSpace(v6, a2, 0);
}

int FFXI::CTk::IwManager::TaskBuffUpdate()
{
	for (int i = 0; i < FFXI::CYy::TaskBuff::BuffSize; ++i) {
		Task* task = IwManager::task_buff->Tasks[i];
		if (task)
			task->field_5 = 1;
	}

	for (int i = 0; i < FFXI::CYy::TaskBuff::BuffSize; ++i) {
		Task* task = IwManager::task_buff->Tasks[i];
		if (task) {
			if (task->ThreadHandle) {
				if (task->field_4 == 1) {
					ResumeThread(task->ThreadHandle);
					task->field_4 = 0;
					task->field_5 = 1;
				}
				if (task->field_5 == 1) {
					while (WaitForSingleObject(task->ThreadHandle, 0) == WAIT_TIMEOUT) {
						Sleep(1u);
						if (task->field_5 != 1)
							goto LABEL_1;
					}
				}
				task->field_5 = 0;
			}
		LABEL_1:
			;
		}
	}
	if (!IwManager::lobby_result)
		return 0;

	if (!IwManager::task_buff)
		return IwManager::lobby_result;

	delete IwManager::task_buff;
	IwManager::task_buff = nullptr;

	return IwManager::lobby_result;
}

void FFXI::CTk::IwManager::IwDestroy()
{
	if (PrimMng::g_pIwPtcBgWin) {
		delete PrimMng::g_pIwPtcBgWin;
		PrimMng::g_pIwPtcBgWin = nullptr;
	}
	if (PrimMng::g_pIwTopMenu) {
		delete PrimMng::g_pIwTopMenu;
		PrimMng::g_pIwTopMenu = nullptr;
	}
	if (PrimMng::g_pIwLobbyMenu) {
		delete PrimMng::g_pIwLobbyMenu;
		PrimMng::g_pIwLobbyMenu = nullptr;
	}
	if (PrimMng::g_pIwRaceMenu) {
		delete PrimMng::g_pIwRaceMenu;
		PrimMng::g_pIwRaceMenu = nullptr;
	}
	if (PrimMng::g_pIwFaceMenu) {
		delete PrimMng::g_pIwFaceMenu;
		PrimMng::g_pIwFaceMenu = nullptr;
	}
	if (PrimMng::g_pIwHairMenu) {
		delete PrimMng::g_pIwHairMenu;
		PrimMng::g_pIwHairMenu = nullptr;
	}
	if (PrimMng::g_pIwSizeMenu) {
		delete PrimMng::g_pIwSizeMenu;
		PrimMng::g_pIwSizeMenu = nullptr;
	}
	if (PrimMng::g_pIwJobsMenu) {
		delete PrimMng::g_pIwJobsMenu;
		PrimMng::g_pIwJobsMenu = nullptr;
	}
	if (PrimMng::g_pIwSelectMenu) {
		delete PrimMng::g_pIwSelectMenu;
		PrimMng::g_pIwSelectMenu = nullptr;
	}
	if (PrimMng::g_pIwChfWin) {
		delete PrimMng::g_pIwChfWin;
		PrimMng::g_pIwChfWin = nullptr;
	}
	if (PrimMng::g_pIwNetWin) {
		delete PrimMng::g_pIwNetWin;
		PrimMng::g_pIwNetWin = nullptr;
	}
	if (PrimMng::g_pIwNetWinSub1) {
		delete PrimMng::g_pIwNetWinSub1;
		PrimMng::g_pIwNetWinSub1 = nullptr;
	}
	if (PrimMng::g_pIwPatchSub4) {
		delete PrimMng::g_pIwPatchSub4;
		PrimMng::g_pIwPatchSub4 = nullptr;
	}
	if (PrimMng::g_pIwLicenceMenu) {
		delete PrimMng::g_pIwLicenceMenu;
		PrimMng::g_pIwLicenceMenu = nullptr;
	}

	if (IwManager::RES_1) {
		(*IwManager::RES_1)->SomeClean3();
		CMoResource::UnlinkFromManager((CYy::CMoResource***)&IwManager::RES_1);
		IwManager::RES_1 = nullptr;
	}
}

int FFXI::CTk::IwManager::IwNetLoginPhase(int a1, int a2, int a3, int a4, int a5)
{
	if (FFXI::CYy::CApp::g_pCApp->g_pNT_SYS->ClientMode == 255) {
		SetDefaultAccount();
		return true;
	}
		
	int total = NT_SYS::getTotalModePhase(1);
	bool v29{ false };
	if (a3)
		total += NT_SYS::getTotalModePhase(19);
	
	switch (a4) {
	case 1:
		total += NT_SYS::getTotalModePhase(3);
		break;
	case 2:
		total += NT_SYS::getTotalModePhase(5);
		total += NT_SYS::getTotalModePhase(19);
		total += NT_SYS::getTotalModePhase(2);
		break;
	case 3:
		if (FFXI::CYy::CApp::g_pCApp->g_pNT_SYS->IsConnect()) {
			total += NT_SYS::getTotalModePhase(20);
			total += NT_SYS::getTotalModePhase(4);
			total += NT_SYS::getTotalModePhase(19);
			total += NT_SYS::getTotalModePhase(2);
			v29 = true;
		}
		else {
			total += NT_SYS::getTotalModePhase(4);
			total += NT_SYS::getTotalModePhase(19);
			total += NT_SYS::getTotalModePhase(2);
		}
		break;
	case 4:
		total += NT_SYS::getTotalModePhase(20);
		break;
	case 5:
		total += NT_SYS::getTotalModePhase(22);
		total += NT_SYS::getTotalModePhase(19);
		total += NT_SYS::getTotalModePhase(2);
		break;
	case 6:
		total += NT_SYS::getTotalModePhase(23);
		total += NT_SYS::getTotalModePhase(19);
		total += NT_SYS::getTotalModePhase(2);
		total += NT_SYS::getTotalModePhase(3);
		break;
	default:
		break;
	}

	PrimMng::g_pIwNetWin->HelpText[0] = 0;
	PrimMng::g_pIwNetWinSub1->progress = 0.0;

	TkManager::g_CTkMenuMng.CreateDrawMenu(Constants::Strings::MenuNetWait, 1, 0);
	TkManager::g_CTkMenuMng.CreateDrawMenu(Constants::Strings::MenuNetBar, 1, 0);

	if (a1) {
		task_buff->task_wait(0);
		FFXI::CYy::CApp::g_pCApp->g_pNT_SYS->modeSet(0, 0);
		if (task_buff->task_wait(0) < 0) {
			TkManager::g_CTkMenuMng.DestroyDrawMenu(Constants::Strings::MenuNetWait);
			TkManager::g_CTkMenuMng.DestroyDrawMenu(Constants::Strings::MenuNetBar);
			int lasterr = FFXI::CYy::CApp::g_pCApp->g_pNT_SYS->getLastError();
			IwManager::IwErrorConst(lasterr);
			task_buff->task_wait(0);
			return -1;
		}
		while (true) {
			int v13 = FFXI::CYy::CApp::g_pCApp->g_pNT_SYS->modeGet();
			if (!v13)
				break;
			if (v13 > 200 || task_buff->task_wait(0) < 0) {
				TkManager::g_CTkMenuMng.DestroyDrawMenu(Constants::Strings::MenuNetWait);
				TkManager::g_CTkMenuMng.DestroyDrawMenu(Constants::Strings::MenuNetBar);
				int lasterr = FFXI::CYy::CApp::g_pCApp->g_pNT_SYS->getLastError();
				IwManager::IwErrorConst(lasterr);
				task_buff->task_wait(0);
				return -1;
			}
		}
	}

	char v30[256];
	if (a4 == 5)
		strcpy_s(v30, FFXI::CYy::CApp::g_pCApp->g_pNT_SYS->getChrName(a5));
	if (WaitAndUpdateProgress(1, 0, 0, total) < 0) {
		TkManager::g_CTkMenuMng.DestroyDrawMenu(Constants::Strings::MenuNetWait);
		TkManager::g_CTkMenuMng.DestroyDrawMenu(Constants::Strings::MenuNetBar);
		int lasterr = FFXI::CYy::CApp::g_pCApp->g_pNT_SYS->getLastError();
		IwManager::IwErrorConst(lasterr);
		task_buff->task_wait(0);
		return -1;
	}

	int v14 = NT_SYS::getTotalModePhase(1);
	if (a3) {
		if (WaitAndUpdateProgress(19, 0, v14, total) < 0) {
			TkManager::g_CTkMenuMng.DestroyDrawMenu(Constants::Strings::MenuNetWait);
			TkManager::g_CTkMenuMng.DestroyDrawMenu(Constants::Strings::MenuNetBar);
			int lasterr = FFXI::CYy::CApp::g_pCApp->g_pNT_SYS->getLastError();
			IwManager::IwErrorConst(lasterr);
			IwManager::IwErrorConst(115);
			task_buff->task_wait(0);
			return -2;
		}
		v14 += NT_SYS::getTotalModePhase(19);
	}

	switch (a4) {
	case 1:
		if (WaitAndUpdateProgress(3, a5, v14, total) < 0) {
			TkManager::g_CTkMenuMng.DestroyDrawMenu(Constants::Strings::MenuNetWait);
			TkManager::g_CTkMenuMng.DestroyDrawMenu(Constants::Strings::MenuNetBar);
			int lasterr = FFXI::CYy::CApp::g_pCApp->g_pNT_SYS->getLastError();
			IwManager::IwErrorConst(lasterr);
			task_buff->task_wait(0);
			return -1;
		}
		IwLoadCharInfo();
		break;
	default:
		IwLoadCharInfo();
		break;
	}

	PrimMng::g_pIwNetWinSub1->progress = 1.0;
	
	for (int i = 0; i < 10; ++i)
		IwManager::task_buff->task_wait(0);

	TkManager::g_CTkMenuMng.DestroyDrawMenu(Constants::Strings::MenuNetWait);
	TkManager::g_CTkMenuMng.DestroyDrawMenu(Constants::Strings::MenuNetBar);
	return 0;
}

int FFXI::CTk::IwManager::IwGetPolIrcStts()
{
	return polircstts;
}

//Transfers content ID info from NT_SYS to SelectMenu
int FFXI::CTk::IwManager::IwLoadCharInfo()
{
	NT_SYS* sys = FFXI::CYy::CApp::g_pCApp->g_pNT_SYS;
	int totalCharacters = sys->getTotalCharacters();

	memset(PrimMng::g_pIwSelectMenu->charInfo, 0, sizeof(PrimMng::g_pIwSelectMenu));
	PrimMng::g_pIwSelectMenu->field_14 = 0;
	PrimMng::g_pIwSelectMenu->field_18 = 0;
	PrimMng::g_pIwSelectMenu->field_1C = 0;
	PrimMng::g_pIwSelectMenu->ContentIDCount = 0;
	PrimMng::g_pIwSelectMenu->CharacterCount = 0;
	PrimMng::g_pIwSelectMenu->field_2C = 0;
	PrimMng::g_pIwSelectMenu->field_32 = 0;
	PrimMng::g_pIwSelectMenu->field_30 = -1;
	PrimMng::g_pIwSelectMenu->field_31 = -1;

	PrimMng::g_pIwLobbyMenu->field_1C = 0;

	memset(PrimMng::g_pIwLobbyMenu->field_20, 0, sizeof(PrimMng::g_pIwLobbyMenu->field_20));
	PrimMng::g_pIwLobbyMenu->field_20[3] = 1;
	PrimMng::g_pIwLobbyMenu->field_20[4] = 1;
	
	PrimMng::g_pIwSelectMenu->ContentIDCount = 0;
	PrimMng::g_pIwSelectMenu->CharacterCount = 0;

	int goodIDcount{ 0 };
	for (int i = 0; i < sizeof(PrimMng::g_pIwSelectMenu->charInfo)/sizeof(PrimMng::g_pIwSelectMenu->charInfo[0]); ++i) {
		int chrStatus = sys->getChrStatus(i);
		IwCharInfo* charinfo = PrimMng::g_pIwSelectMenu->charInfo + i;
		if (chrStatus == 1 || chrStatus == 2) {
			goodIDcount += 1;
			if (strcmp(sys->getChrName(i), FFXI::Constants::Strings::SpaceString) == 0) {
				if (chrStatus == 1) {
					PrimMng::g_pIwLobbyMenu->field_20[1] = 1;
					PrimMng::g_pIwSelectMenu->ContentIDCount += 1;
				}
			}
			else {
				PrimMng::g_pIwLobbyMenu->field_20[0] = 1;
				PrimMng::g_pIwLobbyMenu->field_20[2] = 1;
				strcpy_s(charinfo->CharName, sys->getChrName(i));
				strcpy_s(charinfo->WorldName, sys->getWorldName(i));
				charinfo->CharIndex = i;
				charinfo->Exists = 1;
				charinfo->World = sys->getWorld(i);
				charinfo->RaceGender = sys->getRaceGender(i);
				charinfo->MainJob = sys->getJob(i);
				charinfo->FaceNo = sys->getFaceNo(i);
				charinfo->TownNo = sys->getTownNo(i);
				charinfo->Hair = sys->getHair(i);
				charinfo->Size = sys->getSize(i);
				charinfo->MainJobLevel = sys->getLv(i);
				if (chrStatus == 1) {
					charinfo->field_60 = 0;
					PrimMng::g_pIwSelectMenu->CharacterCount += 1;
					PrimMng::g_pIwSelectMenu->ContentIDCount += 1;
				}
				else
					charinfo->field_60 = 1;

				if (charinfo->MainJobLevel == 0)
					charinfo->MainJobLevel = 1;

				int Area = sys->getArea(i);
				const char* AreaName = FFXI::Text::XiStrGet(FFXI::Constants::Enums::StrTables::XIStr_ZoneShortNames, Area);
				if (AreaName == nullptr)
					AreaName = FFXI::Constants::Strings::Unknown;
				strcpy_s(charinfo->AreaName, AreaName);

				short* Equip = sys->getEquip(i);
				for (int j = 0; j < sizeof(charinfo->Equip) / sizeof(charinfo->Equip[0]); ++j)
					charinfo->Equip[j] = Equip[j];
			}
		}
	}

	if (!PrimMng::g_pIwLobbyMenu->field_20[1] && totalCharacters == 16)
		PrimMng::g_pIwLobbyMenu->field_20[1] = 2;

	return goodIDcount;
}

void FFXI::CTk::IwManager::IwErrorConst(int a2)
{
	IwSetErrorStringMain(a2);
	IwOkMenuConst(g_ErrMsg);
}

void FFXI::CTk::IwManager::IwSetErrorStringMain(int a1)
{
	char str[1024];
	const char* v1 = FFXI::Text::XiStrGet(FFXI::Constants::Enums::StrTables::XIStr_Errors, 0);
	int errornumber = 3000 + abs(a1);
	sprintf_s(str, v1, errornumber);
	*(short*)&str[strlen(str)] = *(short*)Constants::Strings::NewLineStr;

	int errorPage = FFXI::Constants::Enums::StrTables::XIStr_Errors;
	int errorIndex = 0x3E;
	for (int i = 0; i < sizeof(ErrorTable) / sizeof(ErrorTable[0]); ++i) {
		ErrorEntry* err = ErrorTable + i;
		if (err->errorID == errornumber) {
			errorPage = err->PageIndex;
			errorIndex = err->EntryIndex;
			break;
		}
	}
	const char* v2 = Text::XiStrGet(errorPage, errorIndex);
	if (v2 == nullptr)
		v2 = "Unspecified error.";
	strcat_s(str, v2);
	PrimMng::g_pIwOkMenu->SetErrorString(str);
}

void FFXI::CTk::IwManager::IwOkMenuConst(const char* a1)
{
	CIwOkMenu* menu = PrimMng::g_pIwOkMenu;
	IwManager::iwfdi();
	menu->field_258 = 0;
	if (!menu->field_14) {
		menu->field_25C = 0;
		menu->field_24C.__as(nullptr);
		menu->field_14 = 0;
	}
	menu->field_24C.__as(a1);
	TkManager::g_CTkMenuMng.CreateDrawMenu(Constants::Strings::MenuOk, true, 0);
	IwManager::task_buff->task_wait(&menu->field_25C);
	TkManager::g_CTkMenuMng.DestroyDrawMenu(Constants::Strings::MenuOk);
}

void FFXI::CTk::IwManager::iwfdi()
{
	CYyDb::g_pCYyDb->pCMoResourceMng->LoadStartScheduler(0x81A9, 'nifs', 0, 0, 0);
	CYyDb::g_pCYyDb->pCMoResourceMng->LoadStartScheduler(0x81A9, 'niff', 0, 0, 0);
	IwSaMainIdle();
}

void FFXI::CTk::IwManager::iwfdi2()
{
	CYyDb::g_pCYyDb->pCMoResourceMng->LoadStartScheduler(0x81A9, 'nifs', 0, 0, 0);
	IwSaMainIdle();
}

void FFXI::CTk::IwManager::iwfdo()
{
	CYyDb::g_pCYyDb->pCMoResourceMng->LoadStartScheduler(0x81A9, 'tofs', 0, 0, 0);
	CYyDb::g_pCYyDb->pCMoResourceMng->LoadStartScheduler(0x81A9, 'toff', 0, 0, 0);
	IwSaMainIdle2();
}

void FFXI::CTk::IwManager::iwfdo2()
{
	CYyDb::g_pCYyDb->pCMoResourceMng->LoadStartScheduler(0x81A9, 'tofs', 0, 0, 0);
	IwSaMainIdle2();
}


void FFXI::CTk::IwManager::IwSaMainIdle()
{
	for (int i = 0; i < 8; ++i)
		task_buff->task_wait(nullptr);
}

void FFXI::CTk::IwManager::IwSaMainIdle2()
{
	for (int i = 0; i < 34; ++i)
		task_buff->task_wait(nullptr);
}

CYyResourceFile*** FFXI::CTk::IwManager::BootTitleDemo(CYyResourceFile*** a2, int a3)
{
	CYyResourceFile** unused{ nullptr };
	CYyResourceFile** file = *CYyDb::g_pCYyDb->pCMoResourceMng->LoadNumFile(&unused, a3 + 23);
	if (file) {
		CMoResource::DoTheThing(*file);
		CYy::CYyScheduler** sched = *(CYyScheduler***)(*file)->FindResourceUnder((CMoResource***)&unused, Constants::Enums::ResourceType::Scheduler, 'niam');
		if (sched && *sched)
			(*sched)->Execute(0, nullptr);
	}

	*a2 = file;
	return a2;
}

void FFXI::CTk::IwManager::IwBootDemo(int a1)
{
	if (title_demo) return;
	BootTitleDemo(&title_demo, a1);
}

void FFXI::CTk::IwManager::IwReleaseDemo()
{
	if (title_demo && *title_demo) {
		(*title_demo)->StopMovers();
		if (title_demo) {
			(*title_demo)->SomeClean3();
			CMoResource::UnlinkFromManager((CYy::CMoResource***)&title_demo);
			title_demo = nullptr;
		}
	}
}
