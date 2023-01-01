#include "KzCibCollect.h"
using namespace FFXI::CYy;

const char SomeRaceTable[12] = { 0, 0, 1, 2, 3, 4, 5, 6, 7, 0, 0, 0 };
FFXI::CYy::KzCibCollect::KzCibCollect()
{
	this->field_10 = -1;
	this->field_11 = -1;
	this->field_12 = -1;
	this->field_13 = -1;

	for (int i = 0; i < sizeof(this->field_14) / sizeof(this->field_14[0]); ++i) {
		this->field_14[i] = -1;
		this->field_1C[i] = -1;
	}

	this->field_24 = -1;
	this->field_25 = -1;
}

int FFXI::CYy::KzCibCollect::GetSomething(int a2)
{
	//check race
	if (a2 >= 9) {
		return 0;
	}
	unsigned char v5 = this->field_1C[ SomeRaceTable[a2] ];
	if (v5 == 0xFF)
		return 0;

	return v5;
}

double FFXI::CYy::KzCibCollect::GetSomeDouble()
{
	if (this->field_24 == 0xFFu) {
		return 1.0;
	}

	return (double)this->field_24 * 0.0099999998;
}
