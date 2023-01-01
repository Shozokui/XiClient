#include "TkManager.h"
#include "Globals.h"
#include "CTkMouse.h"
#include "CYyDb.h"
#include "PrimMng.h"
#include "CTkEventMsg.h"
#include "CTkMenuMng.h"
#include "StDancer.h"
#include "CTkInputCtrl.h"
#include "CIwOnePic.h"
#include "CTkHelp.h"
#include "CTkTitle.h"
#include "CIwOkMenu.h"

using namespace FFXI::CTk;

CTkMenuMng TkManager::g_CTkMenuMng{};
CTkMsbDataList TkManager::g_CTkMenuDataList{};
CTkMouse* TkManager::g_pCTkMouse{ nullptr };
CTkInputCtrl* TkManager::g_pCTkInputCtrl{ nullptr };
StDancer* TkManager::g_pStDancer{ nullptr };

void FFXI::CTk::TkManager::TkInit()
{
	//sub
	g_CTkMenuMng.Init();
	g_CTkMenuMng.field_7C = 0;
	g_CTkMenuMng.field_7E = 0;
	g_CTkMenuMng.UIXRes = CYyDb::g_pCYyDb->UIXRes;
	g_CTkMenuMng.UIYRes = CYyDb::g_pCYyDb->UIYRes;

	//sub //TODO
	g_pCTkMouse = new CTkMouse();
	//sub //TODO
	
	g_pCTkInputCtrl = new CTkInputCtrl();
	//sub //TODO
	PrimMng::g_pTkEventMsg = new CTkEventMsg();
	
	PrimMng::g_pIwOnePic = new CIwOnePic();
	TkManager::g_pStDancer = new StDancer();
	TkManager::g_CTkMenuMng.SetMenuPhase(1, 1);
}

void FFXI::CTk::TkManager::TkEnd()
{
	//sub //TODO
	if (PrimMng::g_pTkHelp) {
		delete PrimMng::g_pTkHelp;
		PrimMng::g_pTkHelp = nullptr;
	}

	if (PrimMng::g_pTkTitle) {
		delete PrimMng::g_pTkTitle;
		PrimMng::g_pTkTitle = nullptr;
	}

	if (TkManager::g_pCTkMouse) {
		delete TkManager::g_pCTkMouse;
		TkManager::g_pCTkMouse = nullptr;
	}
	//sub //TODO
	if (TkManager::g_pCTkInputCtrl) {
		delete TkManager::g_pCTkInputCtrl;
		TkManager::g_pCTkInputCtrl = nullptr;
	}
	//sub //TODO
	if (PrimMng::g_pTkEventMsg) {
		delete PrimMng::g_pTkEventMsg;
		PrimMng::g_pTkEventMsg = nullptr;
	}

	if (PrimMng::g_pIwOnePic) {
		delete PrimMng::g_pIwOnePic;
		PrimMng::g_pIwOnePic = nullptr;
	}

	if (TkManager::g_pStDancer) {
		delete TkManager::g_pStDancer;
		TkManager::g_pStDancer = nullptr;
	}

	if (PrimMng::g_pIwOkMenu) {
		delete PrimMng::g_pIwOkMenu;
		PrimMng::g_pIwOkMenu = nullptr;
	}

	TkManager::g_CTkMenuMng.DeleteMenuAll();
}
