#include "ModelSubStruct1.h"
#include "Globals.h"
#include "CDx.h"
#include "CYyDb.h"
#include "CYyModel.h"
using namespace FFXI::CYy;

float ModelSubStruct1::g_mss1_float{};
FFXI::CYy::ModelSubStruct1::ModelSubStruct1()
{
	if (Globals::g_pSomeTexture5 == nullptr) {
		Globals::g_pSomeTexture5 = FFXI::CYy::CDx::instance->CreateTexture(
			CYyDb::g_pCYyDb->GetBackgroundXRes(),
			CYyDb::g_pCYyDb->GetBackgroundYRes(),
			&Globals::g_pSomeTexture6,
			1,
			D3DPOOL_DEFAULT
		);
	}

	if (Globals::g_pSomeTexture5 != nullptr) {
		this->field_8 = Globals::g_pSomeTexture5;
		this->field_8->AddRef();
	}
	else
		this->field_8 = nullptr;

	if (Globals::g_pSomeTexture6 != nullptr) {
		this->field_C = Globals::g_pSomeTexture6;
		this->field_C->AddRef();
	}
	else
		this->field_C = nullptr;

	ModelSubStruct1::g_mss1_float = 1.0;
	this->field_14 = { 0.0, 0.0, 0.0 };
	this->field_20 = { 0.0, 0.0, 0.0 };
	this->field_4 = 0;
}

FFXI::CYy::ModelSubStruct1::~ModelSubStruct1()
{
	if (this->field_8 != nullptr) {
		this->field_8->Release();
		this->field_8 = nullptr;
	}

	if (this->field_C != nullptr) {
		this->field_C->Release();
		this->field_C = nullptr;
	}
}

void FFXI::CYy::ModelSubStruct1::Init(D3DXVECTOR3* a2)
{
	if (a2->y > 0.0) {
		a2->x = 0.44999999;
		a2->y = -0.89999998;
		a2->z = 0.0;
	}

	double v9 = CYyDb::g_pCYyDb->CheckTick() * 0.039999999;
	float v33 = v9;
	if (v9 > 1.0)
		v33 = 1.0f;

	
	*a2 -= this->field_20;
	*a2 *= v33;
	this->field_20 += *a2;
	D3DXVECTOR3 a1 = this->field_20;
	Globals::Vec3Normalize(&a1);
	long double mag = sqrt(a1.z * a1.z + a1.x * a1.x);
	float magf = mag;
	if (mag <= 0.000099999997) {
		a1 = { 0.0, -1.0, 0.0 };
	}
	else {
		if (a1.y >= 0.0 
			|| -atan2(a1.y, magf) < FFXI::Constants::Values::ANGLE_PI_OVER_3) {
			a1.y = -(sin(FFXI::Constants::Values::ANGLE_PI_OVER_3) * magf);
		}
		Globals::Vec3Normalize(&a1);
	}

	double v19 = CYyDb::g_pCYyDb->CheckTick() * 0.039999999;
	float v35 = v19;
	if (v19 > 1.0)
		v35 = 1.0f;

	a1 -= this->field_14;
	a1 *= v35;
	this->field_14 += a1;
	if (abs(this->field_14.x) < 0.0099999998) {
		if (abs(this->field_14.z) < 0.0099999998) {
			this->field_14.x = 0.0099999998;
		}
	}
	Globals::Vec3Normalize(&this->field_14);
}

void FFXI::CYy::ModelSubStruct1::DoTheThing(D3DXVECTOR4* a2)
{
	if (Globals::g_some_actordraw_short == 0) {
		if (CYyModel::g_some_short != 1) {
			float v8 = ModelSubStruct1::g_mss1_float * 0.33333334;
			//sub //TODO is this correct?
			if (Globals::g_some_actordraw_float <= 20.0) {
				exit(0x10033056);
			}
			else {
				exit(0x1003306A);
			}
		}
	}
	else if (Globals::g_some_actordraw_short == 1) {
		if (this->field_4 != 0) {
			if (this->field_8 != nullptr) {
				float v8 = ModelSubStruct1::g_mss1_float * 2.4000001;
				exit(0x1003300F);
				this->field_4 = 0;
			}
		}
	}
	else if (Globals::g_some_actordraw_short == 2) {
		if (CYyModel::g_some_short != 1) {
			float v8 = ModelSubStruct1::g_mss1_float * 0.33333334;
			exit(0x10032FC6);
		}
	}
}

bool FFXI::CYy::ModelSubStruct1::PrepareToRender(int* a2, D3DCOLOR a3)
{
	FFXI::Math::WMatrix v6{}, pOut{};
	if (Globals::g_some_actordraw_short == 1 && Globals::g_some_actordraw_float < 4.0 && this->field_8 != nullptr) {
		//statics
		//sub //TODO
		exit(0x180555);
		CDx::instance->AddViewportAtOrigin(this->field_8, this->field_C, NULL);
		if (this->field_4 == 0) {
			CDx::instance->ClearViewport(0, NULL, 1, 0, 0.0, 0);
			this->field_4 = 1;
		}

		CDx::instance->DXDevice->SetRenderState(D3DRS_TEXTUREFACTOR, -2130706433);
		CDx::instance->DXDevice->SetTextureStageState(0, D3DTSS_COLOROP, 3);
		CDx::instance->DXDevice->SetTextureStageState(0, D3DTSS_COLORARG2, 3);
		CDx::instance->DXDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, 3);
		CDx::instance->DXDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, 3);
		CDx::instance->DXDevice->SetTextureStageState(1, D3DTSS_COLOROP, 1);
		CDx::instance->DXDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, 1);
		CDx::instance->DXDevice->SetRenderState(D3DRS_LIGHTING, 0);
		CDx::instance->DXDevice->SetRenderState(D3DRS_FOGENABLE, 0);
		CDx::instance->DXDevice->SetRenderState(D3DRS_ZENABLE, 0);
		CDx::instance->DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, 0);
		CDx::instance->DXDevice->SetRenderState(D3DRS_SHADEMODE, 1);

		D3DXMatrixPerspectiveFovRH(&pOut, 0.19634955, 1.0, 0.1, 65535.0);

		CDx::instance->SetTransform(D3DTS_PROJECTION, &pOut);
		CDx::instance->SetTransform(D3DTS_VIEW, &CYyModel::view_transform);
		CDx::instance->SetTransform(D3DTS_WORLD, &Globals::g_TransformBackup);
		return true;
	}

	return false;
}
