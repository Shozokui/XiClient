#include "CTkResourceID.h"
#include <string>

using namespace FFXI::CTk;

FFXI::CTk::CTkResourceID::CTkResourceID() {
	this->field_0 = 0;
	this->field_2 = 0;
	memset(this->field_4, 0, sizeof(this->field_4));
	this->field_14 = 0;
	this->field_18 = nullptr;
}