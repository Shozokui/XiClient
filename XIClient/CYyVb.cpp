#include "CYyVb.h"
#include "CYyDb.h"
using namespace FFXI::CYy;

const CYyClass CYyVb::CYyVbClass{
	"CYyVb", sizeof(CYyVb), &CYyObject::CYyObjectClass
};
const CYyClass* FFXI::CYy::CYyVb::GetRuntimeClass()
{
	return &CYyVbClass;
}

FFXI::CYy::CYyVb::~CYyVb()
{
	if (this == CYyDb::pCYyVb) return;
	if (this->field_1C) {
		this->field_1C->Release();
		this->field_1C = nullptr;
	}
	CYyVb* v4{ nullptr }, * vb = CYyDb::pCYyVb->field_8;
	if (!vb) return;
	while (vb != this) {
		v4 = vb;
		vb = vb->field_8;
		if (!vb) return;
	}
	vb = this->field_8;
	if (v4)
		v4->field_8 = vb;
	else
		CYyDb::pCYyVb->field_8 = vb;
	this->field_8 = nullptr;
}

FFXI::CYy::CYyVb::CYyVb()
{
	this->field_8 = nullptr;
}

FFXI::CYy::CYyVb::CYyVb(UINT a2, DWORD a3, DWORD a4, D3DPOOL a5, IDirect3DVertexBuffer8* a6)
{
	this->field_8 = nullptr;
	
	CYyVb* v2 = CYyDb::pCYyVb->field_8;
	CYyDb::pCYyVb->field_8 = this;
	this->field_8 = v2;

	this->field_C = a2;
	this->field_10 = a3;
	this->field_18 = a5;
	this->field_4 = 3;
	this->field_14 = a4;
	this->field_1C = a6;
}
