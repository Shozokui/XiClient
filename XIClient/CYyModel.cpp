#include "CYyModel.h"
#include "CYyModelBase.h"
#include "CYyModelDt.h"
#include "Globals.h"
#include "StorageLists.h"
#include "CXiActor.h"
#include "CYyDb.h"
#include "CDx.h"
#include "CMoOs2.h"
using namespace FFXI::CYy;

const CYyClass CYyModel::CYyModelClass{
	"CYyModel", sizeof(CYyModel), &CYyObject::CYyObjectClass
};

D3DMATERIAL9 default_material = {
	{0.5, 0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5, 0.5},
	{0.0, 0.0, 0.0, 0.0},
	{0.0, 0.0, 0.0, 0.0},
	10.0
};
short CYyModel::g_some_short{ 0 };
FFXI::Math::WMatrix CYyModel::FIRST_tex_transform{};
FFXI::Math::WMatrix CYyModel::SECOND_tex_transform{};
FFXI::Math::WMatrix CYyModel::view_transform{};
FFXI::Math::WMatrix CYyModel::temp_proj_mtx{};
FFXI::Math::WMatrix CYyModel::temp_view_mtx{};
FFXI::Math::WMatrix CYyModel::temp_wrld_mtx{};
D3DMATERIAL8 CYyModel::material{};
D3DCOLOR CYyModel::ambient_color{};
int CYyModel::maybe_polys_drawn{};

float CYyModel::some_x_val{ 0.0 };
float CYyModel::some_y_val{ 0.0 };

const CYyClass* FFXI::CYy::CYyModel::GetRuntimeClass()
{
	return &CYyModel::CYyModelClass;
}

FFXI::CYy::CYyModel::CYyModel()
{
	this->field_30 = 0;
	this->field_40 = 0x40000000;
	this->Base = nullptr;
	this->field_A8 = 0;
	this->field_A9 = 0;
	this->field_AA = 0;
	this->field_AB = 0;
	this->field_AC = 0;
	this->field_AD = 0;
	this->field_AE = 0;
	this->field_AF = 0;
	this->field_B0 = 0;
	this->field_B4 = 0;
	this->field_B8 = 0;
}

void FFXI::CYy::CYyModel::Draw(CXiActor* a2, FFXI::Math::WMatrix* a3, D3DXVECTOR4* a4, D3DXVECTOR4* a5, D3DXVECTOR4* a6, D3DXVECTOR4* a7, unsigned int a8)
{
	D3DXVECTOR4 v27 = *a5;
	CYyModel::maybe_polys_drawn = 0;
	FFXI::CYy::CDx::instance->SetActiveVertexShader(0);
	this->field_30 |= 1;
	FFXI::CYy::CDx::instance->GetTransform(D3DTS_WORLD, &CYyModel::temp_wrld_mtx);
	FFXI::CYy::CDx::instance->GetTransform(D3DTS_VIEW, &CYyModel::temp_view_mtx);
	FFXI::CYy::CDx::instance->GetTransform(D3DTS_PROJECTION, &CYyModel::temp_proj_mtx);
	if (CXiActor::somebool == true || this->field_B8 != 1) {
		if (a7->w != -1.0
			&& a7->w != 0.0) {
			if (this->DoingSomething(a2, a3, a2->field_BC, a7) == false) {
				this->field_B4 = CYyModel::maybe_polys_drawn;
				return;
			}
		}
		else if (this->DoingSomething(a2, a3, a2->field_BC, nullptr) == false) {
			this->field_B4 = CYyModel::maybe_polys_drawn;
			return;
		}
	}

	//Label 8
	CYyModel::FIRST_tex_transform.Identity();
	CYyModel::FIRST_tex_transform._22 = -1.0;
	CYyModel::FIRST_tex_transform._33 = -1.0;
	CYyModel::FIRST_tex_transform.SomeCombo(&CYyDb::g_pCYyDb->field_294);
	CYyModel::FIRST_tex_transform.SomeCombo(&CYyModel::view_transform);

	CYyModel::SECOND_tex_transform.Identity();
	CYyModel::SECOND_tex_transform._33 = 0.0;
	CYyModel::SECOND_tex_transform._43 = -1.0;

	CYyModel::ambient_color = a8;

	CYyModel::material = default_material;
	CYyModel::material.Ambient.r = (double)((a8 >> 0) & 0xFF) * 0.0078125;
	CYyModel::material.Ambient.g = (double)((a8 >> 8) & 0xFF) * 0.0078125;
	CYyModel::material.Ambient.b = (double)((a8 >> 16) & 0xFF) * 0.0078125;
	CYyModel::material.Ambient.a = (double)((a8 >> 24) & 0xFF) * 0.0078125;
	FFXI::CYy::CDx::instance->DXDevice->SetMaterial(&CYyModel::material);

	if ((a8 & 0xFF000000) == 0) {
		this->field_B4 = CYyModel::maybe_polys_drawn;
		return;
	}

	CYyModel::ambient_color = a8 & 0xFF000000 | 0x808080;

	bool v9{ false };
	CMoResource** v10 = this->SubStruct5.GetResource();

	if (v10 != nullptr && *v10 != 0) {
		v9 = true;
		exit(0x1002AB13);
		//sub //TODO
	}
	
	ModelSubStruct2::SetStatics(CYyModel::ambient_color, v9);
	this->SubStruct2.PrepareViewport();

	if (CMoOs2::DrawBasicFlagOne == true) {
		CMoOs2::DrawBasicMatrixOne.Identity();
		CMoOs2::DrawBasicMatrixOne.SomeCombo(&CYyDb::g_pCYyDb->field_294);
		D3DXVECTOR3 v19 = { 0.5, 0.5, 0.5 };
		D3DXVECTOR3 v68 = { 0.5, 0.5, 0.5 };
		FFXI::Math::WMatrix va1(true);
		va1.Scale3(&v19);
		va1.AddTranslation3(&v68);
		CMoOs2::DrawBasicMatrixOne.MatrixMultiply(&va1);
		//sub //TODO
		exit(0x1002ABF2);
	}
	
	if (CMoOs2::DrawBasicFlagTwo == true) {
		CMoOs2::DrawBasicMatrixTwo = CYyModel::temp_proj_mtx;
		D3DXVECTOR3 v21{};
		v21.x = CYyModel::some_x_val;
		v21.y = CYyModel::some_y_val;
		v21.z = 0.0;

		CMoOs2::DrawBasicMatrixTwo.Vec3TransformDrop4Self(&v21);

		D3DXVECTOR3 v19 = { 0.5, 0.5, 0.5 };
		D3DXVECTOR3 v68 = { 0.5, 0.5, 0.5 };
		FFXI::Math::WMatrix va1(true);
		va1.Scale3(&v19);
		va1.AddTranslation3(&v68);
		CMoOs2::DrawBasicMatrixTwo.MatrixMultiply(&va1);

		CMoOs2::DrawBasicMatrixTwo._41 += v21.x;
		CMoOs2::DrawBasicMatrixTwo._42 += v21.y;

		if (FFXI::CYy::CDx::instance->CAcc2.Texture[0] != nullptr) {

		}
	}

	FFXI::CYy::CDx::instance->SetRenderStates();
	FFXI::CYy::CDx::instance->DXDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	FFXI::CYy::CDx::instance->DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	FFXI::CYy::CDx::instance->DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
	FFXI::CYy::CDx::instance->DXDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
	FFXI::CYy::CDx::instance->DXDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	FFXI::CYy::CDx::instance->DXDevice->SetTextureStageState(0, (D3DTEXTURESTAGESTATETYPE)D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
	FFXI::CYy::CDx::instance->DXDevice->SetTextureStageState(0, (D3DTEXTURESTAGESTATETYPE)D3DTSS_MINFILTER, D3DTEXF_LINEAR);
	FFXI::CYy::CDx::instance->DXDevice->SetTextureStageState(0, (D3DTEXTURESTAGESTATETYPE)D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP);
	FFXI::CYy::CDx::instance->DXDevice->SetTextureStageState(0, (D3DTEXTURESTAGESTATETYPE)D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP);
	FFXI::CYy::CDx::instance->DXDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE2X);
	FFXI::CYy::CDx::instance->DXDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	FFXI::CYy::CDx::instance->DXDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
	FFXI::CYy::CDx::instance->DXDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE2X);
	FFXI::CYy::CDx::instance->DXDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	FFXI::CYy::CDx::instance->DXDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
	FFXI::CYy::CDx::instance->DXDevice->SetTextureStageState(1, (D3DTEXTURESTAGESTATETYPE)D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
	FFXI::CYy::CDx::instance->DXDevice->SetTextureStageState(1, (D3DTEXTURESTAGESTATETYPE)D3DTSS_MINFILTER, D3DTEXF_LINEAR);
	FFXI::CYy::CDx::instance->DXDevice->SetTextureStageState(1, (D3DTEXTURESTAGESTATETYPE)D3DTSS_MIPFILTER, D3DTEXF_NONE);
	FFXI::CYy::CDx::instance->DXDevice->SetTextureStageState(1, (D3DTEXTURESTAGESTATETYPE)D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP);
	FFXI::CYy::CDx::instance->DXDevice->SetTextureStageState(1, (D3DTEXTURESTAGESTATETYPE)D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP);
	FFXI::CYy::CDx::instance->DXDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE2X);
	FFXI::CYy::CDx::instance->DXDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_CURRENT);
	FFXI::CYy::CDx::instance->DXDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_TFACTOR);
	FFXI::CYy::CDx::instance->DXDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_MODULATE4X);
	FFXI::CYy::CDx::instance->DXDevice->SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
	FFXI::CYy::CDx::instance->DXDevice->SetTextureStageState(1, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
	FFXI::CYy::CDx::instance->DXDevice->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 0);
	FFXI::CYy::CDx::instance->DXDevice->SetTextureStageState(1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	FFXI::CYy::CDx::instance->DXDevice->SetTextureStageState(2, D3DTSS_COLOROP, D3DTOP_DISABLE);
	FFXI::CYy::CDx::instance->DXDevice->SetTextureStageState(2, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	FFXI::CYy::CDx::instance->DXDevice->SetRenderState(D3DRS_FOGENABLE, true);

	if (FFXI::CYy::CDx::instance->field_92D != 0) {
		FFXI::CYy::CDx::instance->DXDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_NONE);
		FFXI::CYy::CDx::instance->DXDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);
		FFXI::CYy::CDx::instance->DXDevice->SetRenderState(D3DRS_RANGEFOGENABLE, false);
	}
	else if (FFXI::CYy::CDx::instance->field_92E != 0) {
		FFXI::CYy::CDx::instance->DXDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);
		FFXI::CYy::CDx::instance->DXDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_NONE);

		if (FFXI::CYy::CDx::instance->field_92F != 0) {
			FFXI::CYy::CDx::instance->DXDevice->SetRenderState(D3DRS_RANGEFOGENABLE, true);
		}
		else {
			FFXI::CYy::CDx::instance->DXDevice->SetRenderState(D3DRS_RANGEFOGENABLE, false);
		}
	}

	FFXI::CYy::CDx::instance->DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	FFXI::CYy::CDx::instance->DXDevice->SetRenderState(D3DRS_SPECULARENABLE, false);

	if (Config::MainRegistryConfig::MIPMapping == 0) {
		FFXI::CYy::CDx::instance->DXDevice->SetTextureStageState(0, (D3DTEXTURESTAGESTATETYPE)D3DTSS_MIPFILTER, D3DTEXF_NONE);
		FFXI::CYy::CDx::instance->DXDevice->SetTextureStageState(1, (D3DTEXTURESTAGESTATETYPE)D3DTSS_MIPFILTER, D3DTEXF_NONE);
	}
	else {
		exit(0x1002B0A3);
	}

	FFXI::CYy::CDx::instance->DXDevice->SetRenderState(D3DRS_ALPHAREF, 0x45);
	this->DrawBaseList(a2, a2->field_BC);
	FFXI::CYy::CDx::instance->DXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	FFXI::CYy::CDx::instance->DXDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	FFXI::CYy::CDx::instance->DXDevice->SetRenderState(D3DRS_ALPHAREF, 0x60);
	FFXI::CYy::CDx::instance->DXDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
	FFXI::CYy::CDx::instance->DXDevice->SetRenderState(D3DRS_NORMALIZENORMALS, false);
	FFXI::CYy::CDx::instance->DXDevice->SetTextureStageState(0, (D3DTEXTURESTAGESTATETYPE)D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
	FFXI::CYy::CDx::instance->DXDevice->SetTextureStageState(0, (D3DTEXTURESTAGESTATETYPE)D3DTSS_MINFILTER, D3DTEXF_LINEAR);
	FFXI::CYy::CDx::instance->DXDevice->SetTextureStageState(0, (D3DTEXTURESTAGESTATETYPE)D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP);
	FFXI::CYy::CDx::instance->DXDevice->SetTextureStageState(0, (D3DTEXTURESTAGESTATETYPE)D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP);
	FFXI::CYy::CDx::instance->DXDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE2X);
	FFXI::CYy::CDx::instance->DXDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	FFXI::CYy::CDx::instance->DXDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
	FFXI::CYy::CDx::instance->DXDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE2X);
	FFXI::CYy::CDx::instance->DXDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	FFXI::CYy::CDx::instance->DXDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
	FFXI::CYy::CDx::instance->DXDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	FFXI::CYy::CDx::instance->DXDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	FFXI::CYy::CDx::instance->DXDevice->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 0);
	FFXI::CYy::CDx::instance->DXDevice->SetTextureStageState(1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	FFXI::CYy::CDx::instance->DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	FFXI::CYy::CDx::instance->DXDevice->SetRenderState(D3DRS_FOGENABLE, false);
	FFXI::CYy::CDx::instance->DXDevice->SetRenderState(D3DRS_RANGEFOGENABLE, false);
	FFXI::CYy::CDx::instance->DXDevice->SetRenderState(D3DRS_SPECULARENABLE, false);

	this->SubStruct2.CleanViewport();
	this->SubStruct5.SetConstColor();
	this->SubStruct5.DrawSomething();
	this->SubStruct5.DrawAnotherSomething();
	FFXI::CYy::CDx::instance->DXDevice->SetTextureStageState(0, (D3DTEXTURESTAGESTATETYPE)D3DTSS_MAGFILTER, D3DTEXF_POINT);
	FFXI::CYy::CDx::instance->DXDevice->SetTextureStageState(0, (D3DTEXTURESTAGESTATETYPE)D3DTSS_MINFILTER, D3DTEXF_POINT);
	this->SubStruct2.DrawSomething();
	FFXI::CYy::CDx::instance->DXDevice->SetTextureStageState(0, (D3DTEXTURESTAGESTATETYPE)D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
	FFXI::CYy::CDx::instance->DXDevice->SetTextureStageState(0, (D3DTEXTURESTAGESTATETYPE)D3DTSS_MINFILTER, D3DTEXF_LINEAR);
	this->SubStruct1.DoTheThing(&v27);
	FFXI::CYy::CDx::instance->DXDevice->SetTextureStageState(0, (D3DTEXTURESTAGESTATETYPE)D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
	FFXI::CYy::CDx::instance->DXDevice->SetTextureStageState(0, (D3DTEXTURESTAGESTATETYPE)D3DTSS_MINFILTER, D3DTEXF_LINEAR);
	this->field_B4 = CYyModel::maybe_polys_drawn;
}

void FFXI::CYy::CYyModel::DrawBaseList(CXiActor* a2, float a3)
{
	if ((this->field_30 & 0x04) != 0)
		return;

	CYyModelBase* base = *this->GetBase();
	if (base == nullptr)
		return;

	while (base != nullptr) {
		base->Draw(a2, this, a3);
		base = base->Previous;
	}
	FFXI::CYy::CDx::instance->DXDevice->SetTextureStageState(1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	FFXI::CYy::CDx::instance->DXDevice->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 0);
	this->field_30 |= 4;
}

CYyModelBase* FFXI::CYy::CYyModel::CreateBase()
{
	char* mem = StorageLists::instance->Get(sizeof(CYyModelBase), FFXI::Constants::Enums::MEM_MODE::Ex);
	if (!mem) return nullptr;

	CYyModelBase* base = new (mem) CYyModelBase();
	if (base == nullptr) return nullptr;

	CYyModelBase** tail = &this->Base;
	while (*tail)
		tail = &(*tail)->Previous;

	*tail = base;
	return base;
}

CYyModelBase** FFXI::CYy::CYyModel::GetBase()
{
	return &this->Base;
}

bool FFXI::CYy::CYyModel::DoingSomething(CXiActor* a2, FFXI::Math::WMatrix* a3, float a4, D3DXVECTOR4* a5)
{
	CYyModelBase* p_base = *this->GetBase();
	while (p_base) {
		if (p_base->DoingSomething(a2, a3, a4, a5, this->field_30) == false)
			return false;
		p_base = p_base->Previous;
	}

	this->field_30 |= 0x12;
	return true;
}

int FFXI::CYy::CYyModel::GetSomeBaseIndex(int a2, int a3)
{
	CYyModelBase** pbase = this->GetBase();
	CYyModelBase* base = *pbase;
	if (base == nullptr)
		return 0;

	int v4{ 0 };
	int v7{ 0 };
	while (base != nullptr) {
		v7 = v4;
		v4 += 1;
		if (v7 == a3)
			break;
		pbase = &base->Previous;
		base = base->Previous;
	}

	return base->GetSomeIndex(a2);
}

bool FFXI::CYy::CYyModel::GetSomething(int a2, int a3, D3DXVECTOR4* a4)
{
	if (a3 == 2) {
		CYyModelBase* v7 = *this->GetBase();
		if (v7 == nullptr)
			return false;

		int v8 = 0;
		while (v7 != nullptr) {
			if (v8 == a2)
				break;
			v8 += 1;
			v7 = v7->Previous;
		}

		D3DXVECTOR3 v14{}, v16{};
		v7->GetSomeVector(2, &v14);
		v16.x = this->field_24.x;
		v16.y = this->field_24.z;
		v16.z = this->field_24.y;
		
		v14.x *= v16.x;
		v14.y *= v16.y;
		v14.z *= v16.z;
		
		a4->x += v14.x;
		a4->y += v14.y;
		a4->z += v14.z;

		return true;
	}

	if ((this->field_30 & 0x10) == 0)
		return false;

	CYyModelBase* v7 = *this->GetBase();
	if (v7 == nullptr)
		return false;

	int v8 = 0;
	while (v7 != nullptr) {
		if (v8 == a2)
			break;
		v8 += 1;
		v7 = v7->Previous;
	}

	v7->TWO_GetSomeVector(a3, a4);
	return true;
}

int FFXI::CYy::CYyModel::GetOs2ResId(int a2)
{
	CYyModelBase* base = *this->GetBase();
	if (base == nullptr)
		return 0;

	CYyModelDt* dt = *base->GetModelDt();
	if (dt == nullptr)
		return 0;

	if (a2 <= 0)
		return dt->GetResId();

	int index = 0;
	while (dt->field_4 != nullptr) {
		dt = dt->field_4;
		index += 1;
		if (index >= a2)
			return dt->GetResId();
	}
	
	return 0;
}

void FFXI::CYy::CYyModel::IsHideOs2(int a2, int a3, int a4)
{
	CYyModelBase* base = *this->GetBase();
	if (base == nullptr)
		return;

	CYyModelDt* dt = *base->GetModelDt();
	if (dt == nullptr)
		return;

	if (a2 <= 0) {
		*((char*)&dt->field_C + a4) = a3;
	}
	else {
		int index = 0;
		while (dt->field_4 != nullptr) {
			dt = dt->field_4;
			index += 1;
			if (index >= a2) {
				*((char*)&dt->field_C + a4) = a3;
			}
		}
	}
}
