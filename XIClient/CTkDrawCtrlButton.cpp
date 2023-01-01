#include "CTkDrawCtrlButton.h"
#include "CTkMenuButton.h"
#include "CTkResourceID.h"
#include "CTkMenuCtrlData.h"
#include "TkManager.h"
#include "CTkMenuMng.h"
#include "CTkMouse.h"
#include "Enums.h"
#include <string.h>

using namespace FFXI::CTk;

FFXI::CTk::CTkDrawCtrlButton::CTkDrawCtrlButton(CTkMenuCtrlData* a2)
{
	this->field_32 = 0;
	this->field_34 = 0;
	this->field_38 = 0;
    this->field_54 = 1;
    this->field_58 = this;
	this->field_5C = a2;
	this->DrawType = Constants::Enums::TkDrawType::TKDraw_Button;
	this->field_4 = 0;
	this->field_8 = 0;
	this->MenuButton = nullptr;
	this->field_24 = 0;
	this->field_26 = 0;
	this->ButtonWidth = 0;
	this->ButtonHeight = 0;
	this->field_3A = 0;
	this->field_40 = nullptr;
	this->field_44 = 0;
    this->SetMenuCtrlID(0);
	this->field_28 = -1;
	this->field_20 = 1.0;
	this->field_1C = 1.0;
	this->ButtonTop = 0;
	this->ButtonLeft = 0;
	this->field_10 = 0;
	this->field_14 = 0;
	this->field_18 = 0;
	this->field_3C = 1;
	this->field_4A = 1;
	this->field_49 = 0;
	this->field_4C = 0;
	this->field_50 = 0;
}

FFXI::CTk::CTkDrawCtrlButton::~CTkDrawCtrlButton()
{
}

void FFXI::CTk::CTkDrawCtrlButton::SetMenuCtrlID(int a2)
{
    switch (a2)
    {
    case 0:
        this->field_28 = 0;
        this->field_10 = this->field_18;
        break;
    case 1:
        this->field_28 = 2;
        this->field_10 = 1;
        break;
    case 3:
        this->field_10 = 3;
        this->field_28 = 0;
        break;
    case 5:
        this->field_28 = 0;
        this->field_10 = 5;
        this->field_18 = 5;
        break;
    case 11:
        this->field_28 = 0;
        this->field_10 = 11;
        this->field_18 = 11;
        break;
    case 15:
        this->field_28 = 0;
        this->field_10 = 15;
        this->field_18 = 15;
        break;
    case 16:
        this->field_10 = 16;
        this->field_18 = 16;
        this->field_28 = 0;
        break;
    default:
        return;
    }
}

void FFXI::CTk::CTkDrawCtrlButton::SetResourceData(CTkMenuButton* a2)
{
    unsigned short* wordData = (unsigned short*)a2->Buffer;

    this->MenuButton = a2;
    this->field_32 = wordData[1];
    this->field_34 = wordData[2];
    this->field_36 = wordData[5] + this->field_32;
    this->field_38 = wordData[6] + this->field_34;
    this->field_3A = wordData[9];

    this->field_40 = nullptr;
    if (a2->field_8 && strlen(a2->field_8))
        this->field_40 = a2->field_8;

    this->field_44 = nullptr;
    if (a2->field_C && strlen(a2->field_C))
        this->field_44 = a2->field_C;
}

void FFXI::CTk::CTkDrawCtrlButton::LocateCalc(short a2, short a3, bool a4)
{
    if (!this->MenuButton) return;

    if (a4) {
        float x{}, y{};
        TkManager::g_CTkMenuMng.GetWinCoefficient(&x, &y);
        this->ButtonLeft = a2 + ((double)this->field_32 * x);
        this->ButtonTop = a3 + ((double)this->field_34 * y);
        this->ButtonWidth = ((double)(short)(this->field_36 - this->field_32) * x);
        this->ButtonHeight = ((double)(short)(this->field_38 - this->field_34) * y);
    }
    else {
        this->ButtonLeft = a2 + this->field_32;
        this->ButtonTop = a3 + this->field_34;
        this->ButtonWidth = this->field_36 - this->field_32;
        this->ButtonHeight = this->field_38 - this->field_34;
    }
}

void FFXI::CTk::CTkDrawCtrlButton::ShapeCalc(bool a2)
{
    if (!this->MenuButton) return;

    if (this->field_28 > 0) {
        this->field_28 -= 1;
        if (this->field_28 <= 0) {
            if (this->field_24)
                this->SetMenuCtrlID(3);
            else
                this->SetMenuCtrlID(0);
        }
    }

    if (this->field_28 < 0) {
        if (TkManager::g_pCTkMouse) {
            if (!this->PointInButton(TkManager::g_pCTkMouse->field_4)) {
                this->field_28 = 0;
                this->SetMenuCtrlID(0);
            }
        }
    }

    if (!this->field_4 || this->field_14 != this->field_10) {
        this->field_14 = this->field_10;
        if (this->MenuButton->Buffer[27]) {
            int v3 = 0;
            do {
                CTkResourceID* resID = this->MenuButton->ResourceIDs + v3;
                if (resID) {
                    short comp = 0;
                    if (resID->field_0)
                        comp = CTkMenuMng::SomeMapping(this->field_10);
                    if (resID->field_0 == comp) {
                        if (!resID->field_18) {
                            resID->field_18 = TkManager::g_CTkMenuDataList.GetSubLists(resID->field_4);
                        }
                        if (!resID->field_18)
                            return;
                        this->field_4 = (*resID->field_18)[resID->field_2];
                        if (resID->field_0)
                            this->field_48 = 1;
                        else
                            this->field_48 = 0;
                    }
                }
                ++v3;
            } while (v3 < this->MenuButton->Buffer[27]);
        }
    }

    if (this->field_8)
        this->field_4 = this->field_8;

    if (a2)
        TkManager::g_CTkMenuMng.GetWinCoefficient(&this->field_1C, &this->field_20);
}

bool FFXI::CTk::CTkDrawCtrlButton::PointInButton(const tagPOINT& a2)
{
    if (this->ButtonLeft > a2.x)
        return false;

    if (this->ButtonTop > a2.y)
        return false;

    if (this->ButtonLeft + this->ButtonWidth < a2.x)
        return false;

    if (this->ButtonTop + this->ButtonHeight < a2.y)
        return false;

    return true;
}
