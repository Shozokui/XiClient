#include "CYyModelBase.h"
#include "CMoSk2.h"
#include "CMoOs2.h"
#include "Globals.h"
#include "StorageLists.h"
#include "CYyBasicDt.h"
#include "CYyAdvancedDt.h"
#include "WMatrix.h"
#include "CXiSkeletonActor.h"
#include "KzCibCollect.h"
#include "CYyModel.h"
#include "CDx.h"
#include "CYyDb.h"
#include "CYyTexMng.h"
#include "CMoResourceMng.h"
#include "CYyOs2VtxBuffer.h"
#include "CYyVbMng.h"
using namespace FFXI::CYy;

const CYyClass CYyModelBase::CYyModelBaseClass{
	"CYyModelBase", sizeof(CYyModelBase), &CYyObject::CYyObjectClass
};

void FFXI::CYy::CYyModelBase::AddDt(CYyModelDt* a2)
{
	CYyModelDt** next = &this->ModelDt;
	while (*next != nullptr) {
		next = &(*next)->field_4;
	}
	*next = a2;
}

const CYyClass* FFXI::CYy::CYyModelBase::GetRuntimeClass()
{
	return &CYyModelBase::CYyModelBaseClass;
}

FFXI::CYy::CYyModelBase::CYyModelBase()
{
	this->Previous = 0;
	this->ModelDt = nullptr;
	this->field_B0 = { 20.0, 0.0, 0.0, -1.0 };
	this->field_C0 = 0;
	this->field_C4 = (CYyTex*) -1;
}

FFXI::CYy::CYyModelBase::~CYyModelBase()
{
	if (this->Skeleton.Resource != nullptr) {
		(*this->Skeleton.Resource)->SomeClean3();
	}

	if (this->Previous != nullptr) {
		delete this->Previous;
		this->Previous = nullptr;
	}

	if (this->ModelDt != nullptr) {
		delete this->ModelDt;
		this->ModelDt = nullptr;
	}

	for (int i = 0; i < sizeof(this->field_24.motions) / sizeof(this->field_24.motions[0]); ++i) {
		this->field_24.motions[i].DeleteAll();
	}
}

void FFXI::CYy::CYyModelBase::SetSkeleton(CMoSk2** a2)
{
	this->Skeleton.Resource = a2;
	this->Skeleton.Init();
	this->Init();
}

void FFXI::CYy::CYyModelBase::Init()
{
	if (this->Skeleton.ResetFlagArray() == false)
		return;

	CYyModelDt** dt = this->GetModelDt();
	while (*dt != nullptr) {
		(*dt)->VirtModelDt4(this->Skeleton.field_14);
		dt = &(*dt)->field_4;
	}

	int v7 = this->GetSomeIndex(0x7F);
	if (v7)
		this->Skeleton.field_14[v7] = 0x80;

	int v9 = this->GetSomeIndex(0x7E);
	if (v9)
		this->Skeleton.field_14[v9] = 0x80;

	unsigned short count = *(unsigned short*)((*this->Skeleton.Resource)->Data + 2);
	for (int j = count - 1; j >= 0; --j) {
		if (this->Skeleton.field_14[j] != 0)
			this->Skeleton.RecurseSetFlags(*this->Skeleton.Resource, count, j);
	}
}

CYyModelDt** FFXI::CYy::CYyModelBase::GetModelDt()
{
	return &this->ModelDt;
}

int FFXI::CYy::CYyModelBase::GetSomeIndex(unsigned int a2)
{
	short* retval = this->Skeleton.GetSomeShortPtr(a2);
	if (retval == nullptr)
		return 0;

	if (a2 < 0x80)
		return *retval;

	return 0;
}

void FFXI::CYy::CYyModelBase::LinkOs2(CMoOs2** a2)
{
	CMoOs2* os2{ nullptr };
	if (a2)
		os2 = *a2;
	//os2 can be nullptr as per client

	CYyModelDt* dt{ nullptr };
	if ( (os2->Data[2] & 0x7F) == 0) {
		char* mem = StorageLists::instance->Get(sizeof(CYyBasicDt), FFXI::Constants::Enums::MEM_MODE::Ex);
		if (mem) {
			dt = new (mem) CYyBasicDt();
		}
	}
	else if ( (os2->Data[2] & 0x7F) == 1) {
		char* mem = StorageLists::instance->Get(sizeof(CYyAdvancedDt), FFXI::Constants::Enums::MEM_MODE::Ex);
		if (mem) {
			CYyAdvancedDt* adt = new (mem) CYyAdvancedDt();
			adt->Init(os2);
			dt = adt;
		}
	}

	//dt can be nullptr as per the client
	dt->field_8 = a2;
	this->AddDt(dt);
	this->Init();
}

bool FFXI::CYy::CYyModelBase::DoingSomething(CXiActor* a2, FFXI::Math::WMatrix* a3, float a4, D3DXVECTOR4* a5, int a6)
{
	bool FlagOne = a2->IsConstrain();
	bool FlagTwo{ FlagOne }, FlagThree{ FlagOne };

	int v1920 = a2->VirtActor192(0);
	int v1921 = a2->VirtActor192(1);

	if ((v1920 | v1921) != 0) {
		FlagTwo = v1920;
		FlagThree = v1921;
	}

	char flags[4] = { -1, -1, -1, -1 };
	if (FlagThree || FlagTwo) {
		KzCibCollect* cib = a2->VirtActor236();
		if (cib->field_10 != -1 && FlagTwo == true) {
			flags[0] = this->GetSomeIndex(0x7E);
			if (flags[0] != 0) {
				flags[2] = this->GetSomeIndex(cib->field_10);
				FlagOne = true;
			}
		}

		if (cib->constrain_no != 1 && FlagThree == true) {
			flags[1] = this->GetSomeIndex(0x7F);
			if (flags[1] != 0) {
				flags[3] = this->GetSomeIndex(cib->constrain_no);
				FlagOne = true;
			}
		}
	}

	if (this->Skeleton.SetStatics() == false)
		return false;

	if (this->field_24.SetStatics() == false 
		&& (a6 & 0x10) == 0)
		return false;

	int value{ 0 };
	if (a5 != 0) {
		int setter{ 0 };
		value = this->GetSomeIndex(3);
		a6 = value;
		if (value != 0) {
			setter = this->GetSomeIndex(7);
			if (setter != 0) {
				if (value < setter)
					value = setter;
				else
					a6 = setter;
			}
		}
		if (setter != -1)
			value = 0;
	}

	this->Skeleton.SetStatics2();
	if (FlagOne == true)
		this->Skeleton.DoingSomething(a3, value, 0, flags);
	else
		this->Skeleton.DoingSomething(a3, value, 0, nullptr);

	if (value != 0) {
		int gamestatus = a2->GetGameStatus();
		exit(0x1002950E);
	}
	if (FlagOne == true)
		this->Skeleton.DoingSomething(a3, 0, 1, flags);
	this->Skeleton.DoingSomething(a3, 0, 2, nullptr);
	return true;
}

void FFXI::CYy::CYyModelBase::GetSomeVector(int a2, D3DXVECTOR3* a3)
{
	short* sptr = this->Skeleton.GetSomeShortPtr(a2);
	if (sptr != nullptr) {
		float* fptr = (float*)(sptr + 7);
		a3->x += fptr[0];
		a3->y += fptr[1];
		a3->z += fptr[2];
	}
}

void FFXI::CYy::CYyModelBase::TWO_GetSomeVector(int a2, D3DXVECTOR4* a3)
{
	*a3 = { 0.0, 0.0, 0.0, 1.0 };
	short* sptr = this->Skeleton.GetSomeShortPtr(a2);
	if (sptr == nullptr)
		return;

	float* fptr = (float*)(sptr + 1);
	FFXI::Math::WMatrix v6{};
	v6.Identity();
	v6.RotateX(fptr[0]);
	v6.RotateY(fptr[1]);
	v6.RotateZ(fptr[2]);

	D3DXVECTOR3 v5{};
	v5.x = fptr[3];
	v5.y = fptr[4];
	v5.z = fptr[5];
	v6.AddTranslation3(&v5);
	v6.MatrixMultiply(this->Skeleton.field_C + sptr[0]);
	v6.Vec4MultiplySelf(a3);
}

void FFXI::CYy::CYyModelBase::Draw(CXiActor* a2, CYyModel* a3, float a4)
{
	CDx* cdx = FFXI::CYy::CDx::instance;
	if (this->field_C4 == (CYyTex*) -1) {
		this->field_C4 = CYyDb::g_pCYyDb->pCYyTexMng->FindD3sTexUnder("cubemap spec    ", (CMoResource*) *CYyDb::g_pCYyDb->pCMoResourceMng->Unknown3);
	}

	cdx->DXDevice->SetRenderState(D3DRS_AMBIENT, CXiSkeletonActor::ActorAmbientLight);

	for (int i = 0; i < 2; ++i) {
		D3DLIGHT8* light = CXiSkeletonActor::g_light_arr + i;
		if (light->Type == NULL) {
			cdx->DXDevice->LightEnable(i, false);
		}
		else {
			cdx->SetLight(i, light);
			cdx->DXDevice->LightEnable(i, true);
		}
	}

	cdx->DXDevice->SetRenderState(D3DRS_LIGHTING, true);
	CYyModelDt* dt = *this->GetModelDt();

	while (dt != nullptr) {
		IDirect3DVertexBuffer8** buffs = dt->field_10.field_C;
		CYyOs2VtxBuffer* vtxbuff = dt->VirtModelDt5();
		if (vtxbuff != nullptr) {
			if (vtxbuff->SomeCount != 0) {
				if (dt->field_10.field_4 == 0) {
					if (buffs[0] == nullptr
						&& CYyDb::g_pCYyDb->pCYyVbMng->InitBuffer(
							4 + 12 * vtxbuff->SomeCount,
							520, 0, Globals::g_VertexBufferD3DPool,
							buffs, 0) < D3D_OK
						|| buffs[2] == nullptr
						&& CYyDb::g_pCYyDb->pCYyVbMng->InitBuffer(
							4 + 12 * vtxbuff->SomeCount,
							520, 0, Globals::g_VertexBufferD3DPool,
							buffs + 2, 0) < D3D_OK
						|| (((*vtxbuff->Os2Resource)->Data[4] & 1) != 0)
						&& (buffs[4] == nullptr
							&& CYyDb::g_pCYyDb->pCYyVbMng->InitBuffer(
								4 + 12 * vtxbuff->SomeCount,
								520, 0, Globals::g_VertexBufferD3DPool,
								buffs + 4, 0) < D3D_OK
							|| buffs[6] == nullptr
							&& CYyDb::g_pCYyDb->pCYyVbMng->InitBuffer(
								4 + 12 * vtxbuff->SomeCount,
								520, 0, Globals::g_VertexBufferD3DPool,
								buffs + 6, 0) < D3D_OK)){
						//one of the above calls failed..
						for (int i = 0; i < 2; ++i) {
							if (buffs[i] != nullptr) {
								CYyDb::g_pCYyDb->pCYyVbMng->DoSomething(buffs + i);
								buffs[i] = nullptr;
							}
							if (buffs[2 + i] != nullptr) {
								CYyDb::g_pCYyDb->pCYyVbMng->DoSomething(buffs + 2 + i);
								buffs[2 + i] = nullptr;
							}
							if (buffs[4 + i] != nullptr) {
								CYyDb::g_pCYyDb->pCYyVbMng->DoSomething(buffs + 4 + i);
								buffs[4 + i] = nullptr;
							}
							if (buffs[6 + i] != nullptr) {
								CYyDb::g_pCYyDb->pCYyVbMng->DoSomething(buffs + 6 + i);
								buffs[6 + i] = nullptr;
							}
						}
					}

					dt->field_10.field_4 = vtxbuff->SomeCount;
				}
			}
		}
		dt = dt->field_4;
	}

	int count1{}, count2{};
	dt = *this->GetModelDt();
	while (dt != nullptr) {
		if (dt->field_C == 0) {
			CYyOs2VtxBuffer* vtxbuff = dt->VirtModelDt5();
			if (vtxbuff != nullptr) {
				vtxbuff->field_44 = dt->field_10.field_C[dt->field_10.field_8];
				vtxbuff->field_48 = dt->field_10.field_C[dt->field_10.field_8 + 2];
				vtxbuff->field_50 = dt->field_10.field_C[dt->field_10.field_8 + 4];
				vtxbuff->field_54 = dt->field_10.field_C[dt->field_10.field_8 + 6];
			}
			dt->VirtModelDt1(a2, a3, this, a4, &count1, &count2);
		}
		dt = dt->field_4;
	}

	for (int i = 0; i < 4; i++) {
		cdx->DXDevice->LightEnable(i, false);
	}
}
