#include "GeneratorTask.h"
#include "CYyGenerator.h"
#include "CYyDb.h"
#include "CMoResourceMng.h"
using namespace FFXI::CYy;

void FFXI::CYy::GeneratorTask::VObj2(int* a2)
{
	if (a2[0] == 1
		|| a2[0] == 4) {
		if (this != nullptr) {
			delete this;
		}
	}
}

void FFXI::CYy::GeneratorTask::OptionalMove()
{
	CYyGenerator** pgen = this->field_38;
	if ((*pgen)->Header.FourCC == '0nus') {
		int a = 1;
	}
	if (pgen == nullptr || *pgen == nullptr
		|| (((*pgen)->field_DC & 1) == 0) && this->field_14 == 0) {
		CYyGenerator* gen = pgen ? *pgen : nullptr;
		if (gen->Idle(0) == true) {
			this->field_3C = 1;
			gen = this->field_38 ? *this->field_38 : nullptr;
			if ((gen->PreHeader.field_14 & 0x7FFF) == 0) {
				delete this;
				if ((gen->PreHeader.field_16 & 4) != 0) {
					FFXI::CYyDb::g_pCYyDb->pCMoResourceMng->Unlink(gen->PreHeader.PointerToThisInRPL);
				}
			}
		}
	}
}

FFXI::CYy::GeneratorTask::~GeneratorTask()
{
	this->Param1 = 0;
	if (this->field_38 != nullptr) {
		(*this->field_38)->field_A8 = nullptr;
	}
}
