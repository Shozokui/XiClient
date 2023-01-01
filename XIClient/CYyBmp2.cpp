#include "Globals.h"
#include "CYyBmp2.h"
#include "CYyDb.h"
#include "CYyTexMng.h"
#include "CYyTex.h"
#include "CYyResourceFile.h"
using namespace FFXI::CYy;

void FFXI::CYy::CYyBmp2::VObj5(void*)
{
	//sub //TODO
	exit(0x1001CDF0);
}

void FFXI::CYy::CYyBmp2::VObj6()
{
	//sub //TODO
	exit(0x1001CE00);
}

void FFXI::CYy::CYyBmp2::Open()
{
	char v8[16];
	memcpy(v8, this->Data + 1, sizeof(v8));
	CYyTex* v4 = CYyDb::pCYyTexMng->GetThing(v8, this, 0);
	if (!v4) {
		char* mem = StorageLists::instance->Get(sizeof(CYyTex), Constants::Enums::MEM_MODE::Ex);
		if (mem) {
			CYyTex* newTex = new (mem) CYyTex();
			v4 = newTex;
			this->DoThing2(newTex);
			CYyDb::g_pCYyDb->pCYyTexMng->AssignTex(newTex);
			this->DoThing();
			if (CYyDb::g_pCYyDb->pCYyTexMng->field_10 == 1)
				newTex->Init();
		}
	}
	if (v4)
		v4->ReferenceCount += 1;
}

void FFXI::CYy::CYyBmp2::Close()
{
	char v4[16];
	memcpy(v4, &this->Data[1], sizeof(v4));
	CYyTex* result = CYyDb::g_pCYyDb->pCYyTexMng->GetThing(v4, this, 0);
	if (!result) return;
	if (result->ReferenceCount)
		result->ReferenceCount -= 1;
	if (!result->ReferenceCount)
		result->Clean();
}

void FFXI::CYy::CYyBmp2::DoThing()
{
	CMoResource* v1 = this->DoSomething3();
	v1->PreHeader.field_18 += 1;
}

void FFXI::CYy::CYyBmp2::DoThing2(CYyTex* a2)
{
	a2->field_8 = this;
	a2->field_2C &= ~0x100;
	if ((*this->Header.RPLHead)->Header.FourCC == 'effe') {
		CMoResource* Head = *(*this->Header.RPLHead)->Header.RPLHead;
		if (Head->Header.FourCC == 'tsys') {
			a2->field_2C |= 0x100;
		}
	}

	a2->DoThing(this->Data + 1, this->Data[0] >> 7, (this->Data[0] >> 4) & 7);
}
