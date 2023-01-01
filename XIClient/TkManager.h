#pragma once
#include "CTkMenuMng.h"
#include "CTkMsbDataList.h"

namespace FFXI {
	namespace CTk {
		class CTkMouse; class CTkInputCtrl;
		class StDancer;
		class TkManager {
		public:
			static void TkInit();
			static void TkEnd();
			static CTkMenuMng g_CTkMenuMng;
			static CTkMsbDataList g_CTkMenuDataList;
			static CTkMouse* g_pCTkMouse;
			static CTkInputCtrl* g_pCTkInputCtrl;
			static StDancer* g_pStDancer;
		};
	}
}