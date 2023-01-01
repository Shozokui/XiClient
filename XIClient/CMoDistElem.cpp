#include "CMoDistElem.h"
#include "CYyDb.h"
#include "BaseProcessor.h"
#include "CMoDx.h"
#include "Globals.h"
#include "CDx.h"
#include "SomeVertexThing.h"
#include "StatusNode.h"

using namespace FFXI::CYy;

DWORD DistStateRS[] = {
	D3DRS_ALPHABLENDENABLE, false,
	D3DRS_ALPHATESTENABLE,	false,
	D3DRS_ZENABLE,			false,
	D3DRS_ZBIAS,			0,
	D3DRS_TEXTUREFACTOR,	0x80808080,
	D3DRS_FOGENABLE,		false,
	D3DRS_FORCE_DWORD,		false
};

DWORD DistStateTSS[] = {
	0,  D3DTSS_TEXCOORDINDEX,			0,
	0,  D3DTSS_TEXTURETRANSFORMFLAGS,	D3DTTFF_DISABLE,
	0,  D3DTSS_ADDRESSU,				D3DTADDRESS_MIRROR,
	0,  D3DTSS_ADDRESSV,				D3DTADDRESS_MIRROR,
	0,  D3DTSS_MAGFILTER,				D3DTEXF_LINEAR,
	0,  D3DTSS_MINFILTER,				D3DTEXF_LINEAR,
	0,  D3DTSS_MIPFILTER,				D3DTEXF_NONE,
	0,  D3DTSS_COLOROP,					D3DTOP_SELECTARG1,
	0,  D3DTSS_COLORARG1,				D3DTA_TEXTURE,
	0,  D3DTSS_ALPHAOP,					D3DTOP_SELECTARG2,
	0,  D3DTSS_ALPHAARG2,				D3DTA_TFACTOR,
	1,  D3DTSS_COLOROP,					D3DTOP_DISABLE,
	1,  D3DTSS_ALPHAOP,					D3DTOP_DISABLE,
	0,  D3DTSS_FORCE_DWORD,				false
};

DWORD DistStateRS2[] = {
	D3DRS_ALPHABLENDENABLE,	true,
	D3DRS_ZENABLE,			true,
	D3DRS_LIGHTING,			false,
	D3DRS_SPECULARENABLE,	false,
	D3DRS_FOGENABLE,		false,
	D3DRS_RANGEFOGENABLE,	false,
	D3DRS_SRCBLEND,			D3DBLEND_SRCALPHA,
	D3DRS_DESTBLEND,		D3DBLEND_INVSRCALPHA,
	D3DRS_FORCE_DWORD,		false
};

DWORD DistStateTSS2[] = {
	0, D3DTSS_COLOROP,		D3DTOP_MODULATE2X,
	0, D3DTSS_COLORARG1,	D3DTA_TEXTURE,
	0, D3DTSS_COLORARG2,	D3DTA_CURRENT,
	0, D3DTSS_ALPHAOP,		D3DTOP_MODULATE4X,
	0, D3DTSS_ALPHAARG2,	D3DTA_TFACTOR,
	0, D3DTSS_ALPHAARG1,	D3DTA_DIFFUSE,
	1, D3DTSS_COLOROP,		D3DTOP_DISABLE,
	1, D3DTSS_ALPHAOP,		D3DTOP_DISABLE,
	0, D3DTSS_FORCE_DWORD,	false
};

const CYyClass FFXI::CYy::CMoDistElem::CMoDistElemClass{
	"CMoDistElem", sizeof(CMoDistElem), &CMoElem::CMoElemClass
};

const CYyClass* FFXI::CYy::CMoDistElem::GetRuntimeClass()
{
	return &CMoDistElem::CMoDistElemClass;
}

FFXI::CYy::CMoDistElem::CMoDistElem()
{
	this->field_176 = sizeof(CMoDistElem);
}

bool FFXI::CYy::CMoDistElem::VirtElem1(FFXI::Constants::Enums::ElemType a2)
{
	return a2 == FFXI::Constants::Enums::ElemType::Dist;
}

void FFXI::CYy::CMoDistElem::VirtElem2(float a2)
{
	this->field_194 = a2;
}

void FFXI::CYy::CMoDistElem::OnDraw()
{
	static D3DXVECTOR3 v1[5] = {
		{ 0.0 , 0.0 , 0.0 },
		{ -1.0, 0.0 , 0.0 },
		{ 0.0 , -1.0, 0.0 },
		{ 1.0 , 0.0 , 0.0 },
		{ 0.0 , 1.0 , 0.0 }
	};
	
	this->SomeMatrixCalc();

	CMoDx* cmodx = CYyDb::g_pCYyDb->g_pCMoDx;
	D3DXVECTOR3 v73[5]{};
	CYyDb::g_pCYyDb->pMoProcessor->VirtProcessor27(v1, v73, 5, &this->field_A0, &cmodx->field_B10, cmodx->field_DC0, cmodx->field_DC4);
	D3DVIEWPORT8* vp = CYyDb::g_pCYyDb->g_pCMoDx->field_DC8->GetTopViewport();

	float v64 = 1.0 / vp->Width;
	float v6 = 1.0 / vp->Height;

	float v68[5]{};
	float v69[5]{};
	float v72[5]{};
	float v722[5]{};
	for (int i = 0; i < 5; ++i) {
		v68[i] = v73[i].y;
		v69[i] = v73[i].x;
		v72[i] = v64 * v73[i].x;
		v722[i] = v64 * v73[i].y;
	}

	IDirect3DTexture8* tex = FFXI::CYy::CDx::instance->CAcc2.Texture[0];
	if (tex == nullptr) {
		return;
	}

	float v68max = v68[0];
	float v68min = v68[0];
	float v69max = v69[0];
	float v69min = v69[0];
	for (int i = 0; i < 4; ++i) {
		if (v68min > v68[i + 1]) {
			v68min = v68[i + 1];
		}
		if (v68max < v68[i + 1]) {
			v68max = v68[i + 1];
		}
		if (v69min > v69[i + 1]) {
			v69min = v69[i + 1];
		}
		if (v69max < v69[i + 1]) {
			v69max = v69[i + 1];
		}
	}

	float v69diff = v69max - v69min;
	float v68diff = v68max - v68min;
	double v25 = 1.0;
	if (v69diff >= 256.0) {
		v25 = 255.0 / v69diff;
	}
	double v30 = 1.0;
	if (v68diff >= 256.0) {
		v30 = 255.0 / v68diff;
	}
	for (int i = 0; i < 5; ++i) {
		v69[i] = (v69[i] - v69min) * v25;
		v68[i] = (v68[i] - v68min) * v30;
	}
	cmodx->svt3->UpdateSubValues(4);
	float* v33 = (float*)cmodx->svt3->LockBuffer();

	v33[5] = v72[1];
	v33[6] = v722[1];
	v33[12] = v72[2];
	v33[13] = v722[2];
	v33[19] = v72[4];
	v33[20] = v722[4];
	v33[26] = v72[3];
	v33[27] = v722[3];
	v33[0] = v69[1];
	v33[1] = v68[1];
	v33[7] = v69[2];
	v33[8] = v68[2];
	v33[14] = v69[4];
	v33[15] = v68[4];
	v33[2] = 0.0;
	v33[3] = 1.0;
	v33[9] = 0.0;
	v33[10] = 1.0;
	v33[16] = 0.0;
	v33[17] = 1.0;
	v33[21] = v69[3];
	v33[22] = v68[3];
	v33[23] = 0.0;
	v33[24] = 1.0;
	cmodx->svt3->UnlockBuffer();
	cmodx->field_EBC->SetTexture(0, nullptr);
	short dets[2] = { 0, 0 };
	cmodx->field_DC8->AddViewport(
		cmodx->elem_cacc->SomeTexture,
		cmodx->elem_cacc->Texture2,
		cmodx->elem_cacc->Surface,
		dets, dets, 1, 0);
	cmodx->field_EBC->SetTexture(0, tex);

	static StatusNode DistState{};
	DistState.ApplyRSandTSS(DistStateRS, DistStateTSS);

	cmodx->svt3->Render(0, 2, D3DPT_TRIANGLESTRIP, 0, -1);
	cmodx->field_EBC->SetTexture(0, nullptr);
	cmodx->ClearRewind(0, 0);

	cmodx->svt4->UpdateSubValues(6);
	float* v49 = (float*)cmodx->svt4->LockBuffer();
	unsigned int* v50 = (unsigned int*)(v49 + 9);
	for (int i = 0; i < 5; ++i) {
		v50[0] = 0x808080;
		v50 += 6;
	}
	*((D3DCOLOR*)v49 + 3) = CMoElem::SomeColor;
	v49[30] = -1.0;
	v49[6] = -1.0;
	v49[13] = -1.0;
	v49[32] = 0.0;
	v49[26] = 0.0;
	v49[20] = 0.0;
	v49[14] = 0.0;
	v49[8] = 0.0;
	v49[2] = 0.0;
	v49[0] = 0.0;
	v49[1] = 0.0;
	v49[31] = 0.0;
	v49[7] = 0.0;
	v49[12] = 0.0;
	v49[18] = 1.0;
	v49[19] = 0.0;
	v49[24] = 0.0;
	v49[25] = 1.0;
	float* v52 = v49 + 4;
	for (int i = 0; i < 5; ++i) {
		v52[0] = v69[i] * 0.0039215689;
		v52[1] = v68[i] * 0.0039215689;
		v52 += 6;
	}
	v49[34] = v69[1] * 0.0039215689;
	v49[35] = v68[1] * 0.0039215689;
	cmodx->svt4->UnlockBuffer();

	FFXI::Math::WMatrix* pop{ nullptr };
	CYyDb::g_pCYyDb->pMoProcessor->PopFromStack(&pop);
	this->SomeMatrixCalc();
	*pop = this->field_A0;
	pop->_41 += this->field_194;
	pop->_42 += this->field_194;
	cmodx->SetWorldTransform(pop);
	cmodx->field_EBC->SetTexture(0, cmodx->elem_cacc->SomeTexture);

	static StatusNode DistState2{};
	DistState2.ApplyRSandTSS(DistStateRS2, DistStateTSS2);
	cmodx->svt4->Render(0, 4, D3DPT_TRIANGLEFAN, 0, -1);
	CYyDb::g_pCYyDb->pMoProcessor->PushToStack(pop);
}
