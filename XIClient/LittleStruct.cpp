#include "LittleStruct.h"
#include "Globals.h"
#include "CDx.h"
#include "CYyDb.h"
#include "CMoDx.h"
#include "CMoOcclusionMng.h"
#include "StatusNode.h"
#include "FVF44Vertex.h"
#include "FVF104Vertex.h"
using namespace FFXI;

const RECT defaultRect = { 0, 0, 16, 16 };

DWORD LSRS[] = {
	D3DRS_STENCILENABLE, false,
	D3DRS_ALPHATESTENABLE, false,
	D3DRS_ALPHABLENDENABLE, false,
	D3DRS_ZENABLE, false,
	D3DRS_ZWRITEENABLE, false,
	D3DRS_LIGHTING, false,
	D3DRS_CULLMODE, D3DCULL_NONE,
	D3DRS_FOGENABLE, false,
	D3DRS_FORCE_DWORD, false
};

DWORD LSTSS[] = {
	0, D3DTSS_COLOROP, D3DTOP_SELECTARG1,
	0, D3DTSS_COLORARG1, D3DTA_DIFFUSE,
	0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1,
	0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE,
	0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE,
	0, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP,
	0, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP,
	1, D3DTSS_COLOROP, D3DTOP_DISABLE,
	1, D3DTSS_ALPHAOP, D3DTOP_DISABLE,
	0, D3DTSS_FORCE_DWORD, false
};

DWORD LSRS2[]{
	D3DRS_ZENABLE, false,
	D3DRS_ZWRITEENABLE, false,
	D3DRS_FORCE_DWORD, false
};

DWORD LSTSS2[]{
	0, D3DTSS_MINFILTER, D3DTEXF_POINT,
	0, D3DTSS_MAGFILTER, D3DTEXF_POINT,
	0, D3DTSS_COLORARG1, D3DTA_TEXTURE,
	0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE,
	0, D3DTSS_FORCE_DWORD, false
};
FFXI::CYy::FVF44Vertex VertsOne[4] = {
	{0.0, 0.0, 0.0, 1.0, 0},
	{0.0, 0.0, 0.0, 1.0, 0},
	{0.0, 0.0, 0.0, 1.0, 0},
	{0.0, 0.0, 0.0, 1.0, 0}
};

FFXI::CYy::FVF44Vertex VertsTwo[4] = {
	{0.0, 0.0, 0.0, 1.0, 0x80FFFFFF},
	{0.0, 0.0, 0.0, 1.0, 0x80FFFFFF},
	{0.0, 0.0, 0.0, 1.0, 0x80FFFFFF},
	{0.0, 0.0, 0.0, 1.0, 0x80FFFFFF}
};

FFXI::CYy::FVF104Vertex VertsThree[4] = {
	{00.0, 00.0, 0.0, 1.0, 0.0, 0.0},
	{15.0, 00.0, 0.0, 1.0, 1.0, 0.0},
	{00.0, 15.0, 0.0, 1.0, 0.0, 1.0},
	{15.0, 15.0, 0.0, 1.0, 1.0, 1.0}
};

FFXI::LittleStruct::LittleStruct()
{
	this->field_4 = nullptr;
	this->field_2 = 0;
}

FFXI::LittleStruct::~LittleStruct()
{
	if (this->field_4 != nullptr) {
		this->field_4->Release();
	}
}

void FFXI::LittleStruct::Init()
{
	FFXI::CYy::CMoOcclusionMng* occmng = CYyDb::g_pCYyDb->g_pCMoDx->g_pCMoOcclusionMng;
	if (occmng->field_8.SomeTexture == nullptr)
		occmng->InitField8();

	if (this->field_4 != nullptr)
		return;

	IDirect3DSurface8* v3{ nullptr };
	FFXI::CYy::CDx::instance->CAcc2.Texture[0]->GetSurfaceLevel(0, &v3);

	D3DSURFACE_DESC8 v4{};
	v3->GetDesc(&v4);
	v3->Release();

	this->field_4 = occmng->field_18;
	if (this->field_4 != nullptr)
		this->field_4->AddRef();

	this->field_0 = 0;
}

int FFXI::LittleStruct::CopyRects(bool a2)
{
	IDirect3DSurface8* v12{ nullptr };
	CYyDb::g_pCYyDb->g_pCMoDx->g_pCMoOcclusionMng->field_8.SomeTexture->GetSurfaceLevel(0, &v12);
	
	int offset = 0;
	if (a2 == true)
		offset = this->field_0;

	CYyDb::g_pCYyDb->g_pCMoDx->field_EBC->CopyRects(v12, &defaultRect, 1, (&this->field_4)[offset], nullptr);
	v12->Release();

	D3DLOCKED_RECT v14{};
	int sum = 0;
	if (this->field_4->LockRect(&v14, &defaultRect, D3DLOCK_READONLY) == D3D_OK) {
		int v13 = 2 * v14.Pitch;
		char* pBits = (char*)v14.pBits;
		for (int i = 0; i < 8; ++i) {
			for (int j = 0; j < 8; ++j) {
				int value = *(unsigned int*)(pBits + 8 * j);
				sum += value & 0x80;
			}
			pBits += v13;
		}
		this->field_4->UnlockRect();
	}

	int v9 = sum >> 5;
	if (a2 == false)
		return v9;

	this->field_0 = 0;
	if (v9 == 0) {
		this->field_2 = 1;
		return 0;
	}

	if (this->field_2 == 0) {
		return v9;
	}

	this->field_2 -= 1;
	return 0;
}

int FFXI::LittleStruct::Update(D3DXVECTOR4* a2, float a3, float a4, float a5, IDirect3DTexture8* a6, IDirect3DTexture8* a7, int a8, int a9, int a10)
{
	int backup = FFXI::CYy::CDx::instance->field_828;
	FFXI::CYy::CDx::instance->field_828 = 0;
	this->Init();
	int result = this->PrepRender(a2, a3, a4, a5, a6, a7, a8, a9, a10);
	FFXI::CYy::CDx::instance->field_828 = backup;
	return result;
}

int FFXI::LittleStruct::PrepRender(D3DXVECTOR4* a2, float a3, float a4, float a5, IDirect3DTexture8* a6, IDirect3DTexture8* a7, int a8, int a9, int a10)
{
	int x = a2->x;
	int y = a2->y;
	int z = a2->z;
	int w = a2->w;

	if (x > z) {
		x = a2->z;
		z = a2->x;
	}
	if (y > w) {
		y = a2->w;
		w = a2->y;
	}

	RECT v21{};
	v21.left = x;
	v21.top = y;
	v21.right = z;
	v21.bottom = w;

	if (x < -256)
		v21.left = -256;
	if (y < -256)
		v21.top = -256;

	D3DVIEWPORT8* tvp = CYyDb::g_pCYyDb->g_pCMoDx->field_DC8->GetTopViewport();
	if (v21.right > tvp->Width + 256) {
		v21.right = tvp->Width + 256;
	}
	if (v21.bottom > tvp->Height + 256) {
		v21.bottom = tvp->Height + 256;
	}
	if (x < 0)
		x = 0;
	if (y < 0)
		y = 0;
	if (z >= tvp->Width)
		z = tvp->Width;
	if (w > tvp->Height)
		w = tvp->Height;

	int v14 = z - x;
	int v15 = w - y;
	if (v14 <= 0 || v15 <= 0) {
		return 0;
	}

	float v19 = (float)(16 * v15 / (v21.bottom - v21.top));
	float v18 = (float)(16 * v14 / (v21.right - v21.left));

	if (a9 == 0) {
		a3 = 0.99999988;
	}

	this->Render(&v21, a3, v18, v19, a4, a5, a6, a7, 0);
	int result = this->CopyRects(false);
	CYyDb::g_pCYyDb->g_pCMoDx->field_EBC->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	CYyDb::g_pCYyDb->g_pCMoDx->field_EBC->SetRenderState(D3DRS_ZWRITEENABLE, true);
	return result;
}

void FFXI::LittleStruct::Render(RECT* a2, float a3, float a4, float a5, float a6, float a7, IDirect3DTexture8* a8, IDirect3DTexture8* a9, int a10)
{
	static FFXI::CYy::StatusNode LSState{};
	LSState.ApplyRSandTSS(LSRS, LSTSS);
	FFXI::CYy::CMoDx* cmodx = CYyDb::g_pCYyDb->g_pCMoDx;
	short v5[2] = { 0, 0 };
	cmodx->field_DC8->AddViewport(a8, a9, FFXI::CYy::CDx::instance->CAcc2.Surface, v5, v5, 1, 0);
	cmodx->field_EBC->SetVertexShader(VertsOne[0].FVF);
	int size = Globals::GetStrideFromFVF(VertsOne[0].FVF);
	VertsOne[0].X = (float)a2->left;
	VertsOne[0].Y = (float)a2->top;
	VertsOne[1].X = (float)a2->right;
	VertsOne[1].Y = (float)a2->top;
	VertsOne[2].X = (float)a2->left;
	VertsOne[2].Y = (float)a2->bottom;
	VertsOne[3].X = (float)a2->right;
	VertsOne[3].Y = (float)a2->bottom;
	cmodx->field_DC8->DrawVerts(D3DPT_TRIANGLESTRIP, 2, VertsOne, size);
	
	cmodx->field_EBC->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	cmodx->field_EBC->SetVertexShader(VertsTwo[0].FVF);
	size = Globals::GetStrideFromFVF(VertsTwo[0].FVF);
	VertsTwo[0].X = (float)a2->left;
	VertsTwo[0].Z = a3;
	VertsTwo[0].Y = (float)a2->top;
	VertsTwo[1].X = (float)a2->right;
	VertsTwo[1].Z = a3;
	VertsTwo[1].Y = (float)a2->top;
	VertsTwo[2].X = (float)a2->left;
	VertsTwo[2].Z = a3;
	VertsTwo[2].Y = (float)a2->bottom;
	VertsTwo[3].X = (float)a2->right;
	VertsTwo[3].Z = a3;
	VertsTwo[3].Y = (float)a2->bottom;

	cmodx->field_DC8->DrawVerts(D3DPT_TRIANGLESTRIP, 2, VertsTwo, size);

	cmodx->field_DC8->DXDevice->SetTexture(0, nullptr);
	cmodx->field_DC8->RewindStage();

	cmodx->field_DC8->AddViewport(
		cmodx->g_pCMoOcclusionMng->field_8.SomeTexture,
		cmodx->g_pCMoOcclusionMng->field_8.Texture2,
		cmodx->g_pCMoOcclusionMng->field_8.Surface,
		v5, v5, 0, 0);

	cmodx->field_EBC->SetTexture(0, a8);
	cmodx->field_DC8->ClearFullViewport(1, 0, 1.0, 0);

	static FFXI::CYy::StatusNode LSState2{};
	LSState2.ApplyRSandTSS(LSRS2, LSTSS2);
	cmodx->field_EBC->SetVertexShader(VertsThree[0].FVF);
	size = Globals::GetStrideFromFVF(VertsThree[0].FVF);
	double v14 = 1.0 / a6;
	double v15 = 1.0 / a7;
	VertsThree[1].X = a4;
	VertsThree[2].Y = a5;
	VertsThree[3].X = a4;
	VertsThree[3].Y = a5;
	VertsThree[0].TexVertX = (float)(a2->left + 1) * v14;
	VertsThree[0].TexVertY = (float)(a2->top + 1) * v15;
	VertsThree[1].TexVertX = (float)a2->right * v14;
	VertsThree[1].TexVertY = (float)(a2->top + 1) * v15;
	VertsThree[2].TexVertX = (float)(a2->left + 1) * v14;
	VertsThree[2].TexVertY = (float)a2->bottom * v15;
	VertsThree[3].TexVertX = (float)a2->right * v14;
	VertsThree[3].TexVertY = (float)a2->bottom * v15;
	cmodx->field_DC8->DrawVerts(D3DPT_TRIANGLESTRIP, 2, VertsThree, size);
	cmodx->field_DC8->DXDevice->SetTexture(0, nullptr);
	cmodx->field_DC8->RewindStage();
}
