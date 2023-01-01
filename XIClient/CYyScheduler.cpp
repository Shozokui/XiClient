#include "CYyScheduler.h"
#include "CMoSchedulerTask.h"
#include "Globals.h"
#include "CYyDb.h"
#include "StorageLists.h"
#include "CMoTaskMng.h"
#include "CXiActor.h"
#include "XiAtelBuff.h"
using namespace FFXI::CYy;

FFXI::CYy::CYyScheduler::CYyScheduler()
{
}

FFXI::CYy::CYyScheduler::~CYyScheduler()
{
}

void FFXI::CYy::CYyScheduler::ConstructFromData(char* a2)
{
	memcpy_s(&this->Header, sizeof(this->Header), a2, sizeof(this->Header));
	int* intData = (int*)a2;
	this->Attachments.field_4 = intData[4];
	this->Attachments.field_8 = intData[5];
	this->Attachments.Caster = (CXiActor*)intData[6];
	this->Attachments.Target = (CXiActor*)intData[7];

	unsigned int datasize = 16 * (((intData[1] >> 7) & 0x7FFFF) - 2);
	memcpy_s(&this->field_70, datasize, a2 + 32, datasize);
	this->Attachments.Init();
}

void FFXI::CYy::CYyScheduler::Open()
{
	if (this->field_70 >= 0x10000)
		return;

	int offset = (int)((char*)this + 0x50);
	this->field_70 += offset;
	this->field_74 += offset;
	this->field_78 += offset;

	this->CalcTotalFrame();
	if (this->IsLoopScheduler())
		this->Execute(0, 0);
}

void FFXI::CYy::CYyScheduler::Close()
{
	if (!this) return;

	CMoTask* i = CYyDb::g_pCYyDb->pCMoTaskMng->field_D50;
	while (i) {
		if (i->Param1 == (int)this && i->Param2 == 'Ms\0\0') {
			i->Destroy();
			i = CYyDb::g_pCYyDb->pCMoTaskMng->field_D50;
			continue;
		}
		i = i->field_C;
	}

	this->SomeClean2Helper();
}

CMoSchedulerTask* FFXI::CYy::CYyScheduler::CreateSchedulerTask(CXiActor* caster, CXiActor* target, void* a5, int a6)
{
	char* mem = StorageLists::instance->Get(sizeof(CMoSchedulerTask), Constants::Enums::MEM_MODE::Ex);
	CMoTaskMng::DeleteThisTask = true;
	
	CMoSchedulerTask* newTask{ nullptr };
	if (mem) {
		newTask = new (mem) CMoSchedulerTask(this, a6, a5);
		newTask->field_8C = (char*)this->field_74;
		newTask->field_88 = newTask->field_8C;
		if (caster == nullptr
			|| caster->AtelBuffer == nullptr) {
			newTask->Param1 = 0;
		}
		else {
			newTask->Param1 = caster->AtelBuffer->field_78;
		}

		newTask->Param2 = 'Ms\0\0';

		if (caster != nullptr) {
			newTask->SetCaster(caster);
		}

		if (target != nullptr) {
			newTask->SetTarget(target);
		}

		newTask->field_94 = 0.0f;
		newTask->field_98 = 0.0f;
	}

	return newTask;
}

CMoSchedulerTask* FFXI::CYy::CYyScheduler::CreateStartTask(CXiActor* a2, CXiActor* a3, int a4, void* a5)
{
	CMoResource::DoTheThing(this);
	CMoSchedulerTask* newTask = this->CreateSchedulerTask(a2, a3, a5, a4);
	if (a5) {
		throw "do this";
	}
	newTask->Execute();
	return newTask;
}

CMoSchedulerTask* FFXI::CYy::CYyScheduler::CreateStartTask(CXiActor* a2, CXiActor* a3, CMoSchedulerTask* a4)
{
	double v5{ 0.0 };
	void* v6{ nullptr };
	if (a4) {
		if (!a4->field_142 && a4->field_9C != 1.0)
			v5 = (double)this->field_7C * a4->field_9C * 0.0;
		v6 = a4->field_90;
	}

	CMoSchedulerTask* task = this->CreateSchedulerTask(a2, a3, v6, v5);
	if (a4) {
		task->field_144 = a4->field_144;
		if (a4->field_142) {
			task->field_9C = a4->field_9C;
			task->field_142 = 1;
		}
	}
	if (task)
		task->Execute();

	return task;
}

CMoSchedulerTask* FFXI::CYy::CYyScheduler::Execute(int a2, void* a3)
{
	CMoResource::DoTheThing(this);
	CMoSchedulerTask* newTask = this->CreateSchedulerTask(nullptr, nullptr, a3, a2);
	if (newTask)
		newTask->Execute();
	return newTask;
}

CMoSchedulerTask* FFXI::CYy::CYyScheduler::Execute(CXiActor* a2, CXiActor* a3, int a4, void* a5)
{
	CMoResource::DoTheThing(this);
	CMoSchedulerTask* newTask = this->CreateSchedulerTask(a2, a3, a5, a4);
	if (a5)
		throw "DO THIS";
	newTask->Execute();
	return newTask;
}

void FFXI::CYy::CYyScheduler::CalcTotalFrame()
{
	char* v1 = (char*)this->field_74;
	int v2 = 0;
	int i = 0;
	while (v1) {
		if (v2 < i + *((short*)v1 + 3))
			v2 = i + *((short*)v1 + 3);
		i += *((short*)v1 + 2);
		if (!(unsigned char)*(unsigned int*)v1)
			break;
		v1 += 4 * ((*(int*)v1 >> 8) & 0x1F);
	}
	this->field_7C = v2;
}

bool FFXI::CYy::CYyScheduler::IsLoopScheduler()
{
	bool result = this->FindFirstTag2(1) || this->FindFirstTag(82) != nullptr;
	if (result)
		return this->FindFirstTag(0xA2) == nullptr;

	return false;
}

char* FFXI::CYy::CYyScheduler::FindFirstTag(int a2)
{
	unsigned int* dwordData = (unsigned int*)this->field_74;
	
	while (dwordData) {
		if ((unsigned char)*dwordData == 0)
			return nullptr;
		else if ((unsigned char)*dwordData == a2)
			return (char*)dwordData;

		dwordData += (*dwordData >> 8) & 0x1F;
	}

	return nullptr;
}

char* FFXI::CYy::CYyScheduler::FindFirstTag2(int a2)
{
	unsigned int* dwordData = (unsigned int*)this->field_78;

	while (dwordData) {
		if ((unsigned char)*dwordData == 0)
			return nullptr;
		else if ((unsigned char)*dwordData == a2)
			return (char*)dwordData;

		dwordData += (*dwordData >> 8) & 0x1F;
	}

	return nullptr;
}

void FFXI::CYy::CYyScheduler::Stop()
{
	CMoTaskMng* tmng = CYyDb::g_pCYyDb->pCMoTaskMng;
	CMoTask* task = tmng->field_D50;
	while (task) {
		if (task->Param2 == 'Ms\0\0') {
			CMoSchedulerTask* stask = (CMoSchedulerTask*)task;
			CYyScheduler* sched = stask->field_80 ? *stask->field_80 : nullptr;
			if (sched == this) {
				task->Destroy();
				task = tmng->field_D50;
				continue;
			}
		}
		task = task->field_C;
	}
}
