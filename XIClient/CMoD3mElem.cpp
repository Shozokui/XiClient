#include "CMoD3mElem.h"
#include "MmbListNode.h"
#include "CYyDb.h"
#include "CDx.h"
#include "CMoDx.h"
#include "CMoD3m.h"
#include "BaseProcessor.h"
#include "CYyGenerator.h"
#include "StatusNode.h"
#include "CTsZoneMap.h"
using namespace FFXI::CYy;

DWORD D3mTSS[] = {
	0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR,
	0, D3DTSS_MINFILTER, D3DTEXF_LINEAR,
	0, D3DTSS_FORCE_DWORD, false
};
double FFXI::CYy::CMoD3mElem::VirtElem6()
{
	float v1{ 0.0 };
	if (this->res != nullptr) {
		if (this->MMBFlag != 0) {
			MmbListNode* mmbs = (MmbListNode*)this->res;
			v1 = mmbs->field_D4;
		}
		else {
			CMoD3m* d3m = (CMoD3m*)*this->res;
			unsigned short* sdata = (unsigned short*)(d3m + 1);
			v1 = (float)(sdata[0] >> 4);
		}
	}
	D3DXVECTOR3 v8 = { v1, 0.0, 0.0 };
	CYyDb::g_pCYyDb->pMoProcessor->VirtProcessor15(&v8, &this->field_60, &v8);
	float v7 = CYyDb::g_pCYyDb->pMoProcessor->Vec3Magnitude(&v8);
	float v9 = CYyDb::g_pCYyDb->pMoProcessor->Vec3Magnitude(&this->field_188);
	return v7 + v9;
}

void FFXI::CYy::CMoD3mElem::OnDraw()
{
	if (this->res == nullptr) {
		return;
	}

	CYyGenerator* gen{ nullptr };
	if (this->field_FC != nullptr) {
		gen = *this->field_FC;
	}

	//gen can be nullptr here as per the client
	if (gen->field_DE >= 0 || CYyGenerator::GetSomeGeneratorScalar() != 0.0) {
		if (this->MMBFlag != 0) {
			this->DoMMBDraw();
		}
		else {
			CMoD3m* d3m = (CMoD3m*)*this->res;
			d3m->Draw(&CMoElem::SomeColor, this);
		}
	}
}

void FFXI::CYy::CMoD3mElem::VirtElem9()
{
	if (this->res == nullptr) {
		return;
	}

	CMoD3m* d3m = (CMoD3m*)*this->res;
	if (d3m == nullptr) {
		return;
	}
	this->SomeMatrixCalc();

	unsigned short* sdata = (unsigned short*)(d3m + 1);
	int count = 3 * sdata[3];
	CYyDb::g_pCYyDb->pMoProcessor->VirtProcessor31(CMoElem::Floats, d3m->GetFloatPointer(), 36, count, &this->field_A0);
}

void FFXI::CYy::CMoD3mElem::DoMMBDraw()
{
	CMoDx* cmodx = CYyDb::g_pCYyDb->g_pCMoDx;
	BaseProcessor* proc = CYyDb::g_pCYyDb->pMoProcessor;

	if ((this->field_17B & 3) != 0) {
		FFXI::Math::WMatrix* pop{ nullptr };
		proc->PopFromStack(&pop);
		proc->MatrixIdentity(pop);
		pop->_31 = this->field_120;
		pop->_32 = this->field_124;
		cmodx->field_EBC->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
		cmodx->field_EBC->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
		cmodx->field_DC8->SetTransform(D3DTS_TEXTURE0, pop);
		proc->PushToStack(pop);
	}
	else {
		cmodx->field_EBC->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
		cmodx->field_EBC->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
		cmodx->field_DC8->SetTransform(D3DTS_TEXTURE0, &proc->field_810);
	}
	static StatusNode D3mState{};
	D3mState.ApplyTSS(D3mTSS);
	cmodx->field_EBC->SetRenderState(D3DRS_TEXTUREFACTOR, CMoElem::SomeColor);
	this->SomeMatrixCalc();
	if ((this->field_16C & 0xFF) == 0x64) {
		CYyDb::g_pCYyDb->g_pTsZoneMap->DoD3mDraw(
			&this->field_A0, (MmbListNode*)this->res,
			1, (this->field_10C & 0x40000) == 0);
	}
	else {
		CYyDb::g_pCYyDb->g_pTsZoneMap->DoD3mDraw(
			&this->field_A0, (MmbListNode*)this->res,
			(this->field_10C >> 28) & 1, (this->field_10C & 0x40000) == 0);
	}

	if ((this->field_17B & 3) != 0) {
		cmodx->field_DC8->SetTransform(D3DTS_TEXTURE0, &proc->field_810);
		cmodx->field_EBC->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	}
}
