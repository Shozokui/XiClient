#include "CIwTopMenu.h"
#include "CTkMenuCtrlData.h"
#include "CTkDrawCtrlFrame.h"
#include "Enums.h"
#include "TkManager.h"
#include "CTkMouse.h"
#include "CTkMenuMng.h"
#include "CTkMsbDataList.h"
#include "_49SubList.h"
#include "Strings.h"
#include "PrimMng.h"
#include "CIwPatchSub4.h"
#include "SoundMng.h"
using namespace FFXI::CTk;

FFXI::CTk::CIwTopMenu::CIwTopMenu()
{
	this->field_18 = 0;
	this->field_14 = 0;
	for (int i = 0; i < sizeof(this->field_1C) / sizeof(this->field_1C[0]); ++i)
		this->field_1C[i] = 1;
}

void FFXI::CTk::CIwTopMenu::OnInitialUpdatePrimitive()
{
	this->MenuCtrlData->field_69 = 0;
	this->MenuCtrlData->field_D0 = 1;
	this->MenuCtrlData->GetDrawCtrlFrame()->field_4C = 1;
	//sub //TODO
}

void FFXI::CTk::CIwTopMenu::OnUpdatePrimitive()
{
	this->MenuCtrlData->field_4C = this->field_14 + 1;
	this->MenuCtrlData->RepositionCursol(this->MenuCtrlData->field_4C);
}

void FFXI::CTk::CIwTopMenu::OnDrawPrimitive()
{
	int width = TkManager::g_CTkMenuMng.UIXRes - TkManager::g_CTkMenuMng.field_7C;
	int height = TkManager::g_CTkMenuMng.UIYRes - TkManager::g_CTkMenuMng.field_7E;
	double adjWidth = (double)width * 0.0009765625f;
	double adjHeight = (double)height * 0.0013020834f;

	CTkDrawCtrlFrame* frame = this->MenuCtrlData->GetDrawCtrlFrame();
	static _49SubList*** sublists{ TkManager::g_CTkMenuDataList.GetSubLists(Constants::Strings::MenuLobbyWin) };
	if (sublists) {
		_49SubList* thing = (*sublists)[186];
		if (thing)
			thing->ExtraDraw(frame->field_1A, frame->field_1C, adjWidth, adjHeight, 0xF0808080, 0, 0, 0);
	}
	if (frame) {
		if (frame->field_8) {
			frame->field_8->DrawTile(-15, -15, 0, 0, width / 2 + 15, height / 2 + 15, 2.0, 2.0, 0x40808080, 0, 0);
			//Draw main logo
			frame->field_8->ExtraDraw(frame->field_1A, frame->field_1C, adjWidth, adjHeight, 0xF0808080, 0, 0, 1);
		}
	}
}

void FFXI::CTk::CIwTopMenu::OnKeyDown(short a2, short a3)
{
	if (this->field_18 == 1) return;

	switch (a2) {
	case 1:
	case 2:
	case 9:
		this->field_14 = a3 - 1;
		PrimMng::g_pIwPatchSub4->field_2C = a3 + 6;
		SoundMng::CYySePlayCursor();
		break;
	case 5:
		if (this->field_1C[this->field_14]) {
			this->field_18 = 1;
			this->MenuCtrlData->field_5D = 1;
			SoundMng::CYySePlayClick();
		}
		else
			SoundMng::CYySePlayBeep();
		
		break;
	case 6:
		this->field_14 = 2;
		PrimMng::g_pIwPatchSub4->field_2C = 9;
		this->MenuCtrlData->field_4C = (this->field_14 & 0xFFFF) + 1;
		this->MenuCtrlData->RepositionCursol(this->MenuCtrlData->field_4C);
		TkManager::g_pCTkMouse->RequestJumpWindow();
		SoundMng::CYySePlayCursor();
		break;
	}
}
