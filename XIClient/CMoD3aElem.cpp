#include "CMoD3aElem.h"
#include "CMoD3a.h"
#include "SomeVertexThing.h"
#include "CMoDx.h"
#include "CYyDb.h"
#include "BaseProcessor.h"
#include "TexHelper.h"
#include "StatusNode.h"
#include <iostream>
#include "CYyGenerator.h"
using namespace FFXI::CYy;

const CYyClass FFXI::CYy::CMoD3aElem::CMoD3aElemClass {
	"CMoD3aElem", sizeof(CMoD3aElem), &CMoElem::CMoElemClass
};
const CYyClass* FFXI::CYy::CMoD3aElem::GetRuntimeClass()
{
	return &CMoD3aElemClass;
}

DWORD TSSOne[] = {
	0,  D3DTSS_TEXCOORDINDEX,			0,
	0,  D3DTSS_TEXTURETRANSFORMFLAGS,	D3DTTFF_DISABLE,
	0,  D3DTSS_COLOROP,					D3DTOP_MODULATE2X,
	0,  D3DTSS_COLORARG1 ,				D3DTA_CURRENT,
	0,  D3DTSS_COLORARG2 ,				D3DTA_TEXTURE,
	0,  D3DTSS_ALPHAOP ,				D3DTOP_SELECTARG1,
	0,  D3DTSS_ALPHAARG1 ,				D3DTA_CURRENT,
	0,  D3DTSS_MAGFILTER ,				D3DTEXF_LINEAR,
	0,  D3DTSS_MINFILTER ,				D3DTEXF_LINEAR,
	1,  D3DTSS_COLOROP ,				D3DTOP_MODULATE2X,
	1,  D3DTSS_COLORARG1 ,				D3DTA_CURRENT,
	1,  D3DTSS_COLORARG2 ,				D3DTA_TFACTOR,
	1,  D3DTSS_ALPHAOP ,				D3DTOP_MODULATE4X,
	1,  D3DTSS_ALPHAARG1 ,				D3DTA_CURRENT,
	1,  D3DTSS_ALPHAARG2 ,				D3DTA_TFACTOR,
	2,  D3DTSS_COLOROP ,				D3DTOP_DISABLE,
	2,  D3DTSS_ALPHAOP ,				D3DTOP_DISABLE,
	0,  D3DTSS_FORCE_DWORD ,			false
};
DWORD TSSTwo[] = {
	0,  D3DTSS_TEXCOORDINDEX,			0,
	0,  D3DTSS_TEXTURETRANSFORMFLAGS,	D3DTTFF_DISABLE,
	0,  D3DTSS_COLOROP,					D3DTOP_MODULATE2X,
	0,  D3DTSS_COLORARG1 ,				D3DTA_CURRENT,
	0,  D3DTSS_COLORARG2 ,				D3DTA_TEXTURE,
	0,  D3DTSS_ALPHAOP ,				D3DTOP_MODULATE2X,
	0,  D3DTSS_ALPHAARG1 ,				D3DTA_CURRENT,
	0,  D3DTSS_ALPHAARG2,				D3DTA_TEXTURE,
	0,  D3DTSS_MAGFILTER ,				D3DTEXF_LINEAR,
	0,  D3DTSS_MINFILTER ,				D3DTEXF_LINEAR,
	1,  D3DTSS_COLOROP ,				D3DTOP_MODULATE2X,
	1,  D3DTSS_COLORARG1 ,				D3DTA_CURRENT,
	1,  D3DTSS_COLORARG2 ,				D3DTA_TFACTOR,
	1,  D3DTSS_ALPHAOP ,				D3DTOP_MODULATE4X,
	1,  D3DTSS_ALPHAARG1 ,				D3DTA_CURRENT,
	1,  D3DTSS_ALPHAARG2 ,				D3DTA_TFACTOR,
	2,  D3DTSS_COLOROP ,				D3DTOP_DISABLE,
	2,  D3DTSS_ALPHAOP ,				D3DTOP_DISABLE,
	0,  D3DTSS_FORCE_DWORD ,			false
};
DWORD TSSThree[] = {
	1, D3DTSS_ALPHAOP,		D3DTOP_SELECTARG2,
	1, D3DTSS_ALPHAARG2,	D3DTA_TFACTOR,
	0, D3DTSS_FORCE_DWORD,	false
};

void FFXI::CYy::CMoD3aElem::PrepD3aTSS()
{
	if ((this->field_10C & 0x10000000) != 0) {
		static StatusNode D3aStateOne{};
		D3aStateOne.ApplyTSS(TSSOne);
	}
	else {
		static StatusNode D3aStateTwo{};
		D3aStateTwo.ApplyTSS(TSSTwo);
	}

	CYyDb::g_pCYyDb->g_pCMoDx->field_EBC->SetRenderState(D3DRS_TEXTUREFACTOR, CMoElem::SomeColor);
	CYyDb::g_pCYyDb->g_pCMoDx->field_EBC->SetRenderState(D3DRS_LIGHTING, false);
	CYyDb::g_pCYyDb->g_pCMoDx->field_EBC->SetRenderState(D3DRS_FOGENABLE, false);
	CYyDb::g_pCYyDb->g_pCMoDx->field_EBC->SetRenderState(D3DRS_RANGEFOGENABLE, false);
	CYyDb::g_pCYyDb->g_pCMoDx->field_EBC->SetRenderState(D3DRS_ZWRITEENABLE, false);

	if ((this->field_16C & 0xFF) == 0x40) {
		static StatusNode D3aStateThree{};
		D3aStateThree.ApplyTSS(TSSThree);
	}
}

void FFXI::CYy::CMoD3aElem::OnDraw()
{
	CYyGenerator* gen = *this->field_FC;

	if (this->res == nullptr) {
		return;
	}

	CMoD3a* d3a = (CMoD3a*)*this->res;
	unsigned char* data = (unsigned char*)(d3a + 1);
	SomeVertexThing* svt = *(SomeVertexThing**)(data + 0x0C);
	if (svt == nullptr) {
		return;
	}

	if (this->field_170 >= *(unsigned short*)(data + 0x02)) {
		this->field_170 = 0;
	}

	TexHelper* th = (TexHelper*)(data + 0x08);
	th->SetTexture(0);
	this->PrepD3aTSS();

	CYyDb::g_pCYyDb->g_pCMoDx->field_EBC->SetRenderState(D3DRS_ZWRITEENABLE, false);
	short* v6 = *(short**)(data + 0x14) + 2 * this->field_170;
	svt->Render(v6[1], 2 * v6[0], D3DPT_TRIANGLELIST, 0, -1);
}

void FFXI::CYy::CMoD3aElem::VirtElem9()
{
	CMoD3a* d3a{ nullptr };
	if (this->res != nullptr) {
		d3a = (CMoD3a*) *this->res;
	}

	unsigned char* data = (unsigned char*)(d3a + 1);
	unsigned short maxcount = *(unsigned short*)(data + 0x02);
	if (this->field_170 >= maxcount) {
		this->field_170 = 0;
	}

	unsigned char* v4 = data + 0x18;
	for (unsigned short i = 0; i < this->field_170; ++i) {
		v4 += 144 * v4[2] + 4;
	}

	this->SomeMatrixCalc();
	CYyDb::g_pCYyDb->pMoProcessor->VirtProcessor31(CMoElem::Floats, (float*)(v4 + 4), 6 * sizeof(float), 6 * v4[2], &this->field_A0);
}
