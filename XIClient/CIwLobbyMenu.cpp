#include "Globals.h"
#include "CIwLobbyMenu.h"
#include "CTkMenuCtrlData.h"
#include "TkManager.h"
#include "CTkMouse.h"
#include "CTkMenuMng.h"
#include "SoundMng.h"
#include "TkRect.h"
#include "CTkMsbDataList.h"
#include "_49SubList.h"
#include "Strings.h"
#include "PrimMng.h"
#include "CIwPatchSub4.h"
#include "CTkDrawCtrlFrame.h"
#include "CApp.h"
#include "NT_SYS.h"
#include "RegistryConfig.h"
#include "XIFileManager.h"
#include "TextRenderer.h"
#include <string>
using namespace FFXI::CTk;

unsigned char enabledButtons[] = { 0x7F, 0x81, 0x83, 0x85, 0xB8 };
unsigned char disabledButtons[] = { 0x80, 0x82, 0x84, 0x85, 0xB8 };

short ExpansionBannerData[][10] = {
	{ 1, 0x8D, 0x046, 0x140, 0x046, 0x122, 0x046, 0x122, 0x946, 0x122 },
	{ 1, 0x9D, 0x104, 0x140, 0x10E, 0x122, 0x10E, 0x122, 0x10E, 0x122 },
	{ 1, 0x9F, 0x050, 0x15E, 0x08C, 0x136, 0x08C, 0x136, 0x98C, 0x136 },
	{ 1, 0xAC, 0x118, 0x15E, 0x078, 0x14F, 0x078, 0x14F, 0x978, 0x14F },
	{ 1, 0xAD, 0x032, 0x127, 0x0A0, 0x0FA, 0x0A0, 0x0FA, 0x0C8, 0x0FA },
	{ 1, 0xAE, 0x096, 0x127, 0x050, 0x10E, 0x032, 0x10E, 0x950, 0x10E },
	{ 1, 0xAF, 0x136, 0x127, 0x0FA, 0x10E, 0x0F0, 0x10E, 0x10E, 0x10E },
	{ 1, 0xB1, 0x136, 0x127, 0x0FA, 0x10E, 0x0F0, 0x10E, 0x10E, 0x10E },
	{ 1, 0xB2, 0x136, 0x127, 0x0FA, 0x10E, 0x0F0, 0x10E, 0x10E, 0x10E },
	{ 1, 0xB3, 0x136, 0x127, 0x0FA, 0x10E, 0x0F0, 0x10E, 0x10E, 0x10E },
	{ 1, 0xB4, 0x136, 0x127, 0x0FA, 0x10E, 0x0F0, 0x10E, 0x10E, 0x10E }
};

FFXI::CTk::CIwLobbyMenu::CIwLobbyMenu()
{
	this->ActiveButtonIndex = 0;
	this->field_1C = 0;
	memset(this->field_20, 0, sizeof(this->field_20));
}

void FFXI::CTk::CIwLobbyMenu::OnInitialUpdatePrimitive()
{
	this->MenuCtrlData->field_5D = 0;
	this->MenuCtrlData->field_85 = 0;
	this->MenuCtrlData->field_86 = 0;
	this->MenuCtrlData->field_84 = 0;
	this->MenuCtrlData->field_D0 = 1;

	int width = TkManager::g_CTkMenuMng.UIXRes - TkManager::g_CTkMenuMng.field_7C;
	int height = TkManager::g_CTkMenuMng.UIYRes - TkManager::g_CTkMenuMng.field_7E;

	double adjWidth = width * 0.0009765625f;
	double adjHeight = height * 0.0013020834f;

	this->MenuCtrlData->GetDrawCtrlFrame()->field_4C = 1;

	TKRECT rect{};
	for (int i = 1; i < 6; ++i) {
		this->MenuCtrlData->DisableDrawButton(i, 0);
		this->MenuCtrlData->GetButtonLocate(&rect, i, true, true);
		
		double adjbtnWidth = (double)(rect.Right - rect.Left) * adjWidth;
		double adjbtnHeight = (double)(rect.Bottom - rect.Top) * adjHeight;
		rect.Left = (double)rect.Left * adjWidth;
		rect.Top = (double)rect.Top * adjHeight;
		rect.Right = (double)rect.Left + adjbtnWidth;
		rect.Bottom = (double)rect.Top + adjbtnHeight;
		this->MenuCtrlData->SetButtonLocate(&rect, i);
	}
	//sub //TODO

	this->MenuCtrlData->field_69 = 0;
}

void FFXI::CTk::CIwLobbyMenu::OnUpdatePrimitive()
{
	this->MenuCtrlData->field_4C = this->ActiveButtonIndex + 1;
	this->MenuCtrlData->RepositionCursol(this->MenuCtrlData->field_4C);
}

void FFXI::CTk::CIwLobbyMenu::OnDrawPrimitive()
{
	static _49SubList*** sublists{ TkManager::g_CTkMenuDataList.GetSubLists(Constants::Strings::MenuLobbyWin) };

	int width = TkManager::g_CTkMenuMng.UIXRes - TkManager::g_CTkMenuMng.field_7C;
	int height = TkManager::g_CTkMenuMng.UIYRes - TkManager::g_CTkMenuMng.field_7E;

	double adjWidth = (double)width * 0.0009765625f;
	double adjHeight = (double)height * 0.0013020834f;

	//Draw version
	char* version = FFXI::File::XIFileManager::g_pXIFileManager->GetVersionString();
	if (version) {
		char versionString[256];
		sprintf_s(versionString, "ver. %s", version);
		FFXI::Text::TextRenderer::SjisFontDrawWidthHSpace(370, 364, versionString, 100, 100);
		
	}
	//Draw year (the time should come from the server, not from local clock)
	char timeStr[64];
	time_t timenow{};
	tm local{};
	time(&timenow);
	localtime_s(&local, &timenow);
	int year = 1900 + local.tm_year;
	if (year <= 2003)
		sprintf_s(timeStr, ",  %d", year);
	else
		sprintf_s(timeStr, "- %d", year);

	int offset = 0;
	float xpos = 278.0;
	while (timeStr[offset]) {
		if (timeStr[offset] == ' ') {
			if (adjWidth <= 1.0)
				xpos += 4.0;
			else
				xpos += 4.0 * adjWidth;
		}
		else {
			int subOffset = 0;
			if (timeStr[offset] == ',')
				subOffset = 10;
			else if (timeStr[offset] == '-')
				subOffset = 11;
			else
				subOffset = timeStr[offset] - '0';
			_49SubList* letter = (*sublists)[145 + subOffset];
			letter->ExtraDraw(adjWidth * xpos, adjHeight * 608.0, adjWidth, adjHeight, 0xFF808080, 0, 0, 0);
			xpos += letter->field_22 - letter->field_20;
		}
		++offset;
	}
	TKRECT rect{};

	//Draw Buttons
	for (int i = 0; i < 5; ++i) {
		this->MenuCtrlData->GetButtonLocate(&rect, i + 1, true, true);

		_49SubList* list{ nullptr };
		if (this->field_20[i] == 1)
			list = (*sublists)[enabledButtons[i] - 1];
		else
			list = (*sublists)[disabledButtons[i] - 1];

		if ((FFXI::Config::RegistryConfig::g_pOptions->Cafe.InCafe & 1) != 0 && i == 1) {
			list->ExtraDraw(rect.Left, rect.Top, adjWidth, adjHeight, 0x80202020, 0, 0, 0);
		}
		else if (this->ActiveButtonIndex == i) {
			if (this->field_1C == 1)
				this->MenuCtrlData->Blur4DrawHelper(list, rect.Left, rect.Top, adjWidth, adjHeight, 9, this->MenuCtrlData->field_5E);
			else
				list->ExtraDraw(rect.Left, rect.Top, adjWidth, adjHeight, 0x80805020, 0, 0, 0);
		}
		else
			list->ExtraDraw(rect.Left, rect.Top, adjWidth, adjHeight, 0x80808080, 0, 0, 0);
	}

	float coef1, coef2;
	TkManager::g_CTkMenuMng.GetWinCoefficient(&coef1, &coef2);
	
	//Draw Expansion Banners
	int expansionCount = sizeof(ExpansionBannerData) / sizeof(ExpansionBannerData[0]);
	bool zilartTenCheck = (FFXI::GlobalStruct::g_GlobalStruct.ClientExpansions & 2) != 0;
	bool zilartFtnCheck = (FFXI::GlobalStruct::g_GlobalStruct.ServerExpansions & 2) != 0;
	for (int i = 0; i < expansionCount; ++i) {
		if (ExpansionBannerData[i][0] == 0) continue;

		_49SubList* sublist = (*sublists)[ExpansionBannerData[i][1]];
		if (!sublist) continue;

		int ander = 1 << (i + 1);
		bool tencheck = (FFXI::GlobalStruct::g_GlobalStruct.ClientExpansions & ander) != 0;
		bool ftncheck = (FFXI::GlobalStruct::g_GlobalStruct.ServerExpansions & ander) != 0;
		if (i >= 4 && i < 7) {
			if (!zilartTenCheck || !zilartFtnCheck)
				tencheck = false;
		}
		if (tencheck && ftncheck)
			sublist->ExtraDraw(0, 0, adjWidth, adjHeight, 0xFF808080, 0, 0, 0);
		else
			sublist->ExtraDraw(0, 0, adjWidth, adjHeight, 0x50404040, 0, 68, 0);
		
	}

	float scaleHeight = adjHeight * 1.8;
	float scaleWidth = adjWidth * 1.7;

	//Draw Authenticator
	if ((FFXI::CYy::CApp::g_pCApp->g_pNT_SYS->field_B4 & 1) == 0) 
		(*sublists)[176]->ExtraDraw(coef1 * 40.0, coef2 * 382.0, scaleWidth, scaleHeight, 0xFF808080, 0, 0, 0);
	
	//Draw Mog Wardrobe 3
	if ((FFXI::CYy::CApp::g_pCApp->g_pNT_SYS->field_B4 & 4) == 0)
		(*sublists)[227]->ExtraDraw(coef1 * 50.0, coef2 * 360.0, scaleWidth, scaleHeight, 0xFF808080, 0, 0, 0);

	//Draw Mog Wardrobe 4
	if ((FFXI::CYy::CApp::g_pCApp->g_pNT_SYS->field_B4 & 8) == 0)
		(*sublists)[228]->ExtraDraw(coef1 * 82.0, coef2 * 360.0, scaleWidth, scaleHeight, 0xFF808080, 0, 0, 0);
}

void FFXI::CTk::CIwLobbyMenu::OnKeyDown(short a2, short a3)
{
	if (this->field_1C == 1) return;

	switch (a2) {
	case 1:
	case 2:
	case 9:
		this->ActiveButtonIndex = a3 - 1;
		this->SetHelpText();
		SoundMng::CYySePlayCursor();
		break;
	case 5:
		if (!this->field_20[this->ActiveButtonIndex] && this->ActiveButtonIndex != 1 || (FFXI::Config::RegistryConfig::g_pOptions->Cafe.InCafe & 1) != 0 && this->ActiveButtonIndex == 2)
			SoundMng::CYySePlayBeep();
		else {
			this->field_1C = 1;
			this->MenuCtrlData->field_5D = 1;
			SoundMng::CYySePlayClick();
		}
		break;
	case 6:
		this->ActiveButtonIndex = 3;
		this->SetHelpText();
		this->MenuCtrlData->field_4C = this->ActiveButtonIndex + 1;
		this->MenuCtrlData->RepositionCursol(this->MenuCtrlData->field_4C);
		TkManager::g_pCTkMouse->RequestJumpWindow();
		SoundMng::CYySePlayCursor();
		break;
	default:
		return;
	}
}

void FFXI::CTk::CIwLobbyMenu::SetHelpText()
{
	if (!PrimMng::g_pIwPatchSub4) return;

	switch (this->ActiveButtonIndex) {
	case 1:
		PrimMng::g_pIwPatchSub4->field_2C = 13;
		break;
	case 2:
		PrimMng::g_pIwPatchSub4->field_2C = 14;
		break;
	case 3:
		PrimMng::g_pIwPatchSub4->field_2C = 15;
		break;
	case 4:
		PrimMng::g_pIwPatchSub4->field_2C = 35;
		break;
	default:
		PrimMng::g_pIwPatchSub4->field_2C = 12;
		break;
	}

}
