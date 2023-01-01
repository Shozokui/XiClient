#include "BlendStruct.h"

void FFXI::Util::BlendStruct::MixSet(unsigned int a2)
{
	this->field_0 = (a2 >> 16) & 0xFF;
	this->field_1 = (a2 >> 8) & 0xFF;
	this->field_2 = (a2 >> 0) & 0xFF;
	this->field_3 = (a2 >> 24) & 0xFF;
}

void FFXI::Util::BlendStruct::Scale(float* a2)
{
	char one{};

	double v3 = (double)this->field_2 * a2[0];
	if (v3 >= 0.0) {
		one = (char)v3;
	}
	else {
		one = 0;
	}
	this->field_2 = one;

	v3 = (double)this->field_1 * a2[1];
	if (v3 >= 0.0) {
		one = (char)v3;
	}
	else {
		one = 0;
	}
	this->field_1 = one;

	v3 = (double)this->field_0 * a2[2];
	if (v3 >= 0.0) {
		one = (char)v3;
	}
	else {
		one = 0;
	}
	this->field_0 = one;

	v3 = (double)this->field_3 * a2[3];
	if (v3 >= 0.0) {
		one = (char)v3;
	}
	else {
		one = 0;
	}

	this->field_3 = one;
}

void FFXI::Util::BlendStruct::Scale(BlendStruct* a2, float a3)
{
	double v3 = (double)this->field_2 * a3;
	if (v3 >= 0.0) {
		a2->field_2 = v3;
	}
	else {
		a2->field_2 = 0;
	}


	v3 = (double)this->field_1 * a3;
	if (v3 >= 0.0) {
		a2->field_1 = v3;
	}
	else {
		a2->field_1 = 0;
	}

	v3 = (double)this->field_0 * a3;
	if (v3 >= 0.0) {
		a2->field_0 = v3;
	}
	else {
		a2->field_0 = 0;
	}


	v3 = (double)this->field_3 * a3;
	if (v3 >= 0.0) {
		a2->field_3 = v3;
	}
	else {
		a2->field_3 = 0;
	}

}

void FFXI::Util::BlendStruct::Offset(BlendStruct* a2)
{
	this->field_2 += a2->field_2;
	this->field_1 += a2->field_1;
	this->field_0 += a2->field_0;
	this->field_3 += a2->field_3;
}

void FFXI::Util::BlendStruct::GetWithScaledAlpha(D3DCOLOR* a2, float a3)
{
	*a2 = *(unsigned int*) &this->field_0;
	if (a3 < 1.0) {
		float newAlpha = this->field_3 * a3;
		if (newAlpha < 0.0) {
			newAlpha = 0.0;
		}
		*a2 &= 0x00FFFFFF;
		*a2 |= ((unsigned char)newAlpha << 24);
	}
}
