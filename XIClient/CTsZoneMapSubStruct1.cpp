#include "CTsZoneMapSubStruct1.h"
#include "MapChunk.h"
#include "Globals.h"
#include "StorageLists.h"
#include "UnderscoreAtStruct.h"
#include "XiZone.h"
using namespace FFXI::CYy;

void FFXI::CYy::CTsZoneMapSubStruct1::MakeChunkList()
{
	int* v1 = this->field_30;
	if (!v1) return;

	for (int i = 0; i < this->field_2C; ++i) {
		int v4 = *v1++;
		if (v4 > 0) {
			while (v4) {
				int v5 = *v1++;
				--v4;
				*(v1 - 1) = (int)(this->MapChunks + v5);
			}
		}
	}
}

void FFXI::CYy::CTsZoneMapSubStruct1::InitUnderscoreAtStructs()
{
	this->field_40 = 0;

	if (this->MapChunkCount > 0) {
		for (unsigned int i = 0; i < this->MapChunkCount; ++i)
			this->MapChunks[i].field_C = 0;
		
		for (unsigned int i = 0; i < this->MapChunkCount; ++i) {
			MapChunk* chunk = this->MapChunks + i;

			if (chunk->field_C0 != 0) {
				if (chunk->field_C == 0) {
					unsigned char v8 = (unsigned char)chunk->field_C0;
					if (v8 == '_' || v8 == '@') {
						this->field_40 += 1;
						for (unsigned int j = 0; j < this->MapChunkCount; ++j) {
							if (chunk->field_C0 == this->MapChunks[j].field_C0)
								this->MapChunks[j].field_C = 1;
						}
					}
				}
			}
		}
	}

	this->UnderscoreAtStructs = nullptr;
	if (this->field_40 == 0) return;

	char* mem = StorageLists::instance->Get(sizeof(UnderscoreAtStruct) * this->field_40 + 4, Constants::Enums::MEM_MODE::Ex);
	MemoryHeader* header = (MemoryHeader*)(mem - sizeof(MemoryHeader));
	header->field_18 |= 2;

	if (mem)
		void* test = new (mem) UnderscoreAtStruct[this->field_40];

	if (this->UnderscoreAtStructs == nullptr) return;

	if (this->MapChunkCount > 0) {
		for (unsigned int i = 0; i < this->MapChunkCount; ++i)
			this->MapChunks[i].field_C = 0;
		
		UnderscoreAtStruct* uas = this->UnderscoreAtStructs;
		for (unsigned int i = 0; i < this->MapChunkCount; ++i) {
			MapChunk* chunk = this->MapChunks + i;
			if (!chunk->field_C) {
				int v20 = (unsigned char)chunk->field_C0;
				if (v20 == '_' || v20 == '@') {
					uas->FourCC = chunk->field_C0;
					int v22 = 0;
					for (unsigned int j = 0; j < this->MapChunkCount; ++j) {
						MapChunk* subchunk = this->MapChunks + j;
						if (chunk->field_C0 == subchunk->field_C0) {
							if (v22 < 4) {
								uas->Subchunks[v22] = subchunk;
								uas->Translations[v22] = subchunk->Translation;
								uas->Rotations[v22] = subchunk->Rotation;
								uas->Scalings[v22] = subchunk->Scaling;
								uas->InitMatrix(v22);
							}
							++v22;
							subchunk->field_C = 1;
						}
					}
					if (v22) {
						if (v22 > 4)
							v22 = 4;
						uas->SubchunkCount = v22;
					}

					++uas;
				}
			}
		}
	}
}

void FFXI::CYy::CTsZoneMapSubStruct1::BuildFourCCList()
{
	this->FourCCListSize = 0;
	this->field_70 = 0;

	if (this->MapChunkCount <= 0) return;

	int* fourCCList = new int[this->MapChunkCount];
	int v4 = 0;
	for (unsigned int i = 0; i < this->MapChunkCount; ++i) {
		MapChunk* chunk = this->MapChunks + i;
		if (chunk->SomeFourCC) {
			int v8 = chunk->field_C0;
			if (!v8 || (char)v8 == '_') {
				int v9 = 0;
				if (v4 > 0) {
					for (int j = 0; j < v4; ++j) {
						if (fourCCList[j] == chunk->SomeFourCC)
							v9 = 1;
					}
				}

				if (!v9)
					fourCCList[v4++] = chunk->SomeFourCC;
			}
			else {
				chunk->SomeFourCC = 0;
			}
		}
	}

	this->FourCCListSize = v4;
	if (v4) {
		int newSize = 4 * v4;
		this->FourCCList = new int[v4];
		memcpy_s(this->FourCCList, newSize, fourCCList, newSize);
	}
	if (fourCCList)
		delete[] fourCCList;
}

void FFXI::CYy::CTsZoneMapSubStruct1::GetChunky(char* a2, int* a3)
{
	int v3 = a3[24];
	if (v3) {
		int* v4 = (int*)(a2 + v3);
		a3[24] = (int)v4;

		int count = a3[25];
		if (count > 0) {
			for (int i = 0; i < count; ++i) {
				int v7 = v4[i];
				v4[i] = (int)(this->MapChunks + v7);
			}
		}
	}
	else {
		int* v8 = a3 + 26;
		for (int i = 0; i < 4; ++i) {
			int v9 = *v8;
			if (v9) {
				*v8 = (int)(a2 + v9);
				this->GetChunky(a2, (int*)(a2 + v9));
			}
			++v8;
		}
	}
}

void FFXI::CYy::CTsZoneMapSubStruct1::CleanFourCCList()
{
	if (!this->field_0) return;

	for (int i = 0; i < this->FourCCListSize; ++i) {
		XiArea* v3 = XiArea::GetByFourCC(this->FourCCList[i]);
		if (v3 && v3 != XiZone::zone) {
			delete v3;
		}
	}
	if (this->FourCCList) {
		delete[] this->FourCCList;
		this->FourCCList = nullptr;
		this->FourCCListSize = 0;
		this->field_70 = 0;
	}
}
