#include "CMoElem.h"
#include "Globals.h"
#include "CYyDb.h"
#include "CMoResourceMng.h"
#include "StorageLists.h"
#include "CYyGenerator.h"
#include "BaseProcessor.h"
#include "CYyCamMng2.h"
#include "CMoDx.h"
#include "CMoOT.h"
#include "CTsZoneMap.h"
#include "XiZone.h"
#include "StatusNode.h"
#include "CDx.h"
#include "SomeVertexThing.h"
#include "FVF44Vertex.h"

using namespace FFXI::CYy;

const CYyClass FFXI::CYy::CMoElem::CMoElemClass = {
	"CMoElem", sizeof(CMoElem), &CMoOtTask::CMoOtTaskClass
};

D3DCOLOR FFXI::CYy::CMoElem::SomeColor{};
float FFXI::CYy::CMoElem::Floats[4] = { 0.0 };

DWORD BlendInitRS[] = {
	19, 10,
	20, 1,
	27, 1,
	14, 0,
	137, 0,
	22, 1,
	28, 0,
	7, 0,
	2147483647, 0
};

DWORD BlendInitTSS[] = {
	0, 1, 2,
	0, 2, 0,
	0, 4, 2,
	0, 5, 0,
	1, 1, 1,
	1, 4, 1,
	0, 2147483647, 0,
};

DWORD BlendCleanRS[] = {
	19, 5,
	20, 2,
	14, 1,
	7, 1,
	137, 1,
	22, 1,
	28, 0,
	2147483647, 0
};

DWORD BlendCleanTSS[] = {
	0, 1, 5,
	0, 2, 2,
	0, 3, 0,
	0, 4, 5,
	0, 5, 2,
	0, 6, 0,
	1, 1, 1,
	1, 4, 1,
	0, 2147483647, 0
};

DWORD ElemRS[] = {
	D3DRS_LIGHTING,			true,
	D3DRS_FOGENABLE,		false,
	D3DRS_RANGEFOGENABLE,	false,
	D3DRS_CULLMODE,			D3DCULL_NONE,
	D3DRS_BLENDOP,			D3DBLENDOP_ADD,
	D3DRS_SRCBLEND,			D3DBLEND_ONE,
	D3DRS_DESTBLEND,		D3DBLEND_ZERO,
	D3DRS_ZBIAS,			0,
	D3DRS_ALPHABLENDENABLE, false,
	D3DRS_ALPHATESTENABLE,  true,
	D3DRS_ALPHAFUNC,		D3DCMP_ALWAYS,
	D3DRS_ALPHAREF,			0x60,
	D3DRS_ZENABLE,			true,
	D3DRS_ZWRITEENABLE,		true,
	D3DRS_ZFUNC,			D3DCMP_LESSEQUAL,
	D3DRS_SPECULARENABLE,	false,
	D3DRS_COLORVERTEX,		true,
	D3DRS_FORCE_DWORD,		false
};

DWORD ElemTSS[] = {
	0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE,
	0, D3DTSS_TEXCOORDINDEX, 0,
	0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR,
	0, D3DTSS_MINFILTER, D3DTEXF_LINEAR,

	1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE,
	1, D3DTSS_TEXCOORDINDEX, 0,
	1, D3DTSS_MAGFILTER, D3DTEXF_LINEAR,
	1, D3DTSS_MINFILTER, D3DTEXF_LINEAR,

	2, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE,
	2, D3DTSS_TEXCOORDINDEX, 0,
	2, D3DTSS_MAGFILTER, D3DTEXF_LINEAR,
	2, D3DTSS_MINFILTER, D3DTEXF_LINEAR,

	3, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE,
	3, D3DTSS_TEXCOORDINDEX, 0,
	3, D3DTSS_MAGFILTER, D3DTEXF_LINEAR,
	3, D3DTSS_MINFILTER, D3DTEXF_LINEAR,

	0, D3DTSS_FORCE_DWORD, 0
};
const CYyClass* FFXI::CYy::CMoElem::GetRuntimeClass()
{
	return &CMoElem::CMoElemClass;
}

FFXI::CYy::CMoElem::CMoElem() {
	this->field_176 = sizeof(CMoElem);
	this->field_164 = 0;
	this->field_FC = nullptr;
	this->field_100 = nullptr;
	this->field_12C = 0;
	this->field_130 = 0;
	this->field_154 = 0;
	this->field_158 = 0;
	this->field_14C = nullptr;
	this->field_150 = nullptr;
	this->field_140 = 0.0;
	this->field_144 = 0.0;
	this->field_108 = nullptr;
	this->field_54 = { 0.0, 0.0, 0.0 };
	this->field_188 = { 0.0, 0.0, 0.0 };
	this->field_138 = 0.0;
	this->field_17A = 0;
	this->field_134 = 1.0;
	this->field_178 = 1;
	this->field_187 = 1;
}

FFXI::CYy::CMoElem::~CMoElem() {
	CYyGenerator* gen{ nullptr };
	if (this->field_FC != nullptr) {
		gen = *this->field_FC;
	}
	this->field_FC = nullptr;

	CYyGenerator* gen2{ nullptr };
	if (this->field_100 != nullptr) {
		gen2 = *this->field_100;
	}

	this->field_100 = nullptr;

	if (gen2 != nullptr) {
		if (this->field_154 != 0) {
			exit(0x10047235);
		}
		else {
			gen2->field_AC = this->field_158;
		}

		if (this->field_158 != 0) {
			exit(0x10047235);
		}

		gen2->SomeClean3();
	}

	if (gen != nullptr) {
		if (this->field_14C != nullptr) {
			this->field_14C->field_150 = this->field_150;
		}
		else {
			gen->field_B0 = this->field_150;
		}

		if (this->field_150 != nullptr) {
			this->field_150->field_14C = this->field_14C;
		}
		gen->SomeClean3();
		gen->SomeClean3();
	}

	if (this->field_164 != 0) {
		exit(0x10043BAA);
	}

	if (gen != nullptr) {
		if ((gen->PreHeader.field_16 & 4) != 0) {
			if ((gen->PreHeader.field_14 & 0x7FFF) == 0) {
				FFXI::CYyDb::g_pCYyDb->pCMoResourceMng->Unlink(gen->PreHeader.PointerToThisInRPL);
				FFXI::CYyDb::g_pCYyDb->pCMoResourceMng->RemoveRes(gen->PreHeader.PointerToThisInRPL);
			}
		}
	}

	if (this->field_108 != nullptr) {
		CYyObject::UnwrapPlus12(this->field_108);
		this->field_108 = nullptr;
	}
}

void FFXI::CYy::CMoElem::VObj2(int* a2)
{

	if (a2[0] == 1 || a2[0] == 4) {
		if (this != nullptr) {
			delete this;
		}
	}
}

char FFXI::CYy::CMoElem::OnMove()
{
	D3DXVECTOR3 backup = this->field_54;
	if (this->ExecTag() == true) {
		return 1;
	}

	this->CalcTrans();

	if (this->field_138 == 0.0) {
		this->field_138 = 1.0;
	}
	else {
		this->CalcMatrix();
		this->CalcRotAhead(backup);
		if (this->CheckSomething() == true) {
			this->CheckSomethingWasTrue();
		}
	}

	return 0;
}

void FFXI::CYy::CMoElem::VirtOt1()
{
	if (this->ModelType == 33) {
		int a = 1;
	}
	this->field_F8.GetWithScaledAlpha(&CMoElem::SomeColor, this->field_138 * this->field_134);
	
	if (this->field_17A != 0) {
		return;
	}

	bool v29 = (this->field_10C & 0x8000000) == 0;
	bool v30{ false };
	if (this->field_128 == 0.0
		|| (this->field_10C & 0x400000) != 0) {
		v29 = false;
	}

	CMoDx* cmodx = CYyDb::g_pCYyDb->g_pCMoDx;
	cmodx->field_EBC->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

	this->OnCalc();

	FFXI::Math::WMatrix* pop{ nullptr };
	FFXI::Math::WMatrix v33{};
	CYyDb::g_pCYyDb->pMoProcessor->PopFromStack(&pop);

	this->PrepDX(0xFFFFu);

	if (v29 == true) {
		cmodx->field_DC8->GetTransform(D3DTS_PROJECTION, &v33);
		v30 = true;
		this->SomeMatrixCalc();
		*pop = this->field_A0;
		long double v6 = fabs(pop->_43);
		if (v6 <= 32.0) {
			long double adjust = (1.0 - v6 * 0.03125) * ((1.0 - v6 * 0.03125) * (1.0 - v6 * 0.03125)) * 0.029999999 * this->field_128;
			float zf = CYyDb::g_pCYyDb->field_2E0 - adjust;
			float zn = CYyDb::g_pCYyDb->field_2DC - adjust;
			float aspect = 1.0 / (CYyDb::g_pCYyDb->field_2F0 * CYyDb::g_pCYyDb->field_2E8);
			float fovy = 2 * atan2(192.0, CYyDb::g_pCYyDb->field_2F4);
			pop->RHPerspective(fovy, aspect, zn, zf);
			cmodx->field_EBC->SetTransform(D3DTS_PROJECTION, pop);
		}
	}
	else if ((this->field_10C & 1) != 0) {
		if ((this->field_10C & 0x1C0) == 0
			&& (this->field_10C & 4) != 0
			&& (this->field_10C & 8) != 0) {
			cmodx->field_DC8->GetTransform(D3DTS_PROJECTION, &v33);
			v30 = true;
			float zf = CYyDb::g_pCYyDb->field_2E0;
			float zn = CYyDb::g_pCYyDb->field_2DC;
			float aspect = 1.0 / (CYyDb::g_pCYyDb->field_2F0 * CYyDb::g_pCYyDb->field_2E8);
			pop->RHPerspective(1.00349, aspect, zn, zf);
			cmodx->field_EBC->SetTransform(D3DTS_PROJECTION, pop);
		}
	}

	this->OnDraw();

	if (v30 == true) {
		cmodx->field_EBC->SetTransform(D3DTS_PROJECTION, &v33);
	}
	
	this->UnPrepDX(0xFFFFu);
	CYyDb::g_pCYyDb->pMoProcessor->PushToStack(pop);
}

bool FFXI::CYy::CMoElem::VirtElem1(FFXI::Constants::Enums::ElemType a2)
{
	return a2 == FFXI::Constants::Enums::ElemType::Base;
}

void FFXI::CYy::CMoElem::VirtElem2(float)
{
	//nullsub
}

void FFXI::CYy::CMoElem::VirtElem3(unsigned short)
{
	//nullsub
}

int FFXI::CYy::CMoElem::VirtElem4()
{
	return 0;
}

double FFXI::CYy::CMoElem::VirtElem6()
{
	if (this->field_188.z > 0.0) {
		return 0.0;
	}

	return CYyDb::g_pCYyDb->pMoProcessor->Vec3Magnitude(&this->field_188);
}

void FFXI::CYy::CMoElem::OnDraw()
{
	//nullsub
}

void FFXI::CYy::CMoElem::OnCalc()
{
	//nullsub
}

void FFXI::CYy::CMoElem::VirtElem9()
{
	D3DVIEWPORT8* vp = CYyDb::g_pCYyDb->g_pCMoDx->field_DC8->GetTopViewport();

	CMoElem::Floats[0] = 0.0;
	CMoElem::Floats[1] = 0.0;
	CMoElem::Floats[2] = vp->Width;
	CMoElem::Floats[3] = vp->Height;
}

bool FFXI::CYy::CMoElem::IsNever()
{
	return this->field_FC != nullptr
		&& *this->field_FC != nullptr
		&& ((*this->field_FC)->IsNever() == true);
	return false;
}

void FFXI::CYy::CMoElem::UnPrepDX(unsigned short a2)
{
	if (a2 == 0xFFFF) {
		a2 = this->field_16C;
	}

	switch ((unsigned char)a2) {
	case 0x41:
	case 0x42:
	case 0x43:
	case 0x62:
		this->DoBlend(false);
		break;
	default:
		break;
	}

	CYyDb::g_pCYyDb->g_pCMoDx->SetViewTransform(&CYyDb::g_pCYyDb->g_pCMoDx->field_B50);

	static StatusNode UnPrepState{};
	UnPrepState.ApplyRS(ElemRS);
}

void FFXI::CYy::CMoElem::PrepDX(unsigned short a2)
{
	if (a2 == 0xFFFF) {
		a2 = this->field_16C;
	}

	FFXI::Math::WMatrix* pop{ nullptr };
	CYyDb::g_pCYyDb->pMoProcessor->PopFromStack(&pop);
	CMoDx* cmodx = CYyDb::g_pCYyDb->g_pCMoDx;
	BaseProcessor* proc = CYyDb::g_pCYyDb->pMoProcessor;

	cmodx->SetViewTransform(&proc->field_810);
	cmodx->field_EBC->SetRenderState(D3DRS_ALPHABLENDENABLE, true);

	bool v30{ false };
	switch ((char)a2) {
	case 0x00:
		cmodx->field_EBC->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
		cmodx->field_EBC->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
		break;
	case 0x03:
	case 0x44:
		cmodx->field_EBC->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		cmodx->field_EBC->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		break;
	case 0x41:
	case 0x43:
	case 0x62:
		if ((char)a2 == 0x62) {
			CMoElem::SomeColor &= 0x00FFFFFF;
			CMoElem::SomeColor |= (a2 & 0xFF00) << 16;
		}
		this->DoBlend(true);
		break;
	case 0x42:
		this->DoBlend(true);
		break;
	case 0x46:
	case 0x47:
		cmodx->field_EBC->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
		cmodx->field_EBC->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		break;
	case 0x48:
		cmodx->field_EBC->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		cmodx->field_EBC->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		v30 = true;
		break;
	case 0x49:
		cmodx->field_EBC->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		cmodx->field_EBC->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		break;
	case 0x64: 
	{
		cmodx->field_EBC->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		cmodx->field_EBC->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		CMoElem::SomeColor &= 0x00FFFFFF;
		unsigned int al = (a2 >> 7) & 0xFFFFFFFE;
		al = al > 0xFFu ? 0xFFu : al & 0xFE;
		CMoElem::SomeColor |= (al << 24);
	}
	break;
	case 0x68:
	{
		cmodx->field_EBC->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		cmodx->field_EBC->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		CMoElem::SomeColor &= 0x00FFFFFF;
		unsigned int al = (a2 >> 7) & 0xFFFFFFFE;
		al = al > 0xFFu ? 0xFFu : al & 0xFE;
		CMoElem::SomeColor |= (al << 24);
	}
		break;
	default:
		break;
	}

	this->SomeMatrixCalc();
	cmodx->SetWorldTransform(&this->field_A0);
	cmodx->field_EBC->SetRenderState(D3DRS_ALPHATESTENABLE, true);
	cmodx->field_EBC->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	cmodx->field_EBC->SetRenderState(D3DRS_ALPHAREF, 0x60);
	cmodx->field_EBC->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);

	if ((this->field_10C & 0x1000) != 0) {
		cmodx->field_EBC->SetRenderState(D3DRS_ZWRITEENABLE, true);
	}
	else {
		cmodx->field_EBC->SetRenderState(D3DRS_ZWRITEENABLE, false);
	}

	if ((this->field_10C & 0x10000) == 0) {
		cmodx->field_EBC->SetRenderState(D3DRS_LIGHTING, false);
	}
	else {
		FFXI::Math::WMatrix* m = this->GetProcOrCModMatrix();
		*pop = *m;
		pop->_41 = 0.0; pop->_42 = 0.0;	pop->_43 = 0.0;	pop->_44 = 1.0;

		bool v10{ false };
		if (this->field_FC != nullptr && *this->field_FC != nullptr) {
			v10 = ((*this->field_FC)->field_DF & 2) != 0;
		}
		m = this->GetSomeMatrix();
		if (this->MMBFlag == 0 && v10 == false && m != nullptr) {
			FFXI::Math::WMatrix* pop2{ nullptr };
			proc->PopFromStack(&pop2);
			*pop2 = *m;
			proc->VirtProcessor25(pop, pop, FFXI::Constants::Values::ANGLE_3PI_OVER_2);
			pop2->MatrixInvert();
			proc->VirtProcessor3(pop, pop, pop2);
			proc->PushToStack(pop2);
		}
		if (((this->field_10C >> 6) & 7) != 0 && ((this->field_10C >> 6) & 7) <= 2 && (this->field_10C & 1) != 0) {
			*pop = cmodx->field_B50;
			pop->_41 = 0.0; pop->_42 = 0.0; pop->_43 = 0.0; pop->_44 = 1.0;
			proc->VirtProcessor25(pop, pop, FFXI::Constants::Values::ANGLE_PI);
		}
		D3DLIGHT8 light{};
		if (CYyDb::g_pCYyDb->g_pTsZoneMap->field_3949C != 0) {
			if (this->field_FC != nullptr
				&& *this->field_FC != nullptr) {
				XiZone::zone->GetAreaLightByFourCC(&light, 0, (*this->field_FC)->field_C4);
			}
			else {
				XiZone::zone->GetAreaLightByFourCC(&light, 0, 0);
			}
			proc->VirtProcessor15(&light.Direction, pop, &light.Direction);
			cmodx->field_DC8->SetLight(0, &light);
			cmodx->field_EBC->LightEnable(0, true);
			cmodx->field_EBC->LightEnable(1, false);
		}
		else {
			D3DLIGHT8 light2{};
			if (this->field_FC != nullptr
				&& *this->field_FC != nullptr) {
				XiZone::zone->GetTwoWeatherLightsByFourCC(&light, &light2, (*this->field_FC)->field_C4);
			}
			else {
				XiZone::zone->GetTwoWeatherLights(&light, &light2);
			}

			proc->VirtProcessor15(&light.Direction, pop, &light.Direction);
			proc->VirtProcessor15(&light2.Direction, pop, &light2.Direction);

			cmodx->field_DC8->SetLight(0, &light);
			cmodx->field_EBC->LightEnable(0, true);
			cmodx->field_DC8->SetLight(1, &light2);
			cmodx->field_EBC->LightEnable(1, true);
		}
		cmodx->field_EBC->LightEnable(2, false);
		cmodx->field_EBC->LightEnable(3, false);
		cmodx->field_EBC->LightEnable(4, false);
		cmodx->field_EBC->LightEnable(5, false);
		cmodx->field_EBC->LightEnable(6, false);
		cmodx->field_EBC->LightEnable(7, false);
		cmodx->field_EBC->SetRenderState(D3DRS_SPECULARENABLE, false);
		cmodx->field_EBC->SetRenderState(D3DRS_LIGHTING, true);
	}

	cmodx->field_EBC->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	cmodx->field_EBC->SetRenderState((D3DRENDERSTATETYPE)D3DRS_ZBIAS, CYyDb::g_pCYyDb->g_pTsZoneMap->SomeInt3);
	cmodx->field_EBC->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	cmodx->field_EBC->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);

	if ((this->field_10C & 0x2000000) != 0) {
		cmodx->field_EBC->SetRenderState(D3DRS_FOGENABLE, false);
		cmodx->field_EBC->SetRenderState(D3DRS_RANGEFOGENABLE, false);
	}
	else {
		if (this->field_FC != nullptr
			&& *this->field_FC != nullptr) {
			XiZone::zone->GetFogByFourCC(&CMoOT::FogColor, &CMoOT::FogEnd, &CMoOT::FogStart, 0, (*this->field_FC)->field_C4);
		}
		else {
			XiZone::zone->GetFogByFourCC(&CMoOT::FogColor, &CMoOT::FogEnd, &CMoOT::FogStart, 0, 0);
		}

		cmodx->field_EBC->SetRenderState(D3DRS_FOGCOLOR, CMoOT::FogColor);
		cmodx->field_EBC->SetRenderState(D3DRS_FOGSTART, *(DWORD*)&CMoOT::FogStart);
		cmodx->field_EBC->SetRenderState(D3DRS_FOGEND, *(DWORD*)&CMoOT::FogEnd);
		cmodx->field_EBC->SetRenderState(D3DRS_FOGENABLE, true);
		if (cmodx->field_DC8->field_92D != 0) {
			cmodx->field_EBC->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_NONE);
			cmodx->field_EBC->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);
			cmodx->field_EBC->SetRenderState(D3DRS_RANGEFOGENABLE, false);
		}
		else if (cmodx->field_DC8->field_92E != 0) {
			cmodx->field_EBC->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);
			cmodx->field_EBC->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_NONE);
			if (cmodx->field_DC8->field_92F != 0) {
				cmodx->field_EBC->SetRenderState(D3DRS_RANGEFOGENABLE, true);
			}
			else {
				cmodx->field_EBC->SetRenderState(D3DRS_RANGEFOGENABLE, false);
			}
		}
		if (v30 == true) {
			cmodx->field_EBC->SetRenderState(D3DRS_FOGCOLOR, 0);
		}
	}

	if ((this->field_10C & 0x2000000) == 0
		|| (this->field_10C & 0x10000) != 0) {
		DWORD ambient{};
		if (this->field_FC == nullptr
			|| *this->field_FC == nullptr) {
			XiZone::zone->GetAmbientByFourCC(&ambient, 0, 0);
		}
		else {
			XiZone::zone->GetAmbientByFourCC(&ambient, 0, (*this->field_FC)->field_C4);
		}
		cmodx->field_EBC->SetRenderState(D3DRS_AMBIENT, ambient);
		cmodx->field_EBC->SetMaterial(&CMoOT::Material);
	}
	
	static StatusNode PrepState{};
	PrepState.ApplyTSS(ElemTSS);

	proc->PushToStack(pop);
}

void FFXI::CYy::CMoElem::LinkToGenerator(CYyGenerator* a2)
{
	if (a2 == nullptr)
		return;

	this->field_FC = (CYyGenerator**)a2->PreHeader.PointerToThisInRPL;
	if (a2->field_B0 != nullptr) {
		a2->field_B0->field_14C = this;
	}
	this->field_14C = 0;
	this->field_150 = a2->field_B0;
	a2->field_B0 = this;
	CMoResource::DoTheThing(a2);
}

void FFXI::CYy::CMoElem::DoBlend(bool a2)
{
	CMoDx* cmodx = CYyDb::g_pCYyDb->g_pCMoDx;
	if ((cmodx->field_DCC.PrimitiveMiscCaps & D3DPMISCCAPS_BLENDOP) != 0) {
		if (a2 == false) {
			cmodx->field_EBC->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		}
		else {
			cmodx->field_EBC->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			cmodx->field_EBC->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			cmodx->field_EBC->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
		}
	}
	else {
		this->VirtElem9();
		if (CMoElem::Floats[0] < 0.0) {
			CMoElem::Floats[0] = 0.0;
		}
		if (CMoElem::Floats[1] < 0.0) {
			CMoElem::Floats[1] = 0.0;
		}

		D3DVIEWPORT8* vp = cmodx->field_DC8->GetTopViewport();
		if (CMoElem::Floats[2] > vp->Width) {
			CMoElem::Floats[2] = vp->Width;
		}
		if (CMoElem::Floats[3] > vp->Height) {
			CMoElem::Floats[3] = vp->Height;
		}

		static StatusNode BlendInitState{};
		BlendInitState.ApplyRSandTSS(BlendInitRS, BlendInitTSS);

		CMoDx::svt2->UpdateSubValues(4);
		FVF44Vertex* buf = (FVF44Vertex*)CMoDx::svt2->LockBuffer();
		buf[0].X = CMoElem::Floats[0];
		buf[0].Y = CMoElem::Floats[1];
		buf[0].Z = 0.0;
		buf[0].RHW = 1.0;
		buf[0].DiffuseColor = 0xFFFFFFFF;

		buf[1].X = CMoElem::Floats[2];
		buf[1].Y = CMoElem::Floats[1];
		buf[1].Z = 0.0;
		buf[1].RHW = 1.0;
		buf[1].DiffuseColor = 0xFFFFFFFF;

		buf[2].X = CMoElem::Floats[0];
		buf[2].Y = CMoElem::Floats[3];
		buf[2].Z = 0.0;
		buf[2].RHW = 1.0;
		buf[2].DiffuseColor = 0xFFFFFFFF;

		buf[3].X = CMoElem::Floats[2];
		buf[3].Y = CMoElem::Floats[3];
		buf[3].Z = 0.0;
		buf[3].RHW = 1.0;
		buf[3].DiffuseColor = 0xFFFFFFFF;

		CMoDx::svt2->UnlockBuffer();
		CMoDx::svt2->Render(0, 2, D3DPT_TRIANGLESTRIP, 0, -1);

		static StatusNode BlendCleanState{};
		BlendCleanState.ApplyRSandTSS(BlendCleanRS, BlendCleanTSS);
	}
}

D3DXVECTOR3* FFXI::CYy::CMoElem::GetFitPosScale()
{
	if (this->field_108 != nullptr) {
		return (D3DXVECTOR3*)(this->field_108 + 1);
	}

	CYyGenerator** gen{ nullptr };
	if (this->field_100 != nullptr) {
		gen = this->field_100;
	}
	else {
		gen = this->field_FC;
	}

	return &(*gen)->PosScale;
}

D3DXVECTOR3* FFXI::CYy::CMoElem::GetFitModelScale()
{
	if (this->field_108 != nullptr) {
		return (D3DXVECTOR3*)((char*)this->field_108 + 80);
	}

	CYyGenerator** gen{ nullptr };
	if (this->field_100 != nullptr) {
		gen = this->field_100;
	}
	else {
		gen = this->field_FC;
	}

	return &(*gen)->ModelScale;
}

FFXI::Math::WMatrix* FFXI::CYy::CMoElem::GetSomeMatrix()
{
	if (this->field_108 != nullptr) {
		return this->field_108;
	}
	
	CYyGenerator** pgen{ nullptr };
	if (this->field_100 != nullptr) {
		pgen = this->field_100;
	}
	else {
		pgen = this->field_FC;
	}

	CYyGenerator* gen{ nullptr };
	if (pgen != nullptr) {
		gen = *pgen;
	}

	if (gen->field_A4 != nullptr) {
		return gen->field_A4;
	}

	return nullptr;
}

FFXI::Math::WMatrix* FFXI::CYy::CMoElem::GetProcOrCModMatrix()
{
	BaseProcessor* proc = CYyDb::g_pCYyDb->pMoProcessor;
	unsigned int v1 = this->field_10C;
	if ((v1 & 8) != 0) {
		return &proc->field_810;
	}

	CMoDx* cmodx = CYyDb::g_pCYyDb->g_pCMoDx;

	FFXI::Math::WMatrix* pop{ nullptr };
	FFXI::Math::WMatrix* ret{ nullptr };
	//Client doesn't use pop
	proc->PopFromStack(&pop);
	int v5 = ((v1 >> 13) & 1) + 2 * ((v1 >> 14) & 1) - 1;
	if (v5 == 0) {
		ret = &cmodx->field_D10;
	}
	else if (v5 == 1) {
		ret = &cmodx->field_D50;
	}
	else if (v5 == 2) {
		ret = &cmodx->field_C10;
	}
	else {
		ret = &cmodx->field_B90;
	}

	proc->PushToStack(pop);
	return ret;
}

void FFXI::CYy::CMoElem::GetWorldPos(D3DXVECTOR3* a2)
{
	D3DXVECTOR3 retval = this->field_54;
	FFXI::Math::WMatrix* mat = this->GetSomeMatrix();
	if (mat != nullptr) {
		if (this->field_FC != nullptr) {
			D3DXVECTOR3* scale = this->GetFitPosScale();
			retval.x *= scale->x;
			retval.y *= scale->y;
			retval.z *= scale->z;
			FFXI::CYyDb::g_pCYyDb->pMoProcessor->VirtProcessor15(&retval, mat, &retval);
		}
	}

	*a2 = retval;
}

bool FFXI::CYy::CMoElem::CheckSomething()
{
	if (this->MMBFlag != 0)
		return true;

	if (this->field_FC == nullptr)
		return true;

	if (*this->field_FC == nullptr)
		return true;

	if ((*this->field_FC)->field_DE < 0)
		return true;

	this->SomeMatrixCalc();

	if ((this->field_10C & 0x40000) == 0)
		return true;

	this->SomeMatrixCalc();

	CMoDx* cmodx = CYyDb::g_pCYyDb->g_pCMoDx;
	BaseProcessor* proc = CYyDb::g_pCYyDb->pMoProcessor;
	
	static D3DXVECTOR4 v1{};
	D3DXVECTOR4 v6{};

	proc->VirtProcessor30(
		&v6, &v1, 1, &this->field_A0, &cmodx->field_B10, 
		cmodx->field_DC0, cmodx->field_DC4
	);
	return proc->field_A94 != 0;
}

void FFXI::CYy::CMoElem::CheckSomethingWasTrue()
{
	if ((double)this->field_138 < 0.0099999998) {
		this->field_138 = 1.0;
	}
	else {
		this->UpdateField34Stuff(this->field_12C);
	}
}

void FFXI::CYy::CMoElem::UpdateField34Stuff(float a2)
{
	this->field_34.field_0 = a2;
	CYyDb::g_pCYyDb->g_pCMoDx->OT->Insert(&this->field_34);
}

bool FFXI::CYy::CMoElem::ExecTag()
{
	CYyGenerator* gen{ nullptr };
	if (this->field_FC != nullptr) {
		gen = *this->field_FC;
	}

	this->field_178 = 1;

	bool b1{ false };
	if (this->field_114 == 0.0) {
		b1 = gen != nullptr;
	}
	else {
		this->Life -= CYyDb::g_pCYyDb->g_pCMoDx->field_EB0;
		if (this->Life < 1.0) {
			if (gen == nullptr) {
				delete this;
				return true;
			}
			gen->ElemDie(this);
			if (this->Life < 1.0) {
				this->field_178 = 0;
				if ((this->field_10C & 0x20000000) == 0) {
					delete this;
					return true;
				}
				this->Life = 0.0;
				if (this->field_164 != 0) {
					exit(0x10043D4A);
				}
				this->field_178 = 1;
			}
		}

		b1 = this->IsNever();
	}

	if (b1 == true) {
		if ((gen->flags & 0x200) != 0) {
			gen->Tracking(this->GetSomeMatrix());
		}
	}

	if (gen != nullptr) {
		gen->ElemIdle(this);
	}
	else {
		this->field_54 = { 0.0, 0.0, 0.0 };
		this->field_188 = { 0.0, 0.0, 0.0 };
	}

	return false;
}

void FFXI::CYy::CMoElem::CalcTrans()
{
	BaseProcessor* proc = FFXI::CYyDb::g_pCYyDb->pMoProcessor;
	CMoDx* cmodx = FFXI::CYyDb::g_pCYyDb->g_pCMoDx;

	if ((this->field_10C & 4) != 0) {
		if ( (this->field_10C & 8) != 0) {
			this->field_188 = this->field_54;
			if (this->field_FC != nullptr) {
				CYyGenerator* gen = *this->field_FC;
				if (gen != nullptr) {
					if ((gen->field_DF & 1) != 0) {
						if (this->field_188.z < 0.60000002) {
							this->field_188.z = 0.60000002;
						}
					}
				}
			}
		}
		else {
			proc->VirtProcessor15(&this->field_188, &cmodx->field_B90, &this->field_54);
		}
	}
	else if ((this->field_10C & 0x20000) != 0) {
		D3DXVECTOR3 v22 = { 0.0, 0.0, 0.0 };
		if (this->field_FC != nullptr
			&& *this->field_FC != nullptr) {
			FFXI::Math::WMatrix* mat = this->GetSomeMatrix();
			if (mat != nullptr) {
				v22.x = mat->_41;
				v22.y = mat->_42;
				v22.z = mat->_43;
			}
		}

		proc->VirtProcessor15(&this->field_188, &cmodx->field_B50, &v22);

		if ((this->field_10C & 1) == 0 || (this->field_10C & 0x1C0) != 0) {
			this->field_188 += this->field_54;
		}
		else {
			this->field_188 -= this->field_54;
		}
	}
	else {
		D3DXVECTOR3 v21{};
		this->GetWorldPos(&v21);
		proc->VirtProcessor15(&this->field_188, &cmodx->field_B50, &v21);
	}

	if ((this->field_10C & 0x400000) != 0) {
		this->field_12C = this->field_128;
	}
	else {
		if ((this->field_10C & 4) == 0 || (this->field_10C & 8) == 0) {
			this->field_12C = this->field_128 - this->field_188.z;
		}
		else {
			double v13 = this->field_128;
			if (v13 < 0) {
				v13 = -v13;
			}
			if (v13 > 0.029999999) {
				this->field_12C = (1.0 - this->field_128) - this->field_188.z;
			}
			else {
				this->field_12C = this->field_128 - this->field_188.z;
			}
		}
	}

	this->field_130 = 0;
	if ((this->field_10C & 0x8000000) != 0) {
		this->field_12C = (this->field_128 - this->field_188.z) + 400.0;
	}
	this->field_13C = this->field_188.z;
}

void FFXI::CYy::CMoElem::CalcMatrix()
{
	if (this->ModelType == 33) {
		if ((*this->field_FC)->Header.FourCC == 'mkat') {
			int a = 1;
		}
	}
	bool v4{};
	if (this->field_187 != 0) {
		v4 = true;
	}
	else if (this->field_108 != nullptr) {
		v4 = false;
	}
	else if (this->field_FC != nullptr) {
		v4 = (*this->field_FC)->field_A4 != nullptr;
	}
	else {
		//this shouldn't ever happen
		exit(0x10043EE4);
	}

	this->field_187 = 0;
	if (v4 == false) {
		return;
	}

	BaseProcessor* proc = FFXI::CYyDb::g_pCYyDb->pMoProcessor;
	proc->MatrixIdentity(&this->field_60);

	if ((this->field_10C & 2) == 0) {
		this->field_60._11 = this->field_EC.x;
		this->field_60._22 = this->field_EC.y;
		this->field_60._33 = this->field_EC.z;

		if (this->field_FC != nullptr
			&& *this->field_FC != nullptr
			&& ((*this->field_FC)->CMoAttachments::field_4 & 0xF000000) != 0) {
			D3DXVECTOR3* scale = this->GetFitModelScale();
			 this->field_60._11 *= scale->x;
			 this->field_60._22 *= scale->y;
			 this->field_60._33 *= scale->z;
		}
	}

	FFXI::Math::WMatrix* v10{};
	proc->PopFromStack(&v10);

	if ((this->field_10C & 0x200) != 0) {
		proc->VirtProcessor22(v10, this->field_E0.x, this->field_E0.y, this->field_E0.z);
	}
	else {
		proc->VirtProcessor17(v10, this->field_E0.x, this->field_E0.y, this->field_E0.z);
	}
	proc->VirtProcessor3(&this->field_60, &this->field_60, v10);

	if ((this->field_10C & 2) != 0) {
		proc->MatrixIdentity(v10);

		v10->_11 *= this->field_EC.x;
		v10->_22 *= this->field_EC.y;
		v10->_33 *= this->field_EC.z;

		if (this->field_FC != nullptr
			&& *this->field_FC != nullptr
			&& ((*this->field_FC)->CMoAttachments::field_4 & 0xF000000) != 0) {
			D3DXVECTOR3* scale = this->GetFitModelScale();
			v10->_11 *= scale->x;
			v10->_22 *= scale->y;
			v10->_33 *= scale->z;
		}

		proc->VirtProcessor3(&this->field_60, &this->field_60, v10);
	}

	proc->PushToStack(v10);
}

void FFXI::CYy::CMoElem::CalcRotAhead(D3DXVECTOR3 a2)
{
	if (this->field_FC == nullptr)
		return;

	CYyGenerator* gen = *this->field_FC;
	if (gen == nullptr)
		return;

	unsigned int flag = (this->field_10C >> 6) & 7;
	if (flag == 0)
		return;

	BaseProcessor* proc = CYyDb::g_pCYyDb->pMoProcessor;
	FFXI::Math::WMatrix* first_pop{}, * second_pop{};
	D3DXVECTOR3 v39{}, v40{}, v41{};

	proc->PopFromStack(&first_pop);
	proc->PopFromStack(&second_pop);

	if (flag <= 2) {
		Math::WMatrix* somemat = this->GetSomeMatrix();
		unsigned int v16 = (gen->CMoAttachments::field_4 & 0xF) + 16 * ((gen->CMoAttachments::field_4 >> 16) & 1);
		if (v16 == 3 || v16 == 6) {
			exit(0x100444F9);
			if (somemat != nullptr) {
				*first_pop = *somemat;
			}
			else {
				proc->MatrixIdentity(first_pop);
			}
			first_pop->_41 = 0.0;
			first_pop->_42 = 0.0;
			first_pop->_43 = 0.0;
			first_pop->_44 = 1.0;
			proc->Vec3Normalize((D3DXVECTOR3*) &(first_pop->m[0][0]));
			proc->Vec3Normalize((D3DXVECTOR3*) &(first_pop->m[1][0]));
			proc->Vec3Normalize((D3DXVECTOR3*) &(first_pop->m[2][0]));
			*second_pop = *first_pop;

			
			D3DXVECTOR3 vec_one = this->field_54;
			D3DXVECTOR3 vec_two{};
			if (somemat != nullptr) {
				D3DXVECTOR3* posscale = this->GetFitPosScale();
				vec_one.x *= posscale->x;
				vec_one.y *= posscale->y;
				vec_one.z *= posscale->z;
				proc->VirtProcessor15(&vec_one, somemat, &vec_one);
				vec_two.x = a2.z;
				
				
				//proc->VirtProcessor15(&a2, somemat, &a2);
			}

			first_pop->MatrixInvert();
			//sub //TODO verify this
			D3DXVECTOR3 diff = vec_one - a2;
			proc->VirtProcessor15(&diff, first_pop, &diff);
			if ((this->field_10C & 1) != 0) {
				proc->MatrixMultiply(first_pop, &CYyDb::g_pCYyDb->g_pCMoDx->field_B50, second_pop);
				first_pop->MatrixInvert();
				D3DXVECTOR3 thisguy = { 0.0, 0.0, -1.0 };
				proc->VirtProcessor15(&thisguy, first_pop, &thisguy);
			}
		}
		else {
			if ((this->field_10C & 1) != 0) {
				v40 = { 0.0, 0.0, -1.0 };
				if (v16 == 20 || (v16 < 1 || v16 >= 14) && v16 <= 15) {
					proc->VirtProcessor15(&v40, &CYyDb::g_pCYyDb->g_pCMoDx->field_C10, &v40);
				}
				else {
					if (somemat != nullptr) {
						*first_pop = *somemat;
						first_pop->_41 = 0.0;
						first_pop->_42 = 0.0;
						first_pop->_43 = 0.0;
						first_pop->_44 = 1.0;
					}
					else {
						proc->MatrixIdentity(first_pop);
					}
					proc->MatrixMultiply(first_pop, &CYyDb::g_pCYyDb->g_pCMoDx->field_B90, first_pop);
					first_pop->MatrixInvert();
					proc->VirtProcessor15(&v40, first_pop, &v40);
				}
			}
			v41 = this->field_54;
			v41 -= a2;
			v39 = v41;
		}
		first_pop->Identity();
		if ((this->field_10C & 1) != 0) {
			proc->VirtProcessor10(&v41.x, &v39.x, &v40.x);
			proc->VirtProcessor34(&first_pop->_21, &v41.x);
			proc->VirtProcessor34(&first_pop->_11, &v39.x);
			proc->VirtProcessor10(&first_pop->_31, &first_pop->_31, &first_pop->_11);
		}
		else {
			if ((this->field_10C & 0x1C0) != 128) {
				float v33 = atan2(v39.y, sqrt(v39.x * v39.x + v39.z * v39.z));
				proc->VirtProcessor21(second_pop, v33);
				proc->MatrixMultiply(first_pop, first_pop, second_pop);
			}
			float v34 = -atan2(v39.z, v39.x);
			proc->VirtProcessor20(second_pop, v34);
			proc->MatrixMultiply(first_pop, first_pop, second_pop);
		}

		proc->VirtProcessor3(&this->field_60, &this->field_60, first_pop);
		this->field_187 = 1;
	}
	else if (flag == 3) {
		FFXI::Math::WMatrix* sm = this->GetSomeMatrix();
		if (sm != nullptr) {
			*first_pop = *sm;
			first_pop->MatrixInvert();
		}
		else {
			proc->MatrixIdentity(first_pop);
		}

		D3DXVECTOR3 vec_int = CYyDb::g_pCYyDb->CameraManager->Position;
		proc->VirtProcessor15(&vec_int, first_pop, &vec_int);
		//sub //TODO verify this
		D3DXVECTOR3 sub = vec_int - this->field_54;
		if ((this->field_10C & 0x1C0) != 0x80) {
			float v39 = atan2(sub.y, sqrt(sub.z * sub.z + sub.x * sub.x));
			proc->VirtProcessor21(first_pop, v39);
		}
		float v40 = -atan2(sub.z, sub.x);
		proc->VirtProcessor20(second_pop, v40);
		proc->MatrixMultiply(first_pop, first_pop, second_pop);
		proc->VirtProcessor3(&this->field_60, &this->field_60, first_pop);
		this->field_187 = 1;
	}
	proc->PushToStack(second_pop);
	proc->PushToStack(first_pop);
}

void FFXI::CYy::CMoElem::SomeMatrixCalc()
{
	BaseProcessor* proc = CYyDb::g_pCYyDb->pMoProcessor;
	Math::WMatrix* pop{};
	proc->PopFromStack(&pop);
	bool v1{ false };
	if ((this->field_10C & 4) != 0
		&& (this->field_10C & 8) != 0) {
		this->field_A0 = this->field_60;
		this->field_A0.m[3][0] = this->field_188.x;
		this->field_A0.m[3][1] = this->field_188.y;
		this->field_A0.m[3][2] = this->field_188.z;

		proc->MatrixIdentity(pop);
		pop->_22 = -1.0;
		proc->VirtProcessor3(&this->field_A0, pop, &this->field_A0);
		v1 = true;
	}
	else if ((this->field_10C & 1) != 0
		&& (this->field_10C & 0x1C0) == 0) {
		proc->VirtProcessor24(&this->field_A0, &this->field_60, FFXI::Constants::Values::ANGLE_PI);
		this->field_A0.m[3][0] = this->field_188.x;
		this->field_A0.m[3][1] = this->field_188.y;
		this->field_A0.m[3][2] = this->field_188.z;
		v1 = true;
	}

	if (v1 == true) {
		double v43 = this->field_A0._43;
		if (v43 < 0.0) {
			v43 = -v43;
		}
		if (v43 >= 0.2) {
			if ((this->field_10C & 4) != 0
				&& (this->field_10C & 8) != 0) {
				this->field_A0._43 = -this->field_A0._43;
				this->field_A0._42 = -this->field_A0._42;
			}
		}
		else if (this->field_A0._43 != 0.0) {
			double v10 = 0.2 / this->field_A0._43;
			this->field_A0._43 *= -v10;
			this->field_A0._11 *= v10;
			this->field_A0._22 *= v10;
			this->field_A0._41 *= v10;
			this->field_A0._42 *= -v10;
		}
	}
	else {
		Math::WMatrix* v12 = this->GetProcOrCModMatrix();
		if ((this->field_10C & 4) != 0) {
			this->field_A0 = this->field_60;
			this->field_A0.m[3][0] = this->field_54.x;
			this->field_A0.m[3][1] = this->field_54.y;
			this->field_A0.m[3][2] = this->field_54.z;
			proc->VirtProcessor3(&this->field_A0, &this->field_A0, v12);
		}
		else if ((this->field_10C & 0x20000) != 0) {
			this->field_A0 = this->field_60;
			this->field_A0.m[3][0] = this->field_188.x;
			this->field_A0.m[3][1] = this->field_188.y;
			this->field_A0.m[3][2] = this->field_188.z;
		}
		else {
			Math::WMatrix* somemat = this->GetSomeMatrix();
			if ((this->field_10C & 0x2000) != 0
				|| (this->field_10C & 0x4000) != 0
				|| somemat == nullptr) {
				proc->VirtProcessor3(&this->field_A0, &this->field_60, v12);
			}
			else {
				CYyGenerator** v14 = this->field_100;
				if (v14 == nullptr) {
					v14 = this->field_FC;
				}
				if (v14 != nullptr && *v14 != nullptr && somemat != nullptr) {
					*pop = *somemat;
					pop->_41 = 0.0;	pop->_42 = 0.0;	pop->_43 = 0.0;	pop->_44 = 1.0;
					proc->VirtProcessor3(&this->field_A0, &this->field_60, pop);
				}
				proc->VirtProcessor3(&this->field_A0, &this->field_A0, v12);
			}
			this->field_A0.m[3][0] = this->field_188.x;
			this->field_A0.m[3][1] = this->field_188.y;
			this->field_A0.m[3][2] = this->field_188.z;
		}
	}
	proc->PushToStack(pop);
}

char* FFXI::CYy::CMoElem::ElemAllocate(int a1, unsigned char a2)
{
	return StorageLists::instance->GetOrUpper(a1 + 4 * a2, FFXI::Constants::Enums::MEM_MODE::Work);
}
