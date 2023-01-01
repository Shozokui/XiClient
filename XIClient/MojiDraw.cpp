#include "MojiDraw.h"
#include "CYyTex.h"
#include "Globals.h"
#include "CDx.h"
#include "CYyDb.h"
#include "CYyTexMng.h"
#include "TextRenderer.h"
#include "_49SubList.h"
#include "Strings.h"
#include "RegistryConfig.h"
#include "XIFileManager.h"
#include "CYyIcon.h"

using namespace FFXI::Text;
tagPOINT MojiDraw::TextPos{};

FFXI::CTk::_49SubList*** MojiDraw::g_pFontUsGaiji{ nullptr };

struct FontQualityEntry {
public:
	int FileID;
	int ImageHeight;
	int ImageWidth;
};

FontQualityEntry FontQualityMap[] = {
	{0x9A77, 0x400, 0x800},
	{0x9A78, 0x800, 0x1000}
};

void InitHQMapData(char** a1, int imageHeight, int imageWidth) {
	int datasize = 61 + 4 * imageHeight * imageWidth;
	char* v3 = new char[datasize];
	if (v3) {
		memset(v3, 0, datasize);
		int* intData = (int*)(v3 + 1);
		short* shortData = (short*)(v3 + 1);
		v3[0] = 48;
		Globals::CopyStrMax16(v3 + 1, FFXI::Constants::Strings::FontHigh);
		intData[6] = imageWidth;
		intData[5] = imageHeight;
		intData[4] = 0;
		shortData[14] = 0;
		shortData[15] = 32;
		intData[8] = 0;
		intData[9] = 0;
		intData[10] = 0;
		intData[11] = 0;
		intData[12] = 256;
		intData[13] = 32;

		v3[57] = 1;
		*a1 = v3;
	}
	else {
		*a1 = nullptr;
	}
}

FFXI::Text::MojiDraw::MojiDraw()
{
	this->Vertices[3].RHW = 1.0;
	this->Vertices[2].RHW = 1.0;
	this->Vertices[1].RHW = 1.0;
	this->Vertices[0].RHW = 1.0;
	this->Vertices[3].Z = 0;
	this->Vertices[2].Z = 0;
	this->Vertices[1].Z = 0;
	this->Vertices[0].Z = 0;
	this->Vertices[3].DiffuseColor = 0x80808080;
	this->Vertices[2].DiffuseColor = 0x80808080;
	this->Vertices[1].DiffuseColor = 0x80808080;
	this->Vertices[0].DiffuseColor = 0x80808080;
	this->BasicMojiMap = nullptr;
	this->HQTex = nullptr;
	this->HQMapData = nullptr;
	this->TryUseHQTex = 1;
}

FFXI::Text::MojiDraw::~MojiDraw()
{
	if (this->HQTex) {
		delete this->HQTex;
		this->HQTex = nullptr;
	}

	if (this->HQMapData) {
		delete[] this->HQMapData;
		this->HQMapData = nullptr;
	}

	this->TryUseHQTex = 0;
}

void FFXI::Text::MojiDraw::YmDrawString(int a1, int a2, const char* a3, int a4, D3DCOLOR a5, int a6, float a7, float a8)
{
	TextRenderer::PrepareDeviceForText(1, a6 & 3, (a6 >> 2) & 3, (a6 >> 6) & 3);
	TextPos.x = a1;
	TextPos.y = a2;
	TextRenderer::PrepareDeviceForText2();
	this->SetMojiTexture();

	unsigned int v11 = 0;

	unsigned char* v6 = (unsigned char*)a3;
	while ((!a4 || v11 < a4) && a3[v11] && *v6) {
		v11 += TextRenderer::GetCodeLen((unsigned char*)a3 + v11);
		short v12 = TextRenderer::GetSqCode(&v6);
		if (v12 == 0x87) {
			if (FFXI::GlobalStruct::g_GlobalStruct.RegionCode == 2 &&
				FFXI::GlobalStruct::g_GlobalStruct.field_18 >= 0 &&
				TextPos.x >= 8)
				TextPos.x -= 8;
		} 
		this->YmDrawChar(v12, a5, a6, a7, a8);
		int v15 = 0;
		this->SetIndent(v12, &v15);
		TextPos.x += (double)v15 * a7 + 0.5;
	}
	this->Vertices[3].DiffuseColor = 0x80808080;
	this->Vertices[2].DiffuseColor = 0x80808080;
	this->Vertices[1].DiffuseColor = 0x80808080;
	this->Vertices[0].DiffuseColor = 0x80808080;

	FFXI::CYy::CDx::instance->DXDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE2X);
	FFXI::CYy::CDx::instance->DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}

void FFXI::Text::MojiDraw::YmDrawChar(short a1, D3DCOLOR a2, int a3, float a4, float a5)
{
	TextRenderer::g_MojiDraw->Vertices[3].DiffuseColor = a2;
	TextRenderer::g_MojiDraw->Vertices[2].DiffuseColor = a2;
	TextRenderer::g_MojiDraw->Vertices[1].DiffuseColor = a2;
	TextRenderer::g_MojiDraw->Vertices[0].DiffuseColor = a2;

	if (a1 < 0x2115) {
		this->moji_print2(a1, &TextPos, a4, a5);
		return;
	}

	if (a1 <= 0x2125) {
		if (Text::MojiDraw::g_pFontUsGaiji) {
			CTk::_49SubList* v7 = (*Text::MojiDraw::g_pFontUsGaiji)[a1 - 0x2115];
			if (v7)
				v7->Draw(TextPos.x, TextPos.y, 0x80808080, 0, 0);
		}
	}

	TextRenderer::PrepareDeviceForText2();
	this->SetMojiTexture();
}

void FFXI::Text::MojiDraw::moji_print2(int a2, tagPOINT* a3, float a5, float a6)
{
	double v7 = (double)a3->x;
	this->Vertices[2].X = v7;
	this->Vertices[0].X = v7;

	double v8 = a5 * 16.0 + (double)a3->x - 0.5;
	this->Vertices[3].X = v8;
	this->Vertices[1].X = v8;

	double v9 = (double)a3->y - 0.5;
	this->Vertices[1].Y = v9;
	this->Vertices[0].Y = v9;

	double v10 = a6 * 16.0 + (double)a3->y - 0.5;
	this->Vertices[3].Y = v10;
	this->Vertices[2].Y = v10;

	double v11 = (double)(a2 % 64);
	double v22 = (double)(a2 / 64);

	this->Vertices[2].TexVertX = 0.015625 * v11;
	this->Vertices[0].TexVertX = 0.015625 * v11;

	double v14 = (v11 + 1.0) * 0.015625;

	this->Vertices[3].TexVertX = v14;
	this->Vertices[1].TexVertX = v14;

	double v15 = v22 * 0.0078125;
	this->Vertices[1].TexVertY = v15;
	this->Vertices[0].TexVertY = v15;

	double v16 = (v22 + 1.0) * 0.0078125;
	this->Vertices[3].TexVertY = v16;
	this->Vertices[2].TexVertY = v16;

	D3DCOLOR backups[4];
	for (int i = 0; i < 4; ++i) {
		backups[i] = this->Vertices[i].DiffuseColor;
		this->Vertices[i].DiffuseColor = Globals::ColorMult(backups[i], 0x80808080);
	}

	FFXI::CYy::CDx::instance->DXDevice->SetVertexShader(this->Vertices->FVF);
	FFXI::CYy::CDx::instance->DrawVerts(D3DPT_TRIANGLESTRIP, 2, this->Vertices, sizeof(this->Vertices[0]));

	for (int i = 0; i < 4; ++i) {
		this->Vertices[i].DiffuseColor = backups[i];
	}
}

void FFXI::Text::MojiDraw::SetMojiTexture()
{
	IDirect3DBaseTexture8* tex{ nullptr };
	if (!this->BasicMojiMap) {
		this->BasicMojiMap = CYyDb::g_pCYyDb->pCYyTexMng->GetByUnique(Constants::Strings::FontMoji);
		if (!this->BasicMojiMap)
			return;
	}

	if (this->TryUseHQTex == 1 && !this->HQTex)
		this->InitHQTex();
	if (this->UseHQTex()) {
		tex = this->HQTex->field_8->RegTex;
		if (!tex)
			tex = this->HQTex->field_8->CubeTex;
	}
	else {
		tex = this->BasicMojiMap->RegTex;
		if (!tex)
			tex = this->BasicMojiMap->CubeTex;
	}

	FFXI::CYy::CDx::instance->DXDevice->SetTexture(0, tex);
}

void FFXI::Text::MojiDraw::SetIndent(int a1, int* a2)
{
	if ((a1 & 0x8000u) == 0) {
		if (a1 < 0x2115 || a1 > 0x2125) {
			if (a1 >= 0x440u)
				*a2 += 14;
			else
				*a2 += TextRenderer::mojiwidth_ALL[a1];
		}
		else {
			CTk::_49SubList* v2 = (*Text::MojiDraw::g_pFontUsGaiji)[a1 - 0x2115];
			*a2 += v2->field_22 - v2->field_20;
		}
	}
}

void FFXI::Text::MojiDraw::InitHQTex()
{
	if (Config::MainRegistryConfig::FontQuality != Constants::Enums::FontQuality::FONT_COMPRESSED) {
		if (!FFXI::CYy::CDx::instance->field_931) {

			int fontMapIndex = Config::MainRegistryConfig::FontQuality - 1;
			if (FFXI::CYy::CDx::instance->field_934 < 0x800u)
				fontMapIndex = 0;

			if (FFXI::CYy::CDx::instance->field_938 < 0x1000u)
				fontMapIndex = 0;

			FontQualityEntry* entry = FontQualityMap + fontMapIndex;
			if (FFXI::File::XIFileManager::g_pXIFileManager->GetFileSizeByNumfile(entry->FileID) >= 0) {
				InitHQMapData(&this->HQMapData, entry->ImageHeight, entry->ImageWidth);
				if (this->HQMapData) {
					this->HQTex = new CYy::CYyIcon(-1, (unsigned char*)this->HQMapData);
				}

				if (this->HQTex) {
					if (this->HQTex->field_8) {
						this->HQTex->field_8->Init();
						if (this->HQTex->field_8->LoadFromNumFile(entry->FileID))
							return;
					}
				}
			}
		}
	}

	this->~MojiDraw();
}

bool FFXI::Text::MojiDraw::UseHQTex()
{
	if (this->TryUseHQTex != 1)
		return false;

	if (!this->HQTex)
		return false;

	if (!this->HQTex->field_8)
		return false;

	return true;
}
