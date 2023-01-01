#pragma once
#include "PrimTableEntry.h"

namespace FFXI {
	namespace CTk {
		class CTkMenuCtrlData;
		class CTkMenuPrimitive;
		class CTkEventMsg;
		class CIwPatchSub4;
		class CIwPtcBgWin;
		class CIwTopMenu;
		class CIwLobbyMenu;
		class CIwLicenceMenu;
		class CYkWndPartyList;
		class CTkDrawMessageWindow;
		class CTkDrawMessageWindow2;
		class CTkTarget;
		class CTkQueryWindow;
		class YkWndYesno;
		class CTkGMTellWindow;
		class CIwOkMenu;
		class CFsConf6Win;
		class CFsTitleConfigMenu;
		class CIwNetWin;
		class CIwNetWinSub1;
		class CIwOnePic;
		class CIwRaceMenu;
		class CIwFaceMenu;
		class CIwHairMenu;
		class CIwSizeMenu;
		class CIwJobsMenu;
		class CIwSelectMenu;
		class CIwChfWin;
		class CTkHelp; class CTkTitle;
		class PrimMng {
		public:
			static CTkHelp* g_pTkHelp;
			static CTkTitle* g_pTkTitle;
			static CTkEventMsg* g_pTkEventMsg;
			static CIwPtcBgWin* g_pIwPtcBgWin;
			static CIwTopMenu* g_pIwTopMenu;
			static CIwLobbyMenu* g_pIwLobbyMenu;
			static CIwPatchSub4* g_pIwPatchSub4;
			static CIwNetWin* g_pIwNetWin;
			static CIwNetWinSub1* g_pIwNetWinSub1;
			static CFsTitleConfigMenu* g_pFsTitleConfigMenu;
			static CIwOnePic* g_pIwOnePic;
			static CIwLicenceMenu* g_pIwLicenceMenu;
			static CIwRaceMenu* g_pIwRaceMenu;
			static CIwFaceMenu* g_pIwFaceMenu;
			static CIwHairMenu* g_pIwHairMenu;
			static CIwSizeMenu* g_pIwSizeMenu;
			static CIwJobsMenu* g_pIwJobsMenu;
			static CYkWndPartyList* g_pYkWndPartyList;
			static CTkDrawMessageWindow* g_pTkDrawMessageWindow;
			static CTkTarget* g_pTkTarget;
			static CTkQueryWindow* g_pTkQueryWindow;
			static CTkDrawMessageWindow2* g_pTkDrawMessageWindow2;
			static YkWndYesno* g_pYkWndYesno;
			static CTkGMTellWindow* g_pTkGMTellWindow;
			static CIwOkMenu* g_pIwOkMenu;
			static CFsConf6Win* g_pFsConf6Win;
			static CIwSelectMenu* g_pIwSelectMenu;
			static CIwChfWin* g_pIwChfWin;
			static PrimTableEntry PrimTable[];
			static CTkMenuPrimitive* FindPrimMenu(char*, bool*);
			static void MenuPrimitiveLink(CTkMenuCtrlData*, char*);
		};
	}
}