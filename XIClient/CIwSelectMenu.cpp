#include "CIwSelectMenu.h"
#include "TkManager.h"
#include "CTkMouse.h"
#include "CTkMenuMng.h"
#include "CTkMenuCtrlData.h"
#include "InputMng.h"
#include "SoundMng.h"
#include "Strings.h"
#include "_49SubList.h"
#include "CTkDrawCtrlFrame.h"
#include "KaWindow.h"
#include "Globals.h"
#include "CYyDb.h"
#include "CYyCamMng2.h"
#include "TextRenderer.h"
#include "StringTables.h"
#include "PrimMng.h"
#include "CIwChfWin.h"
#include <string>
using namespace FFXI::CTk;

bool CIwSelectMenu::Flag{ false };
int CIwSelectMenu::SelectedCharacter{ 0 };
int CIwSelectMenu::g_userfile{ 0 };

const short ContentIDTextSpacings[] = {
	0x241, 0x241, 0x1F2,
	0x190, 0x190, 0x190,
	0x241, 0x241, 0x18A,
	0x23F, 0x23F, 0x18A
};

const short ContentIDGraphicTable[] = {
	0x90, 0x90, 0, 0, 0xA5, 0xA6, 0xA5, 0xA6
};

const short ContentIDXTable[] = { 0, 0, 0x183, 0x183 };
const short ContentIDYTable[] = { 0, 0, 0x23, 0x23 };

void DrawCharInfo(_49SubList*** a1, IwCharInfo* charinfo) {
	int xPos = TkManager::g_CTkMenuMng.Get84(true) * 
		(TkManager::g_CTkMenuMng.UIXRes - TkManager::g_CTkMenuMng.field_7C) 
		* 0.5;

	int yPos = (1.0 - TkManager::g_CTkMenuMng.Get88(true)) *
		(TkManager::g_CTkMenuMng.UIYRes - TkManager::g_CTkMenuMng.field_7E)
		* 0.82589287 - 144;

	//Draw character name
	FFXI::Text::TextRenderer::DrawScriptString(a1, charinfo->CharName, xPos + 16, yPos);

	//Draw Server name
	yPos += 36;
	const char* string = FFXI::Text::XiStrGet(FFXI::Constants::Enums::StrTables::XIStr_CharSelectText, 0);
	FFXI::Text::TextRenderer::DrawScriptString(a1, string, xPos + 16, yPos);
	FFXI::Text::TextRenderer::DrawScriptString(a1, charinfo->WorldName, xPos + 60, yPos);
	
	//Draw race / gender
	yPos += 18;
	string = FFXI::Text::XiStrGet(FFXI::Constants::Enums::StrTables::XIStr_CharSelectText, 1);
	FFXI::Text::TextRenderer::DrawScriptString(a1, string, xPos + 16, yPos);
	string = FFXI::Text::XiStrGet(FFXI::Constants::Enums::StrTables::XIStr_CharSelectRaceGender, charinfo->RaceGender - 1);
	FFXI::Text::TextRenderer::DrawScriptString(a1, string, xPos + 60, yPos);

	//Draw main job
	yPos += 18;
	string = FFXI::Text::XiStrGet(FFXI::Constants::Enums::StrTables::XIStr_CharSelectText, 2);
	FFXI::Text::TextRenderer::DrawScriptString(a1, string, xPos + 16, yPos);
	string = FFXI::Text::XiStrGet(FFXI::Constants::Enums::StrTables::XIStr_JobFullNames, charinfo->MainJob);
	FFXI::Text::TextRenderer::DrawScriptString(a1, string, xPos + 60, yPos);

	//Draw main job level
	yPos += 18;
	string = FFXI::Text::XiStrGet(FFXI::Constants::Enums::StrTables::XIStr_CharSelectText, 3);
	FFXI::Text::TextRenderer::DrawScriptString(a1, string, xPos + 16, yPos);
	char buf[16];
	sprintf_s(buf, "%d", charinfo->MainJobLevel);
	FFXI::Text::TextRenderer::DrawScriptString(a1, buf, xPos + 60, yPos);

	//Draw area name
	yPos += 18;
	string = FFXI::Text::XiStrGet(FFXI::Constants::Enums::StrTables::XIStr_CharSelectText, 4);
	FFXI::Text::TextRenderer::DrawScriptString(a1, string, xPos + 16, yPos);
	FFXI::Text::TextRenderer::DrawScriptString(a1, charinfo->AreaName, xPos + 60, yPos);
}

void DrawContentIDInfo(_49SubList*** a1, int ContentIDCount, int CharacterCount) {

	double v1 = 1.0 - TkManager::g_CTkMenuMng.Get84(true);
	short diff = TkManager::g_CTkMenuMng.UIXRes - TkManager::g_CTkMenuMng.field_7C;
	double xPos = v1 * (double)diff - 640.0;
	char Message[256];
	int RegionCode = FFXI::GlobalStruct::g_GlobalStruct.GetRegionCode();
	if (RegionCode == FFXI::Constants::Enums::LanguageCode::English) {
		sprintf_s(Message, FFXI::Constants::Strings::ContentIDCount, ContentIDCount);
		FFXI::Text::TextRenderer::YkDrawStringScale(xPos + ContentIDTextSpacings[3], 35, Message, 0x80808080, 1.0, 1.0, false);
		sprintf_s(Message, FFXI::Constants::Strings::CharacterCount, CharacterCount);
		FFXI::Text::TextRenderer::YkDrawStringScale(xPos + ContentIDTextSpacings[4], 51, Message, 0x80808080, 1.0, 1.0, false);
		int canmake = ContentIDCount - CharacterCount;
		if (canmake == 1)
			sprintf_s(Message, FFXI::Constants::Strings::CanMakeChar, 1);
		else
			sprintf_s(Message, FFXI::Constants::Strings::CanMakeChars, canmake);
		FFXI::Text::TextRenderer::YkDrawStringScale(xPos + ContentIDTextSpacings[5], 67, Message, 0x80808080, 1.0, 1.0, false);
	}
	else {
		sprintf_s(Message, "%2d", ContentIDCount);
		FFXI::Text::TextRenderer::YkDrawStringScale(xPos + ContentIDTextSpacings[3 * RegionCode], 35, Message, 0x80808080, 1.0, 1.0, false);
		sprintf_s(Message, "%2d", CharacterCount);
		FFXI::Text::TextRenderer::YkDrawStringScale(xPos + ContentIDTextSpacings[3 * RegionCode + 1], 51, Message, 0x80808080, 1.0, 1.0, false);
		
		int canmake = ContentIDCount - CharacterCount;
		sprintf_s(Message, "%2d", canmake);
		FFXI::Text::TextRenderer::YkDrawStringScale(xPos + ContentIDTextSpacings[3 * RegionCode + 2], 67, Message, 0x80808080, 1.0, 1.0, false);
	
		int v9;
		if (RegionCode == FFXI::Constants::Enums::LanguageCode::French)
			v9 = canmake != 1;
		else
			v9 = RegionCode == FFXI::Constants::Enums::LanguageCode::German && canmake > 1;

		int index = ContentIDGraphicTable[2 * RegionCode + v9];
		(*a1)[index]->ExtraDraw(xPos + ContentIDXTable[RegionCode], ContentIDYTable[RegionCode], 1.0, 1.0, 0x80808080, 0, 0, 0);
	}
}

void DrawCharName(short availableWidth, short x, short y, IwCharInfo* charinfo) {
	D3DCOLOR color{ 0x80505050 };
	if (charinfo->field_60 == 0) {
		if (charinfo->RenameFlag == 0)
			color = 0x80808080;
		else
			color = 0x807F7F00;
	}

	FFXI::Text::TextRenderer::YkDrawString(availableWidth, x, y, charinfo->CharName, color);
}

FFXI::CTk::CIwSelectMenu::CIwSelectMenu()
{
	memset(this->charInfo, 0, sizeof(this->charInfo));
	this->field_14 = 0;
	this->field_18 = 0;
	this->field_1C = 0;
	this->ContentIDCount = 0;
	this->CharacterCount = 0;
	this->field_2C = 0;
	this->field_32 = 0;
	this->field_30 = -1;
	this->field_31 = -1;
}

void FFXI::CTk::CIwSelectMenu::OnInitialUpdatePrimitive()
{
	this->field_28 = 0;
	if (CIwSelectMenu::Flag == false) {
		CIwSelectMenu::SelectedCharacter = 0;
	}
	this->field_14 = CIwSelectMenu::SelectedCharacter;
	this->field_18 = CIwSelectMenu::SelectedCharacter;
	this->MenuCtrlData->field_5D = 0;
	this->MenuCtrlData->field_85 = 0;
	this->MenuCtrlData->field_86 = 0;
	this->MenuCtrlData->field_84 = 0;
	this->MenuCtrlData->field_D0 = 1;
	float v5 = 1.0 - TkManager::g_CTkMenuMng.Get84(true) * 0.5;
	float v6 = v5 * (double)TkManager::g_CTkMenuMng.UIYRes;

	float v3 = 1.0 - TkManager::g_CTkMenuMng.Get88(true) * 0.5;
	float v4 = v3 * (double)TkManager::g_CTkMenuMng.UIXRes;

	this->MenuCtrlData->ResizeWindow(
		v4 - 640.0,
		v6 - 480,
		640,
		480,
		1,
		0,
		0);
}

void FFXI::CTk::CIwSelectMenu::OnDrawPrimitive()
{
	_49SubList*** v2 = TkManager::g_CTkMenuDataList.GetSubLists(FFXI::Constants::Strings::MenuLobbyWin);
	
	//Draw FFXI logo
	_49SubList* logo = (*v2)[143];
	tagPOINT pos{};
	TkManager::g_CTkMenuMng.GetSomePosition(&pos, 0, true);
	logo->ExtraDraw(pos.x, pos.y, 1.0, 1.0, 0x80808080, 0, 0, 1);

	//Draw background vertical lines
	CTkDrawCtrlFrame* frame = this->MenuCtrlData->GetDrawCtrlFrame();
	if (frame) {
		KaWindow* window = frame->Window;
		if (window) {
			window->field_0 = TkManager::g_CTkMenuMng.UIXRes;
			window->field_4 = TkManager::g_CTkMenuMng.UIYRes + 32;
			window->Draw(0, -16, 1.0, 1.0, 0x20808080, 0, 0);
		}
	}

	this->field_28 += 1;
	if (this->field_28 >= 150) {
		//sub //TODO
		this->field_28 = 0;
	}

	//Update camera position
	this->CamMove();

	//Draw character names
	for (int i = 0; i < sizeof(this->charInfo) / sizeof(this->charInfo[0]); ++i) {
		IwCharInfo* charinfo = this->charInfo + i;
		if (charinfo->Exists) {
			TKRECT v2{};
			this->MenuCtrlData->SetButtonStatus(0, i + 1);
			this->MenuCtrlData->GetButtonLocate(&v2, i + 1, true, true);
			DrawCharName(v2.Right - v2.Left, v2.Left + 4, v2.Top + 2, charinfo);
		}
		else {
			this->MenuCtrlData->SetButtonStatus(5, i + 1);
		}
	}

	//Draw char info
	int v11 = this->MenuCtrlData->field_4C - 1;
	if (v11 >= 0 && v11 <= 16 && this->charInfo[v11].Exists)
		DrawCharInfo(v2, this->charInfo + v11);

	DrawContentIDInfo(v2, this->ContentIDCount, this->CharacterCount);
}

void FFXI::CTk::CIwSelectMenu::OnUpdatePrimitive()
{
	this->MenuCtrlData->field_4C = SelectedCharacter + 1;
	this->MenuCtrlData->RepositionCursol(this->MenuCtrlData->field_4C);
}

void FFXI::CTk::CIwSelectMenu::OnKeyDown(short a2, short a3)
{
	if (this->field_1C == 1) return;

	this->field_28 = 0;
	this->field_2C = 0;

	switch (a2) {
	case 1:
	case 2:
	case 3:
	case 4:
	case 9:
		SelectedCharacter = a3 - 1;
		if (this->field_18 != SelectedCharacter) {
			this->field_18 = SelectedCharacter;
			SoundMng::CYySePlayCursor();
		}
		//sub //TODO

		break;
	case 5:
		SelectedCharacter = a3 - 1;
		if (this->charInfo[SelectedCharacter].Exists == 0) {
			SoundMng::CYySePlayBeep();
		}
		else {
			SoundMng::CYySePlayClick();
			this->field_1C = 1;
			this->MenuCtrlData->field_5D = 1;
		}
		break;
	case 6:
		this->field_1C = 1;
		this->MenuCtrlData->field_5D = 1;
		SelectedCharacter = -1;
		SoundMng::CYySePlayCancel();
		break;
	case 8:
		if (PrimMng::g_pIwChfWin) {
			if (PrimMng::g_pIwChfWin->MenuCtrlData) {
				if (this->charInfo[SelectedCharacter].Exists) {
					TkManager::g_CTkMenuMng.ActiveDrawMenu(PrimMng::g_pIwChfWin->MenuCtrlData, 0);
					SoundMng::CYySePlayWindowSelect();
				}
				else {
					SoundMng::CYySePlayBeep();
				}
			}
		}
	}
}

void FFXI::CTk::CIwSelectMenu::OnDrawCalc(bool a2)
{
	if (!CIwSelectMenu::g_userfile) return;

	//sub //TODO keytype
	if (FFXI::Input::InputMng::GetKey(FFXI::Constants::Enums::KEYTYPE(63), FFXI::Constants::Enums::TRGTYPE::TRGTYPE_4)) {
		if (!a2) return;
		this->field_32 = 1;
		CIwSelectMenu::SelectedCharacter = this->MenuCtrlData->field_4C - 1;
		
		if (this->charInfo[SelectedCharacter].Exists) {
			this->field_1C = 1;
			this->MenuCtrlData->field_5D = 1;
		}
	}
	else
		this->field_32 = 0;

	if (a2 && TkManager::g_pCTkMouse->CheckMouse()) {
		tagPOINT pos{};
		if (TkManager::g_pCTkMouse)
			pos = TkManager::g_pCTkMouse->field_4;
		if (PrimMng::g_pIwChfWin->MenuCtrlData) {
			if (PrimMng::g_pIwChfWin->MenuCtrlData->IsOnWindow(pos)) {
				SelectedCharacter = this->MenuCtrlData->field_4C - 1;
				this->OnKeyDown(8, 0);
			}
		}
	}
}

void FFXI::CTk::CIwSelectMenu::CamMove()
{
	D3DXVECTOR4 v41[3];
	D3DXVECTOR4 v42[3];
	for (int i = 0; i < (sizeof(v41) / sizeof(v41[0])); ++i) {
		v41[i] = this->field_B38[i];
		v42[i] = this->field_B68[i];
	}
	
	for (int i = 0; i < 2; ++i) {
		if (this->field_AF8[i] < 1.0) {
			//sub //TODO
		}
	}
	CYyDb::g_pCYyDb->CameraManager->SetAt((D3DXVECTOR3*)&this->field_B18);
	CYyDb::g_pCYyDb->CameraManager->SetPos((D3DXVECTOR3*)&this->field_B28);
}
