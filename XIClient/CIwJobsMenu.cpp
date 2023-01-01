#include "CIwJobsMenu.h"
#include "CTkMenuCtrlData.h"
#include "SoundMng.h"
#include "RegistryConfig.h"
using namespace FFXI::CTk;

FFXI::CTk::CIwJobsMenu::CIwJobsMenu()
{
	this->field_1C = 0;
	this->field_18 = 0;
	this->field_14 = 0;
}

void FFXI::CTk::CIwJobsMenu::OnInitialUpdatePrimitive()
{
	this->MenuCtrlData->field_5D = 0;
	this->MenuCtrlData->field_85 = 0;
	this->MenuCtrlData->field_86 = 0;
	this->MenuCtrlData->field_84 = 0;
	//sub //TODO
}

void FFXI::CTk::CIwJobsMenu::OnDrawPrimitive()
{
	//null sub
}

void FFXI::CTk::CIwJobsMenu::OnUpdatePrimitive()
{
	this->MenuCtrlData->field_4C = (this->field_14 & 0xFF) + 1;
	this->MenuCtrlData->RepositionCursol(this->MenuCtrlData->field_4C);
}

void FFXI::CTk::CIwJobsMenu::OnKeyDown(short a2, short a3)
{
	if (this->field_1C == 1) {
		if (this->MenuCtrlData)
			this->MenuCtrlData->SetCursol((this->field_14 & 0xFF) + 1, 0);
	}
	else {
		switch (a2) {
		case 9:
			if (a3 >= 9 && a3 <= 10) break;
			//FALLTHROUGH
		case 1:
		case 2:
			if (a3 < 9) {
				this->field_14 = a3 - 1;
				SoundMng::CYySePlayCursor();
			}
			if (FFXI::Config::MainRegistryConfig::SimplifyCharCreateVisuals) {

			}
			else {

			}
			break;
		case 4:
		case 6:
			this->field_18 = -1;
			this->field_1C = 1;
			SoundMng::CYySePlayCancel();
			break;
		case 5:
			if (a3 == 9) {
				this->OnKeyDown(4, 9);
				break;
			}
			//FALLTHROUGH
		case 3:
			if (a3 != 9 && a3 != 10)
				this->field_14 = a3 - 1;
			this->field_18 = 1;
			this->field_1C = 1;
			SoundMng::CYySePlayClick();
			break;
		}
	}
}
