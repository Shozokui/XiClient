#include "CIwLicenceMenu.h"
#include "CTkMenuCtrlData.h"
#include "TkManager.h"
#include "CTkMenuMng.h"
#include "TkRect.h"
#include "StringTables.h"
#include "TextRenderer.h"
#include "SoundMng.h"
using namespace FFXI::CTk;

FFXI::CTk::CIwLicenceMenu::CIwLicenceMenu()
{
    this->field_1C = 0;
    this->field_14 = 0;
    this->field_18 = 1;
}

void FFXI::CTk::CIwLicenceMenu::OnInitialUpdatePrimitive()
{
    this->MenuCtrlData->field_5D = 0;
    this->MenuCtrlData->field_85 = 0;
    this->MenuCtrlData->field_86 = 0;
    this->MenuCtrlData->field_84 = 0;

    TKRECT v4{};
    v4.Left = 0;
    v4.Top = 0;
    v4.Right = 0;
    v4.Bottom = 0;

    this->MenuCtrlData->GetOriginalRect(&v4);

    int one = ((TkManager::g_CTkMenuMng.UIXRes - TkManager::g_CTkMenuMng.field_7C) - 512) / 2;
    int two = ((TkManager::g_CTkMenuMng.UIYRes - TkManager::g_CTkMenuMng.field_7E) - 448) / 2;
    this->MenuCtrlData->ResizeWindow(
        v4.Left + one,
        v4.Top + two,
        v4.Right - v4.Left,
        v4.Bottom - v4.Top,
        1,
        0,
        0);

}

void FFXI::CTk::CIwLicenceMenu::OnDrawPrimitive()
{
    TKRECT v3{};

    this->MenuCtrlData->GetWindowLocateCompen(&v3);
    CTkMenuMng::MarginComp(&v3);

    char* text = FFXI::Text::XiStrGet(8, 0xA0);
    FFXI::Text::TextRenderer::RenderInButtonMenu(text, &v3);
}

void FFXI::CTk::CIwLicenceMenu::OnUpdatePrimitive()
{
    this->MenuCtrlData->field_4C = 1;
    this->MenuCtrlData->RepositionCursol(1);
}

void FFXI::CTk::CIwLicenceMenu::OnKeyDown(short a2, short a3)
{
    if (this->field_1C != 1) {
        switch (a2) {
        case 3:
        case 4:
        case 9:
            SoundMng::CYySePlayCursor();
            break;
        case 5:
            this->field_1C = 1;
            this->MenuCtrlData->field_5D = 1;
            this->field_18 = a3 - 1;
            SoundMng::CYySePlayClick();
        }
    }
}
