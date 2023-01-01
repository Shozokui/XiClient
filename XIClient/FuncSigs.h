#pragma once
#include "Globals.h"

namespace FFXI {
	class FuncSigs {
	public:
		static void Init();
		static void Destroy();
		static HMODULE MainDLL;
		static HRESULT(__cdecl* WavObjInit)(DWORD**);
		static HRESULT(__cdecl* WavCheckSystem)(DWORD*, DWORD, DWORD);
		static void(__cdecl* Type0Helper)(unsigned __int8 a1, unsigned __int8 a2, char* a3, char* a4, int* a5, unsigned int a6);
	};
}