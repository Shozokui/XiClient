#include "CMoOcclusionMng.h"
#include "Globals.h"
#include "CYyDb.h"
#include "CDx.h"
#include "CMoDx.h"
using namespace FFXI::CYy;

const CYyClass CMoOcclusionMng::CMoOcclusionMngClass{
	"CMoOcclusionMng", sizeof(CMoOcclusionMng), &CYyObject::CYyObjectClass
};
const CYyClass* FFXI::CYy::CMoOcclusionMng::GetRuntimeClass()
{
	return &CMoOcclusionMng::CMoOcclusionMngClass;
}

FFXI::CYy::CMoOcclusionMng::CMoOcclusionMng()
{
	IDirect3DSurface8* v3{ nullptr };
	D3DSURFACE_DESC8 v4{};
	this->field_4 = 0;
	FFXI::CYy::CDx::instance->CAcc2.Texture[0]->GetSurfaceLevel(0, &v3);
	v3->GetDesc(&v4);
	v3->Release();
	CYyDb::g_pCYyDb->g_pCMoDx->field_EBC->CreateRenderTarget(16, 16, v4.Format, D3DMULTISAMPLE_NONE, true, &this->field_18);
}

FFXI::CYy::CMoOcclusionMng::~CMoOcclusionMng()
{
	if (this->field_18) {
		this->field_18->Release();
		this->field_18 = nullptr;
	}
}

void FFXI::CYy::CMoOcclusionMng::InitField8()
{
	this->field_8.SomeTexture = FFXI::CYy::CDx::instance->CreateTexture(16, 16, &this->field_8.Texture2, 1, D3DPOOL_DEFAULT);
}