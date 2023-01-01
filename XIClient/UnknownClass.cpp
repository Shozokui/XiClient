#include "UnknownClass.h"
#include <string>
using namespace FFXI;

UnknownClass::UnknownClass() {
	this->Init();
}

void UnknownClass::Init() {
	memset(this->field_0, 0, sizeof(this->field_0));
	this->field_78 = 0;
}