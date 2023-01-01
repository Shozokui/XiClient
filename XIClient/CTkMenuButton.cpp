#include "CTkMenuButton.h"
#include "CTkResourceID.h"
#include "CTkMenuMng.h"
#include "StringTables.h"
#include "Globals.h"

using namespace FFXI::CTk;

FFXI::CTk::CTkMenuButton::CTkMenuButton()
{
	this->Buffer = nullptr;
	this->ResourceIDs = nullptr;
	this->field_8 = 0;
	this->field_C = 0;
	this->field_10 = 0;
}

FFXI::CTk::CTkMenuButton::~CTkMenuButton()
{
	if (this->ResourceIDs) {
		delete[] this->ResourceIDs;
		this->ResourceIDs = nullptr;
	}
}

void FFXI::CTk::CTkMenuButton::Store(void** a2)
{
	char** charData = (char**)a2;
	unsigned char* byteData = (unsigned char*)*a2;
	this->Buffer = byteData;

	*charData += 32;

	CTkResourceID* v6{ nullptr };
	if (this->Buffer[27]) {
		int v5 = this->Buffer[27];
		v6 = new CTkResourceID[this->Buffer[27]];
		if (!v6)
			return;
		int v10 = 0;
		do {
			Globals::CopyAdvance(&v6[v10].field_0, (void**)charData, sizeof(v6[v10].field_0));
			Globals::CopyAdvance(&v6[v10].field_2, (void**)charData, sizeof(v6[v10].field_2));
			if (!strncmp(*charData, "menu    frames", 0xE)) {
				const char* f = CTkMenuMng::GetLanguageDependentMenu(Constants::Enums::LanguageDependentMenus::LDM_Frames);
				memcpy(*charData, f, sizeof(v6[v10].field_4));
			}
			if (!strncmp(*charData, "menu    comwin", 0xE)) {
				const char* f = CTkMenuMng::GetLanguageDependentMenu(Constants::Enums::LanguageDependentMenus::LDM_ComWin);
				memcpy(*charData, f, sizeof(v6[v10].field_4));
			}
			if (!strncmp(*charData, "menu    key1top", 0xF)) {
				const char* f = CTkMenuMng::GetLanguageDependentMenu(Constants::Enums::LanguageDependentMenus::LDM_Key1Top);
				memcpy(*charData, f, sizeof(v6[v10].field_4));
			}
			if (!strncmp(*charData, "menu    lobbyps2", 0xE))
				memcpy(*charData, "menu    lobbywin", 0x10);
			Globals::CopyAdvance(&v6[v10].field_4, (void**)charData, sizeof(v6[v10].field_4));
			++v10;
		} while (v10 < v5);
	}
	
	this->ResourceIDs = v6;
	this->field_10 = 0;
	this->field_8 = nullptr;
	if (this->Buffer[29]) {
		int v19 = atoi(charData[0]);
		if (v19 >= 0)
			this->field_8 = FFXI::Text::XiStrGet(Constants::Enums::StrTables::XIStr_Menu2, v19);
	}

	*charData += this->Buffer[29] + 1;

	this->field_C = nullptr;
	if (this->Buffer[30]) {
		int v21 = atoi(charData[0]);
		if (v21 >= 0)
			this->field_C = FFXI::Text::XiStrGet(Constants::Enums::StrTables::XIStr_Menu1, v21);
	}

	*charData += this->Buffer[30] + 1;
	*charData = (char*)this->Buffer + *((unsigned short*)this->Buffer);
}
