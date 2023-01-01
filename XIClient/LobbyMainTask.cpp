#include <wtypes.h>
#include "XiDateTime.h"
#include "Globals.h"
#include "XIFileManager.h"
#include "CIwLicenceMenu.h"
#include "IwManager.h"
#include "CIwSelectMenu.h"
#include "SoundMng.h"
#include "CYyCamMng2.h"
#include "TaskBuff.h"
#include "Task.h"
#include "PrimMng.h"
#include "CYyDb.h"
#include "CYyResourceFile.h"
#include "FsConfig.h"
#include "Values.h"
#include "MusicServer.h"
#include "TkManager.h"
#include "Strings.h"
#include "CApp.h"
#include "CIwPatchSub4.h"
#include "XiZone.h"
#include "CIwLobbyMenu.h"
#include "CIwOnePic.h"
#include "CTkMenuMng.h"
#include "StringTables.h"
#include "XiZone.h"
#include "CTkMenuCtrlData.h"
#include "CEnv.h"
#include "StDancer.h"
#include "CIwRaceMenu.h"
#include "CIwFaceMenu.h"
#include "CIwHairMenu.h"
#include "CIwSizeMenu.h"
#include "CIwJobsMenu.h"
#include "CMoTaskMng.h"
#include "XiAtelBuff.h"
#include "XiDancerActorPara.h"
#include "CXiDancerActor.h"

using namespace FFXI::CTk;
using namespace FFXI;

int phaseix{ 0 };
void clearmenuall() {

	TkManager::g_CTkMenuMng.DestroyDrawMenu(FFXI::Constants::Strings::MenuRace1);
	TkManager::g_CTkMenuMng.DestroyDrawMenu(FFXI::Constants::Strings::MenuRace2);
	TkManager::g_CTkMenuMng.DestroyDrawMenu(FFXI::Constants::Strings::MenuRace3);
	TkManager::g_CTkMenuMng.DestroyDrawMenu(FFXI::Constants::Strings::MenuRace4);
	TkManager::g_CTkMenuMng.DestroyDrawMenu(FFXI::Constants::Strings::MenuRace5);
	TkManager::g_CTkMenuMng.DestroyDrawMenu(FFXI::Constants::Strings::MenuRace6);
	TkManager::g_CTkMenuMng.DestroyDrawMenu(FFXI::Constants::Strings::MenuRace7);
	TkManager::g_CTkMenuMng.DestroyDrawMenu(FFXI::Constants::Strings::MenuRace8);
	TkManager::g_CTkMenuMng.DestroyDrawMenu(FFXI::Constants::Strings::MenuCharMakeRace);
	TkManager::g_CTkMenuMng.DestroyDrawMenu(FFXI::Constants::Strings::MenuCharMakeFace);
	TkManager::g_CTkMenuMng.DestroyDrawMenu(FFXI::Constants::Strings::MenuCharMakeHair);
	TkManager::g_CTkMenuMng.DestroyDrawMenu(FFXI::Constants::Strings::MenuCharMakeSize);
	TkManager::g_CTkMenuMng.DestroyDrawMenu(FFXI::Constants::Strings::MenuCharMakeJobs);
	TkManager::g_CTkMenuMng.DestroyDrawMenu(FFXI::Constants::Strings::MenuDummy);

}
void SetTime(int a1, int a2) {
	if (FFXI::CYy::XiZone::zone == nullptr) return;

	XiDateTime v5(0), v6{}, v7{};
	XiDateTime::GetCurrentDateTime(&v7);
	v6.SetTickFrame(v7.Time);
	v6.SetHour(a1);
	v6.SetMinute(a2);
	XiDateTime::SetCurrentDateTime(v6.Time);
	FFXI::CYy::XiZone::zone->SetWeather('ynus', &v7, v5.Time);
}

void SelectMenuChrInit(int a1) {
	CIwSelectMenu::g_userfile = a1;
	FFXI::CYy::XiZone::Open(611 + FFXI::Constants::Enums::Zone::Lobby);

	XiDateTime v58(0), v60{};
	XiDateTime::GetCurrentDateTime(&v60);
	FFXI::CYy::XiZone::zone->SetWeather('ynus', &v60, v58.Time);
	//sub //TODO statics
	PrimMng::g_pIwSelectMenu->field_B08 = { 0.0, 0.0, 0.0, 1.0 };
	Globals::ThreeFloatScale(&PrimMng::g_pIwSelectMenu->field_B08.x, &PrimMng::g_pIwSelectMenu->field_B08.x, -1.0);

	//vcalibrate
	//sub //TODO
	FFXI::Math::WMatrix v78{};
	v78.Identity();
	v78.RotateY(FFXI::Constants::Values::ANGLE_PI_OVER_8);

	D3DXVECTOR4 v64 = { 4.0, 0.0, 0.0, 1.0 };
	D3DXVECTOR4 v69 = { 0.0, 0.0, 0.0, 1.0 };
	//Clear first 16 buffer pointers
	memset(CYy::XiAtelBuff::AtelBuffers, 0, 0x40);
	for (int i = 0; i < sizeof(PrimMng::g_pIwSelectMenu->charInfo) / sizeof(PrimMng::g_pIwSelectMenu->charInfo[0]); ++i) {
		IwCharInfo* charinfo = PrimMng::g_pIwSelectMenu->charInfo + i;
		
		char* mem = StorageLists::instance->Get(sizeof(CYy::XiAtelBuff), FFXI::Constants::Enums::MEM_MODE::Ex);

		if (mem)
			CYy::XiAtelBuff::AtelBuffers[i] = new (mem) CYy::XiAtelBuff();

		v78.Vec4MultiplySelf(&v64);

		D3DXVECTOR3 v75{};
		Globals::ThreeFloatScale(&v75.x, &v64.x, -1.0);
		v69.y = -atan2(v75.z, v75.x);
		D3DXVECTOR4 v2 = v64;
		//VCalibrate //sub //TODO
		charinfo->field_78 = v2;

		FFXI::Math::WMatrix v77{};
		v77.Identity();
		v77.RotateY(v69.y);

		D3DXVECTOR4 v62 = { -4.0, 0.0, 0.0, 1.0 };
		v77.Vec4MultiplySelf(&v62);
		Globals::ThreeFloatScale(&v62.x, &v62.x, 0.35f);

		charinfo->field_88.x = charinfo->field_78.x - v62.x;
		charinfo->field_88.y = charinfo->field_78.y - v62.y;
		charinfo->field_88.z = charinfo->field_78.z - v62.z;

		D3DXVECTOR4 v59{};
		v59.x = charinfo->field_78.x - charinfo->field_88.x;
		v59.y = charinfo->field_78.y - charinfo->field_88.y;
		v59.z = charinfo->field_78.z - charinfo->field_88.z;
		Globals::ThreeFloatScale(&v59.x, &v59.x, -1.0);
		v59.x = v59.x + charinfo->field_88.x;
		v59.y = v59.y + charinfo->field_88.y;
		v59.z = v59.z + charinfo->field_88.z;
		charinfo->field_98.x = v59.x;
		charinfo->field_98.y = v59.y;
		charinfo->field_98.z = v59.z;
		charinfo->field_98.w = v59.w;
		
		if (charinfo->Exists == 0) {
			charinfo->field_74 = nullptr;
		}
		else {
			char name[16];
			strcpy_s(name, charinfo->CharName);

			short data[11] = { 0 };
			for (int j = 0; j < 8; ++j) {
				data[j] = charinfo->Equip[j];
			}
			data[0] &= 0x00FF;

			data[9] = charinfo->RaceGender;
			data[10] = charinfo->Size;

			CYy::XiDancerActorPara para = CYy::XiDancerActorPara(name, data);

			mem = StorageLists::instance->Get(sizeof(CYy::CXiDancerActor), FFXI::Constants::Enums::MEM_MODE::Ex);
			if (mem) {
				CYy::CMoTaskMng::DeleteThisTask = true;
				CYy::CXiDancerActor* actor = new (mem) CYy::CXiDancerActor(&v2, &v69, &para, i);
				
				actor->field_34 = *actor->VirtActor101();
				actor->field_C4 = *actor->VirtActor101();
				actor->field_44 = *actor->VirtActor102();
				Globals::PiClamp(&actor->field_44);

				actor->SetPos(actor->VirtActor101());
				actor->field_5C4 = *actor->VirtActor101();
				CYyDb::g_pCYyDb->pCMoTaskMng->SomeShift4(actor, 4);
				charinfo->field_74 = actor;
			}
		}
	}
	PrimMng::g_pIwSelectMenu->field_B18.x = PrimMng::g_pIwSelectMenu->charInfo[0].field_78.x;
	PrimMng::g_pIwSelectMenu->field_B18.y = PrimMng::g_pIwSelectMenu->charInfo[0].field_78.y;
	PrimMng::g_pIwSelectMenu->field_B18.z = PrimMng::g_pIwSelectMenu->charInfo[0].field_78.z;
	PrimMng::g_pIwSelectMenu->field_B18.w = PrimMng::g_pIwSelectMenu->charInfo[0].field_78.w;
	PrimMng::g_pIwSelectMenu->field_B28.x = PrimMng::g_pIwSelectMenu->charInfo[0].field_98.x;
	PrimMng::g_pIwSelectMenu->field_B28.y = PrimMng::g_pIwSelectMenu->charInfo[0].field_98.y;
	PrimMng::g_pIwSelectMenu->field_B28.z = PrimMng::g_pIwSelectMenu->charInfo[0].field_98.z;
	PrimMng::g_pIwSelectMenu->field_B28.w = PrimMng::g_pIwSelectMenu->charInfo[0].field_98.w;

	PrimMng::g_pIwSelectMenu->field_B18.y -= 1.0;
	PrimMng::g_pIwSelectMenu->field_B28.y -= 1.0;

	CYyDb::g_pCYyDb->CameraManager->SetAt((D3DXVECTOR3*)&PrimMng::g_pIwSelectMenu->field_B18);
	CYyDb::g_pCYyDb->CameraManager->SetPos((D3DXVECTOR3*)&PrimMng::g_pIwSelectMenu->field_B28);

	PrimMng::g_pIwSelectMenu->field_AF8[0] = 1.0;
	PrimMng::g_pIwSelectMenu->field_AF8[1] = 1.0;
}


int SelectMenuProc() {

	//sub //TODO
	SelectMenuChrInit(1);
	if (CIwSelectMenu::Flag == false) {
		CIwSelectMenu::SelectedCharacter = 0;
	}

	PrimMng::g_pIwSelectMenu->field_1C = 0;
	TkManager::g_CTkMenuMng.CreateDrawMenu(FFXI::Constants::Strings::MenuChfWin, true, 0);
	TkManager::g_CTkMenuMng.CreateDrawMenu(FFXI::Constants::Strings::MenuDbNameSE, true, 0);
	PrimMng::g_pIwSelectMenu->field_14 = 2;
	PrimMng::g_pIwSelectMenu->field_2C = 17;

	//sub //TODO

	IwManager::iwfdi();

	if (CIwSelectMenu::Flag == true) {
		CIwSelectMenu::Flag = false;
		PrimMng::g_pIwSelectMenu->field_1C = 0;
	}
	else {
		CIwSelectMenu::SelectedCharacter = 0;
		PrimMng::g_pIwSelectMenu->field_1C = 0;
	}

	if (PrimMng::g_pIwSelectMenu->MenuCtrlData != nullptr) {
		PrimMng::g_pIwSelectMenu->MenuCtrlData->field_5D = 0;
	}

	if (IwManager::task_buff->task_wait(&PrimMng::g_pIwSelectMenu->field_1C) >= 0) {
		while (true) {
			int polirc = IwManager::IwGetPolIrcStts();
			if (polirc < 0) {
				IwManager::IwErrorConst(polirc);
				break;
			}

			if (CIwSelectMenu::SelectedCharacter == -1) {
				IwManager::iwfdo();
				TkManager::g_CTkMenuMng.DestroyDrawMenu(FFXI::Constants::Strings::MenuDbNameSE);
				TkManager::g_CTkMenuMng.DestroyDrawMenu(FFXI::Constants::Strings::MenuChfWin);
				TkManager::g_CTkMenuMng.DestroyDrawMenu(FFXI::Constants::Strings::MenuChsWin);
				return -1;
			}

			exit(0x102395C4);
		}
	}

	IwManager::iwfdo();
	//sub //TODO

	CIwSelectMenu::SelectedCharacter = 0;
	PrimMng::g_pIwSelectMenu->field_1C = 0;
	if (PrimMng::g_pIwSelectMenu->MenuCtrlData != nullptr) {
		PrimMng::g_pIwSelectMenu->MenuCtrlData->field_5D = 0;
	}

	TkManager::g_CTkMenuMng.DestroyDrawMenu(FFXI::Constants::Strings::MenuDbNameSE);
	TkManager::g_CTkMenuMng.DestroyDrawMenu(FFXI::Constants::Strings::MenuChfWin);
	TkManager::g_CTkMenuMng.DestroyDrawMenu(FFXI::Constants::Strings::MenuChsWin);
	//sub //TODO
	return -2;
}

int CharMakeProcess() {
	if (Globals::g_pCenv)
		Globals::g_pCenv->field_1E = 0;

	phaseix = 0;
	StDancer::isdanceractive = false;

	XiDateTime v111{};

LABEL_INIT:
	PrimMng::g_pIwOnePic->init(0);
	TkManager::g_CTkMenuMng.CreateDrawMenu(FFXI::Constants::Strings::MenuDummy, true, 0);
	if (FFXI::Config::MainRegistryConfig::SimplifyCharCreateVisuals) {
		if (CYy::XiZone::zone)
			CYy::XiZone::zone->Close();
		PrimMng::g_pIwOnePic->field_1C = PrimMng::g_pIwHairMenu->field_14;
		PrimMng::g_pIwOnePic->field_1C <<= 8;
		PrimMng::g_pIwOnePic->field_1C |= PrimMng::g_pIwRaceMenu->field_14;
		PrimMng::g_pIwOnePic->field_1C <<= 8;
		PrimMng::g_pIwOnePic->field_1C |= PrimMng::g_pIwFaceMenu->field_14;
		//sub //TODO playmusic
	}
	else if (TkManager::g_pStDancer->IsCheatActive(16)) {
		throw "not implemented";
	}
	else {
		v111.SetHour(11);
		XiDateTime::SetCurrentDateTime(v111.Time);
		CYy::XiZone::Open(611 + FFXI::Constants::Enums::Zone::Lobby);
		XiDateTime v118{};
		XiDateTime::GetCurrentDateTime(&v118);
		XiDateTime v103(0);
		CYy::XiZone::zone->SetWeather('ynus', &v118, v103.Time);
		//sub //TODO volume
		D3DXVECTOR3 pos = { 0.0, -5.0, 40.0 };
		D3DXVECTOR3 look = { 0.0, 0.0, 0.0 };
		CYyDb::g_pCYyDb->CameraManager->SetPos(&pos);
		CYyDb::g_pCYyDb->CameraManager->SetAt(&look);
		TkManager::g_pStDancer->SetVisible(PrimMng::g_pIwRaceMenu->field_14, 1, PrimMng::g_pIwFaceMenu->field_14, 1);
	}

	//sub //TODO
	PrimMng::g_pIwPatchSub4->field_18 = PrimMng::g_pIwRaceMenu->field_14;
	//PrimMng::g_pIwPatchSub4->field_14 = 1;
	PrimMng::g_pIwRaceMenu->DispRace(PrimMng::g_pIwRaceMenu->field_14);
	IwManager::iwfdi2();
	StDancer::isdanceractive = true;
		
	LABEL_CREATE:
	switch (phaseix) {
	case 0:
		PrimMng::g_pIwRaceMenu->field_20 = 0;
		PrimMng::g_pIwRaceMenu->field_18 = 0;
		PrimMng::g_pIwRaceMenu->field_1C = -1;
		TkManager::g_CTkMenuMng.CreateDrawMenu(FFXI::Constants::Strings::MenuCharMakeRace, true, 0);
		break;
	case 1:
		PrimMng::g_pIwFaceMenu->field_1C = 0;
		PrimMng::g_pIwFaceMenu->field_18 = 0;
		TkManager::g_CTkMenuMng.CreateDrawMenu(FFXI::Constants::Strings::MenuCharMakeFace, true, 0);
		break;
	case 2:
		PrimMng::g_pIwHairMenu->field_1C = 0;
		PrimMng::g_pIwHairMenu->field_18 = 0;
		TkManager::g_CTkMenuMng.CreateDrawMenu(FFXI::Constants::Strings::MenuCharMakeHair, true, 0);
		break;
	case 3:
		PrimMng::g_pIwSizeMenu->field_1C = 0;
		PrimMng::g_pIwSizeMenu->field_18 = 0;
		TkManager::g_CTkMenuMng.CreateDrawMenu(FFXI::Constants::Strings::MenuCharMakeSize, true, 0);
		break;
	case 4:
		PrimMng::g_pIwJobsMenu->field_1C = 0;
		PrimMng::g_pIwJobsMenu->field_18 = 0;
		TkManager::g_CTkMenuMng.CreateDrawMenu(FFXI::Constants::Strings::MenuCharMakeJobs, true, 0);
		break;
	}

	int v14 = 0;
	int phaseadvance = 0;
	LABEL_WAIT:
	if (IwManager::task_buff->task_wait(0) < 0)
		goto LABEL_EXIT;

	switch (phaseix) {
	case 0:
		if (PrimMng::g_pIwRaceMenu->field_20 == 1) {
			++v14;
			phaseadvance = PrimMng::g_pIwRaceMenu->field_18;
		}
		break;
	case 1:
		if (PrimMng::g_pIwFaceMenu->field_1C == 1) {
			++v14;
			phaseadvance = PrimMng::g_pIwFaceMenu->field_18;
		}
		break;
	case 2:
		if (PrimMng::g_pIwHairMenu->field_1C == 1) {
			++v14;
			phaseadvance = PrimMng::g_pIwHairMenu->field_18;
		}
		break;
	case 3:
		if (PrimMng::g_pIwSizeMenu->field_1C == 1) {
			++v14;
			phaseadvance = PrimMng::g_pIwSizeMenu->field_18;
		}
		break;
	case 4:
		if (PrimMng::g_pIwJobsMenu->field_1C == 1) {
			++v14;
			phaseadvance = PrimMng::g_pIwJobsMenu->field_18;
		}
		break;
	}
	if (!v14)
		goto LABEL_WAIT;
	if (FFXI::Config::MainRegistryConfig::SimplifyCharCreateVisuals)
		CYyDb::g_pCYyDb->SetField338(0);
	switch (phaseix) {
	case 0:
		TkManager::g_CTkMenuMng.DestroyDrawMenu(FFXI::Constants::Strings::MenuCharMakeRace);
		break;
	case 1:
		TkManager::g_CTkMenuMng.DestroyDrawMenu(FFXI::Constants::Strings::MenuCharMakeFace);
		break;
	case 2:
		TkManager::g_CTkMenuMng.DestroyDrawMenu(FFXI::Constants::Strings::MenuCharMakeHair);
		break;
	case 3:
		TkManager::g_CTkMenuMng.DestroyDrawMenu(FFXI::Constants::Strings::MenuCharMakeSize);
		break;
	case 4:
		TkManager::g_CTkMenuMng.DestroyDrawMenu(FFXI::Constants::Strings::MenuCharMakeJobs);
		break;
	}
	if (phaseix == 5) {
		phaseix += phaseadvance;
		if (phaseix == 4)
			TkManager::g_CTkMenuMng.DestroyDrawMenu(FFXI::Constants::Strings::MenuCharMakeName);
	}
	else if (phaseix == 6) {

	}
	else {
		phaseix += phaseadvance;
	}
		
	if (phaseix != -1 && phaseix != 7)
		goto LABEL_CREATE;

	if (FFXI::Config::MainRegistryConfig::SimplifyCharCreateVisuals) {
		TkManager::g_pStDancer->SetVisible(PrimMng::g_pIwRaceMenu->field_14, 1, PrimMng::g_pIwFaceMenu->field_14, 0);
		//sub //TODO
		if (phaseix == -1) {
			//sub //TODO //music
		}
		else {
			//sub //TODO
		}
	}
	else {

	}
	if (Globals::g_pCenv)
		Globals::g_pCenv->field_1E = 1;
	if (phaseix == -1) {
		TkManager::g_pStDancer->isdanceractive = false;
		IwManager::iwfdo();
		clearmenuall();
		IwManager::IwSaMainIdle2();
		//sub //TODO
		IwManager::IwReleaseDemo();
		return -1;
	}
	LABEL_EXIT:
	return -2;
}
int LobbyMenuProc() {
	PrimMng::g_pIwPatchSub4->field_14 = 2;
	PrimMng::g_pIwLobbyMenu->SetHelpText();
	IwManager::IwLoadCharInfo();
	PrimMng::g_pIwLobbyMenu->field_1C = 0;
	TkManager::g_CTkMenuMng.CreateDrawMenu(Constants::Strings::MenuLoby1Win, 1, 0);
	TkManager::g_CTkMenuMng.CreateDrawMenu(Constants::Strings::MenuLoby2Win, 1, 0);
	int waiter = 0;
	while (IwManager::task_buff->task_wait(0) >= 0) {
		if (++waiter < 10) continue;

		IwManager::iwfdi();
		if (IwManager::IwGetPolIrcStts() < 0) {
			//error stuff
			//sub //todo

		}
		else {
			while (IwManager::task_buff->task_wait(&PrimMng::g_pIwLobbyMenu->field_1C) >= 0) {
				if (PrimMng::g_pIwLobbyMenu->ActiveButtonIndex != 4) {
					//sub //TODO
					IwManager::iwfdo();
					TkManager::g_CTkMenuMng.DestroyDrawMenu(Constants::Strings::MenuLoby2Win);
					TkManager::g_CTkMenuMng.DestroyDrawMenu(Constants::Strings::MenuLoby1Win);
					return PrimMng::g_pIwLobbyMenu->ActiveButtonIndex;
				}
				//Config
				PrimMng::g_pIwLobbyMenu->MenuCtrlData->field_5D = 1;

				TkManager::g_CTkMenuMng.CreateDrawMenu(Constants::Strings::MenuLobycwin, true, 0);
				int ok = 0;
				IwManager::task_buff->task_wait(&ok);
				TkManager::g_CTkMenuMng.DestroyDrawMenu(Constants::Strings::MenuLobycwin);
				PrimMng::g_pIwLobbyMenu->field_1C = 0;
				PrimMng::g_pIwLobbyMenu->MenuCtrlData->field_5D = 0;
				PrimMng::g_pIwLobbyMenu->SetHelpText();
				if (IwManager::IwGetPolIrcStts() < 0) {
					IwManager::IwErrorConst(IwManager::IwGetPolIrcStts());
					IwManager::iwfdo();
					TkManager::g_CTkMenuMng.DestroyDrawMenu(Constants::Strings::MenuLoby2Win);
					TkManager::g_CTkMenuMng.DestroyDrawMenu(Constants::Strings::MenuLoby1Win);
					return -2;
				}

			}
		}
		break;
	}
	IwManager::iwfdo();
	TkManager::g_CTkMenuMng.DestroyDrawMenu(Constants::Strings::MenuLoby2Win);
	TkManager::g_CTkMenuMng.DestroyDrawMenu(Constants::Strings::MenuLoby1Win);
	return -2;
}

int DoLicenseWindow() {
	TkManager::g_CTkMenuMng.CreateDrawMenu(Constants::Strings::MenuLobyLice, true, 0);
	IwManager::task_buff->task_wait(&PrimMng::g_pIwLicenceMenu->field_1C);
	TkManager::g_CTkMenuMng.DestroyDrawMenu(Constants::Strings::MenuLobyLice);
	return PrimMng::g_pIwLicenceMenu->field_18;
}
void StopTasks() {
	CYy::TaskBuff* tb = IwManager::task_buff;
	if (!tb->TaskCount) return;

	tb->TaskCount -= 1;
	Task* task = tb->Tasks[tb->TaskCount];
	if (!task->ThreadHandle) return;

	if (task->field_5 == 1)
		task->field_5 = 0;
}

int PatchStartProc() {
	if (CYyDb::SomeFlag) {
		IwManager::iwfdi2();
		IwManager::IwSaMainIdle();
		IwManager::IwSaMainIdle();
	}
	else {
		PrimMng::g_pIwPatchSub4->field_14 = -1;
		PrimMng::g_pIwPatchSub4->field_2C = 16;
		TkManager::g_CTkMenuMng.CreateDrawMenu(Constants::Strings::MenuPtcBGWin, true, 0);
		TkManager::g_CTkMenuMng.CreateDrawMenu(Constants::Strings::MenuLobyHelp, true, 0);
	
		IwManager::iwfdi2();
		IwManager::IwSaMainIdle();
		IwManager::IwSaMainIdle();
	
		int RegionCode = FFXI::GlobalStruct::g_GlobalStruct.GetRegionCode();
		if (RegionCode == Constants::Enums::LanguageCode::Japanese) 
			FFXI::CYy::CApp::g_pCApp->g_pNT_SYS->field_A8 = 3;
		
		else 
			FFXI::CYy::CApp::g_pCApp->g_pNT_SYS->field_A8 = 12;

		if (DoLicenseWindow() == 1) {
			IwManager::iwfdo2();
			TkManager::g_CTkMenuMng.DestroyDrawMenu(Constants::Strings::MenuPtcBGWin);
			TkManager::g_CTkMenuMng.DestroyDrawMenu(Constants::Strings::MenuLobyHelp);
			return -2;
		}
	}

	if (IwManager::IwNetLoginPhase(1, 1, 0, 0, 0) < 0) {
		IwManager::iwfdo2();
		TkManager::g_CTkMenuMng.DestroyDrawMenu(Constants::Strings::MenuPtcBGWin);
		TkManager::g_CTkMenuMng.DestroyDrawMenu(Constants::Strings::MenuLobyHelp);
		return -2;
	}

	if (IwManager::IwLoadCharInfo() == 0) {
		//Error: Valid Content ID Not Found
		char* v2 = FFXI::Text::XiStrGet(8, 0xD9);
		IwManager::IwOkMenuConst(v2);
		IwManager::iwfdo2();
		TkManager::g_CTkMenuMng.DestroyDrawMenu(Constants::Strings::MenuPtcBGWin);
		TkManager::g_CTkMenuMng.DestroyDrawMenu(Constants::Strings::MenuLobyHelp);
		return -2;
	}

	IwManager::iwfdo2();
	TkManager::g_CTkMenuMng.DestroyDrawMenu(Constants::Strings::MenuPtcBGWin);
	TkManager::g_CTkMenuMng.DestroyDrawMenu(Constants::Strings::MenuLobyHelp);
	return 1;
}

DWORD WINAPI LobbyMainThread(LPVOID) {
	XiDateTime v13{};
	const char* VersionString = FFXI::File::XIFileManager::g_pXIFileManager->GetVersionString();
	if (!VersionString)
		VersionString = Constants::Strings::Unknown;

	FFXI::CYy::CApp::g_pCApp->g_pNT_SYS->SetVersionString(VersionString);
	IwManager::SomeByte1 = 0;
	IwManager::SomeByte2 = 1;
	IwManager::SomeByte3 = 0;

	if (FFXI::SoundMng::g_pSoundMng)
		SoundMng::QueueNewSoundCommand(Constants::Enums::SoundCommand::_6, -1, 0, 0, 0, 0, 0, 0);

	FFXI::GlobalStruct::g_GlobalStruct.field_0 = 64;
	CYyDb::g_pCYyDb->field_334 = 0;

	MusicServer::clear_last_request();
	int themesong = Config::FsConfig::GetConfig(Constants::Enums::FsConfigSubjects::TitleScreenSong);
	//sub //TODO //should play from config
	MusicServer::Play(2);
	XiDateTime::DisableGameTimer();

	if (FFXI::CTk::IwManager::patch_skip || PatchStartProc() >= 0) {

		if (IwManager::title_demo) {
			if (*IwManager::title_demo) {
				(*IwManager::title_demo)->StopMovers();
				if (IwManager::title_demo) {
					(*IwManager::title_demo)->SomeClean3();
					CYy::CMoResource::UnlinkFromManager((CYy::CMoResource***)&IwManager::title_demo);
					IwManager::title_demo = nullptr;
				}
				XiDateTime::SetGameTime(12, 30);
				IwManager::BootTitleDemo(&IwManager::title_demo, 0);
			}
		}
		else {
			XiDateTime::SetGameTime(12, 30);
			IwManager::BootTitleDemo(&IwManager::title_demo, 0);
		}

		PrimMng::g_pIwPatchSub4->field_14 = 2;
		//sub //todo FsTextInput
		TkManager::g_CTkMenuMng.CreateDrawMenu(Constants::Strings::MenuLobyHelp, 1, 0);
		int waiter = 0;
		int select{ 0 };
		bool run = true;
		while (run) {
			IwManager::task_buff->task_wait(0);
			if (++waiter >= 10) {
				if (!IwManager::patch_skip || IwManager::IwNetLoginPhase(1, 1, 0, 0, 0) >= 0) {
					while (true) {
						if (!IwManager::title_demo) {
							XiDateTime::SetGameTime(12, 30);
							IwManager::BootTitleDemo(&IwManager::title_demo, 0);
						}
						select = LobbyMenuProc();
						IwManager::IwReleaseDemo();

						if (select == 0) {
							SetTime(15, 30);
							int result = SelectMenuProc();
							if (result == -2) {
								run = false;
								break;
							}
							else if (result == 1) {

							}
						}
						else if (select == 1) {
							SetTime(6, 30);
							int result = CharMakeProcess();
							if (result == -2) {
								run = false;
								break;
							}
							else if (result == 1){

							}
						}
						else if (select == 2) {
							//Delete Character
						}
						else  {
							//Exit
							run = false;
							break;
						}
					}
				}
			}
		}
	}
	
	//sub //todo ntsys terminateTCP
	XiDateTime::EnableGameTimer();
	IwManager::lobby_result = -1;
	
	IwManager::IwReleaseDemo();

	StopTasks();
	if (FFXI::SoundMng::g_pSoundMng)
		SoundMng::QueueNewSoundCommand(Constants::Enums::SoundCommand::MusicFadeOut, -1, 0, 0, 0, 0, 0, 0);

	MusicServer::clear_last_request();
	return 0;
}