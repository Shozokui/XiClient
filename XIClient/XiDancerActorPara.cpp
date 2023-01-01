#include "XiDancerActorPara.h"
#include <string>
using namespace FFXI::CYy;

FFXI::CYy::XiDancerActorPara::XiDancerActorPara(char* a2, short* a3)
{
	//Copy & null terminate name
	strncpy_s(this->Name, a2, sizeof(this->Name) - 1);
	this->Name[sizeof(this->Name) - 1] = 0;

	for (int i = 0; i < 9; ++i) {
		this->Equipment[i] = a3[i];
	}

	this->RaceGender = a3[9];
	this->Size = *((char*)a3 + 20);
}
