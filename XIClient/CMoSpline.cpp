#include "CMoSpline.h"
#include "Globals.h"
#include "CYyDb.h"
#include "BaseProcessor.h"
using namespace FFXI::CYy;

//~LOCAL FUNCS
void FFXI::CYy::CMoSpline::ReleaseTable()
{
	for (int i = 0; i < sizeof(this->field_C) / sizeof(this->field_C[0]); ++i) {
		if (this->field_C[i]) {
			CYyObject::Unwrap(this->field_C[i]);
			this->field_C[i] = nullptr;
		}
	}

	if (this->field_20) {
		CYyObject::Unwrap(this->field_20);
		this->field_20 = nullptr;
	}
	if (this->field_18) {
		CYyObject::Unwrap(this->field_18);
		this->field_18 = nullptr;
	}
}

FFXI::CYy::CMoSpline::CMoSpline()
{
	this->field_18 = 0;
	this->field_C[2] = 0;
	this->field_C[1] = 0;
	this->field_C[0] = 0;
	this->field_20 = 0;
}

FFXI::CYy::CMoSpline::~CMoSpline()
{
	this->ReleaseTable();
}

void FFXI::CYy::CMoSpline::CalcTable()
{
	BaseProcessor* proc = CYyDb::g_pCYyDb->pMoProcessor;
	for (int i = 0; i < sizeof(this->field_C) / sizeof(this->field_C[0]); ++i) {
		if (this->field_C[i]) {
			float* v3 = (float*)this->field_C[i];
			if (this->field_24) {
				v3[0] = v3[this->field_4 - 1];
				v3[this->field_4] = v3[1];
				v3[this->field_4 + 1] = v3[2];
			}
			else {
				v3[0] = v3[1] - (v3[2] - v3[1]);
				v3[this->field_4 + 1] = v3[this->field_4] - v3[this->field_4 - 1] + v3[this->field_4];
			}
		}
	}

	float* v4 = (float*)this->field_C[0];
	float* v5 = (float*)this->field_C[2];
	float* v6 = this->field_18 + 1;
	float* v34 = v4;
	float* v8 = (float*)this->field_C[1];
	float* v31 = v8;
	float* v28 = (float*)this->field_C[2];
	D3DXVECTOR3 v36{};
	memset(&v36, 0, sizeof(v36));
	this->field_8 = 0.0;
	if (this->field_4 > 1) {
		for (int i = 1; i < this->field_4; ++i) {
			if (v34)
			{
				v36.x = v34[2] - v34[1];
				++v34;
			}
			if (v8)
			{
				v36.y = v8[2] - v8[1];
				v31 = ++v8;
			}
			if (v5)
			{
				v36.z = v5[2] - v5[1];
				v28 = ++v5;
			}
			if (this->field_25)
				*v6 = 1.0;
			else if (v36.x == 0.0 && v36.y == 0.0 && v36.z == 0.0)
				*v6 = 0.01f;
			else {
				*v6 = proc->Vec3Magnitude(&v36);
				if (*v6 < 0.1f)
					*v6 = 0.1f;
			}
			this->field_8 += *v6++;
		}
	}

	*this->field_18 = this->field_18[1];
	this->field_18[this->field_4] = this->field_18[this->field_4 - 1];
	if (this->field_4 > 1) {
		int v18 = 2;
		float* v19 = this->field_18;
		for (int i = 0; i < this->field_4 - 1; ++i) {
			float v32 = v19[v18 - 1];
			float v20 = v19[v18 - 2] / (v32 + v19[v18 - 2]);
			float v21 = v19[v18] + v32;
			float v22 = v32 / v21;
			float v33 = v22 * v20;
			float v29 = 1.0 - v20;
			float v35 = (1.0 - v20) * v29;
			float v23 = -v35 * (1.0 / v20);
			float v24 = v22 * v22 / (1.0 - v22);
			this->field_1C[i]._11 = v23;
			this->field_1C[i]._12 = (v29 + v33) * (1.0 / v20);
			this->field_1C[i]._13 = (-v29 - v33) / (1.0 - v22);
			this->field_1C[i]._14 = v24;
			this->field_1C[i]._21 = (v35 + v35) * (1.0 / v20);
			this->field_1C[i]._22 = (v29 * -2.0 - v33) * (1.0 / v20);
			this->field_1C[i]._23 = (v29 + v29 - (v22 - (v33 + v33))) / (1.0 - v22);
			this->field_1C[i]._24 = -v24;
			this->field_1C[i]._31 = v23;
			this->field_1C[i]._32 = (1.0 - (v20 + v20)) * (1.0 / v20);
			this->field_1C[i]._33 = v20;
			this->field_1C[i]._34 = 0.0;
			this->field_1C[i]._41 = 0.0;
			this->field_1C[i]._42 = 1.0;
			this->field_1C[i]._43 = 0.0;
			this->field_1C[i]._44 = 0.0;
			++v18;
		}
	}
	
	float* v25 = this->field_18 + 1;
	if (this->field_4 <= 1)
		*this->field_18 = 0.0;
	else
	{
		for (int i = 1; i < this->field_4; ++i) {
			*v25 /= this->field_8;
			v25 += 1;
		}
		*this->field_18 = 0.0;
	}
}

void FFXI::CYy::CMoSpline::AllocTable(int a2, int a3, int a4, int a5)
{
	this->ReleaseTable();
	this->field_4 = a2 + (a4 != 0);
	this->field_24 = a4;
	this->field_25 = a5;

	for (int i = 0; i < sizeof(this->field_C) / sizeof(this->field_C[0]); ++i) {
		if (((1 << i) & a3) != 0)
			this->field_C[i] = CYyObject::Wrap(8 + 4 * this->field_4, FFXI::Constants::Enums::MEM_MODE::Ex);
	}

	this->field_18 = (float*)CYyObject::Wrap(4 + 4 * this->field_4, FFXI::Constants::Enums::MEM_MODE::Ex);
	this->field_20 = (float*)CYyObject::Wrap(12 + sizeof(FFXI::Math::WMatrix) * (this->field_4 - 1), FFXI::Constants::Enums::MEM_MODE::Ex);
	this->field_1C = (FFXI::Math::WMatrix*)(this->field_20 + 3);
}

void FFXI::CYy::CMoSpline::Get(float a2, D3DXVECTOR3* a3)
{
	a3->x = SomeFunc(a2, this->field_C[0]);
	a3->y = SomeFunc(a2, this->field_C[1]);
	a3->z = SomeFunc(a2, this->field_C[2]);
}

float FFXI::CYy::CMoSpline::SomeFunc(float a2, void* a3)
{
	if (!a3)
		return 0.0f;

	if (this->field_24) {
		while (a2 < 0.0)
			a2 += 1.0;
		while (a2 > 1.0)
			a2 -= 1.0;
	}

	float v14{ 0.0 };
	float* v16 = this->field_18 + 1;
	int i;
	for (i = 0; i < this->field_4 - 1; ++i) {
		v14 += v16[i];
		if (a2 <= v14)
			break;
	}
	float v18;
	if (i < this->field_4 - 1)
		v18 = (a2 - (v14 - v16[i])) / v16[i];
	else {
		v18 = 1.0;
		i = this->field_4 - 2;
	}

	BaseProcessor* proc = CYyDb::g_pCYyDb->pMoProcessor;
	D3DXVECTOR3 v21{};
	v21.z = v18;
	v21.y = v18 * v18;
	v21.x = v18 * v18 * v18;

	proc->VirtProcessor11(&proc->field_920, this->field_1C + i, &v21);

	float* f = (float*)a3;
	return proc->field_920.x * f[i]
		+ proc->field_920.y * f[i + 1]
		+ proc->field_920.z * f[i + 2]
		+ proc->field_920.w * f[i + 3];
}
