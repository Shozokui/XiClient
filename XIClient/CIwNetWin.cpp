#include "CIwNetWin.h"
#include "CTkMenuCtrlData.h"
#include "TkRect.h"
#include "TkManager.h"
#include "CTkMenuMng.h"
#include "IwManager.h"
using namespace FFXI::CTk;

char* msg[2] = { nullptr, nullptr };
FFXI::CTk::CIwNetWin::CIwNetWin()
{
	this->HelpText[0] = 0;
}

void FFXI::CTk::CIwNetWin::OnInitialUpdatePrimitive()
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

void FFXI::CTk::CIwNetWin::OnDrawPrimitive()
{
	TKRECT a2{};
	msg[0] = this->HelpText;
	this->MenuCtrlData->GetWindowLocateCompen(&a2);
	CTkMenuMng::MarginComp(&a2);
	IwManager::IwPutMes(msg, &a2);
}
