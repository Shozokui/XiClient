#pragma once
#include <wtypes.h>
namespace FFXI {
	struct FFXIRegistry {
		char Init();
	public:
		FFXIRegistry();
		bool QueryKeyString(LPBYTE lpData, LPCSTR lpValueName, LPDWORD lpcbData);
		bool SetKeyString(LPCSTR lpString, LPCSTR lpValueName);
		bool QueryKeyDWORD(LPBYTE lpData, LPCSTR lpValueName);
		bool SetKeyDWORD(int Data, LPCSTR lpValueName);
		HKEY HKey{ 0 };
		int Status{ 0 };
	};
}