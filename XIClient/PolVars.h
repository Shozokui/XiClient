#pragma once
#include <wtypes.h>
namespace FFXI {
	class PolVars {
	public:
		static PolVars instance;
		static void ParseCLA();
		char RegionCode{ 1 };
		HKEY phkResult{ (HKEY)RegionCode };
		char UseImeUi{ 0 };
		int dword_10445854{ 0 };
		HINSTANCE hmod{ nullptr };
		char FFXIDirectory[260];
		char DateString[12] = "Aug  3 2021";
		char TimeString[9] = "19:18:09";
		int g_polErrorCode;
	};
}