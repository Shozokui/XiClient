#include "XiArea.h"
#include "XiWeather.h"
#include "CYyDb.h"
#include "Globals.h"
#include "XiZone.h"
#include "CDx.h"
#include "CYyCamMng2.h"
#include "YmCombineWeather.h"
#include "CYyResourceFile.h"
#include "RuntimeConfig.h"
using namespace FFXI::CYy;
constexpr float DIV255 = (1.0f / 255.0f);

FFXI::CYy::XiArea::XiArea()
{
	this->Init();
}

FFXI::CYy::XiArea::XiArea(CYyResourceFile* a2, int a3)
{
	this->Init();
	this->field_8 = a3;
	
	this->field_14 = a2;
	if (a3 != 0) {
		CYyResourceFile** res{ nullptr };
		a2->AnotherResourceSearcher((CMoResource***) &res, FFXI::Constants::Enums::ResourceType::Rmp, a3);
		if (res != nullptr && *res != nullptr) {
			this->field_14 = *res;
		}
	}

	XiArea* head = XiArea::GetHead();
	while (head != nullptr) {
		if (head->field_4 == this) {
			if (head->CombineWeather != nullptr) {
				XiDateTime v15(0), v14(0);
				int fourcc = head->CombineWeather->fourCC;
				this->SetWeather(fourcc, &v14, v15.Time);
			}
			break;
		}
		head = head->field_4;
	}

	if (this->Weather == nullptr) {
		this->SetCurrentWeather(&XiWeather::default_weather);
	}

	if (a3 == 'lbus') {
		XiZone::zone->field_1C4 = this;
	}
}

FFXI::CYy::XiArea::~XiArea()
{
	if (XiZone::zone->field_1C4 == this)
		XiZone::zone->field_1C4 = nullptr;
	
	this->DeleteCombineWeather();

	if (XiZone::zone == nullptr) 
		return;

	XiArea* area = XiArea::GetHead();
	if (area == this) return;

	area = this->GetPreviousInList();
	if (area)
		area->field_4 = this->field_4;
}

void FFXI::CYy::XiArea::Init()
{
	this->field_4 = nullptr;
	this->field_20 = nullptr;
	this->CombineWeather = nullptr;
	this->field_14 = nullptr;
	this->field_8 = 0;
	this->Weather = nullptr;
	this->field_C = 0;

	this->Link();
	this->field_124 = {-1.0, 0.0, 0.0};
	this->field_130 = 1.0;
	this->field_134 = 0;
	this->field_138 = 4;
	this->field_13C = 10;
	this->field_140 = 0;
	this->field_144 = 0.0;
	this->field_148 = 0;

	this->field_24 = { 0.0, 1.0, 0.0 };

	this->field_30 = 1.0;
	this->field_34 = 0.0;
	this->field_38 = 0.0;
	this->field_3C = 0.0;
	this->field_40 = 1.0;
	this->field_44 = 0.0;
	this->field_48 = 1.0;
	this->field_4C = 0.0;
	this->field_50 = 1.0;
	this->field_54 = 0.0;
	this->field_58 = 0.0;
	this->field_5C = 0.0;
	this->field_60 = 1.0;

	this->field_64 = { 0.0, 0.0, 0.0, 1.0 };
	this->field_74 = { 0.0, 0.0, 0.0, 1.0 };
	this->field_84 = { 0.0, 0.0, 0.0, 1.0 };
	this->field_94 = { 0.0, 0.0, 0.0, 1.0 };

	this->DiffuseLight1Dir = { 0.0, 1.0, 0.0 };
	this->field_B0 = 1.0;
	this->field_B4 = { 0.0, 0.0, 0.0 };
	this->field_C0 = 1.0;
	this->DiffuseLight2Dir = { 0.0, 1.0, 0.0 };
	this->field_D0 = 1.0;
	this->field_D4 = { 0.0, 0.0, 0.0 };
	this->field_E0 = 1.0;
	this->DiffuseLight1 = { 0.0, 0.0, 0.0, 1.0 };
	this->DiffuseLight2 = { 0.0, 0.0, 0.0, 1.0 };
	
	this->field_104 = { 0.0, 0.0, 0.0, 1.0 };
	this->FogColor = { 0.0, 0.0, 0.0, 1.0 };
}

void FFXI::CYy::XiArea::SetCurrentWeather(XiWeather* a2)
{
	if (!a2)
		this->Weather = &XiWeather::default_weather;
	else
		this->Weather = a2;
	this->WorldEnvironment = &this->Weather->world;
	SetFarColor(
		(double)this->Weather->env2.field_0[12]* 255.0,
		(double)this->Weather->env2.field_0[13]* 255.0,
		(double)this->Weather->env2.field_0[14]* 255.0
	);

	CYyDb::SetField300(this->Weather->env2.field_10);
	CYyDb::SetField2FC(this->Weather->env2.field_14);

	CYyDb::SetField314(this->GetWorld8());
	CYyDb::SetField318(this->GetWorld4());
	this->ColorCorrect(this->Weather);
}

void FFXI::CYy::XiArea::ColorCorrect(XiWeather* a2)
{
	XiZone* zone = XiZone::zone;
	if (!zone || this != zone->field_1C4 || zone->ZoneID != FFXI::Constants::Enums::Zone::Walk_of_Echoes && zone->ZoneID != FFXI::Constants::Enums::Zone::Abyssea_La_Theine) {
		this->field_64.r = (double)a2->env1.field_0[0] * DIV255;
		this->field_64.g = (double)a2->env1.field_0[1] * DIV255;
		this->field_64.b = (double)a2->env1.field_0[2] * DIV255;
		this->field_64.a = 1.0;
	}
	this->field_84.r = (double)a2->env1.field_0[8] * DIV255;
	this->field_84.g = (double)a2->env1.field_0[9] * DIV255;
	this->field_84.b = (double)a2->env1.field_0[10] * DIV255;
	this->field_84.a = 1.0;

	this->field_94.r = (double)a2->env1.field_0[12] * DIV255;
	this->field_94.g = (double)a2->env1.field_0[13] * DIV255;
	this->field_94.b = (double)a2->env1.field_0[14] * DIV255;
	this->field_94.a = 1.0;

	this->DiffuseLight1.r = (double)a2->env2.field_0[0] * DIV255;
	this->DiffuseLight1.g = (double)a2->env2.field_0[1] * DIV255;
	this->DiffuseLight1.b = (double)a2->env2.field_0[2] * DIV255;
	this->DiffuseLight1.a = 1.0;

	this->field_104.r = (double)a2->env2.field_0[8] * DIV255;
	this->field_104.g = (double)a2->env2.field_0[9] * DIV255;
	this->field_104.b = (double)a2->env2.field_0[10] * DIV255;
	this->field_104.a = 1.0;

	this->FogColor.r = (double)a2->env2.field_0[12] * DIV255;
	this->FogColor.g = (double)a2->env2.field_0[13] * DIV255;
	this->FogColor.b = (double)a2->env2.field_0[14] * DIV255;
	this->FogColor.a = 1.0;


	if ((a2->field_0 & 1) == 0) {
		this->field_74.r = (double)a2->env1.field_0[4] * DIV255;
		this->field_74.g = (double)a2->env1.field_0[5] * DIV255;
		this->field_74.b = (double)a2->env1.field_0[6] * DIV255;
		this->field_74.a = 1.0;

		this->DiffuseLight2.r = (double)a2->env2.field_0[4] * DIV255;
		this->DiffuseLight2.g = (double)a2->env2.field_0[5] * DIV255;
		this->DiffuseLight2.b = (double)a2->env2.field_0[6] * DIV255;
		this->DiffuseLight2.a = 1.0;
	}
	else {
		this->DiffuseLight1Dir.x = (float)(char)a2->env2.field_0[4];
		this->DiffuseLight1Dir.y = (float)(char)a2->env2.field_0[5];
		this->DiffuseLight1Dir.z = (float)(char)a2->env2.field_0[6];
		this->field_B0 = 1.0;
		Globals::Vec3Normalize(&this->DiffuseLight1Dir);

		this->field_24.x = (char)a2->env1.field_0[4];
		this->field_24.y = (char)a2->env1.field_0[5];
		this->field_24.z = (char)a2->env1.field_0[6];
		this->field_30 = 1.0;

		Globals::Vec3Normalize(&this->field_24);
		this->field_74 = { 0.0, 0.0, 0.0, 1.0 };
		this->DiffuseLight2 = { 0.0, 0.0, 0.0, 1.0 };
		if (this == XiZone::zone) {
			exit(0x88999888);
		}
		this->field_14C.x = this->DiffuseLight1Dir.x;
		this->field_14C.y = this->DiffuseLight1Dir.y;
		this->field_14C.z = this->DiffuseLight1Dir.z;
		this->field_14C.w = this->field_B0;

		this->field_16C = this->DiffuseLight1;
		this->field_17C.x = this->field_24.x;
		this->field_17C.y = this->field_24.y;
		this->field_17C.z = this->field_24.z;
		this->field_17C.w = this->field_30;

		this->field_19C = this->field_64;
	}
}

void FFXI::CYy::XiArea::Link()
{
	if (!XiZone::zone) return;

	if (XiZone::GetHead() != this) {
		XiZone::GetTail()->field_4 = this;
		this->field_4 = nullptr;
	}
}

void FFXI::CYy::XiArea::GetTwoWeatherLights(D3DLIGHT8* a2, D3DLIGHT8* a3)
{
	double DiffuseLightScale = this->GetWeatherDiffuseLightScale();
	
	a2->Type = D3DLIGHT_DIRECTIONAL;
	a2->Diffuse.r = this->DiffuseLight1.r * DiffuseLightScale;
	a2->Diffuse.g = this->DiffuseLight1.g * DiffuseLightScale;
	a2->Diffuse.b = this->DiffuseLight1.b * DiffuseLightScale;
	a2->Diffuse.a = 0.0;
	a2->Ambient.r = 0.0;
	a2->Ambient.g = 0.0;
	a2->Ambient.b = 0.0;
	a2->Ambient.a = 0.0;
	a2->Specular.r = 0.0;
	a2->Specular.g = 0.0;
	a2->Specular.b = 0.0;
	a2->Specular.a = 0.0;
	a2->Direction = this->DiffuseLight1Dir;
	a2->Phi = 1.0;
	a2->Theta = 1.0;
	a2->Attenuation2 = 1.0;
	a2->Attenuation1 = 1.0;
	a2->Attenuation0 = 1.0;
	a2->Falloff = 1.0;
	a2->Range = 1.0;

	a3->Type = D3DLIGHT_DIRECTIONAL;
	a3->Diffuse.r = this->DiffuseLight2.r * DiffuseLightScale;
	a3->Diffuse.g = this->DiffuseLight2.g * DiffuseLightScale;
	a3->Diffuse.b = this->DiffuseLight2.b * DiffuseLightScale;
	a3->Diffuse.a = 0.0;
	a3->Ambient.r = 0.0;
	a3->Ambient.g = 0.0;
	a3->Ambient.b = 0.0;
	a3->Ambient.a = 0.0;
	a3->Specular.r = 0.0;
	a3->Specular.g = 0.0;
	a3->Specular.b = 0.0;
	a3->Specular.a = 0.0;
	a3->Direction = this->DiffuseLight2Dir;
	a3->Phi = 1.0;
	a3->Theta = 1.0;
	a3->Attenuation2 = 1.0;
	a3->Attenuation1 = 1.0;
	a3->Attenuation0 = 1.0;
	a3->Falloff = 1.0;
	a3->Range = 1.0;
}

void FFXI::CYy::XiArea::GetTwoWeatherLightsByFourCC(D3DLIGHT8* a2, D3DLIGHT8* a3, int a4)
{
	XiArea* area = this;
	if (a4 != 0) {
		area = XiArea::GetByFourCC(a4);
		if (area == nullptr) {
			area = this;
		}
	}
	area->GetTwoWeatherLights(a2, a3);
}

void FFXI::CYy::XiArea::GetAreaLightByFourCC(D3DLIGHT8* a2, int a3, int a4)
{
	if (!a4)
		this->GetLight(a2, a3);
	else {
		XiArea* area = XiArea::GetByFourCC(a4);
		if (area == nullptr) {
			this->GetLight(a2, a3);
		}
		else {
			area->GetLight(a2, a3);
		}
	}
}

void FFXI::CYy::XiArea::GetLight(D3DLIGHT8* a2, int a3)
{
	a2->Position.x = 0.0;
	a2->Position.y = 0.0;
	a2->Position.z = 0.0;
	a2->Range = 0.0;
	a2->Falloff = 0.0;
	a2->Attenuation0 = 0.0;
	a2->Attenuation1 = 0.0;
	a2->Attenuation2 = 0.0;
	a2->Theta = 0.0;
	a2->Phi = 0.0;

	D3DXVECTOR4 v17{};
	D3DCOLORVALUE v15{ 0.0, 0.0, 0.0, 0.0 }; //not sure if this is correct
	D3DCOLORVALUE v16{};
	MemoryHeader* header = (MemoryHeader*)((char*)this - 0x20);
	if (header->occupied) {
		if (a3 == 0) {
			v17 = this->field_17C;
			double v13 = this->Weather->env1.field_18;
			Globals::ThreeFloatScale(&v16.r, &this->field_19C.r, v13);
		}
		else if (a3 == 1) {
			v17 = this->field_14C;
			double weatherScale = this->GetWeatherDiffuseLightScale();
			Globals::ThreeFloatScale(&v16.r, &this->field_16C.r, weatherScale);
		}

		a2->Diffuse.r = v16.r;
		a2->Diffuse.g = v16.g;
		a2->Diffuse.b = v16.b;
		a2->Diffuse.a = 1.0;

		a2->Ambient.r = v15.r;
		a2->Ambient.g = v15.g;
		a2->Ambient.b = v15.b;
		a2->Ambient.a = v15.a;

		a2->Direction.x = v17.x;
		a2->Direction.y = v17.y;
		a2->Direction.z = v17.z;
	}
	else {
		a2->Direction.x = 0.0;
		a2->Diffuse.r = 255.0;
		a2->Diffuse.g = 255.0;
		a2->Diffuse.b = 255.0;
		a2->Diffuse.a = 255.0;
		a2->Ambient.r = 255.0;
		a2->Ambient.g = 255.0;
		a2->Ambient.b = 255.0;
		a2->Ambient.a = 255.0;
		a2->Direction.y = 1.0;
		a2->Direction.z = 0.0;
	}

	a2->Specular.r = 0.0;
	a2->Specular.g = 0.0;
	a2->Specular.b = 0.0;
	a2->Specular.a = 0.0;
	a2->Type = D3DLIGHT_DIRECTIONAL;
}

void FFXI::CYy::XiArea::GetAmbientByFourCC(DWORD* a2, int a3, int a4)
{
	if (!a4)
		this->GetAmbient(a2, a3);
	else {
		XiArea* area = XiArea::GetByFourCC(a4);
		if (area == nullptr) {
			this->GetAmbient(a2, a3);
		}
		else {
			area->GetAmbient(a2, a3);
		}
	}

}

void FFXI::CYy::XiArea::GetAmbient(DWORD* a2, int a3)
{
	MemoryHeader* header = (MemoryHeader*)((char*)this - 0x20);
	if (!header->occupied) {
		*a2 = -1;
		return;
	}

	*a2 = 0;
	//huh
	if ((int)this != -36) {
		D3DCOLORVALUE* v7{ nullptr };
		if (a3 == 0) {
			v7 = &this->field_104;
		}
		else if (a3 == 1) {
			v7 = &this->field_84;
		}
		if (v7) {
			if (FFXI::Config::RuntimeConfig::instance.window_mode != FFXI::Config::RuntimeConfig::WindowMode::Fullscreen) {
				if (v7->r < 0.8f && v7->g < 0.8f && v7->b < 0.8f) {
					float v23{}, v24{}, v25{};
					FFXI::CYy::CDx::instance->Gamma.GetLightMultipliers(&v23, &v24, &v25);
					v7->r *= v23;
					v7->g *= v24;
					v7->b *= v25;
					if (v7->r > 1.0)
						v7->r = 1.0;
					if (v7->g > 1.0)
						v7->g = 1.0;
					if (v7->b > 1.0)
						v7->b = 1.0;
				}
			}

			DWORD result = 0;
			int red = v7->r * 128.0;
			int gre = v7->g * 128.0;
			int blu = v7->b * 128.0;
			result = (unsigned char)red;
			result <<= 8;
			result |= (unsigned char)gre;
			result <<= 8;
			result |= (unsigned char)blu;
			*a2 = result;
		}
	}
}

void FFXI::CYy::XiArea::GetFogByFourCC(DWORD* a2, float* a3, float* a4, int a5, int a6)
{
	if (a6 == 0) {
		this->GetFog(a2, a3, a4, a5);
	}
	else {
		XiArea* area = XiArea::GetByFourCC(a6);
		if (area == nullptr) {
			this->GetFog(a2, a3, a4, a5);
		}
		else {
			area->GetFog(a2, a3, a4, a5);
		}
	}
}

void FFXI::CYy::XiArea::GetFog(DWORD* a2, float* a3, float* a4, int a5)
{
	MemoryHeader* header = (MemoryHeader*)((char*)this - 0x20);
	if (!header->occupied) {
		*a2 = 0xFFFFFFFF;
		*a3 = 400.0;
		*a4 = 0.0;
		return;
	}

	*a2 = 0x808080;
	*a3 = 400.0;
	*a4 = 300.0;

	if (this->Weather) {
		D3DCOLORVALUE fogColor = this->FogColor;
		int red = fogColor.r * 255.0;
		int gre = fogColor.g * 255.0;
		int blu = fogColor.b * 255.0;
		DWORD result = (unsigned char)red;
		result <<= 8;
		result |= (unsigned char)gre;
		result <<= 8;
		result |= (unsigned char)blu;
		*a2 = result;
		if (a5 == 0) {
			float mult = this->GetAnotherSomething(false);
			*a3 = this->Weather->env2.field_10 * mult;
			*a4 = this->Weather->env2.field_14 * mult;
		}
		else if (a5 == 1) {
			float mult = this->GetAnotherSomething(true);
			*a3 = this->Weather->env1.field_10 * mult;
			*a4 = this->Weather->env1.field_14 * mult;
		}

		if (*a3 <= *a4)
			*a3 = *a4 + 1.0;
	}
}

void FFXI::CYy::XiArea::GetSomeLight(D3DCOLORVALUE* a2)
{
	MemoryHeader* header = (MemoryHeader*)((char*)this - 0x20);
	if (!header->occupied) {
		a2->a = 255.0;
		a2->b = 255.0;
		a2->g = 255.0;
		a2->r = 255.0;
		return;
	}

	//huh
	if ((int)this == -36) {
		a2->b = 0.0;
		a2->g = 0.0;
		a2->r = 0.0;
		a2->a = 0.0;
	}
	else {
		D3DCOLORVALUE cv = this->field_104;
		if (FFXI::Config::RuntimeConfig::instance.window_mode != FFXI::Config::RuntimeConfig::WindowMode::Fullscreen) {
			if (cv.r < 0.8f && cv.g < 0.8f && cv.b < 0.8f) {
				float v19{}, v20{}, v21{};
				FFXI::CYy::CDx::instance->Gamma.GetLightMultipliers(&v19, &v20, &v21);
				cv.r *= v19;
				cv.g *= v20;
				cv.b *= v21;
				if (cv.r > 1.0)
					cv.r = 1.0;
				if (cv.g > 1.0)
					cv.g = 1.0;
				if (cv.b > 1.0)
					cv.b = 1.0;
			}
		}
		a2->a = 0.0;
		a2->r = cv.r * 0.5;
		a2->g = cv.g * 0.5;
		a2->b = cv.b * 0.5;
	}
}

void FFXI::CYy::XiArea::DeleteCombineWeather()
{
	if (this->field_4)
		this->field_4->DeleteCombineWeather();

	if (this->CombineWeather) {
		YmCombineWeather* cmb = this->CombineWeather;
		while (cmb) {
			YmCombineWeather* acw = cmb->AreasCombineWeather;
			delete cmb;
			cmb = acw;
		}
	}
	this->CombineWeather = nullptr;
}

void FFXI::CYy::XiArea::UpdateCombineWeather()
{
	if (this->field_4 != nullptr)
		this->field_4->UpdateCombineWeather();

	YmCombineWeather* weather = this->CombineWeather;
	while (weather != nullptr) {
		weather->UpdateWeather();
		weather = weather->AreasCombineWeather;
	}
}

void FFXI::CYy::XiArea::ClearCombines()
{
	if (this->field_4)
		this->field_4->ClearCombines();

	this->DeleteCombineWeather();
	this->field_148 = 0;
}

void FFXI::CYy::XiArea::OnMove()
{
	if (this->field_4)
		this->field_4->OnMove();

	this->MakeLightPos();

	if (this->CombineWeather)
		this->CombineWeather->OnMoveAll();

	if (this->CombineWeather) {
		this->SetCurrentWeather(this->CombineWeather->field_20);
		this->field_20 = this->CombineWeather->PrevSound;
	}
	this->WindProc();
}

void FFXI::CYy::XiArea::MakeLightPos()
{
	if (this->CombineWeather || (this->Weather->field_0 & 1) != 0)
		this->SetSunMoonTime(XiDateTime::current_time.Time);
	else {
		this->field_14C.x = this->DiffuseLight1Dir.x;
		this->field_14C.y = this->DiffuseLight1Dir.y;
		this->field_14C.z = this->DiffuseLight1Dir.z;
		this->field_14C.w = this->field_B0;

		this->field_15C.x = this->field_B4.x;
		this->field_15C.y = this->field_B4.y;
		this->field_15C.z = this->field_B4.z;
		this->field_15C.w = this->field_C0;

		this->field_16C = this->DiffuseLight1;

		this->field_17C.x = this->field_24.x;
		this->field_17C.y = this->field_24.y;
		this->field_17C.z = this->field_24.z;
		this->field_17C.w = this->field_30;

		this->field_18C = this->field_34;
		this->field_190 = this->field_38;
		this->field_194 = this->field_3C;
		this->field_198 = this->field_40;

		this->field_19C = this->field_64;
	}
}

void FFXI::CYy::XiArea::WindProc()
{
	this->field_144 -= CYyDb::g_pCYyDb->CheckTick();
	if (this->field_144 < 0.0) {
		bool v4 = this->field_140 == 1;
		this->field_140 ^= 1;
		this->field_144 = (float)(rand() % 60);
		if (!v4)
			this->field_144 *= 1.2;
	}
	if (this->field_140)
		this->field_134 = (double)(this->field_138 + rand() % this->field_13C) * 0.0025f;
	else {
		this->field_134 -= 0.01f;
		if (this->field_134 < 0.0)
			this->field_134 = 0.0;
	}
}

void FFXI::CYy::XiArea::SetSunMoonTime(int a2)
{
	CYyCamMng2* cammng = CYyDb::g_pCYyDb->CameraManager;
	int v4 = XiZone::zone->field_1C8.Time;
	if (v4 < 0)
		v4 = a2;

	double v154 = (double)v4 * 0.00000019290124 * FFXI::Constants::Values::ANGLE_2PI;
	if ((this->Weather->field_0 & 1) != 0) {
		this->field_14C.x = this->DiffuseLight1Dir.x;
		this->field_14C.y = this->DiffuseLight1Dir.y;
		this->field_14C.z = this->DiffuseLight1Dir.z;
		this->field_14C.w = this->field_B0;

		this->field_15C.x = this->field_B4.x;
		this->field_15C.y = this->field_B4.y;
		this->field_15C.z = this->field_B4.z;
		this->field_15C.w = this->field_C0;

		this->field_16C = this->DiffuseLight1;

		this->field_17C.x = this->field_24.x;
		this->field_17C.y = this->field_24.y;
		this->field_17C.z = this->field_24.z;
		this->field_17C.w = this->field_30;

		this->field_18C = this->field_34;
		this->field_190 = this->field_38;
		this->field_194 = this->field_3C;
		this->field_198 = this->field_40;

		this->field_19C = this->field_64;
	}
	else {
		FFXI::Math::WMatrix v171{};
		if (XiZone::zone->field_1CC) {
			v171.Identity();
			float xangle = (double)XiZone::zone->field_1CC * 0.017452778;
			v171.RotateX(xangle);
		}

		this->DiffuseLight2Dir.x = sin(v154);
		this->DiffuseLight2Dir.y = cos(v154);
		this->DiffuseLight2Dir.z = 0.0;
		this->field_D0 = 1.0;

		if (XiZone::zone->field_1CC)
			v171.Vec3TransformDrop4Self(&this->DiffuseLight2Dir);

		this->field_D4.x = cammng->Position.x - this->DiffuseLight2Dir.x * 900.0;
		this->field_D4.y = cammng->Position.y - this->DiffuseLight2Dir.y * 900.0;
		this->field_D4.z = cammng->Position.z - this->DiffuseLight2Dir.z * 900.0;
		this->field_E0 = this->field_D0;

		this->field_44 = this->DiffuseLight2Dir.x;
		this->field_48 = this->DiffuseLight2Dir.y;
		this->field_4C = this->DiffuseLight2Dir.z;
		this->field_50 = this->field_D0;

		this->field_54 = this->field_D4.x;
		this->field_58 = this->field_D4.y;
		this->field_5C = this->field_D4.z;
		this->field_60 = this->field_D0;

		long double v37 = v154 + FFXI::Constants::Values::ANGLE_PI;
		if (v37 > FFXI::Constants::Values::ANGLE_PI)
			v37 -= FFXI::Constants::Values::ANGLE_2PI;
		if (v37 < FFXI::Constants::Values::ANGLE_MINUS_PI)
			v37 += FFXI::Constants::Values::ANGLE_2PI;

		this->DiffuseLight1Dir.x = sin(v37);
		this->DiffuseLight1Dir.y = cos(v37);
		this->DiffuseLight1Dir.z = 0.0;
		this->field_B0 = 1.0;

		if (XiZone::zone->field_1CC)
			v171.Vec3TransformDrop4Self(&this->DiffuseLight1Dir);

		this->field_B4.x = cammng->Position.x - this->DiffuseLight1Dir.x * 1000.0;
		this->field_B4.y = cammng->Position.y - this->DiffuseLight1Dir.y * 1000.0;
		this->field_B4.z = cammng->Position.z - this->DiffuseLight1Dir.z * 1000.0;
		this->field_C0 = this->field_B0;

		this->field_34 = this->field_B4.x;
		this->field_38 = this->field_B4.y;
		this->field_3C = this->field_B4.z;
		this->field_40 = this->field_B0;

		this->field_24 = this->DiffuseLight1Dir;
		this->field_30 = this->field_B0;

		if (FFXI::GlobalStruct::g_GlobalStruct.field_0 == 0x60) {
			throw "not implemented";
			if (this->DiffuseLight1Dir.y < 0.0) {

			}
			else {

			}
		}
		else {
			if (this->DiffuseLight1Dir.y < 0.0) {
				this->field_14C.x = this->DiffuseLight2Dir.x;
				this->field_14C.y = this->DiffuseLight2Dir.y;
				this->field_14C.z = this->DiffuseLight2Dir.z;
				this->field_14C.w = this->field_D0;

				this->field_15C.x = this->field_D4.x;
				this->field_15C.y = this->field_D4.y;
				this->field_15C.z = this->field_D4.z;
				this->field_15C.w = this->field_E0;

				this->field_16C = this->DiffuseLight2;

				this->field_17C.x = this->field_44;
				this->field_17C.y = this->field_48;
				this->field_17C.z = this->field_4C;
				this->field_17C.w = this->field_50;

				this->field_18C = this->field_54;
				this->field_190 = this->field_58;
				this->field_194 = this->field_5C;
				this->field_198 = this->field_60;

				this->field_19C = this->field_74;
			}
			else {
				this->field_14C.x = this->DiffuseLight1Dir.x;
				this->field_14C.y = this->DiffuseLight1Dir.y;
				this->field_14C.z = this->DiffuseLight1Dir.z;
				this->field_14C.w = this->field_B0;

				this->field_15C.x = this->field_B4.x;
				this->field_15C.y = this->field_B4.y;
				this->field_15C.z = this->field_B4.z;
				this->field_15C.w = this->field_C0;;

				this->field_16C = this->DiffuseLight1;

				this->field_17C.x = this->field_24.x;
				this->field_17C.y = this->field_24.y;
				this->field_17C.z = this->field_24.z;
				this->field_17C.w = this->field_30;

				this->field_18C = this->field_34;
				this->field_190 = this->field_38;
				this->field_194 = this->field_3C;
				this->field_198 = this->field_40;

				this->field_19C = this->field_64;
			}
			
		}
	}
}

float FFXI::CYy::XiArea::GetWorld4()
{
	return this->WorldEnvironment->field_4;
}

float FFXI::CYy::XiArea::GetWorld8()
{
	return this->WorldEnvironment->field_8;
}

float FFXI::CYy::XiArea::GetWorldC()
{
	return this->WorldEnvironment->field_C;
}

double FFXI::CYy::XiArea::GetWeatherDiffuseLightScale()
{
	return this->Weather->env2.field_18;
}

float FFXI::CYy::XiArea::GetAnotherSomething(bool a2)
{
	MemoryHeader* header = (MemoryHeader*)((char*)this - 0x20);
	if (!header->occupied)
		return 1.0;
	float result = 1.0;
	if (this->WorldEnvironment) {
		float v3 = this->GetWorldC();
		if (v3 > 500.0) {
			float v8 = a2 ? Config::MainRegistryConfig::flt10445894 : Config::MainRegistryConfig::flt10445884;
			if (v8 != 1.0)
				result = 0.5;
		}
		if (a2)
			result *= Config::MainRegistryConfig::flt104458D0;
		else
			result *= Config::MainRegistryConfig::flt104458E0;
	}
	if (!XiZone::zone)
		return result;
	
	float v10 = XiZone::zone->field_1D4;
	if (v10 <= 0.0) {
		if (v10 < 0.0) {
			if (result > -v10)
				return -v10;
		}
	}
	else if (result < v10)
		return XiZone::zone->field_1D4;

	return result;
}

int FFXI::CYy::XiArea::GetCurrentWeatherID()
{
	if (this->CombineWeather)
		return this->CombineWeather->fourCC;

	return 0;
}

XiArea* FFXI::CYy::XiArea::GetPreviousInList()
{

	XiArea* result { nullptr };
	XiArea* head = XiArea::GetHead();
	if (!head) return nullptr;

	while (head != this) {
		result = head;
		head = head->field_4;
		if (!head)
			return nullptr;
	}
	return result;
}

XiArea* FFXI::CYy::XiArea::GetHead()
{
	if (XiZone::zone)
		return XiZone::zone;
	return nullptr;
}

XiArea* FFXI::CYy::XiArea::GetTail()
{
	XiArea* head = GetHead();
	if (!head)
		return nullptr;

	XiArea* v2{ nullptr };
	do {
		v2 = head;
		head = head->field_4;
	} while (head);
	return v2;
}

XiArea* FFXI::CYy::XiArea::GetByFourCC(int a1)
{
	XiArea* result = XiArea::GetHead();
	if (!result)
		return XiZone::zone;

	while (result->field_8 != a1) {
		result = result->field_4;
		if (!result)
			return XiZone::zone;
	}

	return result;
}

void FFXI::CYy::XiArea::SetFarColor(char a1, char a2, unsigned char a3)
{
	CYyDb::g_pCYyDb->SetFarColor(a1, a2, a3);
}

void FFXI::CYy::XiArea::SetWeather(int a2, XiDateTime* a3, int a4)
{
	if (this->field_14 == nullptr) {
		//There is no weather because there is no area resource
		return;
	}

	if (this->field_4 != nullptr)
		this->field_4->SetWeather(a2, a3, a4);

	if (a4 == 0 && this->GetCurrentWeatherID() != a2) {
		if (this->CombineWeather) {
			YmCombineWeather* cmb = this->CombineWeather->AreasCombineWeather;
			while (cmb) {
				delete cmb;
				cmb = this->CombineWeather->AreasCombineWeather;
			}
		}
		if (this->CombineWeather) {
			delete this->CombineWeather;
		}
		this->CombineWeather = nullptr;
	}

	if (this->GetCurrentWeatherID() == a2)
		return;

	char* mem = StorageLists::instance->Get(sizeof(YmCombineWeather), FFXI::Constants::Enums::MEM_MODE::Ex);
	if (mem)
		new (mem) YmCombineWeather(this, a2, a4);
}
