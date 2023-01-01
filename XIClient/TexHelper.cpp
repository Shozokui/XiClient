#include "TexHelper.h"
#include "CYyDb.h"
#include "CMoDx.h"
#include "CYyTexMng.h"
#include "CYyTex.h"
#include "Globals.h"
#include "CDx.h"
#include "StorageLists.h"
#include "CAcc.h"
#include "StatusNode.h"
#include "SomeVertexThing.h"
#include "FVF144Vertex.h"
using namespace FFXI::CYy;

DWORD THTss[] = {
	0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE,
	0, D3DTSS_TEXCOORDINDEX, 0,
	0, D3DTSS_COLOROP, D3DTOP_SELECTARG1,
	0, D3DTSS_COLORARG1, D3DTA_TEXTURE,
	0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1,
	0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE,
	0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR,
	0, D3DTSS_MINFILTER, D3DTEXF_LINEAR,
	0, D3DTSS_ADDRESSU, D3DTADDRESS_WRAP,
	0, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP,
	1, D3DTSS_COLOROP, D3DTOP_DISABLE,
	1, D3DTSS_ALPHAOP, D3DTOP_DISABLE,
	0, D3DTSS_FORCE_DWORD, false
};

DWORD THRs[] = {
	D3DRS_ALPHABLENDENABLE, false,
	D3DRS_ALPHATESTENABLE, false,
	D3DRS_ZWRITEENABLE, false,
	D3DRS_ZENABLE, D3DZB_FALSE,
	D3DRS_FOGENABLE, false,
	D3DRS_FORCE_DWORD, false
};

void FFXI::CYy::TexHelper::Find(CMoResource* a2)
{
	CYyTex* tex = FFXI::CYyDb::g_pCYyDb->pCYyTexMng->FindD3sTexUnder(this->data, a2);
	if (tex != nullptr) {
		*(CYyTex**)(this->data) = tex;
	}
	else {
		tex = *FFXI::CYyDb::g_pCYyDb->pCYyTexMng->GetTexAddr();
		while (tex != nullptr) {
			if ((tex->field_2C & 0x100) != 0
				|| tex->field_34 == 1) {
				if (memcmp(tex->Unique + 8, this->data + 8, 8) == 0) {
					*(CYyTex**)(this->data) = tex;
					return;
				}
			}

			tex = tex->field_38;
		}

		*(CYyTex**)(this->data) = nullptr;
	}
}

void FFXI::CYy::TexHelper::SetTexture(int a2)
{
	CYyTex* tex = *(CYyTex**)this->data;
	if (tex == nullptr) {
		CYyDb::g_pCYyDb->g_pCMoDx->field_EBC->SetTexture(a2, nullptr);
	}
	else {
		IDirect3DBaseTexture8* newtex{ nullptr };
		if (tex->RegTex != nullptr) {
			newtex = tex->RegTex;
		}
		else {
			newtex = tex->CubeTex;
		}
		CYyDb::g_pCYyDb->g_pCMoDx->field_EBC->SetTexture(a2, newtex);
	}
}

void FFXI::CYy::TexHelper::DoSomething(int a2, float a3, float a4, float a5)
{
	CMoDx* cmodx = CYyDb::g_pCYyDb->g_pCMoDx;
	CYyTex* tex = *(CYyTex**)(&this->data[0]);
	if (tex == nullptr) {
		cmodx->field_EBC->SetTexture(a2, nullptr);
		return;
	}

	if (a4 == 0.0) {
		IDirect3DBaseTexture8* btex = tex->RegTex;
		if (btex == nullptr) {
			btex = tex->CubeTex;
		}
		cmodx->field_EBC->SetTexture(a2, btex);
		return;
	}

	this->CaccSetup();
	cmodx->field_DC8->AddViewportAtOrigin(CMoDx::th_cacc->SomeTexture, CMoDx::th_cacc->Texture2, CMoDx::th_cacc->Surface);
	IDirect3DBaseTexture8* btex = tex->RegTex;
	if (btex == nullptr) {
		btex = tex->CubeTex;
	}
	cmodx->field_EBC->SetTexture(a2, btex);

	static StatusNode THState{};
	THState.ApplyRSandTSS(THRs, THTss);

	float v19 = 1.0 / (double)(CMoDx::ImageWidth - 1);
	float v20 = v19 * a5 * FFXI::Constants::Values::ANGLE_2PI;
	int count = CMoDx::ImageWidth + 1;
	CMoDx::svt3->UpdateSubValues(2 * count);
	FVF144Vertex* buf = (FVF144Vertex*)CMoDx::svt3->LockBuffer();

	long double v14 = a3;
	double v15 = 0.0;
	for (int i = 0; i < count; ++i) {
		buf[2 * i].X = 0.0;
		buf[2 * i].Y = (float)i;
		buf[2 * i].Z = 0.0;
		buf[2 * i].RHW  = 1.0;
		long double v17 = sin(v14) * a4;
		buf[2 * i].TexVertX = v17;
		buf[2 * i].TexVertY = v15;
		buf[2 * i + 1].X = (float)CMoDx::ImageHeight;
		buf[2 * i + 1].Y = (float)i;
		buf[2 * i + 1].Z = 0.0;
		buf[2 * i + 1].RHW = 1.0;
		buf[2 * i + 1].TexVertX = v17 + 1.0;
		buf[2 * i + 1].TexVertY = v15;
		v14 += v20;
		v15 += v19;
	}
	CMoDx::svt3->UnlockBuffer();
	CMoDx::svt3->Render(0, 2 * CMoDx::ImageWidth, D3DPT_TRIANGLESTRIP, 0, -1);
	cmodx->field_EBC->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	cmodx->field_EBC->SetRenderState(D3DRS_ALPHATESTENABLE, true);
	cmodx->ClearRewind(0, 0);
	cmodx->field_EBC->SetTexture(a2, CMoDx::th_cacc->SomeTexture);
}

void FFXI::CYy::TexHelper::CaccSetup()
{
	CYyTex* tex = *(CYyTex**)(&this->data[0]);
	if (tex == nullptr) {
		return;
	}

	if (CMoDx::th_cacc != nullptr) {
		if (CMoDx::ImageHeight == tex->ImageHeight && CMoDx::ImageWidth == tex->ImageWidth) {
			return;
		}

		delete CMoDx::th_cacc;
		CMoDx::th_cacc = nullptr;
	}

	char* mem = StorageLists::instance->Get(sizeof(CAcc), FFXI::Constants::Enums::MEM_MODE::Ex);
	CMoDx::th_cacc = new (mem) CAcc();
	CMoDx::ImageHeight = tex->ImageHeight;
	CMoDx::ImageWidth = tex->ImageWidth;
	CMoDx::th_cacc->SomeTexture = CYyDb::g_pCYyDb->g_pCMoDx->field_DC8->CreateTexture(CMoDx::ImageHeight, CMoDx::ImageWidth, &CMoDx::th_cacc->Texture2, 1, D3DPOOL_DEFAULT);
	CMoDx::th_cacc->Surface = FFXI::CYy::CDx::instance->CreateDepthStencilSurface(CMoDx::ImageHeight, CMoDx::ImageWidth);
}

unsigned short FFXI::CYy::TexHelper::GetHeight()
{
	CYyTex* tex = *(CYyTex**)this->data;
	if (tex == nullptr)
		return 1;
	return tex->ImageHeight;
}

unsigned short FFXI::CYy::TexHelper::GetWidth()
{
	CYyTex* tex = *(CYyTex**)this->data;
	if (tex == nullptr)
		return 1;
	return tex->ImageWidth;
}
