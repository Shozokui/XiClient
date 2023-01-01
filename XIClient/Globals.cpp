#pragma once
#include "Globals.h"
#include "SoundMng.h"
#include "FFXIDX.h"
#include "CApp.h"
#include "XIFileManager.h"
#include "CYyDb.h"
#include "CDx.h"
#include "CXITimerLow.h"
#include "TkManager.h"
#include "Strings.h"
#include "DatIndices.h"
#include "CYyCamMng2.h"
#include "NT_SYS.h"
#include <string>
#include <iostream>

#include <float.h>
#include <timeapi.h>
#include <commdlg.h>
#include "WindowManager.h"
#include "RuntimeConfig.h"
using namespace FFXI;
using namespace FFXI::CTk;

const char WindowClass[] = "FFXiClass";
const char WindowName[] = "FINAL FANTASY XI";

std::vector<std::string> Globals::g_lpSubKey = {
	"SOFTWARE\\PlayOnline\\DebugPatch",
	"SOFTWARE\\PlayOnlineUS\\DebugPatch",
	"SOFTWARE\\PlayOnlineEU\\DebugPatch"
};

HHOOK Globals::g_hhk{ nullptr };
CYy::CEnv* Globals::g_pCenv{ nullptr };

char Globals::g_Str[0x104];
D3DPOOL Globals::g_VertexBufferD3DPool{ D3DPOOL_DEFAULT};
FFXI::Math::WMatrix Globals::g_D3DTransformMatrix{};
FFXI::Math::WMatrix Globals::g_TransformBackup{};

IDirect3DVertexBuffer8* Globals::g_pVertexBuffers[4];
int Globals::g_pVertexBuffersInitialized{ 0 };
IDirect3DTexture8* Globals::g_pSomeTexture1{ nullptr };
IDirect3DTexture8* Globals::g_pSomeTexture2{ nullptr };
IDirect3DTexture8* Globals::g_pSomeTexture3{ nullptr };
IDirect3DTexture8* Globals::g_pSomeTexture4{ nullptr };
IDirect3DTexture8* Globals::g_pSomeTexture5{ nullptr };
IDirect3DTexture8* Globals::g_pSomeTexture6{ nullptr };
IDirect3DTexture8* Globals::g_pSomeTexture7{ nullptr };
IDirect3DTexture8* Globals::g_pSomeTexture8{ nullptr };
IDirect3DTexture8* Globals::g_pSomeTexture9{ nullptr };
IDirect3DTexture8* Globals::g_pSomeTexture10{ nullptr };
IDirect3DTexture8* Globals::g_pSomeTexture11{ nullptr };
IDirect3DTexture8* Globals::g_pSomeTexture12{ nullptr };

const unsigned int* Globals::LanguageDependentDatTable{ nullptr };
char Globals::NullString = '\0';

CYy::CYyCamMng2* Globals::g_pCYyCamMng2{ nullptr };
short Globals::g_some_actordraw_short{ 0 };
float Globals::g_some_actordraw_float{ 0.0 };

FFXI::Math::WMatrix Globals::emulate_matrix{};
char Globals::g_CursorList[18][32] = {
	"mousenor.ani",
	"mousehit.ani",
	"oarw_n.ani",
	"oarw_ne.ani",
	"oarw_e.ani",
	"oarw_se.ani",
	"oarw_s.ani",
	"oarw_sw.ani",
	"oarw_w.ani",
	"oarw_nw.ani",
	"arw_n.ani",
	"arw_ne.ani",
	"arw_e.ani",
	"arw_se.ani",
	"arw_s.ani",
	"arw_sw.ani",
	"arw_w.ani",
	"arw_nw.ani"
};

void SwitchDialogLang(int p_region) {
	char check = *(char*)&FFXI::GlobalStruct::g_GlobalStruct.field_18;
	switch (p_region) {
	case Constants::Enums::Japanese:
		Globals::LanguageDependentDatTable = FFXI::Constants::DatIndices::JapaneseDatTable;
		break;
	case Constants::Enums::English:
		Globals::LanguageDependentDatTable = FFXI::Constants::DatIndices::EnglishDatTable;
		break;
	case Constants::Enums::French:
		if (check >= 0)
			Globals::LanguageDependentDatTable = FFXI::Constants::DatIndices::FrenchDatTable;
		else
			Globals::LanguageDependentDatTable = FFXI::Constants::DatIndices::EnglishDatTable;
		break;
	case Constants::Enums::German:
		if (check >= 0)
			Globals::LanguageDependentDatTable = FFXI::Constants::DatIndices::GermanDatTable;
		else
			Globals::LanguageDependentDatTable = FFXI::Constants::DatIndices::EnglishDatTable;
		break;
	}
}

void Globals::SetSomeManagerFloats(int a1)
{
	double v2{ 0 };
	if (!FFXI::CYyDb::g_pCYyDb) return;
	if (a1)
		v2 = 0.75;
	else
		v2 = 1.0;
	FFXI::CYyDb::g_pCYyDb->field_2E4 = 1.0 / v2;
	FFXI::CYyDb::g_pCYyDb->field_2F4 = 350.0;
	FFXI::CYyDb::g_pCYyDb->field_2F8 = 350.0;
	FFXI::CYyDb::g_pCYyDb->field_2D8 = 16777215.0;
	FFXI::CYyDb::g_pCYyDb->field_2D4 = 0.1;
	FFXI::CYyDb::g_pCYyDb->field_2DC = 0.1;
	FFXI::CYyDb::g_pCYyDb->field_2E0 = 65535.0;
	FFXI::CYyDb::g_pCYyDb->SomeCalc1();
	FFXI::CYyDb::g_pCYyDb->field_194 = FFXI::CYyDb::g_pCYyDb->field_154;

}

int Globals::GetLanguageDependentDatIndex(FFXI::Constants::Enums::LanguageDependentDats code)
{
	int RegionCode = FFXI::GlobalStruct::g_GlobalStruct.GetRegionCode();

	SwitchDialogLang(RegionCode);
	return Globals::LanguageDependentDatTable[(int)code];
}

void Globals::CopyAdvance(void* dst, void** src, int length)
{
	char** data = (char**)(src);
	memcpy(dst, *data, length);
	*data += length;
}

bool Globals::CheckFloat(float a1)
{
	int v1 = _fpclass(a1);
	if (v1 == _FPCLASS_PINF) return false;
	if (v1 == _FPCLASS_PD) return false;
	if (v1 == _FPCLASS_ND) return false;
	if (v1 == _FPCLASS_SNAN) return false;
	if (v1 == _FPCLASS_QNAN) return false;
	if (v1 == _FPCLASS_NINF) return false;

	return true;
}

void Globals::CopyStrMax16(char* a1, const char* a2)
{
	int v3 = strlen(a2);
	if (v3 > 16)
		v3 = 16;

	if (v3 <= 0) {
		memset(a1, 0, 16);
		return;
	}

	memcpy(a1, a2, v3);

	if (v3 < 16)
		memset(a1 + v3, 0, 16 - v3);
}

void Globals::ThreeFloatScale(float* a1, float* a2, float a3)
{
	a1[0] = a2[0] * a3;
	a1[1] = a2[1] * a3;
	a1[2] = a2[2] * a3;
}

D3DCOLOR Globals::ColorMult(D3DCOLOR col1, D3DCOLOR col2)
{
	unsigned char b31, b21, b11, b01;
	unsigned char b32, b22, b12, b02;

	b31 = (col1 >> 24) & 0xFF;
	b32 = (col2 >> 24) & 0xFF;

	b21 = (col1 >> 16) & 0xFF;
	b22 = (col2 >> 16) & 0xFF;

	b11 = (col1 >> 8) & 0xFF;
	b12 = (col2 >> 8) & 0xFF;

	b01 = col1 & 0xFF;
	b02 = col2 & 0xFF;

	unsigned int v2, v3, v4, v5;
	v2 = b31 * b32;
	v2 >>= 7;

	if (v2 > 0xFF)
		v2 = 0xFF;

	v3 = b21 * b22;
	v3 >>= 7;

	if (v3 > 0xFF)
		v3 = 0xFF;

	v4 = b11 * b12;
	v4 >>= 7;

	if (v4 > 0xFF)
		v4 = 0xFF;

	v5 = b01 * b02;
	v5 >>= 7;

	if (v5 > 0xFF)
		v5 = 0xFF;

	return (unsigned char)v5 | (((unsigned char)v4 | (((v2 << 8) | (unsigned char)v3) << 8)) << 8);
}

void Globals::Vec3Normalize(D3DXVECTOR3* a1)
{
	long double v1 = sqrt(a1->x * a1->x + a1->y * a1->y + a1->z * a1->z);
	long double v2{ 9999999.0 };
	if (v1 != 0.0)
		v2 = 1.0 / v1;
	a1->x *= v2;
	a1->y *= v2;
	a1->z *= v2;
}

void Globals::Vec3Lerp(D3DXVECTOR3* a1, D3DXVECTOR3* a2, D3DXVECTOR3* a3, float a4)
{
	double v4 = 1.0 - a4;
	a1->x = v4 * a3->x + a4 * a2->x;
	a1->y = v4 * a3->y + a4 * a2->y;
	a1->z = v4 * a3->z + a4 * a2->z;
}

void Globals::PiClamp(D3DXVECTOR4* a1) {
	if (a1->x > FFXI::Constants::Values::ANGLE_PI)
		a1->x -= FFXI::Constants::Values::ANGLE_2PI;
	else if (a1->x < FFXI::Constants::Values::ANGLE_MINUS_PI)
		a1->x += FFXI::Constants::Values::ANGLE_2PI;

	if (a1->y > FFXI::Constants::Values::ANGLE_PI)
		a1->y -= FFXI::Constants::Values::ANGLE_2PI;
	else if (a1->y < FFXI::Constants::Values::ANGLE_MINUS_PI)
		a1->y += FFXI::Constants::Values::ANGLE_2PI;

	if (a1->z > FFXI::Constants::Values::ANGLE_PI)
		a1->z -= FFXI::Constants::Values::ANGLE_2PI;
	else if (a1->z < FFXI::Constants::Values::ANGLE_MINUS_PI)
		a1->z += FFXI::Constants::Values::ANGLE_2PI;
}

int Globals::GetStrideFromFVF(unsigned int a1)
{
	int size{ 0 };
	if ((a1 & D3DFVF_XYZ) != 0)
		size = 3;
	if ((a1 & D3DFVF_XYZRHW) != 0)
		size = 4;
	if ((a1 & D3DFVF_NORMAL) != 0)
		size += 3;
	if ((a1 & D3DFVF_DIFFUSE) != 0)
		size += 1;
	if ((a1 & D3DFVF_SPECULAR) != 0)
		size += 1;
	if ((a1 & D3DFVF_TEX1) != 0)
	{
		switch (a1 & 0x30000) {
		case D3DFVF_TEXCOORDSIZE2(0):
			size += 2;
			break;
		case D3DFVF_TEXCOORDSIZE3(0):
			size += 3;
			break;
		case D3DFVF_TEXCOORDSIZE4(0):
			size += 4;
			break;
		case D3DFVF_TEXCOORDSIZE1(0):
			size += 1;
			break;
		}
	}

	return 4 * size;
}

void Globals::CopyLight(D3DLIGHT8* a1, D3DLIGHT8* a2)
{
	if (a2 == nullptr) {
		a1->Type = (D3DLIGHTTYPE)NULL;
	}
	else {
		if (a2->Type == D3DLIGHT_POINT) {
			a1->Type = D3DLIGHT_POINT;
			a1->Diffuse = a2->Diffuse;
			a1->Position = a2->Position;
			a1->Range = a2->Range;
			a1->Attenuation2 = a2->Attenuation2;
		}
		else if (a2->Type == D3DLIGHT_DIRECTIONAL) {
			a1->Type = D3DLIGHT_DIRECTIONAL;
			a1->Diffuse = a2->Diffuse;
			a1->Direction = a2->Direction;
		}
		else {
			a1->Type = (D3DLIGHTTYPE)NULL;
		}
	}
}

double Globals::Vec3Inner(const D3DXVECTOR3* a1, const D3DXVECTOR3* a2)
{
	return a1->z * a2->z + a1->y * a2->y + a1->x * a2->x;
}

double Globals::Vec3Inner(const D3DXVECTOR4* a1, const D3DXVECTOR4* a2)
{
	return a1->z * a2->z + a1->y * a2->y + a1->x * a2->x;
}

void Globals::Vec3Outer(D3DXVECTOR3* a1, const D3DXVECTOR3* a2, const D3DXVECTOR3* a3)
{
	D3DXVECTOR3 v1{};
	v1.x = a3->z * a2->y - a2->z * a3->y;
	v1.y = a2->z * a3->x - a3->z * a2->x;
	v1.z = a2->x * a3->y - a3->x * a2->y;
	*a1 = v1;
}


void Globals::MixOneByteFractional(unsigned char* a1, unsigned char a2, unsigned char a3, float a4) 
{
	*a1 = ((double)(a2 - a3) * a4) + a3;
}
void Globals::MixByteFractionals(unsigned char* a1, unsigned char* a2, unsigned char* a3, float a4)
{
	a1[0] = a3[0] + (long long)((double)(a2[0] - a3[0]) * a4);
	a1[1] = a3[1] + (long long)((double)(a2[1] - a3[1]) * a4);
	a1[2] = a3[2] + (long long)((double)(a2[2] - a3[2]) * a4);
	a1[3] = a3[3] + (long long)((double)(a2[3] - a3[3]) * a4);
}

void Globals::MixFloats(float* a1, float a2, float a3, float a4)
{
	*a1 = (a2 - a3) * a4 + a3;
}