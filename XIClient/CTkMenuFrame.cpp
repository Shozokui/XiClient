#include "CTkMenuFrame.h"
#include "CTkResourceID.h"
#include "CTkMenuMng.h"
#include "StringTables.h"
#include "Enums.h"
#include "Globals.h"

using namespace FFXI::CTk;

FFXI::CTk::CTkMenuFrame::CTkMenuFrame()
{
	this->Buffer = nullptr;
	this->ResourceIDs = nullptr;
	this->field_8 = 0;
	this->field_C = 0;
}

FFXI::CTk::CTkMenuFrame::~CTkMenuFrame()
{
	if (this->ResourceIDs) {
		delete[] this->ResourceIDs;
		this->ResourceIDs = nullptr;
	}
}

void FFXI::CTk::CTkMenuFrame::Store(void** a1)
{
	char** charData = (char**)a1;
	unsigned char* byteData = (unsigned char*)*a1;
	this->Buffer = byteData;

	*charData += 32;

	CTkResourceID* v6{ nullptr };
	if (byteData[20]) {
		int v5 = byteData[20];
		v6 = new CTkResourceID[v5];
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
	this->field_8 = nullptr;
	if (this->Buffer[21]) {
		int v19 = atoi(*charData);
		if (v19 >= 0)
			this->field_8 = Text::XiStrGet(Constants::Enums::StrTables::XIStr_Menu2, v19);
	}
	*charData += this->Buffer[21] + 1;
	this->field_C = nullptr;
	if (this->Buffer[22]) {
		int v21 = atoi(*charData);
		if (v21 >= 0)
			this->field_C = Text::XiStrGet(Constants::Enums::StrTables::XIStr_Menu1, v21);
	}
	*charData += this->Buffer[22] + 1;
	*charData = (char*)this->Buffer + *((unsigned short*)this->Buffer);
}

CTkResourceID *FFXI::CTk::CTkMenuFrame::GetResourceID(int a2)
{
	unsigned char* byteData = (unsigned char*)this->Buffer;

	int v2 = 0;
	int v3 = byteData[20];
	if (!v3)
		return 0;

	CTkResourceID* v4 = this->ResourceIDs;

	for (CTkResourceID* i = v4; i->field_0 != a2; ++i) {
		if (++v2 >= v3)
			return 0;
	}
	return &v4[v2];
}
