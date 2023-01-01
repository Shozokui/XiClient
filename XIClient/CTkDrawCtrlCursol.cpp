#include "CTkDrawCtrlCursol.h"
#include "CTkDrawCtrlButton.h"
#include "CTkMenuButton.h"
#include "CTkMenuCursol.h"
#include "CTkMsbDataList.h"
#include "_49SubList.h"
#include "TkManager.h"
#include "Enums.h"
#include "Globals.h"
#include "CYyDb.h"

using namespace FFXI::CTk;

FFXI::CTk::CTkDrawCtrlCursol::CTkDrawCtrlCursol()
{
    this->DrawType = Constants::Enums::TkDrawType::TKDraw_Cursol;
    this->field_4 = 0;
    this->field_8 = 0;
    this->field_C = 0;
    this->field_10 = 0;
    this->field_18 = 0;
    this->MenuCursol = nullptr;
    this->field_1E = -1;
    this->field_22 = 0;
    this->field_20 = 0;
    this->field_2E = 0;
    this->field_2C = 3;
    this->field_24 = 1.0;
    this->field_2A = 0;
    this->field_28 = 0;
}

FFXI::CTk::CTkDrawCtrlCursol::~CTkDrawCtrlCursol()
{
}

void FFXI::CTk::CTkDrawCtrlCursol::SetResourceData(CTkMenuCursol* a2)
{
    this->MenuCursol = a2;
    this->ShapeInit();
}

void FFXI::CTk::CTkDrawCtrlCursol::LocateCalc(CTkDrawCtrlButton* a2)
{
    if (a2) {
        if (this->MenuCursol) {
            short* shortData = (short*)a2->MenuButton->Buffer;
            this->field_20 = a2->ButtonLeft + this->MenuCursol->field_24 + shortData[3];
            this->field_22 = a2->ButtonTop + this->MenuCursol->field_22 + shortData[4];
        }
    }
}

void FFXI::CTk::CTkDrawCtrlCursol::ShapeInit()
{
    if (!this->MenuCursol) return;

    if (!this->field_4)
        this->field_4 = TkManager::g_CTkMenuDataList.GetSubLists(this->MenuCursol->field_0);
    
    if (this->field_4) {
        if (this->MenuCursol->field_20) {
            this->field_30 = 0;
            this->field_2E = 0;
            this->field_C = (*this->field_4)[this->MenuCursol->field_20];
        }
        else {
            this->field_30 = 1;
            this->field_C = (*this->field_4)[this->MenuCursol->field_20];
        }
    }
}

void FFXI::CTk::CTkDrawCtrlCursol::ShapeCalc()
{
    if (!this->field_30) return;
    if (!this->field_4) return;
    this->field_2C -= (short)CYyDb::g_pCYyDb->CheckTick();
    if (this->field_2C < 0) {
        this->field_2C = 3;
        this->field_C = (*this->field_4)[this->field_2E];
        this->field_2E += 1;
        if (!(*this->field_4)[this->field_2E])
            this->field_2E = 0;
    }
}
