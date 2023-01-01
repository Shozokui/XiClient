#include "CYyReferenceReadTask.h"
#include "CYyResourceFile.h"
#include "ReferenceReadBase.h"
#include "Globals.h"
#include "CYyDb.h"
#include "CMoResourceMng.h"
using namespace FFXI::CYy;

void FFXI::CYy::CYyReferenceReadTask::VObj2(int* a2)
{
	if (*a2 == 1 || *a2 == 4 || *a2 == 7) {
		if (this->field_40 != 0)
			this->Callback(this->ResourceFile, this->Context);
		
		this->Context = nullptr;
		delete this;
	}
}

char FFXI::CYy::CYyReferenceReadTask::OnMove()
{
	if (CYyDb::g_pCYyDb->pCMoResourceMng->InList((CMoResource**)this->ResourceFile) == true) {
		if (CMoResourceMng::CheckResourceThing((CMoResource***)&this->ResourceFile) == false)
			return 0;
	}
	this->Callback(this->ResourceFile, this->Context);
	delete this;
	return 1;
}

FFXI::CYy::CYyReferenceReadTask::CYyReferenceReadTask()
{
	this->Context = nullptr;
}

FFXI::CYy::CYyReferenceReadTask::~CYyReferenceReadTask()
{
	//nullsub
}

void FFXI::CYy::CYyReferenceReadTask::Start(CYyResourceFile** a2, void(__cdecl* a3)(CYyResourceFile**, File::ReferenceReadBase*), File::ReferenceReadBase* a4, char a5)
{
	this->ResourceFile = a2;
	this->Callback = a3;
	this->Context = a4;
	this->TaskMngShift4(0);
}
