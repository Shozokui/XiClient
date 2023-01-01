#include "CYyMotionQue.h"
#include "CMoResourceMng.h"
#include "CMoMo2.h"
using namespace FFXI::CYy;

const CYyClass CYyMotionQue::CYyMotionQueClass{
	"CYyMotionQue", sizeof(CYyMotionQue), &CYyQue::CYyQueClass
};

const CYyClass* FFXI::CYy::CYyMotionQue::GetRuntimeClass()
{
	return &CYyMotionQue::CYyMotionQueClass;
}

void FFXI::CYy::CYyMotionQue::VirtQue1()
{
}

int FFXI::CYy::CYyMotionQue::GetResID()
{
	return this->field_30;
}

void* FFXI::CYy::CYyMotionQue::GetMod()
{
	return this->field_34;
}

void FFXI::CYy::CYyMotionQue::SetMod(void* a2)
{
	this->field_34 = (CMoMo2**)a2;
}

void FFXI::CYy::CYyMotionQue::SetSpeed(float a2)
{
	this->Speed = a2;
}

float FFXI::CYy::CYyMotionQue::GetFrame()
{
	return this->Frame;
}

void FFXI::CYy::CYyMotionQue::UpdateFrame()
{
	//sub //TODO
}

void FFXI::CYy::CYyMotionQue::CalcMotion(int a2, int a3, void* a4, CYyQue* a5)
{
	float v9 = this->field_10;
	if (a5 == nullptr && a2 >= 5)
		v9 = 1.0;

	if (this->field_34 == nullptr)
		return;

	if (CMoResourceMng::CheckResourceThing((CMoResource***) &this->field_34) == false)
		return;

	(*this->field_34)->CalcMotionData(a2, this->Frame, v9, (MotionManStaticStruct*)a4);
}

bool FFXI::CYy::CYyMotionQue::IsZombi()
{
	return this->field_2C < 0;
}

FFXI::CYy::CYyMotionQue::CYyMotionQue()
{
	this->Frame = 0;
	this->Speed = 0;
	this->field_2C = 0;
	this->field_30 = 0;
	this->field_34 = 0;
	this->field_3C = 0;
	this->field_38 = 0;
}

FFXI::CYy::CYyMotionQue::~CYyMotionQue()
{
	if (this->field_34 != nullptr) {
		(*this->field_34)->SomeClean3();
	}
}
