#include "CIwOnePic.h"
#include "CTkMenuCtrlData.h"
using namespace FFXI::CTk;

FFXI::CTk::CIwOnePic::CIwOnePic()
{
	this->init(0);
}

void FFXI::CTk::CIwOnePic::OnInitialUpdatePrimitive()
{
	this->MenuCtrlData->field_85 = 0;
	this->MenuCtrlData->field_86 = 0;
	this->MenuCtrlData->field_84 = 0;
	this->field_28 = 0;
	//sub //TODO
}

void FFXI::CTk::CIwOnePic::OnDrawPrimitive()
{
	if (this->field_2C == 0) {

	}
}

void FFXI::CTk::CIwOnePic::OnDrawCalc(bool)
{
	if (this->field_18 == 0) {
		if (this->field_1C != this->field_20) {
			this->field_20 = this->field_1C;
			this->field_18 = 1;
		}
	}
	else if (this->field_18 < 10) {
		this->field_18 += 1;
	}
	else if (this->field_18 == 10) {
		int v6 = -(this->field_20 != this->field_1C);
		v6 &= 0xFFFFFFF5;
		this->field_18 = v6 + 11;
	}

	if (this->field_2C == 0) {
		switch (this->field_18) {
		case 11:
			break;
		case 12:
			break;
		case 13:
			break;
		case 14:
			break;
		case 15:
			break;

		}
	}
}

void FFXI::CTk::CIwOnePic::init(int a2)
{
	this->field_24 = 128;
	this->field_1C = -1;
	this->field_20 = -1;
	this->field_18 = 0;
	this->field_30 = 0;
	this->field_34 = 0;
	this->field_2C = a2;
}
