#include "CTkMenuCursol.h"
#include "CTkResourceID.h"
#include "CTkMenuMng.h"
#include "Enums.h"
#include <string>

using namespace FFXI::CTk;

FFXI::CTk::CTkMenuCursol::CTkMenuCursol()
{
	this->field_22 = 0;
	this->field_24 = 0;
}

FFXI::CTk::CTkMenuCursol::~CTkMenuCursol()
{
}

void FFXI::CTk::CTkMenuCursol::SetCursolID(CTkResourceID* a2, unsigned short a3, unsigned short a4)
{
	memcpy(this->field_0, a2->field_4, sizeof(this->field_0));

	if (!strncmp(this->field_0, "menu    frames", 0xE)) {
		const char* f = CTkMenuMng::GetLanguageDependentMenu(Constants::Enums::LanguageDependentMenus::LDM_Frames);
		memcpy(this->field_0, f, sizeof(this->field_0));
	}
	if (!strncmp(this->field_0, "menu    comwin", 0xE)) {
		const char* f = CTkMenuMng::GetLanguageDependentMenu(Constants::Enums::LanguageDependentMenus::LDM_ComWin);
		memcpy(this->field_0, f, sizeof(this->field_0));
	}
	if (!strncmp(this->field_0, "menu    key1top", 0xF)) {
		const char* f = CTkMenuMng::GetLanguageDependentMenu(Constants::Enums::LanguageDependentMenus::LDM_Key1Top);
		memcpy(this->field_0, f, sizeof(this->field_0));
	}
	if (!strncmp(this->field_0, "menu    lobbyps2", 0xE))
		memcpy(this->field_0, "menu    lobbywin", 0x10);

	this->field_10 = 0;
	this->field_20 = a2->field_2;
	this->field_24 = a3;
	this->field_22 = a4;
}
