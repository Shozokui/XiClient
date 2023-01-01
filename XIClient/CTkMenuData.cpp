#include "CTkMenuData.h"
#include "CTkMenuFrame.h"
#include "CTkMenuCursol.h"
#include "CTkMenuButton.h"
#include "Globals.h"

#include <string>
using namespace FFXI::CTk;

FFXI::CTk::CTkMenuData::CTkMenuData()
{
	this->field_10 = 0;
	this->field_12 = 0;
	this->field_14 = 0;
	this->field_16 = 0;
	memset(this->field_1E, 0, sizeof(this->field_1E));
	this->field_3E = 0;
	this->field_40 = 0;
	this->MenuButtons = 0;
	this->MenuFrame = nullptr;
	this->MenuCursol = nullptr;
	this->field_12 = 0;
	this->field_16 = 0;
	this->field_10 = 0;
	this->field_14 = 0;
	this->ButtonCount = 0;
	this->field_1C = 0;
}

FFXI::CTk::CTkMenuData::~CTkMenuData()
{
	if (this->MenuFrame) {
		delete this->MenuFrame;
		this->MenuFrame = nullptr;
	}
	if (this->MenuButtons) {
		delete[] this->MenuButtons;
		this->MenuButtons = nullptr;
	}

	if (this->MenuCursol) {
		delete this->MenuCursol;
		this->MenuCursol = nullptr;
	}
}

void FFXI::CTk::CTkMenuData::Store(void** a1)
{
	unsigned char v20[0x10];

	Globals::CopyAdvance(this->field_46, a1, sizeof(this->field_46));
	Globals::CopyAdvance(v20, a1, sizeof(v20));

	this->MenuFrame = new CTkMenuFrame();
	if (this->MenuFrame == nullptr)
		return;

	this->MenuFrame->Store(a1);

	unsigned short* wordData = (unsigned short*)this->MenuFrame->Buffer;
	
	this->field_10 = wordData[1];
	this->field_12 = wordData[2];
	this->field_14 = this->field_10 + wordData[5];
	this->field_16 = this->field_12 + wordData[6];

	CTkResourceID* ResourceID = this->MenuFrame->GetResourceID(6);

	if (ResourceID) {
		this->MenuCursol = new CTkMenuCursol();
		if (!this->MenuCursol)
			return;

		this->MenuCursol->SetCursolID(ResourceID, wordData[3], wordData[4]);
	}

	if (v20[1]) {
		this->ButtonCount = v20[1];
		this->MenuButtons = new CTkMenuButton[this->ButtonCount];

		if (!this->MenuButtons) {
			delete this->MenuCursol;
			this->MenuCursol = nullptr;
			return;
		}

		for (int i = 0; i < this->ButtonCount; ++i)
			this->MenuButtons[i].Store(a1);
	}
}

CTkMenuFrame* FFXI::CTk::CTkMenuData::GetFrame()
{
	return this->MenuFrame;
}

CTkMenuButton* FFXI::CTk::CTkMenuData::GetButton(int index)
{
	if (this->ButtonCount > index - 1)
		return this->MenuButtons + index - 1;

	return nullptr;
}

CTkMenuCursol* FFXI::CTk::CTkMenuData::GetCursol()
{
	return this->MenuCursol;
}

void FFXI::CTk::CTkMenuData::SetLastCursol(short a2, short a3)
{
	this->field_1C = a3;
	this->field_1A = a2;
}

void FFXI::CTk::CTkMenuData::GetLastCursol(short* a2, short* a3)
{
	if (a3)
		*a3 = this->field_1C;
	*a2 = this->field_1A;
}

short FFXI::CTk::CTkMenuData::GetCommand(short a2, short a3)
{
	if (this->ButtonCount <= a2 - 1) return 0;
	if (!a2) return 0;

	char* data = (char*)this->MenuButtons[a2 - 1].Buffer;
	short* shortData = (short*)data;

	if (shortData[9] != a2) return 0;

	switch (a3) {
	case 1:
		return data[24];
	case 2:
		return data[23];
	case 3:
		return data[25];
	case 4:
		return data[26];
	default:
		return 0;
	}
	return 0;
}
