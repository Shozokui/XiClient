#include "CIwNetWinSub1.h"
#include "CTkMenuCtrlData.h"
#include "TkRect.h"
#include "TkManager.h"
#include "CTkMenuMng.h"
#include "CTkMsbDataList.h"
#include "Strings.h"
#include "_49SubList.h"
using namespace FFXI::CTk;

void FFXI::CTk::CIwNetWinSub1::OnInitialUpdatePrimitive()
{
	this->MenuCtrlData->field_85 = 0;
	this->MenuCtrlData->field_86 = 0;
	this->MenuCtrlData->field_84 = 0;

	TKRECT a2{};
	this->MenuCtrlData->GetOriginalRect(&a2);

	int adjustX = ((TkManager::g_CTkMenuMng.UIXRes - TkManager::g_CTkMenuMng.field_7C) - 512) / 2;
	int adjustY = ((TkManager::g_CTkMenuMng.UIYRes - TkManager::g_CTkMenuMng.field_7E) - 448) / 2;
	this->MenuCtrlData->ResizeWindow(
		a2.Left + adjustX,
		a2.Top + adjustY,
		a2.Right - a2.Left,
		a2.Bottom - a2.Top,
		1, 0, 0);
}

void FFXI::CTk::CIwNetWinSub1::OnDrawPrimitive()
{
	_49SubList*** sl = TkManager::g_CTkMenuDataList.GetSubLists(Constants::Strings::MenuLobbyWin);
	TKRECT a2{};

	this->MenuCtrlData->GetWindowLocate(&a2);
	if (this->progress > 0.01f && this->progress < 1.1f) {
		double adjuster{ 0.0 };
		if (this->progress < 0.5)
			adjuster = (this->progress - 0.5) * -128.0;
		else
			adjuster = (0.5 - this->progress) * 128.0;

		(*sl)[77]->ExtraDraw(a2.Left - adjuster, a2.Top, this->progress * 2, 0.99f, 0x80808080, 1, 0, 0);
	}
}
