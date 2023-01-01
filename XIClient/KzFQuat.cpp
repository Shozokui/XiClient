#include "KzFQuat.h"
#include "Globals.h"
using namespace FFXI::Math;

FFXI::Math::KzFQuat::KzFQuat()
{
	this->Init();
}

void FFXI::Math::KzFQuat::Init()
{
	this->x = 0.0;
	this->y = 0.0;
	this->z = 0.0;
	this->w = 1.0;
}

void FFXI::Math::KzFQuat::SetQuatFromVector(D3DXVECTOR3* a2, D3DXVECTOR3* a3, float a4)
{
	float v7 = Globals::Vec3Inner(a2, a3);

	//Clamp the inner product to -1.0, 1.0
	if (v7 <= 1.0) {
		if (v7 < -1.0)
			v7 = -1.0;
	}
	else
		v7 = 1.0;

	float v12 = acos(v7) * a4 * 0.5;
	float v16 = sin(v12);

	D3DXVECTOR3 v13{};
	Globals::Vec3Outer(&v13, a2, a3);
	Globals::Vec3Normalize(&v13);

	this->x = v13.x * v16;
	this->y = v13.y * v16;
	this->z = v13.z * v16;
	this->w = cos(v12);
}

void FFXI::Math::KzFQuat::GetMatrix(WMatrix* a2)
{
	double v2{}, v3{}, v4{}, v5{}, v6{}, v7{};
	float v8{}, v9{}, v10{}, v11{};

	a2->_14 = 0.0;
	a2->_24 = 0.0;
	a2->_34 = 0.0;
	a2->_41 = 0.0;
	a2->_42 = 0.0;
	a2->_43 = 0.0;
	a2->_44 = 1.0;
	v2 = this->x * this->x;
	v3 = this->y * this->y;
	v4 = this->z * this->z;

	v5 = this->y * this->x;
	v10 = this->z * this->y;
	v6 = this->z * this->w;
	v9 = this->z * this->x;
	v8 = this->y * this->w;
	v7 = this->w * this->x;

	a2->_11 = 1.0 - (v4 + v3 + v4 + v3);
	a2->_12 = v6 + v5 + v6 + v5;
	a2->_13 = v9 - v8 + v9 - v8;
	a2->_21 = v5 - v6 + v5 - v6;
	a2->_22 = 1.0 - (v4 + v2 + v4 + v2);
	a2->_23 = v11 + v10 + v11 + v10;
	a2->_31 = v8 + v9 + v8 + v9;
	a2->_32 = v10 - v11 + v10 - v11;
	a2->_33 = 1.0 - (v3 + v2 + v3 + v2);
}

void FFXI::Math::KzFQuat::Normalize()
{
	long double magsq = this->x * this->x + this->y * this->y + this->z * this->z + this->w * this->w;
	long double mag = 1.0;

	if (magsq > 0.0)
		mag /= sqrt(magsq);

	this->x *= mag;
	this->y *= mag;
	this->z *= mag;
	this->w *= mag;
}

void FFXI::Math::KzFQuat::Multiply(KzFQuat* output, KzFQuat* a3)
{
	double v4{}, v5{}, v8{}, v9{};
	KzFQuat v10{};
	v4 = this->y * a3->z - a3->y * this->z;
	v5 = a3->x * this->z - this->x * a3->z;
	v8 = a3->y * this->x - this->y * a3->x;
	v9 = -a3->y * this->y - a3->z * this->z;

	v10.x = a3->w * this->x + a3->x * this->w + v4;
	v10.y = a3->y * this->w + a3->w * this->y + v5;
	v10.z = a3->w * this->z + a3->z * this->w + v8;
	v10.w = a3->w * this->w - a3->x * this->x + v9;

	output->x = v10.x;
	output->y = v10.y;
	output->z = v10.z;
	output->w = v10.w;
}

void FFXI::Math::KzFQuat::DoTheThing(D3DXVECTOR3* a2, D3DXVECTOR3* a3, float a4) {
	long double v4 = a4 * 0.5;
	float v6 = sin(v4);
	D3DXVECTOR3 v7{};
	Globals::Vec3Outer(&v7, a2, a3);
	Globals::Vec3Normalize(&v7);
	this->x = v7.x * v6;
	this->y = v7.y * v6;
	this->z = v7.z * v6;
	this->w = cos(v4);
}

void FFXI::Math::KzFQuat::lerp(KzFQuat* a2, KzFQuat* a3, float a4)
{
	if (a4 == 1.0)
	{
		*a2 = *a3;
	}
	else
	{
		KzFQuat v7{};
		if (this->w * a3->w + a3->z * this->z + a3->y * this->y + a3->x * this->x >= 0.0)
		{
			v7 = *a3;
		}
		else
		{
			v7.x = -a3->x;
			v7.y = -a3->y;
			v7.z = -a3->z;
			v7.w = -a3->w;
		}
		float v9 = 1.0 - a4;
		KzFQuat v8{};
		v8.x = v9 * this->x + v7.x * a4;
		v8.y = v9 * this->y + v7.y * a4;
		v8.z = v9 * this->z + v7.z * a4; 
		v8.w = v9 * this->w; +v7.w * a4;
		
		a2->x = v8.x;
		a2->y = v8.y;
		a2->z = v8.z;
		a2->w = v8.w;
	}
}
