#include "CIwRaceMenu.h"
#include "CTkMenuCtrlData.h"
#include "SoundMng.h"
#include "RegistryConfig.h"
#include "TkManager.h"
#include "CTkMenuMng.h"
#include "Strings.h"
#include "PrimMng.h"
#include "CIwFaceMenu.h"
#include "CIwHairMenu.h"
#include "CIwOnePic.h"

using namespace FFXI::CTk;

FFXI::CTk::CIwRaceMenu::CIwRaceMenu()
{
   this->field_20 = 0;
   this->field_18 = 0;
   this->field_1C = -1;
   this->field_14 = 0;
}

void FFXI::CTk::CIwRaceMenu::OnInitialUpdatePrimitive()
{
	this->MenuCtrlData->field_5D = 0;
	this->MenuCtrlData->field_85 = 0;
	this->MenuCtrlData->field_86 = 0;
	this->MenuCtrlData->field_84 = 0;
}

void FFXI::CTk::CIwRaceMenu::OnDrawPrimitive()
{
	//nullsub
}

void FFXI::CTk::CIwRaceMenu::OnUpdatePrimitive()
{
	this->MenuCtrlData->field_4C = (this->field_14 & 0xFF) + 1;
	this->MenuCtrlData->RepositionCursol(this->MenuCtrlData->field_4C);
}

void FFXI::CTk::CIwRaceMenu::OnKeyDown(short a2, short a3)
{
	if (this->field_20 == 1) {
		if (this->MenuCtrlData)
			this->MenuCtrlData->SetCursol((this->field_14 & 0xFF) + 1, 0);
	}
	else {
		switch (a2) {
		case 9:
			if (a3 >= 9 && a3 <= 10) break;
			//FALLTHROUGH
		case 1:
		case 2:
			if (a3 < 9) {
				this->field_14 = a3 - 1;
				if (this->field_1C != a3) {
					this->DispRace(a3 - 1);
					SoundMng::CYySePlayCursor();
				}
			}
			if (FFXI::Config::MainRegistryConfig::SimplifyCharCreateVisuals) {
				PrimMng::g_pIwOnePic->field_1C = PrimMng::g_pIwHairMenu->field_14;
				PrimMng::g_pIwOnePic->field_1C <<= 8;
				PrimMng::g_pIwOnePic->field_1C |= PrimMng::g_pIwRaceMenu->field_14;
				PrimMng::g_pIwOnePic->field_1C <<= 8;
				PrimMng::g_pIwOnePic->field_1C |= PrimMng::g_pIwFaceMenu->field_14;
			}
			else {

			}
			this->field_1C = a3;
			break;
		case 4:
		case 6:
			this->field_18 = -1;
			this->field_20 = 1;
			SoundMng::CYySePlayCancel();
			this->field_1C = a3;
			break;
		case 5:
			if (a3 == 9) {
				this->OnKeyDown(4, 9);
				this->field_1C = 9;
				break;
			}
			//FALLTHROUGH
		case 3:
			if (a3 != 9 && a3 != 10)
				this->field_14 = a3 - 1;
			this->field_18 = 1;
			this->field_20 = 1;
			SoundMng::CYySePlayClick();
			this->field_1C = a3;
			break;
		}
	}
}

void FFXI::CTk::CIwRaceMenu::DispRace(int a2)
{
	TkManager::g_CTkMenuMng.DestroyDrawMenu(FFXI::Constants::Strings::MenuRace1);
	TkManager::g_CTkMenuMng.DestroyDrawMenu(FFXI::Constants::Strings::MenuRace2);
	TkManager::g_CTkMenuMng.DestroyDrawMenu(FFXI::Constants::Strings::MenuRace3);
	TkManager::g_CTkMenuMng.DestroyDrawMenu(FFXI::Constants::Strings::MenuRace4);
	TkManager::g_CTkMenuMng.DestroyDrawMenu(FFXI::Constants::Strings::MenuRace5);
	TkManager::g_CTkMenuMng.DestroyDrawMenu(FFXI::Constants::Strings::MenuRace6);
	TkManager::g_CTkMenuMng.DestroyDrawMenu(FFXI::Constants::Strings::MenuRace7);
	TkManager::g_CTkMenuMng.DestroyDrawMenu(FFXI::Constants::Strings::MenuRace8);
	switch (a2) {
	case 0:
		TkManager::g_CTkMenuMng.CreateDrawMenu(FFXI::Constants::Strings::MenuRace1, true, 0);
		this->field_1C = a2;
		break;
	case 1:
		TkManager::g_CTkMenuMng.CreateDrawMenu(FFXI::Constants::Strings::MenuRace2, true, 0);
		this->field_1C = a2;
		break;
	case 2:
		TkManager::g_CTkMenuMng.CreateDrawMenu(FFXI::Constants::Strings::MenuRace3, true, 0);
		this->field_1C = a2;
		break;
	case 3:
		TkManager::g_CTkMenuMng.CreateDrawMenu(FFXI::Constants::Strings::MenuRace4, true, 0);
		this->field_1C = a2;
		break;
	case 4:
		TkManager::g_CTkMenuMng.CreateDrawMenu(FFXI::Constants::Strings::MenuRace5, true, 0);
		this->field_1C = a2;
		break;
	case 5:
		TkManager::g_CTkMenuMng.CreateDrawMenu(FFXI::Constants::Strings::MenuRace6, true, 0);
		this->field_1C = a2;
		break;
	case 6:
		TkManager::g_CTkMenuMng.CreateDrawMenu(FFXI::Constants::Strings::MenuRace7, true, 0);
		this->field_1C = a2;
		break;
	case 7:
		TkManager::g_CTkMenuMng.CreateDrawMenu(FFXI::Constants::Strings::MenuRace8, true, 0);
		this->field_1C = a2;
		break;
	case 8:
		TkManager::g_CTkMenuMng.CreateDrawMenu(FFXI::Constants::Strings::MenuRace1, true, 0);
		this->field_1C = a2;
		break;
	}
}
