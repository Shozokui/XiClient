#pragma once
#define WIN32_LEAN_AND_MEAN
#include "StorageListObject.h"
#include "d3dx9math.h"
#include "d3d8to9/d3d8types.hpp"
namespace FFXI {
	class XiWeather;
	class XiDateTime;
	class XiWorldEnv;
	namespace CYy {
		class YmCombineWeather;
		class CYyResourceFile;
		class CYySepRes;
		class XiArea : public StorageListObject {
		public:
			XiArea();
			XiArea(CYyResourceFile*, int);
			virtual ~XiArea();
			static XiArea* GetHead();
			static XiArea* GetTail();
			static XiArea* GetByFourCC(int);
			static void SetFarColor(char, char, unsigned char);
			void SetWeather(int, XiDateTime*, int);
			void Init();
			void SetCurrentWeather(XiWeather*);
			void ColorCorrect(XiWeather*);
			void Link();
			void GetTwoWeatherLights(D3DLIGHT8*, D3DLIGHT8*);
			void GetTwoWeatherLightsByFourCC(D3DLIGHT8*, D3DLIGHT8*, int);
			void GetAreaLightByFourCC(D3DLIGHT8*, int, int);
			void GetLight(D3DLIGHT8*, int);
			void GetAmbientByFourCC(DWORD*, int, int);
			void GetAmbient(DWORD*, int);
			void GetFogByFourCC(DWORD*, float*, float*, int, int);
			void GetFog(DWORD*, float*, float*, int);
			void GetSomeLight(D3DCOLORVALUE*);
			void DeleteCombineWeather();
			void UpdateCombineWeather();
			void ClearCombines();
			void OnMove();
			void MakeLightPos();
			void WindProc();
			void SetSunMoonTime(int);
			float GetWorld4();
			float GetWorld8();
			float GetWorldC();
			double GetWeatherDiffuseLightScale();
			float GetAnotherSomething(bool);
			int GetCurrentWeatherID();
			XiArea* GetPreviousInList();
			XiArea* field_4;
			int field_8;
			char field_C;
			char field_D;
			char field_E;
			char field_F;
			YmCombineWeather* CombineWeather;
			CYyResourceFile* field_14;
			XiWeather* Weather;
			XiWorldEnv* WorldEnvironment;
			CYySepRes** field_20;
			D3DXVECTOR3 field_24;
			float field_30;
			float field_34;
			float field_38;
			float field_3C;
			float field_40;
			float field_44;
			float field_48;
			float field_4C;
			float field_50;
			float field_54;
			float field_58;
			float field_5C;
			float field_60;
			D3DCOLORVALUE field_64;
			D3DCOLORVALUE field_74;
			D3DCOLORVALUE field_84;
			D3DCOLORVALUE field_94;
			D3DXVECTOR3 DiffuseLight1Dir;
			float field_B0;
			D3DVECTOR field_B4;
			float field_C0;
			D3DXVECTOR3 DiffuseLight2Dir;
			float field_D0;
			D3DVECTOR field_D4;
			float field_E0;
			D3DCOLORVALUE DiffuseLight1;
			D3DCOLORVALUE DiffuseLight2;
			D3DCOLORVALUE field_104;
			D3DCOLORVALUE FogColor;
			D3DXVECTOR3 field_124;
			int field_130;
			float field_134;
			int field_138;
			int field_13C;
			int field_140;
			float field_144;
			char field_148;
			char field_149;
			char field_14A;
			char field_14B;
			D3DXVECTOR4 field_14C;
			D3DXVECTOR4 field_15C;
			D3DCOLORVALUE field_16C;
			D3DXVECTOR4 field_17C;
			float field_18C;
			float field_190;
			float field_194;
			float field_198;
			D3DCOLORVALUE field_19C;
		};
	}
}