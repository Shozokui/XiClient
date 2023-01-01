#include "CYyAdvancedDt.h"
#include "CMoOs2.h"
using namespace FFXI::CYy;

FFXI::CYy::CYyAdvancedDt::CYyAdvancedDt()
{
	this->field_40 = nullptr;
	this->field_3C = 1;
}

FFXI::CYy::CYyAdvancedDt::~CYyAdvancedDt()
{
	if (this->field_40 != nullptr) {
		CYyObject::Unwrap(this->field_40);
		this->field_40 = nullptr;
	}
}

void FFXI::CYy::CYyAdvancedDt::VirtModelDt1(CXiActor* a2, CYyModel* a3, CYyModelBase* a4, float a5, int* a6, int* a7)
{
	exit(0x10024200);
}

void FFXI::CYy::CYyAdvancedDt::VirtModelDt2()
{
}

void FFXI::CYy::CYyAdvancedDt::VirtModelDt3()
{
}

void FFXI::CYy::CYyAdvancedDt::VirtModelDt4(unsigned char* a2)
{
	if (this->field_8 == nullptr)
		return;

	CMoOs2* os2 = *this->field_8;
	if (os2 == nullptr)
		return;

	unsigned short count = *(unsigned short*)(os2->Data + 16);
	unsigned short* shortdata = *(unsigned short**)(os2->Data + 12);
	for (unsigned short i = 0; i < count; ++i) {
		unsigned short index = shortdata[i];
		a2[index] = 0x80;
	}
}

CYyOs2VtxBuffer* FFXI::CYy::CYyAdvancedDt::VirtModelDt5()
{
	return nullptr;
}

void FFXI::CYy::CYyAdvancedDt::Init(CMoOs2* a2)
{
	unsigned short* short18 = *(unsigned short**)(a2->Data + 18);
	unsigned int count = 1 + short18[0] + short18[1];
	this->field_40 = CYyObject::Wrap(60 * count, FFXI::Constants::Enums::MEM_MODE::Ex);
	if (this->field_40 == nullptr)
		return;

	this->field_44 = count;
}
