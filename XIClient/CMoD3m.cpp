#include "CMoD3m.h"
#include "d3dx9math.h"
#include "CYyDb.h"
#include "CMoDx.h"
#include "CDx.h"
#include "BaseProcessor.h"
#include "Globals.h"
#include "CTsZoneMap.h"
#include "TexHelper.h"
#include "SomeVertexThing.h"
#include "CMoD3mElem.h"
#include "StatusNode.h"
using namespace FFXI::CYy;

DWORD ZeroOneTSS[] = {
	0, D3DTSS_COLOROP,		5,
	0, D3DTSS_COLORARG1,	1,
	0, D3DTSS_COLORARG2,	3,
	0, D3DTSS_ALPHAOP,		6,
	0, D3DTSS_ALPHAARG1,	0,
	0, D3DTSS_ALPHAARG2,	3,
	1, D3DTSS_COLOROP,		1,
	1, D3DTSS_ALPHAOP,		1,
	0, D3DTSS_FORCE_DWORD,	false
};

DWORD ZeroTwoTSS[] = {
	0, D3DTSS_ALPHAOP,		5,
	0, D3DTSS_FORCE_DWORD,	false
};

DWORD NonZeroOneTSS[] = {
	0, 1, 5,
	0, 2, 1,
	0, 3, 2,
	0, 4, 2,
	0, 5, 0,
	0, 13, 1,
	0, 14, 1,
	1, 1, 5,
	1, 2, 1,
	1, 3, 3,
	1, 4, 6,
	1, 5, 1,
	1, 6, 3,
	1, 16, 2,
	1, 17, 2,
	2, 1, 1,
	2, 4, 1,
	0, D3DTSS_FORCE_DWORD, false
};

DWORD NonZeroTwoTSS[] = {
	0, 1, 5,
	0, 2, 1,
	0, 3, 2,
	0, 4, 5,
	0, 5, 0,
	0, 6, 2,
	0, 13, 1,
	0, 14, 1,
	1, 1, 5,
	1, 2, 1,
	1, 3, 3,
	1, 4, 6,
	1, 5, 1,
	1, 6, 3,
	1, 16, 2,
	1, 17, 2,
	2, 1, 1,
	2, 4, 1,
	0, D3DTSS_FORCE_DWORD, false
};

DWORD NonZeroThreeTSS[] = {
	0, 1, 2,
	0, 2, 1,
	0, 4, 5,
	0, 5, 1,
	0, 6, 2,
	0, 16, 2,
	0, 17, 2,
	1, 1, 3,
	1, 3, 3,
	1, 4, 6,
	1, 5, 1,
	1, 6, 3,
	2, 1, 1,
	2, 4, 1,
	0, D3DTSS_FORCE_DWORD, false
};

DWORD NonZeroFourTSS[] = {
	0, 1, 5,
	0, 2, 1,
	0, 3, 3,
	0, 4, 6,
	0, 5, 1,
	0, 6, 3,
	1, 1, 1,
	1, 4, 1,
	0, D3DTSS_FORCE_DWORD, false
};

void FFXI::CYy::CMoD3m::Open()
{
	unsigned char* data = (unsigned char*)(this + 1);
	unsigned short* sdata = (unsigned short*)data;
	if ((sdata[1] & 0x8080) != 0) {
		return;
	}

	unsigned short v2 = sdata[0];
	if ((v2 & 0xF) == 6 || (v2 & 0xF) == 5) {
		unsigned char count = data[4];
		if (count != 0) {
			unsigned short* data8 = this->GetShortPointer();
			int total{ 0 };
			for (unsigned char i = 0; i < count; ++i) {
				total += data8[i];
			}
			if (total > 3 * sdata[3]) {
				sdata[0] = (v2 & 0xFFF0) | 7;
			}
		}
		else {
			D3DXVECTOR3 v36[3];
			float* fptr = this->GetFloatPointer();
			for (int i = 0; i < 3; ++i) {
				memcpy(v36, fptr + 9 * i, sizeof(v36));
				double v9 = CYyDb::g_pCYyDb->pMoProcessor->Vec3Magnitude(v36 + 1);
				if (v9 > 2.0 || v9 < 0.1
					|| Globals::CheckFloat(v36[0].x) == false
					|| Globals::CheckFloat(v36[0].y) == false
					|| Globals::CheckFloat(v36[0].z) == false
					|| Globals::CheckFloat(v36[2].y) == false
					|| Globals::CheckFloat(v36[2].z) == false) {
					sdata[0] = v2 & 0xFFF0 | 7;
					break;
				}
			}
		}
	}

	long double v15 = 1.0 + this->GetSomeMagnitude();
	if (v15 > 4095.0) {
		v15 = 4095.0;
	}

	sdata[0] = (16 * (__int64)v15) ^ ((unsigned __int8)(16 * (__int64)v15) ^ data[0]) & 0xF;
	
	TexHelper* th = (TexHelper*)this->GetAnotherShortPointer();
	unsigned char count = data[4];

	for (unsigned char i = 0; i < count; ++i) {
		th[i].Find(this);
	}

	float* fptr = this->GetFloatPointer();
	unsigned short scount = sdata[3] * 3;

	if (CYyDb::g_pCYyDb->g_pTsZoneMap != nullptr) {
		if (CYyDb::g_pCYyDb->g_pTsZoneMap->field_54 != 0) {
			unsigned int* v24 = (unsigned int*)(fptr + 6);

			for (unsigned short i = 0; i < scount; ++i) {
				unsigned int v26 = 2 * (unsigned __int8)*v24;
				unsigned int v27 = (*v24 >> 7) & 0x1FE;
				unsigned int v28 = (*v24 >> 15) & 0x1FE;
				unsigned int v29 = (*v24 >> 23) & 0x1FE;
				if (v26 > 0xFF)
					v26 = 255;
				if (v27 > 0xFF)
					v27 = 255;
				if (v28 > 0xFF)
					v28 = 255;
				if (v29 > 0xFF)
					v29 = 255;
				*v24 = v26 | ((v27 | ((v28 | (v29 << 8)) << 8)) << 8);
				v24 += 9;
			}
		}
	}

	SomeVertexThing* svt = new SomeVertexThing();
	if (svt != nullptr) {
		svt->Init(scount, D3DFVF_TEX1 | D3DFVF_DIFFUSE | D3DFVF_NORMAL | D3DFVF_XYZ);
		svt->UpdateSubValues(scount);
		BYTE* v33 = svt->LockBuffer();
		if (v33 != nullptr) {
			memcpy(v33, fptr, 36 * scount);
			*((int*)th + 1) = (int)svt;
		}
		else {
			*((int*)th + 1) = 0;
		}
		svt->UnlockBuffer();
	}

	sdata[1] |= 0x8080u;
}

void FFXI::CYy::CMoD3m::Close()
{
	unsigned char* data = (unsigned char*)(this + 1);
	unsigned short* sdata = (unsigned short*)data;

	if ((sdata[1] & 0x8080) == 0) {
		return;
	}

	sdata[1] &= 0x7F7Fu;
	int** bait = (int**)((int*)this->GetAnotherShortPointer() + 1);
	SomeVertexThing* svt = (SomeVertexThing*)*bait;
	if (svt != nullptr) {
		delete svt;
		*bait = nullptr;
	}
}

unsigned short* FFXI::CYy::CMoD3m::GetShortPointer()
{
	return (unsigned short*)(this + 1) + 4;
}

unsigned short* FFXI::CYy::CMoD3m::GetAnotherShortPointer()
{
	unsigned char* data = (unsigned char*)(this + 1);
	unsigned char v1 = data[0] & 0xF;
	unsigned short* shortpointer = this->GetShortPointer();

	if (v1 == 5 || v1 == 6) {
		int v7 = data[4] + data[5];
		int v8 = v7 & 3;
		if (v8 != 0) {
			return shortpointer + v7 - v8 + 3;
		}
		else {
			return shortpointer + v7;
		}
	}
	if (v1 == 7) {
		if (data[4] == 0) {
			return shortpointer;
		}
		int v3 = data[4] + data[5];
		int v4 = v3 & 3;
		if (v4 != 0) {
			return shortpointer + v3 - v4 + 3;
		}
		else {
			return shortpointer + v3;
		}
	}

	return (unsigned short*)(data + 0x10);
}

float* FFXI::CYy::CMoD3m::GetFloatPointer()
{
	unsigned char* data = (unsigned char*)(this + 1);
	if ((data[0] & 0xF) < 5 || (data[0] & 0xF) > 7) {
		return (float*)(data + 0x50);
	}

	unsigned short* sptr = this->GetAnotherShortPointer();
	return (float*)(sptr + 8 * data[4]);
}

float FFXI::CYy::CMoD3m::GetSomeMagnitude()
{
	unsigned char* data = (unsigned char*)(this + 1);
	unsigned short* sdata = (unsigned short*)data;
	unsigned short count = 3 * sdata[3];
	
	if (count == 0) {
		return 0.0;
	}

	float two_min = 1000000.0;
	float two_max = -1000000.0;
	float one_min = 1000000.0;
	float zero_min = 1000000.0;
	float one_max = -1000000.0;
	float zero_max = -1000000.0;
	float* fptr = this->GetFloatPointer();

	for (unsigned short i = 0; i < count; ++i) {
		unsigned int index = 9 * i;
		if (zero_min > fptr[0 + index]) {
			zero_min = fptr[0 + index];
		}
		if (one_min > fptr[1 + index]) {
			one_min = fptr[1 + index];
		}
		if (two_min > fptr[2 + index]) {
			two_min = fptr[2 + index];
		}
		if (zero_max < fptr[0 + index]) {
			zero_max = fptr[0 + index];
		}
		if (one_max , fptr[1 + index]) {
			one_max = fptr[1 + index];
		}
		if (two_max < fptr[2 + index]) {
			two_max = fptr[2 + index];
		}
	}

	float zero_diff = zero_max - zero_min;
	float one_diff = one_max - one_min;
	float two_diff = two_max - two_min;
	long double v18 = two_diff * two_diff + one_diff * one_diff + zero_diff * zero_diff;
	
	if (v18 == 0.0) {
		return 0.0;
	}

	return sqrt(v18) * 0.5;
}

void FFXI::CYy::CMoD3m::Draw(D3DCOLOR* a2, CMoD3mElem* a3)
{
	int* ptr = (int*)this->GetAnotherShortPointer();
	SomeVertexThing* svt = (SomeVertexThing*)ptr[1];
	if (svt == nullptr) {
		return;
	}

	CMoDx* cmodx = CYyDb::g_pCYyDb->g_pCMoDx;
	BaseProcessor* proc = CYyDb::g_pCYyDb->pMoProcessor;
	unsigned char* data = (unsigned char*)(this + 1);
	D3DCOLOR v6 = *a2;
	if ((a3->field_16C & 0xFF) == 0x44) {
		if (((v6 >> 24) & 0xFF) >= 0x7Fu) {
			v6 |= 0xFF000000;
		}
	}

	cmodx->field_EBC->SetRenderState(D3DRS_TEXTUREFACTOR, v6);
	if (data[0x04] == 0) {
		cmodx->field_EBC->SetTexture(0, nullptr);
		static StatusNode ZeroStateOne{};
		ZeroStateOne.ApplyTSS(ZeroOneTSS);
		if ((a3->field_16C & 0xFF) == 0x44) {
			static StatusNode ZeroStateTwo{};
			ZeroStateTwo.ApplyTSS(ZeroTwoTSS);
		}
		svt->Render(0, *(short*)(data + 6), D3DPT_TRIANGLELIST, 0, -1);
	}
	else {
		if ((a3->field_10C & 0x10000000) != 0) {
			static StatusNode NonZeroStateOne{};
			NonZeroStateOne.ApplyTSS(NonZeroOneTSS);
		}
		else {
			static StatusNode NonZeroStateTwo{};
			NonZeroStateTwo.ApplyTSS(NonZeroTwoTSS);
		}

		if ((a3->field_17B & 3) != 0) {
			FFXI::Math::WMatrix* pop{ nullptr };
			proc->PopFromStack(&pop);
			proc->MatrixIdentity(pop);
			pop->_31 = a3->field_120;
			pop->_32 = a3->field_124;
			cmodx->field_EBC->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
			cmodx->field_DC8->SetTransform(D3DTS_TEXTURE0, pop);
			proc->PushToStack(pop);
		}

		if ((a3->field_16C & 0xFF) == 0x49) {
			static StatusNode NonZeroStateThree{};
			NonZeroStateThree.ApplyTSS(NonZeroThreeTSS);
			cmodx->field_EBC->SetRenderState(D3DRS_TEXTUREFACTOR, *a2 | 0xFFFFFF);
			cmodx->field_EBC->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_DESTCOLOR);
			cmodx->field_EBC->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCALPHA);
		}

		unsigned short* sptr = this->GetShortPointer();
		int v13{ 0 };
		for (unsigned char i = 0; i < data[0x04]; ++i) {
			TexHelper* th = (TexHelper*)ptr + i;
			th->SetTexture(0);
			svt->Render(v13, sptr[i], D3DPT_TRIANGLELIST, 0, -1);
			v13 += 3 * sptr[i];
		}

		if (data[0x05] != 0) {
			static StatusNode NonZeroStateFour{};
			NonZeroStateFour.ApplyTSS(NonZeroFourTSS);
			int v21 = *(unsigned short*)(data + 6) - v13 / 3;
			svt->Render(v13, v21, D3DPT_TRIANGLELIST, 0, -1);
		}

		if ((a3->field_17B & 3) != 0) {
			cmodx->field_DC8->SetTransform(D3DTS_TEXTURE0, &proc->field_810);
			cmodx->field_EBC->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
		}
	}
}
