#include "CYyQue.h"
using namespace FFXI::CYy;

const CYyClass CYyQue::CYyQueClass{
	"CYyQue", sizeof(CYyQue), &CYyObject::CYyObjectClass
};

const CYyClass* FFXI::CYy::CYyQue::GetRuntimeClass()
{
	return &CYyQue::CYyQueClass;
}

FFXI::CYy::CYyQue::CYyQue()
{
	this->Previous = nullptr;
}

FFXI::CYy::CYyQue::~CYyQue()
{
	//nullsub
}

void FFXI::CYy::CYyQue::Kill(float a2)
{
	switch (this->field_8) {
	case 0:
		this->field_8 = 2;
		this->field_C = a2;
		break;
	case 1:
		this->field_8 = 3;
		this->field_14 = a2;
		this->field_C = a2;
		break;
	case 2:
	case 3:
		if (a2 <= 1.0) {
			this->field_C = 0.0;
			this->field_8 = 2;
		}
		break;
	default:
		break;
	}
}

void FFXI::CYy::CYyQue::ControlBlendWeight()
{
	//nullsub
}
