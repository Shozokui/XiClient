#include "VbList.h"
#include "CYyVbMng.h"
using namespace FFXI::CYy;

FFXI::CYy::VbList::VbList()
{
	this->field_4  = 0;
	this->field_8  = 0;
	memset(this->field_C, 0, sizeof(this->field_C));
}

FFXI::CYy::VbList::~VbList()
{
	for (int i = 0; i < 2; ++i) {
		CYyVbMng::DoSomething(this->field_C + i);
		CYyVbMng::DoSomething(this->field_C + 2 + i);
		CYyVbMng::DoSomething(this->field_C + 4 + i);
		CYyVbMng::DoSomething(this->field_C + 6 + i);
	}
}
