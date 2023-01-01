#define WIN32_LEAN_AND_MEAN
#include "DMsg.h"
#include "Globals.h"
#include "XIFileManager.h"
using namespace FFXI::Text;

void NotBytes(bool flag, unsigned char* data, int length) {
	if (flag) {
		for (int i = 0; i < length; ++i)
			data[i] = ~data[i];
	}
}

FFXI::Text::DMsg::~DMsg()
{
	if (this->Buffer) {
		delete[] this->Buffer;
		this->Buffer = nullptr;
		this->field_0 = nullptr;
	}
}

bool DMsg::LoadFromFile(DMsg* p_table, int p_index) {
	int fileSize = FFXI::File::XIFileManager::g_pXIFileManager->GetFileSizeByNumfile(p_index);
	
	if (fileSize < 0)
		return false;

	char* v4 = new char[fileSize];
	if (FFXI::File::XIFileManager::g_pXIFileManager->ReadNumfileNow(p_index, v4, fileSize, 0) < 0)
		return false;

	p_table->LoadFromData(v4, fileSize);
	return true;
}

void DMsg::LoadFromData(char* a2, int a3)
{
	this->Parse(a2, a3);
	if (this->Buffer)
		this->field_0 = a2;
}

void DMsg::Parse(char* a2, int a3)
{
	unsigned short* wordData = (unsigned short*)a2;
	unsigned int* dwordData = (unsigned int*)a2;

	int v4 = strcmp(a2, "d_msg");

	if (!v4) {
		this->Buffer = a2;
		int v6 = wordData[4];
		if (v6 == 16) {
			this->FirstEntryPos = a2 + 64;
		}
		else {
			if (v6 != 1)
				throw "NOT IMPLEMENETED"; //sub TODO
			if (wordData[6] != 3 || wordData[7]) {
				this->Buffer = nullptr;
			}
			else {
				bool bytesEncrypted = wordData[5];
				unsigned int EntryCount = dwordData[10];

				if (dwordData[7]) {
					this->EntryListPos = a2 + 64;
					this->FirstEntryPos = a2 + 8 * EntryCount + 64;
					NotBytes(bytesEncrypted, (unsigned char*)(a2 + 64), 8 * EntryCount);
				}
				else {
					this->FirstEntryPos = a2 + 64;
				}

				int v5 = 0;
				unsigned int v10{}, v11{};
				
				while (v5 < EntryCount) {
					if (dwordData[7]) {
						if (wordData[4] != 1)
							;//sub
						v10 = *(unsigned int*)(this->EntryListPos + 8 * v5 + 4);
						v11 = *(unsigned int*)(this->EntryListPos + 8 * v5);
					}
					else {
						v10 = dwordData[8];
						v11 = v5 * v10;
					}
					NotBytes(bytesEncrypted, (unsigned char*)(this->FirstEntryPos + v11),v10);
					if (wordData[4] != 1)
						;//sub
					++v5;
				}
			}
		}
	}
}

char* FFXI::Text::DMsg::GetEntry0(int tableIndex)
{
	return this->GetEntry(tableIndex, 0);
}

char* FFXI::Text::DMsg::GetEntry(int tableIndex, int entryIndex)
{
	unsigned int* dwordData = (unsigned int*)this->Buffer;
	unsigned short* wordData = (unsigned short*)this->Buffer;

	unsigned int tableCount = dwordData[10];

	if (tableIndex >= tableCount)
		return nullptr;

	if (wordData[4] == 16) {
		if (!this->FirstEntryPos)
			return &Globals::NullString;

		int v5 = entryIndex + tableIndex * wordData[31];
		char* entry = this->FirstEntryPos + 8 * v5;
		unsigned v7 = *(unsigned int*)entry;
		unsigned v6 = *(unsigned int*)(entry + 4);

		if (v6)
			return this->FirstEntryPos + v7;

		return &Globals::NullString;
	}

	char* Table = this->GetTable(tableIndex);
	return DMsg::GetTableEntry(Table, entryIndex);
}

char* FFXI::Text::DMsg::GetTable(int tableIndex)
{
	unsigned int* dwordData = (unsigned int*)this->Buffer;
	unsigned int tableCount = dwordData[10];

	if (tableIndex >= tableCount)
		return nullptr;

	if (dwordData[7])
		return this->FirstEntryPos + *(unsigned int*)(this->EntryListPos + 8 * tableIndex);
	
	return this->FirstEntryPos + tableIndex * dwordData[8];
}

char* FFXI::Text::DMsg::GetTableEntry(char* data, int entryIndex)
{
	unsigned short* wordData = (unsigned short*)data;
	unsigned int* dwordData = (unsigned int*)data;

	unsigned short entryCount = wordData[0];

	if (!data)
		return nullptr;

	if (entryIndex >= entryCount)
		return nullptr;

	if (dwordData[2 + 2 * entryIndex])
		return nullptr;

	char* v3 = &data[dwordData[1 + 2 * entryIndex]];
	if (*(short*)v3 == 1)
		return v3 + 28;

	return nullptr;
}
