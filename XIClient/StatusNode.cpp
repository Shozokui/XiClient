#include "StatusNode.h"
#include "Globals.h"
#include "CYyDb.h"
#include "CMoDx.h"
#include "CMoDxStatusMng.h"
using namespace FFXI::CYy;

FFXI::CYy::StatusNode::StatusNode()
{
	this->token = -1;
	this->field_4 = 0;
	this->field_8 = 0;
}

FFXI::CYy::StatusNode::~StatusNode()
{
	if (this->token != -1) {
		CYyDb::g_pCYyDb->g_pCMoDx->field_EBC->DeleteStateBlock(this->token);
		this->token = -1;
		if (this->field_4)
			this->field_4->field_8 = this->field_8;
		if (this->field_8)
			this->field_8->field_4 = this->field_4;
	}
}

void FFXI::CYy::StatusNode::ApplyRS(DWORD* a2)
{
	CMoDx* cmodx = CYyDb::g_pCYyDb->g_pCMoDx;

	while (this->token == -1) {
		cmodx->field_EBC->BeginStateBlock();
		if (a2 != nullptr) {
			while (a2[0] != D3DRS_FORCE_DWORD) {
				cmodx->field_EBC->SetRenderState((D3DRENDERSTATETYPE)a2[0], a2[1]);
				a2 += 2;
			}
		}
		cmodx->field_EBC->EndStateBlock(&this->token);
		this->Link();
	}
	cmodx->field_EBC->ApplyStateBlock(this->token);
}

void FFXI::CYy::StatusNode::ApplyTSS(DWORD* a2)
{
	CMoDx* cmodx = CYyDb::g_pCYyDb->g_pCMoDx;

	while (this->token == -1) {
		cmodx->field_EBC->BeginStateBlock();
		if (a2 != nullptr) {
			while (a2[1] != D3DTSS_FORCE_DWORD) {
				cmodx->field_EBC->SetTextureStageState(a2[0], (D3DTEXTURESTAGESTATETYPE)a2[1], a2[2]);
				a2 += 3;
			}
		}
		cmodx->field_EBC->EndStateBlock(&this->token);
		this->Link();
	}
	cmodx->field_EBC->ApplyStateBlock(this->token);
}

void FFXI::CYy::StatusNode::ApplyRSandTSS(DWORD* a2, DWORD* a3)
{
	CMoDx* modx = CYyDb::g_pCYyDb->g_pCMoDx;
	while (this->token == -1) {
		modx->field_EBC->BeginStateBlock();
		if (a2) {
			while (a2[0] != D3DRS_FORCE_DWORD) {
				modx->field_EBC->SetRenderState((D3DRENDERSTATETYPE)a2[0], a2[1]);
				a2 += 2;
			}
		}
		if (a3) {
			while (a3[1] != D3DTSS_FORCE_DWORD) {
				modx->field_EBC->SetTextureStageState(a3[0], (D3DTEXTURESTAGESTATETYPE)a3[1], a3[2]);
				a3 += 3;
			}
		}
		modx->field_EBC->EndStateBlock(&this->token);
		this->Link();
	}
	modx->field_EBC->ApplyStateBlock(this->token);
}

void FFXI::CYy::StatusNode::Link()
{
	this->field_8 = CYyDb::g_pCYyDb->g_pCMoDx->g_pCMoDxStatusMng->field_4.field_8;
	CYyDb::g_pCYyDb->g_pCMoDx->g_pCMoDxStatusMng->field_4.field_8 = this;
	if (this->field_8)
		this->field_8->field_4 = this;
}


