#include "CMoVtxList.h"
#include "CMoSpline.h"
#include "Globals.h"
#include "StorageLists.h"

void FFXI::CMoVtxList::GetSplinePos(D3DXVECTOR3* a2, float a3)
{
	FFXI::CYy::CMoSpline* spl = this->GetSpline();
	spl->Get(a3, a2);
}

void FFXI::CMoVtxList::RemoveRef()
{
	this->field_8 -= 1;
	if (this->field_8 == 0) {
		if (this->spline != nullptr) {
			delete this->spline;
			this->spline = nullptr;
		}
	}
}

float* FFXI::CMoVtxList::GetFloatPtr(int a2)
{
	return (float*)this + 4 + 4 * (a2 % this->field_0);
}

FFXI::CYy::CMoSpline* FFXI::CMoVtxList::GetSpline()
{
	if (this->spline != nullptr) {
		return this->spline;
	}

	char* mem = StorageLists::instance->Get(sizeof(FFXI::CYy::CMoSpline), FFXI::Constants::Enums::MEM_MODE::Ex);
	if (mem != nullptr) {
		this->spline = new (mem) FFXI::CYy::CMoSpline();
	}
	this->spline->AllocTable(this->field_0, 7, 0, 0);
	float* c0 = (float*)this->spline->field_C[0] + 1;
	float* c1 = (float*)this->spline->field_C[1] + 1;
	float* c2 = (float*)this->spline->field_C[2] + 1;
	for (int i = 0; i < this->field_0; ++i) {
		float* fptr = this->GetFloatPtr(i);
		c0[i] = fptr[0];
		c1[i] = fptr[1];
		c2[i] = fptr[2];
	}
	this->spline->CalcTable();
	return this->spline;
}
