#include "CIwOkMenu.h"
#include "CTkMenuCtrlData.h"
#include "TkRect.h"
#include "TkManager.h"
#include "CTkMenuMng.h"
#include "TextRenderer.h"
#include "SoundMng.h"
#include "IwManager.h"

using namespace FFXI::CTk;

FFXI::CTk::CIwOkMenu::CIwOkMenu() : field_24C(nullptr)
{
	this->field_E = -1;
	this->MenuCtrlData = nullptr;
	this->field_C = 1;
	this->field_14 = nullptr;
	this->field_258 = 0;
	this->field_25C = 0;
	this->field_24C.__as(nullptr);
}

void FFXI::CTk::CIwOkMenu::OnInitialUpdatePrimitive()
{
	this->MenuCtrlData->field_5D = 0;
	this->MenuCtrlData->field_85 = 0;
	this->MenuCtrlData->field_86 = 0;
	this->MenuCtrlData->field_84 = 0;

	TKRECT a2{};

	int xfactor = ((TkManager::g_CTkMenuMng.UIXRes - TkManager::g_CTkMenuMng.field_7C) - 512) / 2;
	int yfactor = ((TkManager::g_CTkMenuMng.UIYRes - TkManager::g_CTkMenuMng.field_7E) - 448) / 2;
	
	this->MenuCtrlData->GetOriginalRect(&a2);
	this->MenuCtrlData->ResizeWindow(
		a2.Left + xfactor,
		a2.Top + yfactor,
		a2.Right - a2.Left,
		a2.Bottom - a2.Top,
		1,
		0,
		0
	);

}

void FFXI::CTk::CIwOkMenu::OnDrawPrimitive()
{
	TKRECT rect{};
	this->MenuCtrlData->GetWindowLocateCompen(&rect);
	TkManager::g_CTkMenuMng.MarginComp(&rect);
	FFXI::Text::TextRenderer::RenderInButtonMenu(this->field_24C.c_str(), &rect);
}

void FFXI::CTk::CIwOkMenu::OnKeyDown(short a2, short a3)
{
	if (this->field_25C != 1 && a2 == 5) {
		this->MenuCtrlData->field_5D = 1;
		this->field_25C = 1;
		SoundMng::CYySePlayClick();
		if (this->field_14) {
			this->field_14(this->field_18);
			TkManager::g_CTkMenuMng.DestroyDrawMenu(this->MenuCtrlData, true);
		}
		if (this->field_258)
			TkManager::g_CTkMenuMng.DestroyDrawMenu(this->MenuCtrlData, true);
	}
}

void FFXI::CTk::CIwOkMenu::SetErrorString(const char* a2)
{
	if (this->field_14) return;

	strncpy_s(this->field_1C, a2, sizeof(this->field_1C));
	this->field_1C[559] = 0;
	IwManager::g_ErrMsg = this->field_1C;
}
