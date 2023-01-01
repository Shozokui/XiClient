#pragma once
#define WIN32_LEAN_AND_MEAN
#include "NT_SYS.h"
#include "GlobalStruct.h"
#include "PolVars.h"
#include "Enums.h"
#include "MemoryHeader.h"
#include "d3d8to9/d3d8.hpp"
#include "WMatrix.h"
#include "RegistryConfig.h"
#include <inttypes.h>
#include <mmeapi.h>
#include <string>
#include <vector>

namespace FFXI {
	class StorageLists;
	namespace CYy {
		class CYyCamMng2; class CApp; class CDx; class CXITimerLow; class CEnv;
		class CMoTaskMng;
	}
}
class Globals {
public:
	static std::vector<std::string> g_lpSubKey;

	static HHOOK g_hhk;
	static FFXI::CYy::CEnv* g_pCenv;
	static char g_Str[0x104];
	static char g_CursorList[18][32];
	static D3DPOOL g_VertexBufferD3DPool;
	static FFXI::Math::WMatrix g_D3DTransformMatrix;
	static FFXI::Math::WMatrix g_TransformBackup;

	static IDirect3DVertexBuffer8* g_pVertexBuffers[4];
	static int g_pVertexBuffersInitialized;

	static IDirect3DTexture8* g_pSomeTexture1;
	static IDirect3DTexture8* g_pSomeTexture2;
	static IDirect3DTexture8* g_pSomeTexture3;
	static IDirect3DTexture8* g_pSomeTexture4;
	static IDirect3DTexture8* g_pSomeTexture5;
	static IDirect3DTexture8* g_pSomeTexture6;
	static IDirect3DTexture8* g_pSomeTexture7;
	static IDirect3DTexture8* g_pSomeTexture8;
	static IDirect3DTexture8* g_pSomeTexture9;
	static IDirect3DTexture8* g_pSomeTexture10;
	static IDirect3DTexture8* g_pSomeTexture11;
	static IDirect3DTexture8* g_pSomeTexture12;
	
	static const unsigned int* LanguageDependentDatTable;
	static char NullString;

	static FFXI::CYy::CYyCamMng2* g_pCYyCamMng2;
	static short g_some_actordraw_short;
	static float g_some_actordraw_float;

	static FFXI::Math::WMatrix emulate_matrix;
	//Methods


	static void SetSomeManagerFloats(int);
	static int GetLanguageDependentDatIndex(FFXI::Constants::Enums::LanguageDependentDats);
	static void CopyAdvance(void*, void**, int);
	static bool CheckFloat(float);
	static D3DCOLOR ColorMult(D3DCOLOR, D3DCOLOR);
	static void ThreeFloatScale(float*, float*, float);
	static void CopyStrMax16(char*, const char*);
	static void MixOneByteFractional(unsigned char*, unsigned char, unsigned char, float);
	static void MixByteFractionals(unsigned char*, unsigned char*, unsigned char*, float);
	static void MixFloats(float*, float, float, float);
	static double Vec3Inner(const D3DXVECTOR3*, const D3DXVECTOR3*);
	static double Vec3Inner(const D3DXVECTOR4*, const D3DXVECTOR4*);
	static void Vec3Outer(D3DXVECTOR3*, const D3DXVECTOR3*, const D3DXVECTOR3*);
	static void Vec3Normalize(D3DXVECTOR3*);
	static void Vec3Lerp(D3DXVECTOR3*, D3DXVECTOR3*, D3DXVECTOR3*, float);
	static void PiClamp(D3DXVECTOR4*);
	static int GetStrideFromFVF(unsigned int);
	static void CopyLight(D3DLIGHT8*, D3DLIGHT8*);
	template <typename T>
	inline static void __fastcall Set(char* a1, int a2, T a3) {*(T*)(a1 + a2) = a3;}
	template <typename T>
	inline static T __fastcall Get(char* a1, int a2) { return *(T*)(a1 + a2); }
};