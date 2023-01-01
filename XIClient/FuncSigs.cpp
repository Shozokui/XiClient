#include "FuncSigs.h"
#include <TlHelp32.h>
#include <stdio.h>

using namespace FFXI;

HMODULE FuncSigs::MainDLL{ nullptr };
HRESULT(__cdecl* FuncSigs::WavObjInit)(DWORD**) { nullptr };
HRESULT(__cdecl* FuncSigs::WavCheckSystem)(DWORD*, DWORD, DWORD) { nullptr };
void(__cdecl* FuncSigs::Type0Helper)(unsigned __int8 a1, unsigned __int8 a2, char* a3, char* a4, int* a5, unsigned int a6);
const char* WavObjInitPattern = "\x56\x57\x6A\x30\xE8\xDD\x91\x04\x00";
const char* WavCheckSystemPattern = "\x8B\x44\x24\x04\x8B\x4C\x24\x08\x8B\x54\x24\x0C\x50\x89\x48\x08";
const char* WavCheckSystemSubPattern = "\x8B\x4C\x24\x04\x53\x55\x56\x8B\x71\x0C\x57\x8B\x79\x24\x33\xDB";
const char* Type0HelperPattern = "\x8B\x4C\x24\x04\x83\xEC\x08\x83\xF9\x02\x53\x55\x56\x8B\x74\x24";

BYTE* MatchPattern(BYTE* baseAddr, DWORD Size, int matchIndex, const char* Pattern) {
	if (Pattern[0] == 0) return nullptr;

	const BYTE* patternByte = reinterpret_cast<const BYTE*>(Pattern);
	int patternLen = strlen(Pattern);
	
	int matched = 0;
	int matchedindex = 0;
	unsigned int pos = 0;
	while (pos < Size) {
		if (patternByte[matched] == '?' || baseAddr[pos] == patternByte[matched]) {
			++matched;
			if (matched >= patternLen) {
				if (matchedindex == matchIndex) {
					return baseAddr + pos - matched + 1;
				}
				matched = 0;
				++matchedindex;
			}
		}
		else
			matched = 0;
		++pos;
	}
	return nullptr;
}

bool GetBaseAddress(const wchar_t* lib, BYTE** outBase, DWORD* outSize) {
	HANDLE snap{ INVALID_HANDLE_VALUE };
	while (true) {
		snap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE32 | TH32CS_SNAPMODULE, NULL);
		if (snap) {
			if (snap == INVALID_HANDLE_VALUE) {
				if (GetLastError() == ERROR_BAD_LENGTH)
					continue;
				return false;
			}
			break;
		}
		return false;
	}

	MODULEENTRY32 me32{};
	me32.dwSize = sizeof(me32);
	if (!Module32First(snap, &me32)) {
		CloseHandle(snap);
		return false;
	}
	BYTE* baseAddr{ nullptr };
	DWORD baseSize{ 0 };
	do {
		if (!wcscmp(me32.szModule, lib)) {
			baseAddr = me32.modBaseAddr;
			baseSize = me32.modBaseSize;
		}
	} while (Module32Next(snap, &me32));

	if (!baseAddr || !baseSize) {
		CloseHandle(snap);
		return false;
	}

	CloseHandle(snap);

	*outBase = baseAddr;
	*outSize = baseSize;
	return true;
}
void FFXI::FuncSigs::Init()
{
	FuncSigs::MainDLL = LoadLibraryA("FFXIMain.dll");

	BYTE* baseAddr{ nullptr };
	DWORD Size{ 0 };
	if (GetBaseAddress(L"FFXIMain.dll", &baseAddr, &Size)) {

		BYTE* func = MatchPattern(baseAddr, Size, 0, WavObjInitPattern);
		if (func) 
			FuncSigs::WavObjInit = (HRESULT(__cdecl*)(DWORD**))func;
		
		int index = 0;
		BYTE* subfunc = MatchPattern(baseAddr, Size, 0, WavCheckSystemSubPattern);
		if (subfunc) {
			while (!FuncSigs::WavCheckSystem && (func = MatchPattern(baseAddr, Size, index, WavCheckSystemPattern))) {
				int jmpindex = 0;
				BYTE* jump{ nullptr };
				while (jump = MatchPattern(func, 0x10, jmpindex, "\xE8????")) {
					int* jumpaddr = (int*)(jump + 1);
					if (jump + *jumpaddr + 5 == subfunc) {
						FuncSigs::WavCheckSystem = (HRESULT(__cdecl*)(DWORD*, DWORD, DWORD))func;
						break;
					}
					++jmpindex;
				}
				++index;
			}
		}

		func = MatchPattern(baseAddr, Size, 0, Type0HelperPattern);
		if (func)
			FuncSigs::Type0Helper = (void(__cdecl*)(unsigned __int8 a1, unsigned __int8 a2, char* a3, char* a4, int* a5, unsigned int a6))func;
	}

}

void FFXI::FuncSigs::Destroy()
{
	if (FuncSigs::MainDLL)
		FreeLibrary(FuncSigs::MainDLL);
}
