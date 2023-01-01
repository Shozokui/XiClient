#include "YkManager.h"
#include "TkManager.h"
#include "CTkMsbDataList.h"
#include "Strings.h"
#include "CMoResourceMng.h"
#include "Globals.h"
#include "CYyDb.h"
#include "CYyResourceFile.h"
#include "StringTables.h"
#include <string>
using namespace FFXI::Yk;

FFXI::CTk::_49SubList*** YkManager::g_shape[18];
FFXI::CYy::CYyResourceFile** YkManager::SomeFile{ nullptr };
char YkManager::SomeChar{ 0 };
void FFXI::Yk::YkManager::YkInit()
{
	YkCommonInit();
}

void FFXI::Yk::YkManager::YkEnd()
{
	if (SomeFile && *SomeFile) {
		(*SomeFile)->SomeClean3();
		CYy::CMoResource::UnlinkFromManager((CYy::CMoResource***)&SomeFile);
		SomeFile = nullptr;
	}
}

void FFXI::Yk::YkManager::YkCommonInit()
{
	SomeChar = *FFXI::Text::XiStrGet(FFXI::Constants::Enums::StrTables::XIStr_MiscMenuText, 0x5B);
	//sub //TODO

	memset(g_shape, 0, sizeof(g_shape));
	g_shape[0] = FFXI::CTk::TkManager::g_CTkMenuDataList.GetSubLists(FFXI::Constants::Strings::FontFontShp);
	g_shape[1] = FFXI::CTk::TkManager::g_CTkMenuDataList.GetSubLists(FFXI::Constants::Strings::MenuWindowPs);
	g_shape[2] = FFXI::CTk::TkManager::g_CTkMenuDataList.GetSubLists(FFXI::Constants::Strings::MenuKeyTops3);
	g_shape[3] = FFXI::CTk::TkManager::g_CTkMenuDataList.GetSubLists(FFXI::Constants::Strings::MenuPName);

	const char* LanguageDependentMenu = CTk::TkManager::g_CTkMenuMng.GetLanguageDependentMenu(0);
	g_shape[4] = FFXI::CTk::TkManager::g_CTkMenuDataList.GetSubLists(LanguageDependentMenu);
	g_shape[5] = FFXI::CTk::TkManager::g_CTkMenuDataList.GetSubLists(FFXI::Constants::Strings::MenuContents);
	g_shape[6] = FFXI::CTk::TkManager::g_CTkMenuDataList.GetSubLists(FFXI::Constants::Strings::AncL);
	g_shape[7] = FFXI::CTk::TkManager::g_CTkMenuDataList.GetSubLists(FFXI::Constants::Strings::AncS);
	g_shape[8] = FFXI::CTk::TkManager::g_CTkMenuDataList.GetSubLists(FFXI::Constants::Strings::AncScan);
	g_shape[9] = FFXI::CTk::TkManager::g_CTkMenuDataList.GetSubLists(FFXI::Constants::Strings::AncRegi);
	g_shape[10] = FFXI::CTk::TkManager::g_CTkMenuDataList.GetSubLists(FFXI::Constants::Strings::AncEqui);
	g_shape[11] = FFXI::CTk::TkManager::g_CTkMenuDataList.GetSubLists(FFXI::Constants::Strings::AncKaiPage);
	g_shape[12] = FFXI::CTk::TkManager::g_CTkMenuDataList.GetSubLists(FFXI::Constants::Strings::MenuHPCol);
	g_shape[13] = FFXI::CTk::TkManager::g_CTkMenuDataList.GetSubLists(FFXI::Constants::Strings::MenuNumCtrl);
	g_shape[14] = FFXI::CTk::TkManager::g_CTkMenuDataList.GetSubLists(FFXI::Constants::Strings::FontUsGaiji);
	g_shape[15] = FFXI::CTk::TkManager::g_CTkMenuDataList.GetSubLists(FFXI::Constants::Strings::MenuFep);
	g_shape[16] = FFXI::CTk::TkManager::g_CTkMenuDataList.GetSubLists(FFXI::Constants::Strings::MenuNetmeter);
	g_shape[17] = CYyDb::g_pCYyDb->pCMoResourceMng->LoadNumFileGet49(0x5C, &SomeFile);
}

FFXI::CTk::_49SubList*** FFXI::Yk::YkManager::GetMenuRes(int a2)
{
	if (a2 < 0 || a2 >= (sizeof(g_shape) / sizeof(g_shape[0])))
		return nullptr;

	return g_shape[a2];
}
