#include "CYyVbMng.h"
#include "CYyVb.h"
#include "CYyDb.h"
#include "Globals.h"
#include "CDx.h"
#include <iostream>
using namespace FFXI::CYy;

const CYyClass CYyVbMng::CYyVbMngClass{
	"CYyVbMng", sizeof(CYyVbMng), &CYyObject::CYyObjectClass
};

int VBLockSizes[] = { 0x7D0, 0xFA0, 0x1F40, 0x2EE0 };
const CYyClass* FFXI::CYy::CYyVbMng::GetRuntimeClass()
{
	return &CYyVbMngClass;
}

FFXI::CYy::CYyVbMng::~CYyVbMng()
{
	CYyVb* v3{ nullptr }, * vb{ nullptr };
	vb = CYyDb::pCYyVb->field_8;
	if (vb) {
		do {
			v3 = vb->field_8;
			delete vb;
			vb = v3;
		} while (v3);
	}
	if (CYyDb::pCYyVb) {
		delete CYyDb::pCYyVb;
		CYyDb::pCYyVb = nullptr;
	}
}

FFXI::CYy::CYyVbMng::CYyVbMng()
{
	IDirect3DVertexBuffer8* v6{ nullptr };
	LPBYTE v7{ nullptr };
	int* v3 = VBLockSizes;

	char* mem = StorageLists::instance->Get(sizeof(CYyVb), Constants::Enums::MEM_MODE::Ex);
	if (mem)
		CYyDb::pCYyVb = new (mem) CYyVb();

	this->field_4 = 0;
	this->field_8 = 0;
	int v8{ 0 };

	while (this->InitBuffer(*v3, 520, 0, D3DPOOL_DEFAULT, &v6, 1) >= 0) {
		while (v6->Lock(0, *v3, &v7, NULL) != D3D_OK)
			Sleep(0);
		for (int i = 1; i <= *v3; ++i)
			v7[i - 1] = 0;
		while (v6->Unlock() != D3D_OK)
			Sleep(0);
		DoSomething(&v6);
		if (++v8 >= 50) {
			++v3;
			if (*v3) {
				v8 = 0;
				continue;
			}
			return;
		}

	}
}

void FFXI::CYy::CYyVbMng::DoSomething(IDirect3DVertexBuffer8** a1)
{
	CYyVb* v1 = CYyDb::pCYyVb->field_8;

	while (v1 != nullptr) {
		if (v1->field_4 == 3 && v1->field_1C == *a1) {
			v1->field_4 -= 1;
			return;
		}
			
		v1 = v1->field_8;
	}
}

int FFXI::CYy::CYyVbMng::InitBuffer(int p_LockSize, int a3, int a4, D3DPOOL a5, IDirect3DVertexBuffer8** a6, int a7)
{
	unsigned int uVar8 = ((p_LockSize + 1999) / 2000) * 2000;
	if (!a7) {
		CYyVb* piVar1 = CYyDb::pCYyVb->field_8;
		CYyVb* piVar2 = piVar1;
		CYyVb* piVar4{ nullptr }, * piVar5{ nullptr }, * piVar7{ nullptr };
		while (piVar5 = piVar2, piVar7 = piVar4, piVar5 != nullptr) {
			piVar2 = piVar5->field_8;
			if (piVar5->field_10 == a3 &&
				piVar5->field_18 == a5 &&
				piVar5->field_14 == a4 &&
				piVar5->field_4 == 0) {
				unsigned int uVar3 = piVar5->field_C;
				piVar7 = piVar5;
				if (uVar3 == uVar8) break;
				if (uVar8 <= uVar3 &&
					(piVar4 == nullptr || (uVar3 <= piVar4->field_C && piVar4->field_C != uVar3))) {
					piVar4 = piVar5;
				}
			}
		}
		while (piVar2 = piVar1, piVar2 != nullptr) {
			piVar1 = piVar2->field_8;
			if (piVar7 != piVar2) {
				int iVar6 = piVar2->field_4;
				if (iVar6 == 0) {
					if (300 < this->field_4) {
						delete piVar2;
						this->field_4 -= 1;
					}
				}
				else if (iVar6 < 3) {
					piVar2->field_4 = iVar6 - 1;
				}
			}
		}
		if (piVar7 != nullptr) {
			piVar7->field_4 = 3;
			*a6 = piVar7->field_1C;
			return 0;
		}
	}
	IDirect3DDevice8* DXDevice = FFXI::CYy::CDx::instance->DXDevice;
	if (DXDevice == nullptr) 
		return -1;

	HRESULT result = DXDevice->CreateVertexBuffer(uVar8, a3, a4, a5, a6);
	if (result < D3D_OK) 
		return result;

	char* mem = StorageLists::instance->Get(sizeof(CYyVb), Constants::Enums::MEM_MODE::Ex);
	CYyVb* newvb{ nullptr };
	if (mem)
		newvb = new (mem) CYyVb(uVar8, a3, a4, a5, *a6);
	else 
		return -1;

	if (!a7) {
		this->field_4 += 1;
		this->field_8 += uVar8;
	}
	else {
		newvb->field_4 = 4;
	}
	return result;
}
