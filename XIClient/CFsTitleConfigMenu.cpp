#include "CFsTitleConfigMenu.h"
#include "CTkMenuCtrlData.h"
#include "FsConfig.h"
#include "SoundMng.h"
#include "PrimMng.h"
#include "CIwPatchSub4.h"
using namespace FFXI::CTk;

void updateHelp(int a2) {
	if (PrimMng::g_pIwPatchSub4 == nullptr)
		return;

	switch (a2) {
    case 1:
        PrimMng::g_pIwPatchSub4->field_2C = 36;
        break;
    case 2:
        PrimMng::g_pIwPatchSub4->field_2C = 37;
        break;
    case 3:
        PrimMng::g_pIwPatchSub4->field_2C = 38;
        break;
    case 4:
        PrimMng::g_pIwPatchSub4->field_2C = 39;
        break;
    case 5:
        PrimMng::g_pIwPatchSub4->field_2C = 42;
        break;
    case 6:
        PrimMng::g_pIwPatchSub4->field_2C = 48;
        break;
    case 7:
        PrimMng::g_pIwPatchSub4->field_2C = 43;
        break;
    case 8:
        PrimMng::g_pIwPatchSub4->field_2C = 44;
        break;
    case 9:
        PrimMng::g_pIwPatchSub4->field_2C = 45;
        break;
    case 10:
        PrimMng::g_pIwPatchSub4->field_2C = 46;
        break;
    case 11:
        PrimMng::g_pIwPatchSub4->field_2C = 47;
    default:
        PrimMng::g_pIwPatchSub4->field_2C = 36;
	}
}

FFXI::CTk::CFsTitleConfigMenu::CFsTitleConfigMenu()
{
}

void FFXI::CTk::CFsTitleConfigMenu::OnInitialUpdatePrimitive()
{
    using FFXI::Constants::Enums::FsConfigSubjects;
	this->field_28 = FFXI::Config::FsConfig::GetConfig(FsConfigSubjects::Subject168);
	this->field_30 = FFXI::Config::FsConfig::GetConfig(FsConfigSubjects::Subject171);
	this->field_34 = FFXI::Config::FsConfig::GetConfig(FsConfigSubjects::Subject172);
	this->field_38 = FFXI::Config::FsConfig::GetConfig(FsConfigSubjects::Subject173);
	this->field_3C = FFXI::Config::FsConfig::GetConfig(FsConfigSubjects::TitleScreenSong);

	for (int i = 4; i >= 0; --i) {
		if ((SoundMng::GetMusicTable2Index(i) & 0x80) != 0)
			this->MenuCtrlData->SetButtonStatus(5, i + 7);
	}

	this->MenuCtrlData->SetCursol(5, 0);
	this->MenuCtrlData->field_5D = 0;
	updateHelp(this->MenuCtrlData->field_4C);
}
