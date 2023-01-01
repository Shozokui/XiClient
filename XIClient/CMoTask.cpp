#include "CMoTask.h"
#include "CMoTaskMng.h"
#include "CYyDb.h"
#include "Globals.h"
#include "CMoTaskMng.h"
using namespace FFXI::CYy;

const CYyClass CMoTask::CMoTaskClass{
	"CMoTask", sizeof(CMoTask), &CYyObject::CYyObjectClass
};

CMoTask::CMoTask() {
	if (CMoTaskMng::DontInitThisTask) return;

	this->Reset();
	this->DoSomething3();
}

const CYyClass* FFXI::CYy::CMoTask::GetRuntimeClass()
{
	return &CMoTaskClass;
}

CMoTask::~CMoTask() {
	this->Clean(1);
}

void CMoTask::VTask1() {
//nullsub
}

char CMoTask::OnMove() {
	return 0; 
}
void CMoTask::OptionalMove() {
	//nullsub
}
void CMoTask::VTask4(CMoTask*) {
	//nullsub
}

void FFXI::CYy::CMoTask::Reset()
{
	this->field_C = 0;
	this->field_8 = 0;
	this->Callback = nullptr;
	this->Param1 = 0;
	this->Param2 = 0;
	this->field_20 = CMoTaskMng::DeleteThisTask;
	CMoTaskMng::DeleteThisTask = false;
	this->field_14 = 0;
	this->field_22 = 0;
	this->field_10 = nullptr;
	this->field_24 = 0;
	this->field_28 = nullptr;
}

void FFXI::CYy::CMoTask::Destroy()
{
	if (this->field_20 != 0)
		delete this;
	else
		this->Clean(1);
}

void FFXI::CYy::CMoTask::Clean(int p1)
{
	if (this->field_10 != nullptr) {
		this->field_10->DoSomething(this);
	}

	if (this->field_24 != nullptr) {
		this->field_24->RecurseDelete();
	}

	if (this->Callback == nullptr) 
	{
		return;
	}

	if (p1 != 0) {
		this->DoSomething2();
	}

	if (this->field_20 != 0) {
		return;
	}

	CYyDb::pCMoTaskMng->field_D5C = CYyDb::pCMoTaskMng->field_D50;
	CYyDb::pCMoTaskMng->field_D50 = this;
	this->Callback = nullptr;
	CYyDb::pCMoTaskMng->field_D44 -= 1;
}

void FFXI::CYy::CMoTask::DoSomething(CMoTask* a2)
{
	CMoTask* Task = this->field_24;
	CMoTask* v3{ nullptr };
	if (!Task) return;
	while (Task != a2) {
		v3 = Task;
		Task = Task->field_28;
		if (!Task) return;
	}
	if (v3) {
		Task = Task->field_28;
		v3->field_28 = Task;
	}
	else {
		this->field_24 = Task->field_28;
	}
	a2->field_28 = nullptr;
	a2->field_10 = nullptr;
}

void FFXI::CYy::CMoTask::DoSomethingManager()
{
	CYyDb::g_pCYyDb->pCMoTaskMng->DoSomething(this);
}

void FFXI::CYy::CMoTask::RecurseDelete()
{
	if (this->field_28)
		this->field_28->RecurseDelete();

	this->field_10 = nullptr;
	delete this;
}

int FFXI::CYy::CMoTask::SomeRecursive()
{
	int result{ 0 };
	for (CMoTask* i = this->field_24; i; i = i->field_28)
		result = i->SomeRecursive();
	++this->field_22;
	return result;
}

void FFXI::CYy::CMoTask::SomeRecursive2()
{
	for (CMoTask* i = this->field_24; i; i = i->field_28)
		i->SomeRecursive2();
	if (this->field_22)
		this->field_22 -= 1;
}

void FFXI::CYy::CMoTask::DoSomething2()
{
	CYyDb::pCMoTaskMng->SomeShift2(this);
}

void FFXI::CYy::CMoTask::DoSomething3()
{
	CYyDb::pCMoTaskMng->SomeShift3(this);
}

void FFXI::CYy::CMoTask::TaskMngShift4(int a2)
{
	CYyDb::pCMoTaskMng->SomeShift4(this, a2);
}

void FFXI::CYy::CMoTask::DoIt(CMoTask* a2)
{
	if (!a2) return;
	this->field_10 = a2;
	a2->DoItHelper(this);
}

void FFXI::CYy::CMoTask::DoItHelper(CMoTask* a2)
{
	if (a2) {
		if (this->field_24)
			a2->field_28 = this->field_24;
	}
	this->field_24 = a2;
}

CMoTask* FFXI::CYy::CMoTask::Clone(unsigned int a2)
{
	CMoTaskMng::DontInitThisTask = true;
	CMoTaskMng::DeleteThisTask = true;
	char* mem = StorageLists::instance->Get(a2, FFXI::Constants::Enums::MEM_MODE::Ex);
	CMoTask* newtask{ nullptr };
	newtask = new (mem) CMoTask();
	CMoTaskMng::DontInitThisTask = false;

	memcpy(newtask, this, a2);
	newtask->field_8 = 0;
	newtask->field_C = 0;
	newtask->field_10 = 0;
	newtask->field_24 = 0;
	newtask->field_28 = 0;
	if (this->field_10 != nullptr) {
		this->field_10->DoItHelper(newtask);
	}
	newtask->DoSomething3();
	this->VTask4(newtask);
	return newtask;
}
