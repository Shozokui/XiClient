#include "CIwChfWin.h"
#include "CTkMenuCtrlData.h"
#include "PrimMng.h"
#include "CIwPatchSub4.h"
#include "TkManager.h"
#include "CTkMenuMng.h"
#include "CIwSelectMenu.h"
#include "SoundMng.h"
using namespace FFXI::CTk;

void FFXI::CTk::CIwChfWin::OnInitialUpdatePrimitive()
{
	this->MenuCtrlData->field_84 = 0;
}

void FFXI::CTk::CIwChfWin::OnDrawCalc(bool a2)
{
	if (a2 == false) return;

	if (this->MenuCtrlData->field_4C == 2)
		PrimMng::g_pIwPatchSub4->field_2C = 50;
	else
		PrimMng::g_pIwPatchSub4->field_2C = 49;
}

void FFXI::CTk::CIwChfWin::OnKeyDown(short a2, short a3)
{
	int v4{ 0 };
	if (a2 == 5) {
		if (a3 != 1) {
			if (a3 != 2)
				return;
			v4 = 1;
		}

		//sub //TODO
	}
	else if (a2 == 6 || a2 == 8) {
		TkManager::g_CTkMenuMng.ActiveDrawMenu(PrimMng::g_pIwSelectMenu->MenuCtrlData, 0);
		SoundMng::CYySePlayWindowSelect();
		PrimMng::g_pIwPatchSub4->field_2C = 17;
	}
}
