#include "FFXIRegistry.h"
#include "Globals.h"
#include <stdio.h>
#include <iostream>
using namespace FFXI;
 
const char* AppRegistryKeys[] = {
    "SOFTWARE\\PlayOnline\\SQUARE\\",
    "SOFTWARE\\PlayOnlineUS\\SquareEnix\\",
    "SOFTWARE\\PlayOnlineEU\\SquareEnix\\",
    "SOFTWARE\\PlayOnlineEU\\SquareEnix\\"
};
char FFXIRegistry::Init() {
    LSTATUS v2;
    char result;
    HKEY phkResult;
    DWORD dwDisposition;
    CHAR SubKey[256];

    sprintf_s(SubKey, "%s%s", AppRegistryKeys[(int)(FFXI::GlobalStruct::g_GlobalStruct.phkResult)], "FinalFantasyXI");
    phkResult = 0;
    v2 = RegCreateKeyExA(HKEY_LOCAL_MACHINE, SubKey, 0, 0, 0, 0xF003Fu, 0, &phkResult, &dwDisposition);
    if (v2)
        return 0;
    if (this->HKey)
    {
        v2 = RegCloseKey(this->HKey);
        this->HKey = 0;
    }
    this->HKey = phkResult;
    if (v2)
        return 0;
    result = 1;
    this->Status = 1;
    return result;
}

FFXI::FFXIRegistry::FFXIRegistry()
{
    this->Init();
}

bool FFXI::FFXIRegistry::QueryKeyString(LPBYTE lpData, LPCSTR lpValueName, LPDWORD lpcbData)
{
    HKEY HKey;
    DWORD Type;

    Type = (DWORD)this;
    HKey = this->HKey;
    Type = 0;
    return RegQueryValueExA(HKey, lpValueName, 0, &Type, lpData, lpcbData) == ERROR_SUCCESS;
}

bool FFXI::FFXIRegistry::SetKeyString(LPCSTR lpString, LPCSTR lpValueName)
{
    int v4;

    v4 = lstrlenA(lpString);
    return RegSetValueExA(this->HKey, lpValueName, 0, 1u, (const BYTE*)lpString, v4 + 1) == ERROR_SUCCESS;
}

bool FFXI::FFXIRegistry::QueryKeyDWORD(LPBYTE lpData, LPCSTR lpValueName)
{
    HKEY HKey;
    DWORD cbData;
    DWORD Type;

    HKey = this->HKey;
    Type = 0;
    cbData = 4;
    return RegQueryValueExA(HKey, lpValueName, 0, &Type, lpData, &cbData) == ERROR_SUCCESS;
}

bool FFXI::FFXIRegistry::SetKeyDWORD(int Data, LPCSTR lpValueName)
{
    return RegSetValueExA(this->HKey, lpValueName, 0, 4u, (const BYTE*)&Data, 4u) == ERROR_SUCCESS;
}
