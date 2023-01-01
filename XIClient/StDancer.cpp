#include "StDancer.h"
using namespace FFXI::CTk;

bool StDancer::isdanceractive{ false };
FFXI::CTk::StDancer::StDancer()
{
	this->field_4 = 1;
	this->field_0 = 0;
	this->field_8 = 0;
	this->field_C = 0;
	this->field_10 = 0;
	this->field_14 = 0;
	this->field_18 = 0;
	this->field_1C = 0;
	this->field_20 = 0;
}

FFXI::CTk::StDancer::~StDancer()
{
	//nullsub
}

bool FFXI::CTk::StDancer::IsCheatActive(int a2)
{
	return (this->field_4 & a2) != 0;
}

void FFXI::CTk::StDancer::Update()
{
	if ((this->field_4 & 4) != 0) {

	}
	else if ((this->field_4 & 2) != 0) {

	}
}

void FFXI::CTk::StDancer::SetVisible(int a2, int a3, int a4, int a5)
{
	if ((this->field_4 & 1) == 0) return;
	int v7 = this->field_4 & 4;

	if (a5 == -1)
		this->field_4 ^= 4;
	else if (a5 == 0)
		this->field_4 &= 0xFFFFFFFB;
	else {
		if ((this->field_4 & 4) != 0) return;
		this->field_4 |= 4;
	}

	if ((this->field_4 & 4) != 0)
		this->field_4 &= 0xFFFFFFF7;
	
	if ((this->field_4 & 4) != 0) {
		this->field_10 = a4;
		this->field_4 &= 0xFFFFFFFB;
		this->field_8 = a2;
		this->field_C = a3;
		this->field_4 |= 2;
	}
	else {
		if (v7) {
			throw "not implemented";
		}
		this->field_4 &= 0xFFFFFFFD;
	}
}
