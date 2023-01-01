#include "CYyTexBase.h"
#include "Globals.h"
#include "CYyDb.h"

using namespace FFXI::CYy;

const CYyClass CYyTexBase::CYyTexBaseClass{
	"CYyTexBase", sizeof(CYyTexBase), &CYyObject::CYyObjectClass
};
int CYyTexBase::Counter{ 0 };

char Unique[16] = "-uniq";
const CYyClass* FFXI::CYy::CYyTexBase::GetRuntimeClass()
{
	return &CYyTexBaseClass;
}

FFXI::CYy::CYyTexBase::CYyTexBase()
{
	this->ReferenceCount = 0;
	this->field_8 = 0;
	this->ImageWidth = 0;
	this->field_28 = 0;
	this->BitDepth = 0;
	char v2 = 1;
	if (CYyDb::g_pCYyDb->UseMIPMapping) {
		v2 = CYyDb::g_pCYyDb->UseMIPMapping;
	}
	this->field_2A = v2;
	this->field_20 = 0;
	this->field_2C = 0;
	this->field_1C = 0;

	CYyTexBase::Counter += 1;
	sprintf_s(&Unique[5], 11, "%10d", CYyTexBase::Counter);
	memcpy(this->Unique, Unique, sizeof(Unique));
}
