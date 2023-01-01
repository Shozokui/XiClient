#include "CKaTarget.h"

using namespace FFXI::CTk;

FFXI::CTk::CKaTarget::CKaTarget()
{
	this->ClearTarget();
}

void FFXI::CTk::CKaTarget::ClearTarget()
{
	this->field_0 = 0;
	this->field_4 = 0x04000000;
	this->field_8 = 0;
	this->field_C = 0;
	this->field_24 = 0;
	this->field_20 = 0;
	this->field_21 = 0;
	this->field_22 = 0;
	this->field_10 = 0;
	this->field_14 = 0;
	this->field_18 = 0;
	this->field_1C = 1.0;
}
