#include "SomeTaskType.h"
#include "Globals.h"
#include "CYyDb.h"
#include "CMoDx.h"
#include "CMoOcclusionMng.h"
#include "CMoDxStatusMng.h"
#include "SomeCMoObj.h"
#include "SomeVertexThing.h"
#include "SVTSStorage.h"
#include "CMoOT.h"
#include "CAcc.h"

using namespace FFXI::CYy;

char SomeTaskType::OnMove() {
	if (this->field_34 == 0)
		this->field_34 = 1;
	else if (this->field_34 == 1) {
		CYyDb::g_pCYyDb->g_pCMoDx->CheckInput();
		CMoDx::g_pSomeCMoObj->Update();
		CYyDb::g_pCYyDb->g_pCMoDx->DoSomething(0, 0, 0, 0);
		return 0;
	}
	return 0;
}

FFXI::CYy::SomeTaskType::~SomeTaskType()
{
	CMoDx* cmodx = CYyDb::g_pCYyDb->g_pCMoDx;

	if (cmodx->OT != nullptr) {
		delete cmodx->OT;
		cmodx->OT = nullptr;
	}

	if (cmodx->g_pCMoOcclusionMng != nullptr) {
		delete cmodx->g_pCMoOcclusionMng;
		cmodx->g_pCMoOcclusionMng = nullptr;
	}

	if (cmodx->g_pCMoDxStatusMng != nullptr) {
		delete cmodx->g_pCMoDxStatusMng;
		cmodx->g_pCMoDxStatusMng = nullptr;
	}

	if (CMoDx::svt1 != nullptr) {
		delete CMoDx::svt1;
		CMoDx::svt1 = nullptr;
	}

	if (CMoDx::svt2 != nullptr) {
		delete CMoDx::svt2;
		CMoDx::svt2 = nullptr;
	}

	if (CMoDx::svt3 != nullptr) {
		delete CMoDx::svt3;
		CMoDx::svt3 = nullptr;
	}

	if (CMoDx::svt4 != nullptr) {
		delete CMoDx::svt4;
		CMoDx::svt4 = nullptr;
	}

	if (CMoDx::svt5 != nullptr) {
		delete CMoDx::svt5;
		CMoDx::svt5 = nullptr;
	}

	if (CMoDx::elem_cacc != nullptr) {
		delete CMoDx::elem_cacc;
		CMoDx::elem_cacc = nullptr;
	}

	if (CMoDx::g_pSomeCMoObj != nullptr) {
		StorageLists::instance->Delete(CMoDx::g_pSomeCMoObj);
		CMoDx::g_pSomeCMoObj = nullptr;

	}

	if (CMoDx::svtsstorage != nullptr) {
		CMoDx::svtsstorage->~SVTSStorage();
		StorageLists::instance->Delete(CMoDx::svtsstorage);
		CMoDx::svtsstorage = nullptr;
	}

	if (CYyDb::g_pCYyDb->g_pCMoDx != nullptr) {
		delete CYyDb::g_pCYyDb->g_pCMoDx;
		CYyDb::g_pCYyDb->g_pCMoDx = nullptr;
	}

	if (CMoDx::th_cacc != nullptr) {
		delete CMoDx::th_cacc;
	}
}
