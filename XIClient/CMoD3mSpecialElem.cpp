#include "CMoD3mSpecialElem.h"
#include "CYyDb.h"
#include "BaseProcessor.h"
using namespace FFXI::CYy;

FFXI::CYy::CMoD3mSpecialElem::CMoD3mSpecialElem()
{
	this->field_176 = sizeof(CMoD3mSpecialElem);
}

void FFXI::CYy::CMoD3mSpecialElem::VirtElem3(unsigned short a2)
{
	this->field_198 = a2;
}

int FFXI::CYy::CMoD3mSpecialElem::VirtElem4()
{
	return this->field_198;
}
