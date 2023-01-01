#include "WMatrix.h"
#include "Globals.h"
#include "BaseProcessor.h"
#include "CYyDb.h"

FFXI::Math::WMatrix::WMatrix(bool a2)
{
	if (a2 == true) {
		this->Identity();
	}
}

void FFXI::Math::WMatrix::Identity()
{
	D3DXMatrixIdentity(this);
}

bool FFXI::Math::WMatrix::CheckMatrix()
{
	if (!Globals::CheckFloat(this->_11)) return false;
	if (!Globals::CheckFloat(this->_12)) return false;
	if (!Globals::CheckFloat(this->_13)) return false;
	if (!Globals::CheckFloat(this->_14)) return false;
	if (!Globals::CheckFloat(this->_21)) return false;
	if (!Globals::CheckFloat(this->_22)) return false;
	if (!Globals::CheckFloat(this->_23)) return false;
	if (!Globals::CheckFloat(this->_24)) return false;
	if (!Globals::CheckFloat(this->_31)) return false;
	if (!Globals::CheckFloat(this->_32)) return false;
	if (!Globals::CheckFloat(this->_33)) return false;
	if (!Globals::CheckFloat(this->_34)) return false;
	if (!Globals::CheckFloat(this->_41)) return false;
	if (!Globals::CheckFloat(this->_42)) return false;
	if (!Globals::CheckFloat(this->_43)) return false;
	if (!Globals::CheckFloat(this->_44)) return false;
	return true;
}

void FFXI::Math::WMatrix::Vec3TransformDrop4Self(D3DXVECTOR3* a2)
{
	D3DXVECTOR3 v1 = *a2;
	this->Vec3TransformDrop4(a2, &v1);
}

void FFXI::Math::WMatrix::Vec3TransformDrop4(D3DXVECTOR3* out, D3DXVECTOR3* in)
{
	out->x = this->_11 * in->x + this->_21 * in->y + this->_31 * in->z + this->_41;
	out->y = this->_12 * in->x + this->_22 * in->y + this->_32 * in->z + this->_42;
	out->z = this->_13 * in->x + this->_23 * in->y + this->_33 * in->z + this->_43;
}

void FFXI::Math::WMatrix::Vec3TransformNormal(D3DXVECTOR3* a2, D3DXVECTOR3* a3)
{
	a2->x = this->_31 * a3->z + this->_21 * a3->y + a3->x * this->_11;
	a2->y = this->_32 * a3->z + this->_22 * a3->y + this->_12 * a3->x;
	a2->z = this->_33 * a3->z + this->_23 * a3->y + this->_13 * a3->x;
}

void FFXI::Math::WMatrix::Vec3TransformNormalSelf(D3DXVECTOR3* a2)
{
	const D3DXVECTOR3 a1 = *a2;
	D3DXVec3TransformNormal(a2, &a1, this);
}

void FFXI::Math::WMatrix::RotateX(float angle)
{
	WMatrix a1{};
	D3DXMatrixRotationX(&a1, angle);
	this->MatrixMultiply(&a1);
}

void FFXI::Math::WMatrix::RotateY(float angle)
{
	WMatrix a1{};
	D3DXMatrixRotationY(&a1, angle);
	this->MatrixMultiply(&a1);
}

void FFXI::Math::WMatrix::RotateZ(float angle)
{
	WMatrix a1{};
	D3DXMatrixRotationZ(&a1, angle);
	this->MatrixMultiply(&a1);
}

void FFXI::Math::WMatrix::Scale3(D3DXVECTOR3* a2)
{
	memset(this, 0, sizeof(*this));
	this->_11 = a2->x;
	this->_22 = a2->y;
	this->_33 = a2->z;
	this->_44 = 1.0;
}

void FFXI::Math::WMatrix::MatrixMultiply(WMatrix* a1)
{
	WMatrix p_src{};
	D3DXMatrixMultiply(&p_src, this, a1);
	*this = p_src;
}

void FFXI::Math::WMatrix::MatrixInvert()
{
	for (int i = 0; i < 4; ++i) {
		double factor = this->m[i][i];
		if (factor == 0.0) {
			factor = 1.0;
		}
		double invert = 1.0 / factor;
		if (invert == 0.0) {
			invert = 1.0;
		}
		for (int j = 0; j < 4; ++j) {
			this->m[i][j] *= invert;
		}
		this->m[i][i] = invert;
		for (int j = 0; j < 4; ++j) {
			if (j != i) {
				double v12 = this->m[j][i];
				for (int k = 0; k < 4; ++k) {
					if (k == i) {
						this->m[j][k] = -(v12 * invert);
					}
					else {
						this->m[j][k] -= v12 * this->m[i][k];
					}
				}
			}
		}
	}
}

void FFXI::Math::WMatrix::Vec4TransformSelf(D3DXVECTOR4* a1)
{
	const D3DXVECTOR4 v4 = *a1;
	D3DXVec4Transform(a1, &v4, this);
}

void FFXI::Math::WMatrix::Vec4MultiplySelf(D3DXVECTOR4* a2)
{
	D3DXVECTOR4 a1 = *a2;
	this->Vec4Multiply(a2, &a1);
}

void FFXI::Math::WMatrix::Vec4Multiply(D3DXVECTOR4* a2, D3DXVECTOR4* a3)
{
	a2->x = this->_41 * a3->w + this->_21 * a3->y + this->_31 * a3->z + a3->x * this->_11;
	a2->y = this->_42 * a3->w + this->_22 * a3->y + this->_12 * a3->x + this->_32 * a3->z;
	a2->z = this->_43 * a3->w + this->_23 * a3->y + this->_13 * a3->x + this->_33 * a3->z;
	a2->w = this->_44 * a3->w + this->_24 * a3->y + this->_14 * a3->x + this->_34 * a3->z;
}

void FFXI::Math::WMatrix::SomeCombo(WMatrix* a2) {
	WMatrix copy = *a2;

	a2->_11 = copy._11 * this->_11 + copy._31 * this->_13 + copy._21 * this->_12;
	a2->_12 = copy._12 * this->_11 + copy._32 * this->_13 + copy._22 * this->_12;
	a2->_13 = copy._13 * this->_11 + copy._33 * this->_13 + copy._23 * this->_12;

	a2->_11 = copy._11 * this->_11 + copy._31 * this->_13 + copy._21 * this->_12;
	a2->_12 = copy._12 * this->_11 + copy._32 * this->_13 + copy._22 * this->_12;
	a2->_23 = copy._13 * this->_21 + copy._33 * this->_23 + copy._23 * this->_22;

	a2->_11 = copy._11 * this->_11 + copy._31 * this->_13 + copy._21 * this->_12;
	a2->_12 = copy._12 * this->_11 + copy._32 * this->_13 + copy._22 * this->_12;
	a2->_33 = copy._13 * this->_31 + copy._33 * this->_33 + copy._23 * this->_32;

	a2->_11 = copy._11 * this->_11 + copy._31 * this->_13 + copy._21 * this->_12;
	a2->_12 = copy._12 * this->_11 + copy._32 * this->_13 + copy._22 * this->_12;
	a2->_43 = copy._13 * this->_41 + copy._33 * this->_43 + copy._23 * this->_42;
}
void FFXI::Math::WMatrix::DoSomething()
{
	D3DXVECTOR4 a1 = this->m[3];
	WMatrix p_src{};

	this->_44 = 0.0;
	this->_43 = 0.0;
	this->_42 = 0.0;
	this->_41 = 0.0;
	D3DXMatrixTranspose(&p_src, this);
	p_src.Vec4TransformSelf(&a1);

	p_src._41 = -a1.x;
	p_src._42 = -a1.y;
	p_src._43 = -a1.z;
	p_src._44 = 1.0;

	*this = p_src;
}

bool FFXI::Math::WMatrix::SomeDrawCalc(D3DXVECTOR3* a2)
{
	D3DXVECTOR4 res{};
	int ander = 0x3F;
	for (int i = 0; i < 8; ++i) {
		D3DXVec3Transform(&res, a2 + i, this);
		int v3 = 0;
		if (res.w < 0.0)
			res.w = -res.w;
		if (res.x > res.w)
			v3 = 1;
		if (res.x < -res.w)
			v3 |= 2;
		if (res.y > res.w)
			v3 |= 4;
		if (res.y < -res.w)
			v3 |= 8;
		if (res.z > res.w)
			v3 |= 16;
		if (res.z < -res.w)
			v3 |= 32;
		ander &= v3;
		if (ander == 0)
			return false;
	}

	return true;
}

bool FFXI::Math::WMatrix::SomeDrawCalc2(D3DXVECTOR3* a2)
{
	D3DXVECTOR4 res{};
	int ander = 0x3F;
	for (int i = 0; i < 8; ++i) {
		D3DXVec3Transform(&res, a2 + i, this);
		int v3 = 0;
		if (res.w < 0.0)
			res.w = -res.w;
		if (res.x > res.w)
			v3 = 1;
		if (res.x < -res.w)
			v3 |= 2;
		if (res.y > res.w)
			v3 |= 4;
		if (res.y < -res.w)
			v3 |= 8;
		if (res.z < -res.w)
			v3 |= 32;
		ander &= v3;
		if (ander == 0)
			return false;
	}

	return true;
}

void FFXI::Math::WMatrix::CreateScaling(D3DXVECTOR4* a2)
{
	this->Identity();
	this->_11 = a2->x;
	this->_22 = a2->y;
	this->_33 = a2->z;
	this->_44 = a2->w;
}

void FFXI::Math::WMatrix::AddTranslation4(D3DXVECTOR4* a2)
{
	this->_41 = a2->x;
	this->_42 = a2->y;
	this->_43 = a2->z;
	this->_44 = a2->w;
}

void FFXI::Math::WMatrix::AddTranslation3(D3DXVECTOR3* a2)
{
	this->_41 = a2->x;
	this->_42 = a2->y;
	this->_43 = a2->z;
}

void __fastcall FFXI::Math::WMatrix::Vec3TransNorm(D3DXVECTOR3* a2, float a3, float a4, float a5)
{
	a2->x = a3 * this->_11 + a4 * this->_21 + a5 * this->_31 + this->_41;
	a2->y = a3 * this->_12 + a4 * this->_22 + a5 * this->_32 + this->_42;
	a2->z = a3 * this->_13 + a4 * this->_23 + a5 * this->_33 + this->_43;
}

void FFXI::Math::WMatrix::RHPerspective(float fovy, float Aspect, float zn, float zf)
{
	float v8 = fovy * 0.5;
	double v6 = cos(v8) / sin(v8);
	double v7 = zf / (zn - zf);

	this->_34 = -1.0;
	this->_11 = v6 / Aspect;
	this->_12 = 0.0;
	this->_13 = 0.0;
	this->_14 = 0.0;
	this->_21 = 0.0;
	this->_22 = v6;
	this->_23 = 0.0;
	this->_24 = 0.0;
	this->_31 = 0.0;
	this->_32 = 0.0;
	this->_33 = v7;
	this->_41 = 0.0;
	this->_42 = 0.0;
	this->_43 = v7 * zn;
	this->_44 = 0.0;
}
