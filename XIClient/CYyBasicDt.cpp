#include "CYyBasicDt.h"
#include "CMoOs2.h"
using namespace FFXI::CYy;

void FFXI::CYy::CYyBasicDt::VirtModelDt1(CXiActor* a2, CYyModel* a3, CYyModelBase* a4, float a5, int* a6, int* a7)
{
	(*this->field_8)->DrawBasic(a2, a3, a4, a5, a6, a7);
}

void FFXI::CYy::CYyBasicDt::VirtModelDt2()
{
}

void FFXI::CYy::CYyBasicDt::VirtModelDt3()
{
}

void FFXI::CYy::CYyBasicDt::VirtModelDt4(unsigned char* a2)
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

CYyOs2VtxBuffer* FFXI::CYy::CYyBasicDt::VirtModelDt5()
{
	return *(CYyOs2VtxBuffer**)((*this->field_8)->Data + 46);
}
