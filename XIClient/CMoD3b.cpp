#include "CMoD3b.h"
#include "CMoD3bElem.h"
#include "CYyDb.h"
#include "CMoDx.h"
#include "CDx.h"
#include "SomeVertexThing.h"
#include "StatusNode.h"
#include "BaseProcessor.h"
#include "FVF152Vertex.h"

DWORD D3bTSS[] = {
	0, D3DTSS_TEXCOORDINDEX,	0,
	0, D3DTSS_COLOROP,			D3DTOP_MODULATE2X,
	0, D3DTSS_COLORARG1,		D3DTA_TEXTURE,
	0, D3DTSS_COLORARG2,		D3DTA_CURRENT,
	0, D3DTSS_ALPHAOP,			D3DTOP_MODULATE2X,
	0, D3DTSS_ALPHAARG1,		D3DTA_TEXTURE,
	0, D3DTSS_ALPHAARG2,		D3DTA_CURRENT,
	0, D3DTSS_MAGFILTER,		D3DTEXF_LINEAR,
	0, D3DTSS_MINFILTER,		D3DTEXF_LINEAR,
	0, D3DTSS_ADDRESSU,			D3DTADDRESS_WRAP,
	0, D3DTSS_ADDRESSV,			D3DTADDRESS_WRAP,
	1, D3DTSS_COLOROP,			D3DTOP_MODULATE2X,
	1, D3DTSS_COLORARG1,		D3DTA_CURRENT,
	1, D3DTSS_COLORARG2,		D3DTA_TFACTOR,
	1, D3DTSS_ALPHAOP,			D3DTOP_MODULATE4X,
	1, D3DTSS_ALPHAARG1,		D3DTA_CURRENT,
	1, D3DTSS_ALPHAARG2,		D3DTA_TFACTOR,
	2, D3DTSS_COLOROP,			D3DTOP_DISABLE,
	0, D3DTSS_FORCE_DWORD,		false
};

void FFXI::CYy::CMoD3b::Open()
{
	this->field_40.Find(this);
}

void FFXI::CYy::CMoD3b::Close()
{
	//nullsub
}

void FFXI::CYy::CMoD3b::Draw(CMoD3bElem* a2, D3DCOLOR* a3, float* a4, float* a5, float* a6, bool a7)
{
	CMoDx* cmodx = CYyDb::g_pCYyDb->g_pCMoDx;
	if ((this->field_30 & 0x800000) != 0) {
		cmodx->field_EBC->SetRenderState(D3DRS_ALPHATESTENABLE, true);
		cmodx->field_EBC->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		cmodx->field_EBC->SetRenderState(D3DRS_ALPHAREF, 0x60);
	}

	if (a7 == false) {
		this->field_40.DoSomething(0, a2->field_140, a2->field_144, a2->field_148);
	}

	if (a6 == nullptr) {
		a6 = this->MakeObj(a4, a5);
	}

	int v9 = this->field_3E;
	if (this->field_3C >= this->field_3E) {
		v9 += 0x10000;
	}

	char* datastart = (char*) & this->field_30;
	unsigned int* a6a = (unsigned int*)(datastart + this->field_3C);
	float* a5a = (float*)(datastart + v9);
	unsigned int offset = (this->field_30 >> 24) & 0xFF;
	unsigned short* v10 = (unsigned short*)(datastart + 0x10000 * offset + this->field_38);
	unsigned short* v11 = v10 + 3 * this->field_3A;

	CMoDx::svt5->UpdateSubValues(3 * this->field_3A);
	FVF152Vertex* buf = (FVF152Vertex*)CMoDx::svt5->LockBuffer();

	int count = 3 * this->field_3A;
	if (a7 == true) {
		for (int i = 0; i < count; ++i) {
			float* fptr = a6 + 9 * v10[i];
			buf[i].X = fptr[0];
			buf[i].Y = fptr[1];
			buf[i].Z = fptr[2];

			buf[i].TexVertX = fptr[7];
			buf[i].TexVertY = fptr[8];

			fptr = a6 + 9 * v11[i] + 3;
			buf[i].N_X = fptr[0];
			buf[i].N_Y = fptr[1];
			buf[i].N_Z = fptr[2];

			buf[i].Diffuse = a6a[i];
		}
	}
	else {
		for (int i = 0; i < count; ++i) {
			float* fptr = a6 + 9 * v10[i];
			buf[i].X = fptr[0];
			buf[i].Y = fptr[1];
			buf[i].Z = fptr[2];

			fptr = a6 + 9 * v11[i] + 3;
			buf[i].N_X = fptr[0];
			buf[i].N_Y = fptr[1];
			buf[i].N_Z = fptr[2];

			unsigned int v22 = 2 * (unsigned char)a6a[i];
			unsigned int v23 = (a6a[i] >> 7) & 0x1FE;
			unsigned int v24 = (a6a[i] >> 15) & 0x1FE;
			unsigned int v25 = (a6a[i] >> 23) & 0x1FE;
			if (v22 > 0xFF)
				v22 = 255;
			if (v23 > 0xFF)
				v23 = 255;
			if (v24 > 0xFF)
				v24 = 255;
			if (v25 > 0xFF)
				v25 = 255;

			buf[i].Diffuse = v22 | ((v23 | ((v24 | (v25 << 8)) << 8)) << 8);
			buf[i].TexVertX = a5a[0];
			buf[i].TexVertY = a5a[1];
			a5a += 2;
		}
	}
	CMoDx::svt5->UnlockBuffer();
	CYyObject::Unwrap(a6);

	static StatusNode D3bState{};
	D3bState.ApplyTSS(D3bTSS);

	cmodx->field_EBC->SetRenderState(D3DRS_TEXTUREFACTOR, *a3);
	if (a2 != nullptr && (a2->field_17B & 3) != 0) {
		FFXI::Math::WMatrix* pop{ nullptr };
		CYyDb::g_pCYyDb->pMoProcessor->PopFromStack(&pop);
		CYyDb::g_pCYyDb->pMoProcessor->MatrixIdentity(pop);
		pop->_31 = a2->field_120;
		pop->_32 = a2->field_124;
		cmodx->field_EBC->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
		cmodx->field_DC8->SetTransform(D3DTS_TEXTURE0, pop);
		CYyDb::g_pCYyDb->pMoProcessor->PushToStack(pop);
	}

	CMoDx::svt5->Render(0, this->field_3A, D3DPT_TRIANGLELIST, 0, -1);
	
	if (a2 != nullptr && (a2->field_17B & 3) != 0) {
		cmodx->field_DC8->SetTransform(D3DTS_TEXTURE0, &CYyDb::g_pCYyDb->pMoProcessor->field_810);
		cmodx->field_EBC->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	}
}

float* FFXI::CYy::CMoD3b::MakeObj(float* a2, float* a3)
{
	unsigned short count = this->field_36;
	if (this->field_34 > count) {
		count = this->field_34;
	}

	float* obj = (float*)CYyObject::Wrap(36 * count, FFXI::Constants::Enums::MEM_MODE::Ex);
	char num = (this->field_30 >> 16) & 0xFF;
	num = (2 * num) >> 1;
	if (num > 5) {
		//this shouldn't happen?
		exit(0xD3AD);
	}
	float total{ 0.0 };
	if (num > 0) {
		for (int i = 0; i < num; ++i) {
			total += a2[i];
		}
	}
	int v6 = 0;
	if (total <= 0.0) {
		int v10 = 1;
		while (v10 < num) {
			if (a3[v6] < a3[v10]) {
				v6 = v10;
			}
			v10 += 1;
		}
		total = 1.0;
		a2[v6] = 1.0;
	}

	total = 1.0 / total;
	float v34[5];
	float* v35[5];
	int* v36[5];
	if (num > 0) {
		int step = this->field_36 + 3 * this->field_34;
		float* pnt1 = (float*)(this + 1);
		int* pnt2 = (int*)(this + 1) + 3 * this->field_34;
		for (int i = 0; i < num; ++i) {
			v34[i] = total * a2[i];
			v35[i] = pnt1;
			v36[i] = pnt2;
			pnt1 += step;
			pnt2 += step;
		}
	}

	if (this->field_34 > 0) {
		float* v20 = obj;
		for (int i = 0; i < this->field_34; ++i) {
			v20[0] = 0.0; 
			v20[1] = 0.0; 
			v20[2] = 0.0;
			if (num > 0) {
				for (int j = 0; j < num; ++j) {
					float* fpnt = v35[j];
					v20[0] += fpnt[0] * v34[j];
					v20[1] += fpnt[1] * v34[j];
					v20[2] += fpnt[2] * v34[j];
					v35[j] += 3;
				}
			}
			v20 += 9;
		}
	}

	if (this->field_36 > 0) {
		float* v25 = obj + 3;
		for (int i = 0; i < this->field_36; ++i) {
			v25[0] = 0.0;
			v25[1] = 0.0;
			v25[2] = 0.0;
			if (num > 0) {
				for (int j = 0; j < num; ++j) {
					int val = (v36[j])[0];
					v25[0] += (double)(val << 22 >> 22) * v34[j] / 512.0;
					v25[1] += (double)(val << 12 >> 22) * v34[j] / 512.0;
					v25[2] += (double)((val * 4) >> 22) * v34[j] / 512.0;
					v36[j] += 1;
				}
			}
			v25 += 9;
		}
	}

	return obj;
}
