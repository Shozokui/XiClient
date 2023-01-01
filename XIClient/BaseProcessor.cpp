#include "BaseProcessor.h"
#include "Globals.h"
#include "CYyDb.h"
#include "CMoDx.h"
#include "StorageLists.h"
#include "Enums.h"
using namespace FFXI::CYy;

const FFXI::Math::WMatrix BaseProcessor::IdentityMatrix(true);

void FFXI::CYy::BaseProcessor::InitProcessor()
{
	//Intel / AMD processor optimization classes are not implemented
	char* mem = StorageLists::instance->Get(sizeof(BaseProcessor), FFXI::Constants::Enums::MEM_MODE::Ex);
	if (mem)
		CYyDb::pMoProcessor = new (mem) BaseProcessor();
}

void FFXI::CYy::BaseProcessor::DestroyProcessor()
{
	if (!CYyDb::pMoProcessor) return;

	StorageLists::instance->Delete(CYyDb::pMoProcessor);
	CYyDb::pMoProcessor = nullptr;
}

void FFXI::CYy::BaseProcessor::VirtProcessor1(FFXI::Math::WMatrix* a2)
{
	float* v2 = (float*)a2;
	float* v4 = (float*)a2;
	float* v5 = (float*)a2;
	float* v18 = (float*)a2;
	float* v19 = (float*)a2;
	int v3 = 0;
	int v17 = 0;
	while (true) {
		float _11 = *v4;
		if (_11 == 0.0)
			_11 = 0.00001f;

		double v7 = 1.0 / _11;
		float* p__11 = v5;
		for (int i = 0; i < 4; ++i)
			p__11[i] *= v7;
		*v4 = v7;
		float* v11 = v19;
		float* v20 = p__11;
		for (int i = 0; i < 4; ++i) {
			if (i != v3) {
				double v13 = *v11;
				float* v14 = v2;
				float* v15 = v5;
				for (int j = 0; j < 4; ++j) {
					if (j == v17)
						*v14 = -(v13 * v7);
					else
						*v14 = *v14 - v13 * *v15;
					++v15;
					++v14;
				}
				v3 = v17;
				p__11 = v20;
			}
			v11 += 4;
			v2 += 4;
		}
		v17 = ++v3;
		v18 += 5;
		++v19;
		v5 = p__11;
		if (v3 >= 4)
			break;
		v4 = v18;
		v2 = (float*)a2;
	}
}

void FFXI::CYy::BaseProcessor::VirtProcessor2()
{
}

void FFXI::CYy::BaseProcessor::VirtProcessor3(FFXI::Math::WMatrix* a2, FFXI::Math::WMatrix* a3, FFXI::Math::WMatrix* a4)
{
	this->MatrixMultiply(a2, a3, a4);
}

void FFXI::CYy::BaseProcessor::MatrixMultiply(FFXI::Math::WMatrix* a1, FFXI::Math::WMatrix* a2, FFXI::Math::WMatrix* a3)
{
	FFXI::Math::WMatrix v15{};
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			v15.m[i][j] = a2->m[i][0] * a3->m[0][j] + a2->m[i][1] * a3->m[1][j] + a2->m[i][2] * a3->m[2][j] + a2->m[i][3] * a3->m[3][j];
		}
	}
	*a1 = v15;
}

void FFXI::CYy::BaseProcessor::VirtProcessor5()
{
}

void FFXI::CYy::BaseProcessor::VirtProcessor6()
{
}

void FFXI::CYy::BaseProcessor::VirtProcessor7()
{
}

void FFXI::CYy::BaseProcessor::VirtProcessor8()
{
}

double FFXI::CYy::BaseProcessor::Vec3Dot(D3DXVECTOR3* a2, D3DXVECTOR3* a3)
{
	return a2->x * a3->x + a2->y * a3->y + a2->z * a3->z;
}

void FFXI::CYy::BaseProcessor::VirtProcessor10(float* a2, float* a3, float* a4)
{
	float v5 = a4[2] * a3[1] - a3[2] * a4[1];
	float v6 = a3[2] * *a4 - *a3 * a4[2];
	float v4 = *a3 * a4[1] - *a4 * a3[1];
	*a2 = v5;
	a2[1] = v6;
	a2[2] = v4;
}

void FFXI::CYy::BaseProcessor::VirtProcessor11(D3DXVECTOR4* a2, FFXI::Math::WMatrix* a3, D3DXVECTOR3* a4)
{
	double x;
	double y;
	double z;

	x = a4->x;
	y = a4->y;
	z = a4->z;
	a2->x = z * a3->_31 + y * a3->_21 + x * a3->_11 + a3->_41;
	a2->y = z * a3->_32 + y * a3->_22 + x * a3->_12 + a3->_42;
	a2->z = z * a3->_33 + y * a3->_23 + x * a3->_13 + a3->_43;
	a2->w = z * a3->_34 + y * a3->_24 + x * a3->_14 + a3->_44;
}

void FFXI::CYy::BaseProcessor::VirtProcessor12()
{
}

void FFXI::CYy::BaseProcessor::VirtProcessor13()
{
}

void FFXI::CYy::BaseProcessor::VirtProcessor14(D3DXVECTOR3* a2, FFXI::Math::WMatrix* a3, D3DXVECTOR3* a4)
{
	float x = a4->x;
	float y = a4->y;
	float z = a4->z;

	a2->x = z * a3->_31 + y * a3->_21 + x * a3->_11 + a3->_41;
	a2->y = z * a3->_32 + y * a3->_22 + x * a3->_12 + a3->_42;
	a2->z = z * a3->_33 + y * a3->_23 + x * a3->_13 + a3->_43;
}

void FFXI::CYy::BaseProcessor::VirtProcessor15(D3DVECTOR* a2, FFXI::Math::WMatrix* a3, D3DVECTOR* a4)
{
	float x = a4->x;
	float y = a4->y;
	float z = a4->z;

	a2->x = z * a3->_31 + y * a3->_21 + x * a3->_11 + a3->_41;
	a2->y = z * a3->_32 + y * a3->_22 + x * a3->_12 + a3->_42;
	a2->z = z * a3->_33 + y * a3->_23 + x * a3->_13 + a3->_43;
}

void FFXI::CYy::BaseProcessor::VirtProcessor16(D3DXVECTOR3* a2, D3DXVECTOR3* a3, D3DXVECTOR3* a4, float a5)
{
	a2->x = (a4->x - a3->x) * a5 + a3->x;
	a2->y = (a4->y - a3->y) * a5 + a3->y;
	a2->z = (a4->z - a3->z) * a5 + a3->z;
}

void FFXI::CYy::BaseProcessor::VirtProcessor17(FFXI::Math::WMatrix* a2, float a3, float a4, float a5)
{
	static FFXI::Math::WMatrix v1(true);
	static FFXI::Math::WMatrix v2(true);
	static FFXI::Math::WMatrix v3(true);

	v1._23 = sin(a3);
	v1._32 = -v1._23;
	v1._33 = cos(a3);
	v1._22 = v1._33;

	v2._31 = sin(a4);
	v2._13 = -v2._31;
	v2._33 = cos(a4);
	v2._11 = v2._33;

	v3._12 = sin(a5);
	v3._21 = -v3._12;
	v3._22 = cos(a5);
	v3._11 = v3._22;

	this->MatrixMultiply(a2, &v3, &v2);
	this->MatrixMultiply(a2, a2, &v1);
}

void FFXI::CYy::BaseProcessor::VirtProcessor18()
{
}

void FFXI::CYy::BaseProcessor::VirtProcessor19()
{
}

void FFXI::CYy::BaseProcessor::VirtProcessor20(FFXI::Math::WMatrix* a2, float a3)
{
	static FFXI::Math::WMatrix v1(true);
	v1._33 = cos(a3);
	v1._11 = v1._33;
	v1._31 = sin(a3);
	v1._13 = -v1._31;
	*a2 = v1;
}

void FFXI::CYy::BaseProcessor::VirtProcessor21(FFXI::Math::WMatrix* a2, float a3)
{
	static FFXI::Math::WMatrix v1(true);
	v1._22 = cos(a3);
	v1._11 = v1._22;
	v1._12 = sin(a3);
	v1._21 = -v1._12;
	*a2 = v1;
}

void FFXI::CYy::BaseProcessor::VirtProcessor22(FFXI::Math::WMatrix*a2, float a3, float a4, float a5)
{
	static FFXI::Math::WMatrix v1(true);
	static FFXI::Math::WMatrix v2(true);
	static FFXI::Math::WMatrix v3(true);

	v1._23 = sin(a3);
	v1._32 = -v1._23;
	v1._33 = cos(a3);
	v1._22 = v1._33;

	v2._31 = sin(a4);
	v2._13 = -v2._31;
	v2._33 = cos(a4);
	v2._11 = v2._33;

	v3._12 = sin(a5);
	v3._21 = -v3._12;
	v3._22 = cos(a5);
	v3._11 = v3._22;

	this->MatrixMultiply(a2, &v1, &v2);
	this->MatrixMultiply(a2, a2, &v3);
}

void FFXI::CYy::BaseProcessor::VirtProcessor23()
{
}

void FFXI::CYy::BaseProcessor::VirtProcessor24(FFXI::Math::WMatrix* a2, FFXI::Math::WMatrix* a3, float a4)
{
	long double v4 = sin(a4);
	long double v5 = cos(a4);
	this->field_850._23 = v4;
	this->field_850._32 = -v4;
	this->field_850._33 = v5;
	this->field_850._22 = v5;
	this->VirtProcessor3(a2, a3, &this->field_850);
}

void FFXI::CYy::BaseProcessor::VirtProcessor25(FFXI::Math::WMatrix* a2, FFXI::Math::WMatrix* a3, float a4)
{
	long double v4 = cos(a4);
	long double v5 = sin(a4);

	this->field_890._33 = v4;
	this->field_890._11 = v4;
	this->field_890._31 = v5;
	this->field_890._13 = -v5;
	this->VirtProcessor3(a2, a3, &this->field_890);
}

void FFXI::CYy::BaseProcessor::VirtProcessor26()
{
}

void FFXI::CYy::BaseProcessor::VirtProcessor27(D3DXVECTOR3* a2, D3DXVECTOR3* a3, int a4, D3DXMATRIX* a5, D3DXMATRIX* a6, float a7, float a8)
{
	if (a4 <= 0) {
		return;
	}

	for (int i = 0; i < a4; ++i) {
		double v12 = a5->_11 * a2[i].x + a5->_21 * a2[i].y + a5->_31 * a2[i].z + a5->_41;
		double v13 = a5->_12 * a2[i].x + a5->_22 * a2[i].y + a5->_32 * a2[i].z + a5->_42;
		double v14 = a2[i].z * a5->_33 + a5->_13 * a2[i].x + a2[i].y * a5->_23 + a5->_43;
		float v20 = v14 * a6->_31 + v13 * a6->_21 + v12 * a6->_11 + a6->_41;
		float v21 = v14 * a6->_32 + v13 * a6->_22 + v12 * a6->_12 + a6->_42;
		float v15 = v12 * a6->_14 + v14 * a6->_34 + v13 * a6->_24 + a6->_44;
		a3[i].z = v15;
		if (v15 < 0.0) {
			v15 = -v15;
		}
		else if (v15 == 0.0) {
			v15 = 1.0;
		}
		double v19 = 1.0 / v15;
		a3[i].x = (v20 * v19 + 1.0) * a7;
		a3[i].y = (1.0 - v19 * v21) * a8;
	}
}

void FFXI::CYy::BaseProcessor::VirtProcessor28()
{
}

void FFXI::CYy::BaseProcessor::VirtProcessor29()
{
}

void FFXI::CYy::BaseProcessor::VirtProcessor30(D3DXVECTOR4* a2, D3DXVECTOR4* a3, int a4, Math::WMatrix* a5, Math::WMatrix* a6, float a7, float a8)
{
	this->field_A94 = 0;
	this->field_A98 = 10000.0;
	this->field_A9C = -10000.0;

	Math::WMatrix* pop{};
	this->PopFromStack(&pop);

	this->MatrixMultiply(pop, a5, a6);
	D3DXVECTOR4 retval = { 10000.0, 10000.0, -10000.0, -10000.0 };
	for (int i = 0; i < a4; ++i) {
		D3DXVECTOR4* input = a3 + i;
		
		double v14 = input->x * pop->_11 + pop->_31 * input->z + pop->_21 * input->y + pop->_41;
		double v15 = pop->_32 * input->z + input->x * pop->_12 + pop->_22 * input->y + pop->_42;
		double v16 = input->x * pop->_14 + input->z * pop->_34 + pop->_24 * input->y + pop->_44;
		
		if (v16 == 0.0) {
			v16 = 1.0;
		}
		else {
			double v18 = (pop->_13 * input->x + pop->_23 * input->y + pop->_33 * input->z + pop->_43) / v16;
			if (v18 < this->field_A98) {
				this->field_A98 = v18;
			}
			if (v18 > this->field_A9C) {
				this->field_A9C = v18;
			}
			if (v16 < 0.0) {
				v16 = -v16;
			}
		}

		double v22 = 1.0 / v16;
		float v53 = (v22 * v14 + 1.0) * a7;
		float v52 = (1.0 - v22 * v15) * a8;

		if (v53 < retval.x) {
			retval.x = v53;
		}
		if (v53 > retval.z) {
			retval.z = v53;
		}
		if (v52 < retval.y) {
			retval.y = v52;
		}
		if (v52 > retval.w) {
			retval.w = v52;
		}
		if (v53 > 0.0) {
			double v36 = a7 + a7;
			if (v36 > v53) {
				if (v52 > 0.0) {
					double v45 = a8 + a8;
					if (v45 > v52) {
						this->field_A94 += 1;
					}
				}
			}
		}
	}
	*a2 = retval;
	this->PushToStack(pop);
}

void FFXI::CYy::BaseProcessor::VirtProcessor31(float* a2, float* a3, int a4, int a5, Math::WMatrix* a6)
{
	CMoDx* cmodx = CYyDb::g_pCYyDb->g_pCMoDx;
	if (cmodx == nullptr) {
		return;
	}

	float retval[4] = { 10000.0, 10000.0, -10000.0, -10000.0 };

	Math::WMatrix* pop{};
	this->PopFromStack(&pop);

	*pop = *a6;
	FFXI::Math::WMatrix* v7 = &cmodx->field_B10;

	for (int i = 0; i < a5; ++i) {
		double v13 = pop->_31 * a3[2] + a3[1] * pop->_21 + pop->_11 * *a3 + pop->_41;
		double v14 = *a3 * pop->_12 + pop->_32 * a3[2] + a3[1] * pop->_22 + pop->_42;
		double v15 = pop->_33 * a3[2] + a3[1] * pop->_23 + pop->_13 * *a3 + pop->_43;
		float v30 = v13 * v7->_11 + v15 * v7->_31 + v14 * v7->_21 + v7->_41;
		float v28 = v13 * v7->_12 + v15 * v7->_32 + v14 * v7->_22 + v7->_42;
		float v16 = v15 * v7->_34 + v14 * v7->_24 + v13 * v7->_14 + v7->_44;

		if (v16 < 0.0)
			v16 = -v16;
		if (v16 == 0.0)
			v16 = 1.0;

		double v17 = 1.0 / v16;
		float v31 = (1.0 + v17 * v30) * cmodx->field_DC0;
		double v18 = (1.0 - v17 * v28) * cmodx->field_DC4;
		
		if (v31 < retval[0])
			retval[0] = v31;
		if (v31 > retval[2])
			retval[2] = v31;
		if (v18 < retval[1])
			retval[1] = v18;
		if (v18 > retval[3])
			retval[3] = v18;

		a3 = (float*)((char*)a3 + a4);
	}
	memcpy_s(a2, 4 * sizeof(float), retval, sizeof(retval));
	this->PushToStack(pop);
}

float FFXI::CYy::BaseProcessor::Vec3Magnitude(D3DXVECTOR3* a2)
{
	long double v1 = a2->x * a2->x + a2->y * a2->y + a2->z * a2->z;
	if (v1 == 0.0)
		return 0.0;

	return sqrt(v1);
}

void FFXI::CYy::BaseProcessor::VirtProcessor33()
{
}

void FFXI::CYy::BaseProcessor::VirtProcessor34(float* a2, float* a3)
{
	long double magsq = a3[0] * a3[0] + a3[1] * a3[1] + a3[2] * a3[2];
	long double mag = sqrt(magsq);
	a2[0] = a3[0] / mag;
	a2[1] = a3[1] / mag;
	a2[2] = a3[2] / mag;
}

void FFXI::CYy::BaseProcessor::VirtProcessor35(D3DXVECTOR3* a2)
{
	this->Vec3Normalize(a2);
}

void FFXI::CYy::BaseProcessor::Vec3Normalize(D3DXVECTOR3* a2)
{
	long double v1 = sqrt(a2->x * a2->x + a2->y * a2->y + a2->z * a2->z);
	if (v1 != 0.0) {
		a2->x /= v1;
		a2->y /= v1;
		a2->z /= v1;
	}
}

void FFXI::CYy::BaseProcessor::VirtProcessor37()
{
}

void FFXI::CYy::BaseProcessor::VirtProcessor38()
{
}

void FFXI::CYy::BaseProcessor::VirtProcessor39()
{
}

void FFXI::CYy::BaseProcessor::VirtProcessor40(FFXI::Math::WMatrix* a2, FFXI::Math::WMatrix* a3)
{
	this->MatrixTranspose(a2, a3);
}

void FFXI::CYy::BaseProcessor::VirtProcessor41()
{
}

void FFXI::CYy::BaseProcessor::MatrixTranspose(FFXI::Math::WMatrix* a2, FFXI::Math::WMatrix* a3)
{
	a2->_11 = a3->_11;
	a2->_12 = a3->_21;
	a2->_13 = a3->_31;
	a2->_14 = a3->_41;
	a2->_21 = a3->_12;
	a2->_22 = a3->_22;
	a2->_23 = a3->_32;
	a2->_24 = a3->_42;
	a2->_31 = a3->_13;
	a2->_32 = a3->_23;
	a2->_33 = a3->_33;
	a2->_34 = a3->_43;
	a2->_41 = a3->_14;
	a2->_42 = a3->_24;
	a2->_43 = a3->_34;
	a2->_44 = a3->_44;
}

void FFXI::CYy::BaseProcessor::VirtProcessor43()
{
}

void FFXI::CYy::BaseProcessor::MatrixIdentity(FFXI::Math::WMatrix* a2)
{
	memcpy_s(a2, sizeof(FFXI::Math::WMatrix), IdentityMatrix, sizeof(IdentityMatrix));
}

void FFXI::CYy::BaseProcessor::VirtProcessor45()
{
}

FFXI::CYy::BaseProcessor::BaseProcessor()
{
	D3DXMatrixIdentity(&this->field_810);
	D3DXMatrixIdentity(&this->field_850);
	D3DXMatrixIdentity(&this->field_890);
	D3DXMatrixIdentity(&this->field_8D0);

	this->MatrixPointerIndex = 0;
	for (int i = 0; i < sizeof(this->Matrices) / sizeof(this->Matrices[0]); ++i)
		this->MatrixPointers[i] = this->Matrices + i;
	
}

void FFXI::CYy::BaseProcessor::PopFromStack(FFXI::Math::WMatrix** a2)
{
	*a2 = this->MatrixPointers[this->MatrixPointerIndex];
	this->MatrixPointerIndex += 1;
}

void FFXI::CYy::BaseProcessor::PushToStack(FFXI::Math::WMatrix* a2)
{
	this->MatrixPointerIndex -= 1;
	this->MatrixPointers[this->MatrixPointerIndex] = a2;
}
