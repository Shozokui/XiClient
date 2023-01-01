#include "CYyModelDt.h"
#include "CMoOs2.h"
using namespace FFXI::CYy;

const CYyClass CYyModelDt::CYyModelDtClass{
	"CYyModelDt", sizeof(CYyModelDt), &CYyObject::CYyObjectClass
};

const CYyClass* FFXI::CYy::CYyModelDt::GetRuntimeClass()
{
	return &CYyModelDt::CYyModelDtClass;
}

FFXI::CYy::CYyModelDt::CYyModelDt()
{
	this->field_4 = nullptr;
	this->field_8 = 0;
	this->field_C = 0;
}
FFXI::CYy::CYyModelDt::~CYyModelDt()
{
	//sub //TODO
}

int FFXI::CYy::CYyModelDt::GetResId()
{
	if (this->field_8 == nullptr)
		return 0;

	return (*this->field_8)->Header.FourCC;
}
