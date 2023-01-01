#include "FsConfig.h"
#include "FsConfigSubject.h"
#include "CFsTitleConfigMenu.h"
#include "PrimMng.h"
#include "Globals.h"
#include "XIFileManager.h"
#include "CYyDb.h"
#include "GlobalStruct.h"
#include "FsCallbackListLink.h"

using namespace FFXI::Config;

int s2[2] = { 'FKSL', 'THJ1'};
int s3[2] = { 'FKSL', 'KRJ1'};
int s5[2] = { 'FKSL', 'TSP1'};
int s6[2] = { 'FKSL', 'DKP1'};

int s8[2] = { 'LOPL', 'SS_1'};
int s24[2] = { 'NPOG', 'FH_1'};

int s54[2] = { 'FKSL', 'KTJ1'};
int s65[2] = { 'NPOG', 'OA_1'};

int s147[2] = { 'LOPL', 'XDP1'};
int s148[2] = { 'LOPL', 'YDP1'};
int s149[2] = { 'LOPL', 'YDW1'};
int s150[2] = { 'LOPL', 'CSW1'};

void FFXI::Config::FsConfig::RegisterTextColorFunctions()
{
	FsConfig::RegisterConfigCallback(FFXI::Constants::Enums::FsConfigSubjects::Subject27, nullptr, 0);
	FsConfig::RegisterConfigCallback(FFXI::Constants::Enums::FsConfigSubjects::Subject29, nullptr, 0);
	FsConfig::RegisterConfigCallback(FFXI::Constants::Enums::FsConfigSubjects::Subject30, nullptr, 0);
	FsConfig::RegisterConfigCallback(FFXI::Constants::Enums::FsConfigSubjects::Subject31, nullptr, 0);
	FsConfig::RegisterConfigCallback(FFXI::Constants::Enums::FsConfigSubjects::Subject179, nullptr, 0);
	FsConfig::RegisterConfigCallback(FFXI::Constants::Enums::FsConfigSubjects::Subject176, nullptr, 0);
	FsConfig::RegisterConfigCallback(FFXI::Constants::Enums::FsConfigSubjects::Subject203, nullptr, 0);
	FsConfig::RegisterConfigCallback(FFXI::Constants::Enums::FsConfigSubjects::Subject204, nullptr, 0);
	FsConfig::RegisterConfigCallback(FFXI::Constants::Enums::FsConfigSubjects::Subject32, nullptr, 0);
	FsConfig::RegisterConfigCallback(FFXI::Constants::Enums::FsConfigSubjects::Subject33, nullptr, 0);
	FsConfig::RegisterConfigCallback(FFXI::Constants::Enums::FsConfigSubjects::Subject34, nullptr, 0);
	FsConfig::RegisterConfigCallback(FFXI::Constants::Enums::FsConfigSubjects::Subject28, nullptr, 0);
	FsConfig::RegisterConfigCallback(FFXI::Constants::Enums::FsConfigSubjects::Subject155, nullptr, 0);
	FsConfig::RegisterConfigCallback(FFXI::Constants::Enums::FsConfigSubjects::Subject35, nullptr, 0);
	FsConfig::RegisterConfigCallback(FFXI::Constants::Enums::FsConfigSubjects::Subject36, nullptr, 0);
	FsConfig::RegisterConfigCallback(FFXI::Constants::Enums::FsConfigSubjects::Subject37, nullptr, 0);
	FsConfig::RegisterConfigCallback(FFXI::Constants::Enums::FsConfigSubjects::Subject38, nullptr, 0);
	FsConfig::RegisterConfigCallback(FFXI::Constants::Enums::FsConfigSubjects::Subject39, nullptr, 0);
	FsConfig::RegisterConfigCallback(FFXI::Constants::Enums::FsConfigSubjects::Subject40, nullptr, 0);
	FsConfig::RegisterConfigCallback(FFXI::Constants::Enums::FsConfigSubjects::Subject41, nullptr, 0);
	FsConfig::RegisterConfigCallback(FFXI::Constants::Enums::FsConfigSubjects::Subject42, nullptr, 0);
	FsConfig::RegisterConfigCallback(FFXI::Constants::Enums::FsConfigSubjects::Subject43, nullptr, 0);
	FsConfig::RegisterConfigCallback(FFXI::Constants::Enums::FsConfigSubjects::Subject44, nullptr, 0);
	FsConfig::RegisterConfigCallback(FFXI::Constants::Enums::FsConfigSubjects::Subject45, nullptr, 0);
	FsConfig::RegisterConfigCallback(FFXI::Constants::Enums::FsConfigSubjects::Subject46, nullptr, 0);
	FsConfig::RegisterConfigCallback(FFXI::Constants::Enums::FsConfigSubjects::Subject47, nullptr, 0);
	FsConfig::RegisterConfigCallback(FFXI::Constants::Enums::FsConfigSubjects::Subject48, nullptr, 0);
	FsConfig::RegisterConfigCallback(FFXI::Constants::Enums::FsConfigSubjects::Subject49, nullptr, 0);
	FsConfig::RegisterConfigCallback(FFXI::Constants::Enums::FsConfigSubjects::Subject27, nullptr, 0);
	FsConfig::RegisterConfigCallback(FFXI::Constants::Enums::FsConfigSubjects::Subject29, nullptr, 0);
	FsConfig::RegisterConfigCallback(FFXI::Constants::Enums::FsConfigSubjects::Subject176, nullptr, 0);
	FsConfig::RegisterConfigCallback(FFXI::Constants::Enums::FsConfigSubjects::Subject30, nullptr, 0);
	FsConfig::RegisterConfigCallback(FFXI::Constants::Enums::FsConfigSubjects::Subject31, nullptr, 0);
	FsConfig::RegisterConfigCallback(FFXI::Constants::Enums::FsConfigSubjects::Subject179, nullptr, 0);
	FsConfig::RegisterConfigCallback(FFXI::Constants::Enums::FsConfigSubjects::Subject203, nullptr, 0);
	FsConfig::RegisterConfigCallback(FFXI::Constants::Enums::FsConfigSubjects::Subject204, nullptr, 0);
	FsConfig::RegisterConfigCallback(FFXI::Constants::Enums::FsConfigSubjects::Subject32, nullptr, 0);
	FsConfig::RegisterConfigCallback(FFXI::Constants::Enums::FsConfigSubjects::Subject33, nullptr, 0);
	FsConfig::RegisterConfigCallback(FFXI::Constants::Enums::FsConfigSubjects::Subject34, nullptr, 0);
	FsConfig::RegisterConfigCallback(FFXI::Constants::Enums::FsConfigSubjects::Subject28, nullptr, 0);
	FsConfig::RegisterConfigCallback(FFXI::Constants::Enums::FsConfigSubjects::Subject155, nullptr, 0);
	FsConfig::RegisterConfigCallback(FFXI::Constants::Enums::FsConfigSubjects::Subject35, nullptr, 0);
	FsConfig::RegisterConfigCallback(FFXI::Constants::Enums::FsConfigSubjects::Subject36, nullptr, 0);
	FsConfig::RegisterConfigCallback(FFXI::Constants::Enums::FsConfigSubjects::Subject37, nullptr, 0);
	FsConfig::RegisterConfigCallback(FFXI::Constants::Enums::FsConfigSubjects::Subject38, nullptr, 0);
	FsConfig::RegisterConfigCallback(FFXI::Constants::Enums::FsConfigSubjects::Subject39, nullptr, 0);
	FsConfig::RegisterConfigCallback(FFXI::Constants::Enums::FsConfigSubjects::Subject40, nullptr, 0);
	FsConfig::RegisterConfigCallback(FFXI::Constants::Enums::FsConfigSubjects::Subject41, nullptr, 0);
	FsConfig::RegisterConfigCallback(FFXI::Constants::Enums::FsConfigSubjects::Subject42, nullptr, 0);
	FsConfig::RegisterConfigCallback(FFXI::Constants::Enums::FsConfigSubjects::Subject43, nullptr, 0);
	FsConfig::RegisterConfigCallback(FFXI::Constants::Enums::FsConfigSubjects::Subject44, nullptr, 0);
	FsConfig::RegisterConfigCallback(FFXI::Constants::Enums::FsConfigSubjects::Subject45, nullptr, 0);
	FsConfig::RegisterConfigCallback(FFXI::Constants::Enums::FsConfigSubjects::Subject46, nullptr, 0);
	FsConfig::RegisterConfigCallback(FFXI::Constants::Enums::FsConfigSubjects::Subject47, nullptr, 0);
	FsConfig::RegisterConfigCallback(FFXI::Constants::Enums::FsConfigSubjects::Subject48, nullptr, 0);
	FsConfig::RegisterConfigCallback(FFXI::Constants::Enums::FsConfigSubjects::Subject49, nullptr, 0);
}

void FFXI::Config::FsConfig::loadTIGConfig()
{
	char data[824];
	if (FFXI::File::XIFileManager::g_pXIFileManager->readUserFileNow("tig.dat", data, sizeof(data), 0, 0) < 0)
		return;

	int* intdata = (int*)data;
	if (intdata[0] != 1)
		return;

	intdata += 1;
	for (int i = 0; i < (int)FFXI::Constants::Enums::FsConfigSubjects::count; ++i) {
		FsConfigSubject* subj = this->subjects + i;
		if (subj->type == FsConfigSubject::type::TIG) {
			int value = intdata[0];
			intdata += 1;
			this->setConfig(subj, value);
		}
	}
}

void FFXI::Config::FsConfig::FsConfigInit()
{
	CYyDb::g_pCYyDb->g_pFsConfig = new FsConfig();
	CTk::PrimMng::g_pFsTitleConfigMenu = new CTk::CFsTitleConfigMenu();

	using FFXI::Constants::Enums::FsConfigSubjects;
	int languagecode = FFXI::GlobalStruct::g_GlobalStruct.GetRegionCode();
	if (languagecode == FFXI::Constants::Enums::LanguageCode::Japanese)
	{
		FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject2, nullptr, nullptr);
		FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject3, nullptr, nullptr);
		FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject5, nullptr, nullptr);
		FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject68, nullptr, nullptr);
	}
	else
	{
		FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject2, nullptr, nullptr);
		FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject3, nullptr, nullptr);
		FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject5, nullptr, nullptr);
		FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject68, nullptr, nullptr);
	}
	RegisterTextColorFunctions();
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject7, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject8, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject9, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject10, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject55, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject56, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject57, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject154, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject202, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject52, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject180, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject181, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject18, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject164, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject14, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject15, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject16, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject156, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject17, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject182, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject183, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject184, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject197, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject198, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject19, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject58, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject59, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject60, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject153, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject165, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject62, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject63, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject64, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject67, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject157, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject158, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject190, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject191, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject192, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject194, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject195, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject153, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject201, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject24, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject3, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject201, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject68, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject54, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject180, nullptr, 0);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject181, nullptr, 0);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject7, nullptr, 0);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject18, nullptr, 0);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject14, nullptr, 0);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject15, nullptr, 0);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject16, nullptr, 0);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject156, nullptr, 0);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject17, nullptr, 0);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject182, nullptr, 0);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject183, nullptr, 0);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject184, nullptr, 0);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject197, nullptr, 0);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject198, nullptr, 0);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject50, nullptr, 0);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject51, nullptr, 0);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject53, nullptr, 0);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject185, nullptr, 0);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject186, nullptr, 0);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject187, nullptr, 0);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject188, nullptr, 0);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject9, nullptr, 0);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject10, nullptr, 0);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject8, nullptr, 0);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject149, nullptr, 0);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject150, nullptr, 0);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject58, nullptr, 0);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject59, nullptr, 0);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject60, nullptr, 0);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject55, nullptr, 0);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject56, nullptr, 0);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject57, nullptr, 0);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject62, nullptr, 0);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject63, nullptr, 0);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject64, nullptr, 0);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject67, nullptr, 0);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject24, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject66, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject74, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject75, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject76, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject77, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject78, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject79, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject80, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject81, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject82, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject83, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject84, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject85, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject86, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject87, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject88, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject89, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject91, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject90, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject92, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject93, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject94, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject95, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject96, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject97, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject98, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject99, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject100, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject101, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject103, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject102, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject104, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject105, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject106, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject107, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject108, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject109, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject110, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject111, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject112, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject113, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject114, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject115, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject116, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject117, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject118, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject119, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject120, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject121, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject122, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject123, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject124, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject125, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject126, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject127, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject128, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject129, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject130, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject131, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject132, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject177, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject133, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject189, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject134, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject205, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject135, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject136, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject137, nullptr, nullptr);
	//sub //TODO
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject168, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject171, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject172, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::Subject173, nullptr, nullptr);
	FsConfig::RegisterConfigCallback(FsConfigSubjects::TitleScreenSong, nullptr, CTk::PrimMng::g_pFsTitleConfigMenu);
}

void FFXI::Config::FsConfig::FsConfigUninit()
{
	if (CYyDb::g_pCYyDb->g_pFsConfig != nullptr) {
		delete CYyDb::g_pCYyDb->g_pFsConfig;
		CYyDb::g_pCYyDb->g_pFsConfig = nullptr;
	}

	//sub //TODO
	if (CTk::PrimMng::g_pFsTitleConfigMenu != nullptr) {
		delete CTk::PrimMng::g_pFsTitleConfigMenu;
		CTk::PrimMng::g_pFsTitleConfigMenu = nullptr;
	}
}

void FFXI::Config::FsConfig::RegisterConfigCallback(FFXI::Constants::Enums::FsConfigSubjects a2, void(__cdecl* a3)(int, int, CTk::CTkMenuPrimitive*), CTk::CTkMenuPrimitive* a4)
{
	if (CYyDb::g_pFsConfig == nullptr)
		return;

	CYyDb::g_pFsConfig->registerConfigCallback((int)a2, a3, a4);
}

int FFXI::Config::FsConfig::GetConfig(FFXI::Constants::Enums::FsConfigSubjects a1)
{
	if (CYyDb::g_pFsConfig == nullptr)
		return -1;

	return CYyDb::g_pFsConfig->getConfig((int)a1);
}

FFXI::Config::FsConfig::FsConfig()
{
	using FFXI::Constants::Enums::FsConfigSubjects;

	this->subjects = new FsConfigSubject[(int)FsConfigSubjects::count];
	this->initConfig(FsConfigSubjects::Subject2, 0, 1, s2, 0, 1, 0, 0);
	this->initConfig(FsConfigSubjects::Subject3, 0, 1, s3, 0, 1, 0, 0);
	this->initConfig(FsConfigSubjects::Subject4, 0, 4, 0, 0, 0, 0, 0);
	this->initConfig(FsConfigSubjects::Subject5, 1, 1, s5, 0, 1, 1, 1);
	this->initConfig(FsConfigSubjects::Subject6, 0, 1, s6, 0, 1, 0, 0);
	this->initConfig(FsConfigSubjects::Subject7, 0, 4, 0, 0, 1, 0, 1);
	this->initConfig(FsConfigSubjects::Subject8, 0, 1, s8, 0, 1, 0, 0);
	this->initConfig(FsConfigSubjects::Subject9, 100, 4, 0, 0, 100, 100, 1);
	this->initConfig(FsConfigSubjects::Subject10, 100, 4, 0, 0, 100, 100, 1);
	this->initConfig(FsConfigSubjects::Subject11, 60, 4, 0, 0, 127, 60, 0);
	this->initConfig(FsConfigSubjects::Subject12, 0, 4, 0, 0, 512, 0, 0);
	this->initConfig(FsConfigSubjects::Subject13, 0, 4, 0, 0, 7, 0, 0);
	this->initConfig(FsConfigSubjects::Subject14, 0, 4, 0, 0, 1, 0, 0);
	this->initConfig(FsConfigSubjects::Subject15, 8, 4, 0, 4, 20, 8, 0);
	this->initConfig(FsConfigSubjects::Subject16, 0, 4, 0, 0, 20, 0, 0);
	this->initConfig(FsConfigSubjects::Subject156, 100, 4, 0, 0, 100, 100, 0);
	this->initConfig(FsConfigSubjects::Subject17, 5, 4, 0, 3, 30, 5, 0);
	this->initConfig(FsConfigSubjects::Subject197, -1, 4, 0, -1, 100, -1, 0);
	this->initConfig(FsConfigSubjects::Subject198, -1, 4, 0, -1, 30, -1, 0);
	this->initConfig(FsConfigSubjects::Subject157, 0, 4, 0, 0, 1, 0, 0);
	this->initConfig(FsConfigSubjects::Subject158, 0, 4, 0, 0, 1, 0, 0);
	this->initConfig(FsConfigSubjects::Subject18, 0, 4, 0, 0, 7, 0, 0);
	this->initConfig(FsConfigSubjects::Subject19, 0, 4, 0, 0, 2, 0, 0);
	this->initConfig(FsConfigSubjects::Subject20, 0, 5, 0, 0, 1, 0, 0);
	this->initConfig(FsConfigSubjects::Subject21, 1, 4, 0, 0, 1, 1, 0);
	this->initConfig(FsConfigSubjects::Subject22, 100, 4, 0, 0, 100, 100, 0);
	this->initConfig(FsConfigSubjects::Subject23, 0, 4, 0, 0, 1, 0, 1);
	this->initConfig(FsConfigSubjects::Subject24, 0, 1, s24, 0, 1, 0, 0);
	this->initConfig(FsConfigSubjects::Subject25, 0, 5, 0, 0, 1, 0, 0);
	this->initConfig(FsConfigSubjects::Subject27, -2139062144, 4, 0, 0, -1, -2139062144, 0);
	this->initConfig(FsConfigSubjects::Subject28, -2136977344, 4, 0, 0, -1, -2136977344, 0);
	this->initConfig(FsConfigSubjects::Subject29, -2136981344, 4, 0, 0, -1, -2136981344, 0);
	this->initConfig(FsConfigSubjects::Subject30, -2145337184, 4, 0, 0, -1, -2145337184, 0);
	this->initConfig(FsConfigSubjects::Subject31, -2142175392, 4, 0, 0, -1, -2142175392, 0);
	this->initConfig(FsConfigSubjects::Subject179, -2147431424, 4, 0, 0, -1, -2147431424, 0);
	this->initConfig(FsConfigSubjects::Subject203, -2147462913, 4, 0, 0, -1, -2147462913, 0);
	this->initConfig(FsConfigSubjects::Subject204, -2147454721, 4, 0, 0, -1, -2147454721, 0);
	this->initConfig(FsConfigSubjects::Subject176, -2130727105, 4, 0, 0, -1, -2130727105, 0);
	this->initConfig(FsConfigSubjects::Subject178, 1, 4, 0, 0, 1, 1, 0);
	this->initConfig(FsConfigSubjects::Subject32, -2141171552, 4, 0, 0, -1, -2141171552, 0);
	this->initConfig(FsConfigSubjects::Subject33, -2136944432, 4, 0, 0, -1, -2136944432, 0);
	this->initConfig(FsConfigSubjects::Subject34, -2139062144, 4, 0, 0, -1, -2139062144, 0);
	this->initConfig(FsConfigSubjects::Subject155, -2136981456, 4, 0, 0, -1, -2136981456, 0);
	this->initConfig(FsConfigSubjects::Subject35, -2141155136, 4, 0, 0, -1, -2141155136, 0);
	this->initConfig(FsConfigSubjects::Subject36, -2136981440, 4, 0, 0, -1, -2136981440, 0);
	this->initConfig(FsConfigSubjects::Subject37, -2139062144, 4, 0, 0, -1, -2139062144, 0);
	this->initConfig(FsConfigSubjects::Subject38, -2139062144, 4, 0, 0, -1, -2139062144, 0);
	this->initConfig(FsConfigSubjects::Subject39, -2139062192, 4, 0, 0, -1, -2139062192, 0);
	this->initConfig(FsConfigSubjects::Subject40, -2139062144, 4, 0, 0, -1, -2139062144, 0);
	this->initConfig(FsConfigSubjects::Subject41, -2137997072, 4, 0, 0, -1, -2137997072, 0);
	this->initConfig(FsConfigSubjects::Subject42, -2134867840, 4, 0, 0, -1, -2134867840, 0);
	this->initConfig(FsConfigSubjects::Subject43, -2139062144, 4, 0, 0, -1, -2139062144, 0);
	this->initConfig(FsConfigSubjects::Subject44, -2139062144, 4, 0, 0, -1, -2139062144, 0);
	this->initConfig(FsConfigSubjects::Subject45, -2136965056, 4, 0, 0, -1, -2136965056, 0);
	this->initConfig(FsConfigSubjects::Subject46, -2140114832, 4, 0, 0, -1, -2140114832, 0);
	this->initConfig(FsConfigSubjects::Subject47, -2139062256, 4, 0, 0, -1, -2139062256, 0);
	this->initConfig(FsConfigSubjects::Subject48, -2134875952, 4, 0, 0, -1, -2134875952, 0);
	this->initConfig(FsConfigSubjects::Subject49, -2134851504, 4, 0, 0, -1, -2134851504, 0);
	this->initConfig(FsConfigSubjects::Subject50, 0, 5, 0, 0, -1, 0, 1);
	this->initConfig(FsConfigSubjects::Subject51, 0, 5, 0, 0, -1, 0, 1);
	this->initConfig(FsConfigSubjects::Subject52, 0, 4, 0, 0, -1, 0, 0);
	this->initConfig(FsConfigSubjects::Subject53, 0, 5, 0, 0, 3, 0, 1);
	this->initConfig(FsConfigSubjects::Subject54, 0, 1, s54, 0, 1, 0, 1);
	this->initConfig(FsConfigSubjects::Subject61, 128, 4, 0, 0, 255, 128, 0);
	this->initConfig(FsConfigSubjects::Subject65, -1, 1, s65, -1, 60, -1, 0);
	this->initConfig(FsConfigSubjects::Subject66, 2, 4, 0, 0, 2, 2, 0);
	this->initConfig(FsConfigSubjects::Subject55, 50, 4, 0, 0, 100, 50, 0);
	this->initConfig(FsConfigSubjects::Subject56, 50, 4, 0, 0, 100, 50, 0);
	this->initConfig(FsConfigSubjects::Subject57, 50, 4, 0, 0, 100, 50, 0);
	this->initConfig(FsConfigSubjects::Subject58, 0, 4, 0, 0, 2, 0, 0);
	this->initConfig(FsConfigSubjects::Subject59, 1, 4, 0, 0, 1, 1, 0);
	this->initConfig(FsConfigSubjects::Subject60, 50, 4, 0, 25, 51, 50, 0);
	this->initConfig(FsConfigSubjects::Subject64, 3, 4, 0, 0, 3, 3, 0);
	this->initConfig(FsConfigSubjects::Subject62, 25, 4, 0, 0, 25, 25, 0);
	this->initConfig(FsConfigSubjects::Subject63, 0, 4, 0, 0, 1, 0, 0);
	this->initConfig(FsConfigSubjects::Subject67, 0, 4, 0, 0, 5, 0, 1);
	this->initConfig(FsConfigSubjects::Subject68, 0, 4, 0, 0, 1, 0, 0);
	this->initConfig(FsConfigSubjects::Subject69, 1, 4, 0, 0, 1, 1, 0);
	this->initConfig(FsConfigSubjects::Subject70, 1, 4, 0, 0, 1, 1, 0);
	this->initConfig(FsConfigSubjects::Subject71, 0, 4, 0, 0, 1, 0, 0);
	this->initConfig(FsConfigSubjects::Subject72, 1, 4, 0, 0, 1, 1, 0);
	this->initConfig(FsConfigSubjects::Subject73, 350, 4, 0, 242, 902, 350, 0);
	this->initConfig(FsConfigSubjects::Subject74, 17, 4, 0, 0, 255, 17, 0);
	this->initConfig(FsConfigSubjects::Subject75, 31, 4, 0, 0, 255, 31, 0);
	this->initConfig(FsConfigSubjects::Subject76, 30, 4, 0, 0, 255, 30, 0);
	this->initConfig(FsConfigSubjects::Subject77, 32, 4, 0, 0, 255, 32, 0);
	this->initConfig(FsConfigSubjects::Subject78, 23, 4, 0, 0, 255, 23, 0);
	this->initConfig(FsConfigSubjects::Subject79, 37, 4, 0, 0, 255, 37, 0);
	this->initConfig(FsConfigSubjects::Subject80, 36, 4, 0, 0, 255, 36, 0);
	this->initConfig(FsConfigSubjects::Subject81, 38, 4, 0, 0, 255, 38, 0);
	this->initConfig(FsConfigSubjects::Subject82, 0, 4, 0, 0, 255, 0, 0);
	this->initConfig(FsConfigSubjects::Subject83, 0, 4, 0, 0, 255, 0, 0);
	this->initConfig(FsConfigSubjects::Subject84, 0, 4, 0, 0, 255, 0, 0);
	this->initConfig(FsConfigSubjects::Subject85, 0, 4, 0, 0, 255, 0, 0);
	this->initConfig(FsConfigSubjects::Subject86, 52, 4, 0, 0, 255, 52, 0);
	this->initConfig(FsConfigSubjects::Subject87, 51, 4, 0, 0, 255, 51, 0);
	this->initConfig(FsConfigSubjects::Subject88, 0, 4, 0, 0, 255, 0, 0);
	this->initConfig(FsConfigSubjects::Subject89, 0, 4, 0, 0, 255, 0, 0);
	this->initConfig(FsConfigSubjects::Subject90, 0, 4, 0, 0, 255, 0, 0);
	this->initConfig(FsConfigSubjects::Subject91, 0, 4, 0, 0, 255, 0, 0);
	this->initConfig(FsConfigSubjects::Subject92, 21, 4, 0, 0, 255, 21, 0);
	this->initConfig(FsConfigSubjects::Subject93, 49, 4, 0, 0, 255, 49, 0);
	this->initConfig(FsConfigSubjects::Subject94, 47, 4, 0, 0, 255, 47, 0);
	this->initConfig(FsConfigSubjects::Subject95, 19, 4, 0, 0, 255, 19, 0);
	this->initConfig(FsConfigSubjects::Subject96, 35, 4, 0, 0, 255, 35, 0);
	this->initConfig(FsConfigSubjects::Subject97, 33, 4, 0, 0, 255, 33, 0);
	this->initConfig(FsConfigSubjects::Subject98, 12, 4, 0, 0, 255, 12, 0);
	this->initConfig(FsConfigSubjects::Subject99, 44, 4, 0, 0, 255, 44, 0);
	this->initConfig(FsConfigSubjects::Subject100, 27, 4, 0, 0, 255, 27, 0);
	this->initConfig(FsConfigSubjects::Subject101, 26, 4, 0, 0, 255, 26, 0);
	this->initConfig(FsConfigSubjects::Subject102, 18, 4, 0, 0, 255, 18, 0);
	this->initConfig(FsConfigSubjects::Subject103, 16, 4, 0, 0, 255, 16, 0);
	this->initConfig(FsConfigSubjects::Subject104, 0, 4, 0, 0, 255, 0, 0);
	this->initConfig(FsConfigSubjects::Subject105, 0, 4, 0, 0, 255, 0, 0);
	if (FFXI::GlobalStruct::g_GlobalStruct.RegionCode == FFXI::Constants::Enums::LanguageCode::French)
	{
		this->initConfig(FsConfigSubjects::Subject92, 44, 4, 0, 0, 255, 44, 0);
		this->initConfig(FsConfigSubjects::Subject99, 21, 4, 0, 0, 255, 21, 0);
		this->initConfig(FsConfigSubjects::Subject98, 243, 4, 0, 0, 255, 243, 0);
		this->initConfig(FsConfigSubjects::Subject100, 26, 4, 0, 0, 255, 26, 0);
		this->initConfig(FsConfigSubjects::Subject101, 27, 4, 0, 0, 255, 27, 0);
	}
	else if (FFXI::GlobalStruct::g_GlobalStruct.RegionCode == FFXI::Constants::Enums::LanguageCode::German)
	{
		this->initConfig(FsConfigSubjects::Subject74, 44, 4, 0, 0, 255, 44, 0);
		this->initConfig(FsConfigSubjects::Subject76, 16, 4, 0, 0, 255, 16, 0);
		this->initConfig(FsConfigSubjects::Subject98, 252, 4, 0, 0, 255, 252, 0);
		this->initConfig(FsConfigSubjects::Subject99, 17, 4, 0, 0, 255, 17, 0);
		this->initConfig(FsConfigSubjects::Subject100, 249, 4, 0, 0, 255, 249, 0);
		this->initConfig(FsConfigSubjects::Subject101, 250, 4, 0, 0, 255, 250, 0);
		this->initConfig(FsConfigSubjects::Subject86, 39, 4, 0, 0, 255, 39, 0);
		this->initConfig(FsConfigSubjects::Subject87, 51, 4, 0, 0, 255, 51, 0);
		this->initConfig(FsConfigSubjects::Subject103, 30, 4, 0, 0, 255, 30, 0);
	}
	this->initConfig(FsConfigSubjects::Subject106, 30, 4, 0, 0, 255, 30, 0);
	this->initConfig(FsConfigSubjects::Subject107, 23, 4, 0, 0, 255, 23, 0);
	this->initConfig(FsConfigSubjects::Subject108, 50, 4, 0, 0, 255, 50, 0);
	this->initConfig(FsConfigSubjects::Subject109, 36, 4, 0, 0, 255, 36, 0);
	this->initConfig(FsConfigSubjects::Subject110, 17, 4, 0, 0, 255, 17, 0);
	this->initConfig(FsConfigSubjects::Subject111, 48, 4, 0, 0, 255, 48, 0);
	this->initConfig(FsConfigSubjects::Subject112, 21, 4, 0, 0, 255, 21, 0);
	this->initConfig(FsConfigSubjects::Subject113, 49, 4, 0, 0, 255, 49, 0);
	this->initConfig(FsConfigSubjects::Subject115, 33, 4, 0, 0, 255, 33, 0);
	this->initConfig(FsConfigSubjects::Subject118, 44, 4, 0, 0, 255, 44, 0);
	this->initConfig(FsConfigSubjects::Subject121, 0, 4, 0, 0, 255, 0, 0);
	this->initConfig(FsConfigSubjects::Subject122, 0, 4, 0, 0, 255, 0, 0);
	this->initConfig(FsConfigSubjects::Subject123, 18, 4, 0, 0, 255, 18, 0);
	this->initConfig(FsConfigSubjects::Subject124, 47, 4, 0, 0, 255, 47, 0);
	this->initConfig(FsConfigSubjects::Subject125, 35, 4, 0, 0, 255, 35, 0);
	this->initConfig(FsConfigSubjects::Subject126, 46, 4, 0, 0, 255, 46, 0);
	this->initConfig(FsConfigSubjects::Subject127, 31, 4, 0, 0, 255, 31, 0);
	this->initConfig(FsConfigSubjects::Subject128, 25, 4, 0, 0, 255, 25, 0);
	this->initConfig(FsConfigSubjects::Subject129, 20, 4, 0, 0, 255, 20, 0);
	this->initConfig(FsConfigSubjects::Subject130, 38, 4, 0, 0, 255, 38, 0);
	this->initConfig(FsConfigSubjects::Subject131, 19, 4, 0, 0, 255, 19, 0);
	this->initConfig(FsConfigSubjects::Subject132, 22, 4, 0, 0, 255, 22, 0);
	this->initConfig(FsConfigSubjects::Subject177, 0, 4, 0, 0, 1, 0, 0);
	this->initConfig(FsConfigSubjects::Subject133, 45, 4, 0, 0, 255, 45, 0);
	this->initConfig(FsConfigSubjects::Subject189, 0, 4, 0, 0, 1, 0, 0);
	this->initConfig(FsConfigSubjects::Subject134, 16, 4, 0, 0, 255, 16, 0);
	this->initConfig(FsConfigSubjects::Subject205, 0, 4, 0, 0, 1, 0, 0);
	this->initConfig(FsConfigSubjects::Subject135, 0, 4, 0, 0, 255, 0, 0);
	this->initConfig(FsConfigSubjects::Subject136, 0, 4, 0, 0, 255, 0, 0);
	this->initConfig(FsConfigSubjects::Subject137, 0, 4, 0, 0, 255, 0, 0);
	this->initConfig(FsConfigSubjects::Subject114, 12, 4, 0, 0, 255, 12, 0);
	this->initConfig(FsConfigSubjects::Subject116, 27, 4, 0, 0, 255, 27, 0);
	this->initConfig(FsConfigSubjects::Subject117, 26, 4, 0, 0, 255, 26, 0);
	this->initConfig(FsConfigSubjects::Subject119, 52, 4, 0, 0, 255, 52, 0);
	this->initConfig(FsConfigSubjects::Subject120, 51, 4, 0, 0, 255, 51, 0);
	if (FFXI::GlobalStruct::g_GlobalStruct.RegionCode == 2)
	{
		this->initConfig(FsConfigSubjects::Subject112, 44, 4, 0, 0, 255, 44, 0);
		this->initConfig(FsConfigSubjects::Subject118, 21, 4, 0, 0, 255, 21, 0);
		this->initConfig(FsConfigSubjects::Subject114, 243, 4, 0, 0, 255, 243, 0);
		this->initConfig(FsConfigSubjects::Subject116, 26, 4, 0, 0, 255, 26, 0);
		this->initConfig(FsConfigSubjects::Subject117, 27, 4, 0, 0, 255, 27, 0);
	}
	else if (FFXI::GlobalStruct::g_GlobalStruct.RegionCode == 3)
	{
		this->initConfig(FsConfigSubjects::Subject114, 252, 4, 0, 0, 255, 252, 0);
		this->initConfig(FsConfigSubjects::Subject116, 249, 4, 0, 0, 255, 249, 0);
		this->initConfig(FsConfigSubjects::Subject117, 250, 4, 0, 0, 255, 250, 0);
		this->initConfig(FsConfigSubjects::Subject119, 39, 4, 0, 0, 255, 39, 0);
		this->initConfig(FsConfigSubjects::Subject120, 51, 4, 0, 0, 255, 51, 0);
	}
	this->initConfig(FsConfigSubjects::Subject138, 2000, 4, 0, 1000, 4000, 2000, 0);
	this->initConfig(FsConfigSubjects::Subject140, 0, 4, 0, 0, 1, 0, 0);
	this->initConfig(FsConfigSubjects::Subject139, 0, 4, 0, 0, 1, 0, 0);
	this->initConfig(FsConfigSubjects::Subject142, 0, 4, 0, 0, 1, 0, 0);
	this->initConfig(FsConfigSubjects::Subject141, 0, 4, 0, 0, 1, 0, 0);
	this->initConfig(FsConfigSubjects::Subject143, 0, 4, 0, 0, 1, 0, 0);
	this->initConfig(FsConfigSubjects::Subject144, 0, 4, 0, 0, 1, 0, 0);
	this->initConfig(FsConfigSubjects::Subject145, 0, 4, 0, 0, 1, 0, 0);
	this->initConfig(FsConfigSubjects::Subject146, 0, 4, 0, 0, -1, 0, 0);
	this->initConfig(FsConfigSubjects::Subject147, 0, 1, s147, -255, 255, 0, 0);
	this->initConfig(FsConfigSubjects::Subject148, 0, 1, s148, -255, 255, 0, 0);
	this->initConfig(FsConfigSubjects::Subject149, 0, 2, s149, 0, 255, 0, 1);
	this->initConfig(FsConfigSubjects::Subject150, 0, 2, s150, 0, 255, 0, 1);
	this->initConfig(FsConfigSubjects::Subject151, 0, 4, 0, 0, 1, 0, 0);
	this->initConfig(FsConfigSubjects::Subject152, 0, 4, 0, 0, 0xFFFF, 0, 0);
	this->initConfig(FsConfigSubjects::Subject199, 0, 4, 0, 0, 0xFFFF, 0, 0);
	this->initConfig(FsConfigSubjects::Subject200, 1, 4, 0, 0, 1, 1, 0);
	this->initConfig(FsConfigSubjects::Subject153, 0, 4, 0, 0, 0x7FFFFFFF, 0, 0);
	this->initConfig(FsConfigSubjects::Subject154, 0, 4, 0, 0, 1, 0, 1);
	this->initConfig(FsConfigSubjects::Subject161, 1801548915, 4, 0, 1801548915, 1801548915, 1801548915, 0);
	this->initConfig(FsConfigSubjects::Subject162, 0, 4, 0, 0, 3, 0, 0);
	this->initConfig(FsConfigSubjects::Subject163, 1936222574, 4, 0, 1936222574, 1936222574, 1936222574, 0);
	this->initConfig(FsConfigSubjects::Subject164, 0, 4, 0, 0, 1, 0, 0);
	this->initConfig(FsConfigSubjects::Subject165, 0, 4, 0, 0, 1, 0, 0);
	this->initConfig(FsConfigSubjects::Subject166, 0, 4, 0, 0, 1, 0, 0);
	this->initConfig(FsConfigSubjects::Subject167, 0, 4, 0, 0, 1, 0, 0);
	this->initConfig(FsConfigSubjects::Subject168, 100, 6, 0, 0, 100, 100, 1);
	this->initConfig(FsConfigSubjects::Subject171, 0, 6, 0, 0, 5, 0, 1);
	this->initConfig(FsConfigSubjects::Subject172, 0, 6, 0, 0, 1, 0, 1);
	this->initConfig(FsConfigSubjects::Subject173, 0, 6, 0, 0, 1, 0, 1);
	this->initConfig(FsConfigSubjects::TitleScreenSong, 0, 6, 0, 0, 8, 0, 1);
	this->initConfig(FsConfigSubjects::Subject169, 0, 4, 0, 0, 1, 0, 0);
	this->initConfig(FsConfigSubjects::Subject170, 0, 4, 0, 0, 1, 0, 0);
	this->initConfig(FsConfigSubjects::Subject174, 0, 4, 0, 0, 1, 0, 0);
	this->initConfig(FsConfigSubjects::Subject180, 0, 4, 0, 0, 2, 0, 0);
	this->initConfig(FsConfigSubjects::Subject181, 0, 4, 0, 0, 2, 0, 0);
	this->initConfig(FsConfigSubjects::Subject182, 0, 4, 0, 0, 1, 0, 0);
	this->initConfig(FsConfigSubjects::Subject183, 8, 4, 0, 4, 20, 8, 0);
	this->initConfig(FsConfigSubjects::Subject184, 0, 4, 0, 0, 20, 0, 0);
	this->initConfig(FsConfigSubjects::Subject185, 0, 4, 0, 0, -1, 0, 0);
	this->initConfig(FsConfigSubjects::Subject186, -1, 4, 0, 0, -1, -1, 0);
	this->initConfig(FsConfigSubjects::Subject187, -1, 4, 0, 0, -1, -1, 0);
	this->initConfig(FsConfigSubjects::Subject188, 0, 4, 0, 0, -1, 0, 0);
	this->initConfig(FsConfigSubjects::Subject190, 0, 4, 0, 0, 1, 0, 0);
	this->initConfig(FsConfigSubjects::Subject191, 0, 4, 0, 0, 1, 0, 0);
	this->initConfig(FsConfigSubjects::Subject192, 0, 4, 0, 0, 1, 0, 0);
	this->initConfig(FsConfigSubjects::Subject193, 0, 4, 0, 0, 1, 0, 0);
	this->initConfig(FsConfigSubjects::Subject194, 0, 4, 0, 1, 1, 0, 0);
	this->initConfig(FsConfigSubjects::Subject195, 0, 4, 0, 1, 1, 0, 0);
	this->initConfig(FsConfigSubjects::Subject196, 10, 4, 0, 1, 100, 10, 0);
	this->initConfig(FsConfigSubjects::Subject201, 0, 4, 0, 0, 2, 0, 0);
	this->initConfig(FsConfigSubjects::Subject202, 0, 4, 0, 0, 1, 0, 0);
	this->field_9 = 0;
	this->field_A = 0;
}

FFXI::Config::FsConfig::~FsConfig()
{
	if (this->subjects != nullptr) {
		delete[] this->subjects;
		this->subjects = nullptr;
	}
}

void FFXI::Config::FsConfig::initConfig(Constants::Enums::FsConfigSubjects a2, int a3, int a4, int* a5, int a6, int a7, int a8, int a9)
{
	if (this->subjects == nullptr)
		return;

	FsConfigSubject* subj = this->subjects + (int)a2;
	subj->Index = (int)a2;
	subj->field_8 = a3;
	subj->type = a4;
	if (a5 != nullptr) {
		if (a4 == 1 || a4 == 2) {
			subj->field_10 = a5[0];
			subj->field_14 = a5[1];
		}
	}
	subj->field_18 = a6;
	subj->field_1C = a7;
	subj->field_20 = a8;
	subj->field_28 = a9;
}

void FFXI::Config::FsConfig::initConfig()
{
	this->setAllConfig(false);
	this->field_9 = 0;
	this->field_A = 0;
}

void FFXI::Config::FsConfig::setAllConfig(bool a2)
{
	for (int i = 0; i < (int)Constants::Enums::FsConfigSubjects::count; ++i) {
		FsConfigSubject* subj = this->subjects + i;
		if (a2 == true) {
			if (subj->type == FsConfigSubject::type::TIG) {
				continue;
			}
		}

		subj->field_8 = subj->field_20;
		if (subj->field_24 != nullptr)
			subj->field_24->runAllCallbacks(subj->Index, subj->field_8);
	}
}

void FFXI::Config::FsConfig::setConfig(FsConfigSubject* a2, int a3)
{
	if (a2->field_8 == a3 && (a2->field_28 & 1) == 0)
		return;

	int v3 = a3;
	if (a2->field_1C != 1 && (a2->field_18 > a3 || a3 > a2->field_1C))
		v3 = a2->field_20;

	a2->field_8 = v3;
	if (a2->field_24 != nullptr)
		a2->field_24->runAllCallbacks(a2->Index, a2->field_8);

}

void FFXI::Config::FsConfig::registerConfigCallback(int a2, void(__cdecl* a3)(int, int, CTk::CTkMenuPrimitive*), CTk::CTkMenuPrimitive* a4)
{
	if (a3 == nullptr)
		return;

	FsConfigSubject* subj = this->findConfigSubject(a2);
	if (subj == nullptr)
		return;

	if (subj->field_24 != nullptr) {
		subj->field_24->addToEnd(a3, a4);
	}
	else {
		subj->field_24 = new FsCallbackListLink(a3, a4);
	}
	if (subj->field_24 != nullptr) {
		subj->field_24->runAllCallbacks(subj->Index, subj->field_8);
	}
}

int FFXI::Config::FsConfig::getConfig(int a2)
{
	FsConfigSubject* subj = this->findConfigSubject(a2);
	if (subj == nullptr)
		return -1;
	
	return subj->field_8;
}

FsConfigSubject* FFXI::Config::FsConfig::findConfigSubject(unsigned int a2)
{
	if (this->subjects == nullptr)
		return nullptr;

	return this->subjects + a2;
}
