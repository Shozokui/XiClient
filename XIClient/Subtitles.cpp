#include "Subtitles.h"
#include "Enums.h"
#include "TkManager.h"
#include "Globals.h"
#include "CYyDb.h"
#include "CMoResourceMng.h"
#include "CMoResource.h"
#include "CTkMenuMng.h"
#include "DatIndices.h"
#include "_49SubList.h"
#include "CYyTexMng.h"
#include "CYyResourceFile.h"
#include "Strings.h"
using namespace FFXI::CYy;
using namespace FFXI::CTk;

const int Timings[48] = {
	0x56, 0x0A0, 0x0A0, 0x0E6, 0x0F0, 0x18A, 0x18A, 0x21C, 0x21C,
	0x28A, 0x28A, 0x320, 0x320, 0x39D, 0x3A2, 0x410, 0x578, 0x5C8,
	0x5DC, 0x636, 0x64A, 0x6C2, 0x6E0, 0x780, 0x7BC, 0x852, 0x870,
	0x8FC, 0x92E, 0x0A28, 0x1C5C, 0x1CB1, 0x1CB6, 0x1CE8, 0x1E78,
	0x1EA0, 0x1EAA, 0x1EF0, 0x1EF0, 0x1F0E, 0x2044, 0x208A, 0x211B,
	0x214D, 0x2B07, 0x2B98, 0x0, 0x0
};
Subtitles::Subtitles(int region) {
	this->field_4 = 0;
	int NumFile{ 0 };
	if (region == Constants::Enums::LanguageCode::English) {
		//There aren't any english subtitles
		return; 
	}
	else if (region == Constants::Enums::LanguageCode::Japanese) {
		NumFile = Constants::DatIndices::TEX_OpeningMovieSubs_JP;
	}
	else {
		char check = *(char*)&FFXI::GlobalStruct::g_GlobalStruct.field_18;
		if (check < 0) return;
		if (region == Constants::Enums::LanguageCode::German) {
			NumFile = Constants::DatIndices::TEX_OpeningMovieSubs_GE;
		}
		else if (region == Constants::Enums::LanguageCode::French) {
			NumFile = Constants::DatIndices::TEX_OpeningMovieSubs_FR;
		}
		else
			return;
	}

	CYyDb::pCMoResourceMng->LoadNumFile(&this->field_4, NumFile);
	if (this->field_4 != nullptr) {
		CMoResource::DoTheThing(*this->field_4);
		this->SubLists = TkManager::g_CTkMenuDataList.GetSubLists(FFXI::Constants::Strings::OpeningMovieSubtitles);
	}

	this->Init();
}

Subtitles::~Subtitles() {

}

void FFXI::CYy::Subtitles::Init()
{
	if (this->field_4) {
		if (*this->field_4) {
			this->field_C = 0;
			this->field_10 = 0;
		}
	}
}

void FFXI::CYy::Subtitles::Update()
{
	if (this->field_4) {
		if (this->field_C < 24) {
			if (Timings[2 * this->field_C] <= this->field_10) {
				int v4 = Timings[2 * this->field_C + 1];
				if (v4 > this->field_10) {
					_49SubList* sublist = (*this->SubLists)[this->field_C];
					double one = (double)(TkManager::g_CTkMenuMng.UIXRes - TkManager::g_CTkMenuMng.field_7C) / 2.0;
					double two = (double)(sublist->field_22 - sublist->field_20) / 2.0;
					double three = (double)(TkManager::g_CTkMenuMng.UIYRes - TkManager::g_CTkMenuMng.field_7E) * 400.0 / 512.0;
					sublist->Draw((int)(one - two), (int)three, 0x80808080, 0, 0);
				}
			}

			if (Timings[2 * this->field_C + 1] <= this->field_10) {
				this->field_C += 1;
				if (Timings[2 * this->field_C + 1] <= this->field_10) {
					this->field_C += 1;
				}
			}
		}
	}
}
