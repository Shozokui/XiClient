#include "CMoTaskMng.h"

using namespace FFXI::CYy;

const CYyClass CMoTaskMng::CMoTaskMngClass{
	"CMoTaskMng", sizeof(CMoTaskMng), &CYyObject::CYyObjectClass
};

bool CMoTaskMng::DontInitThisTask{ false };
bool CMoTaskMng::DeleteThisTask{ false };
CMoDisplayColorTask* CMoTaskMng::current_display_color_task_f{ nullptr };
CMoDisplayColorTask* CMoTaskMng::current_display_color_task_51{ nullptr };
CMoDisplayColorTask* CMoTaskMng::current_display_color_task_72{ nullptr };

const CYyClass* FFXI::CYy::CMoTaskMng::GetRuntimeClass()
{
	return &CMoTaskMngClass;
}

FFXI::CYy::CMoTaskMng::~CMoTaskMng()
{
	CMoTask* v2 = this->field_D50;
	if (v2) {
		CMoTask* v3{ nullptr };
		do {
			v3 = v2->field_C;
			v2->Destroy();
			v2 = v3;
		} while (v3);
	}
	
	int TaskListSize = sizeof(this->Tasks) / sizeof(this->Tasks[0]);
	for (int i = TaskListSize - 1; i >= 0; --i)
		this->Tasks[i].~CMoTask();
}

FFXI::CYy::CMoTaskMng::CMoTaskMng()
{
	int TaskListSize = sizeof(this->Tasks) / sizeof(this->Tasks[0]);
	this->Tasks[0].Reset();
	this->Tasks[0].field_C = &this->Tasks[1];
	for (int i = 1; i < TaskListSize - 1; ++i) {
		this->Tasks[i].Reset();
		this->Tasks[i].field_8 = &this->Tasks[i - 1];
		this->Tasks[i].field_C = &this->Tasks[i + 1];
	}
	this->Tasks[TaskListSize - 1].Reset();
	this->Tasks[TaskListSize - 1].field_8 = &this->Tasks[TaskListSize - 2];
	this->field_D58 = this->Tasks;
	this->field_D54 = nullptr;
	this->field_D50 = nullptr;
	this->field_D44 = nullptr;
	for (int i = 0; i < sizeof(this->TaskPointers) / sizeof(this->TaskPointers[0]); ++i)
		this->TaskPointers[i] = this->SomeShift(0);
	
	this->field_D4C = this->field_D50;
}

void FFXI::CYy::CMoTaskMng::TaskCallback(CMoTask* a1)
{
	if (a1->OnMove() == 0)
		a1->OptionalMove();
}

CMoTask* FFXI::CYy::CMoTaskMng::SomeShift(void(__cdecl*p_callback)(CMoTask*))
{
	CMoTask* v3 = this->field_D58;
	this->field_D58 = v3->field_C;
	v3->Reset();
	v3->field_8 = this->field_D54;
	CMoTask* v5 = this->field_D54;
	if (v5 == this->field_D4C)
		this->field_D4C = v3;
	if (v5)
		v5->field_C = v3;
	CMoTask* v6 = this->field_D50;
	this->field_D54 = v3;
	if (!v6)
		this->field_D50 = v3;
	v3->Callback = p_callback;
	++this->field_D44;
	return v3;
}

CMoTask* FFXI::CYy::CMoTaskMng::SomeShift2(CMoTask* a2)
{
	CMoTask* result; // eax
	CMoTask* v3; // edx
	CMoTask* v4; // edx

	result = a2;
	if (a2 == this->field_D4C)
		this->field_D4C = a2->field_8;
	v3 = a2->field_8;
	if (v3)
		v3->field_C = a2->field_C;
	v4 = a2->field_C;
	if (v4)
		v4->field_8 = a2->field_8;
	if (this->field_D50 == a2)
		this->field_D50 = a2->field_C;
	if (this->field_D54 == a2)
		this->field_D54 = a2->field_8;
	if (this->field_D5C != a2)
		return result;
	result = a2->field_C;
	this->field_D5C = result;
	return result;
}

CMoTask* FFXI::CYy::CMoTaskMng::SomeShift3(CMoTask* a2)
{
	CMoTask* v3; // eax
	CMoTask* v5; // eax

	this->SomeShift2(a2);
	v3 = this->field_D4C;
	if (!v3)
		return this->SomeShift4(a2, 9);
	a2->field_C = v3->field_C;
	v5 = this->field_D4C->field_C;
	if (v5)
		v5->field_8 = a2;
	a2->field_8 = this->field_D4C;
	this->field_D4C->field_C = a2;
	this->field_D5C = a2;
	a2->Callback = CMoTaskMng::TaskCallback;
	a2->field_20 = 1;
	return a2;
}

CMoTask* FFXI::CYy::CMoTaskMng::SomeShift4(CMoTask* task, int a3)
{
	int v4; // eax
	CMoTask* v5; // ecx
	CMoTask* v6; // edx
	CMoTask* result; // eax

	this->SomeShift2(task);
	v4 = a3;
	if (v4 >= 16)
		v4 = 15;
	else if (v4 < 0)
		v4 = 0;

	task->field_C = this->TaskPointers[v4]->field_C;
	v5 = this->TaskPointers[v4]->field_C;
	if (v5)
		v5->field_8 = task;
	task->field_8 = this->TaskPointers[v4];
	this->TaskPointers[v4]->field_C = task;
	task->Callback = CMoTaskMng::TaskCallback;
	task->field_20 = 1;
	v6 = this->TaskPointers[v4];
	result = task;
	if (v6 == this->field_D54)
		this->field_D54 = task;
	return result;
}

void FFXI::CYy::CMoTaskMng::DoSomething(CMoTask* a2)
{
	this->SomeShift2(a2);
	bool v6{ false };
	if (this->field_D4C) {
		a2->field_C = this->field_D4C;
		if (this->field_D4C->field_8)
			this->field_D4C->field_8->field_C = a2;
		a2->field_8 = this->field_D4C->field_8;
		this->field_D4C->field_8 = a2;
		v6 = this->field_D50 == this->field_D4C;
	}
	else {
		CMoTask* v4 = this->TaskPointers[9];
		a2->field_C = v4;
		if (v4->field_8)
			v4->field_8->field_C = a2;
		a2->field_8 = v4->field_8;
		v4->field_8 = a2;
		v6 = this->field_D50 == v4;
	}
	if (v6)
		this->field_D50 = a2;
	a2->Callback = CMoTaskMng::TaskCallback;
	a2->field_20 = 1;
}

void FFXI::CYy::CMoTaskMng::DoTasks()
{
	this->field_D48 = 0;
	this->field_D4C = this->field_D50;

	while (this->field_D4C) {
		this->field_D5C = this->field_D4C->field_C;
		if (this->field_D4C->Callback != nullptr && !this->field_D4C->field_22) {
			this->field_D4C->Callback(this->field_D4C);
		}
		this->field_D4C = this->field_D5C;
		this->field_D48 += 1;
	}

	this->field_D4C = nullptr;
}

void FFXI::CYy::CMoTaskMng::InitStackTask(void(__cdecl* a2)(CMoTask*), int a3)
{
	int v3 = a3;
	if (v3 >= 16) {
		v3 = 15;
	}
	else if (v3 < 0) {
		v3 = 0;
	}

	CMoTask* task = this->TaskPointers[v3];
	CMoTask* v5 = this->field_D58;
	this->field_D58 = v5->field_C;
	v5->Reset();
	v5->field_8 = task;
	v5->field_C = task->field_C;
	task->field_C = v5;
	if (v5->field_C != nullptr) {
		v5->field_C->field_8 = v5;
	}
	CMoTask* v8 = this->field_D4C;
	if (v8 == task) {
		this->field_D5C = v5;
	}
	if (v8 == this->field_D54) {
		this->field_D54 = v5;
	}
	v5->Callback = a2;
	this->field_D44 += 1;
}

CMoTask* FFXI::CYy::CMoTaskMng::FindInQueue(int a2, int a3)
{
	CMoTask* result = this->field_D50;
	
	while (result) {
		if (result->Param1 == a2 && result->Param2 == a3)
			return result;

		result = result->field_C;
	}
	
	return nullptr;
}
