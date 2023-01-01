#include "a32or27thing.h"

using namespace FFXI::CYy;

void a32or27thing::Open() {
	this->helper.Find(this);
}

void FFXI::CYy::a32or27thing::Init(char* a1)
{
	int* intData = (int*)a1;
	*((int*)this->helper.data + 0) = intData[0];
	*((int*)this->helper.data + 4) = intData[1];
	*((int*)this->helper.data + 8) = intData[2];
	*((int*)this->helper.data + 12) = intData[3];
}
