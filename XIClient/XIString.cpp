#include "XIString.h"
#include "StringTables.h"
#include "Globals.h"
#include "XIFileManager.h"

int calcNewLen(int a1) {
	if (a1 >= 64)
		return (3 * a1) >> 1;

	return 64;
}

bool FFXI::Text::XIString::LoadFromFile(int StrPageIndex, int fileIndex)
{
	int fileSize = FFXI::File::XIFileManager::g_pXIFileManager->GetFileSizeByNumfile(fileIndex);
	if (fileSize < 0)
		return false;

	char* v4 = new char[fileSize];
	if (!v4)
		return false;

	if (FFXI::File::XIFileManager::g_pXIFileManager->ReadNumfileNow(fileIndex, v4, fileSize, 0) < 0)
		return false;

	FFXI::Text::XIStrTable[StrPageIndex] = v4;
	return true;
}

char* FFXI::Text::XIString::GetEntry(char* data, int entryIndex)
{
	short* shortData = (short*)data;
	unsigned int* dwordData = (unsigned int*)data;

	unsigned int EntryCount = dwordData[9];

	if (entryIndex < 0)
		return nullptr;

	if (entryIndex >= EntryCount)
		return nullptr;

	if (shortData[5] < 2)
		return data + 4 * EntryCount + 0x38 + shortData[28 + 2 * entryIndex];

	return data + 12 * EntryCount + 0x38 + dwordData[14 + 3 * entryIndex];
}

FFXI::Text::XIString::XIString()
{
	this->TextLength = 0;
	this->BufferLength = 64;
	this->Text = new char[this->BufferLength];
	this->Text[0] = 0;
}

FFXI::Text::XIString::XIString(const char* a2)
{
	if (a2) {
		unsigned int v4 = strlen(a2) + 1;
		this->TextLength = v4 - 1;
		unsigned int v6 = calcNewLen(v4);
		this->BufferLength = v6;
		this->Text = new char[v6];
		strcpy_s(this->Text, this->BufferLength, a2);
	}
	else {
		this->BufferLength = 64;
		this->Text = new char[64];
		this->TextLength = 0;
		this->Text[0] = 0;
	}
}

FFXI::Text::XIString::XIString(const char* a1, int a2)
{
	if (a1) {
		this->TextLength = a2;
		this->BufferLength = a2 + 1;
		this->Text = new char[this->BufferLength];
		strncpy_s(this->Text, this->BufferLength, a1, a2);
		this->Text[this->TextLength] = 0;
	}
	else {
		this->BufferLength = 64;
		this->TextLength = 0;
		this->Text = new char[this->BufferLength];
		this->Text[this->TextLength] = 0;
	}
}

FFXI::Text::XIString::~XIString()
{
	if (!this->Text) return;

	this->BufferLength = 0;
	this->TextLength = 0;
	delete[] this->Text;
	this->Text = nullptr;
}

void FFXI::Text::XIString::AppendStrLen(const char* a2, int a3)
{
	if (!a2 || !*a2) return;

	if (a3 == -1)
		a3 = strlen(a2);

	if ((this->TextLength + a3) >= this->BufferLength) {
		int v9 = calcNewLen(this->TextLength + a3 + 1);
		char* v10 = new char[v9];
		v10[0] = 0;
		this->BufferLength = v9;
		if (this->Text) {
			strcpy_s(v10, v9, this->Text);
			delete[] this->Text;
		}
		strncat_s(v10, v9, a2, a3);
		this->Text = v10;
	}
	else
		strncpy_s(this->Text + this->TextLength, this->BufferLength, a2, a3);

	this->TextLength += a3;
	this->Text[this->TextLength] = 0;
}

void FFXI::Text::XIString::AppendStr(const char* a2)
{
	this->AppendStrLen(a2, -1);
}

const char* FFXI::Text::XIString::c_str()
{
	if (this->TextLength)
		return this->Text;

	return &Globals::NullString;
}

void FFXI::Text::XIString::__as(const char* a2)
{
	__asLen(a2, -1);
}

void FFXI::Text::XIString::__asLen(const char* a2, int a3)
{
	if (this->Text == a2) return;

	if (!a2 || !*a2) {
		this->ClearText();
		return;
	}

	if (a3 == -1)
		this->TextLength = strlen(a2);
	else
		this->TextLength = a3;

	if (this->Text >= a2 || a2 > this->Text + this->TextLength) {
		if (this->TextLength >= this->BufferLength) {
			if (this->Text)
				delete[] this->Text;

			int v8 = calcNewLen(this->TextLength + 1);
			this->BufferLength = v8;
			this->Text = new char[v8];
		}

		strncpy_s(this->Text, this->BufferLength, a2, this->TextLength);
		this->Text[this->TextLength] = 0;
	}
	else
		memcpy(this->Text, a2, this->TextLength + 1);
}

void FFXI::Text::XIString::ClearText()
{
	this->TextLength = 0;
	if (this->Text)
		this->Text[0] = 0;
}
