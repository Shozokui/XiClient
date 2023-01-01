#include "CYyResourceFile.h"
#include "ResourceList.h"
#include "CYyGenerator.h"
#include "CYyScheduler.h"
#include "Globals.h"
#include "CYyDb.h"
#include "CMoResourceMng.h"
#include "CMoTaskMng.h"
#include "CMoSchedulerTask.h"
#include "CYyReferenceReadWithAttachmentTask.h"
#include <iostream>
using namespace FFXI::CYy;

void FFXI::CYy::CYyResourceFile::ReferenceRead(CYyResourceFile*** a2, void(__cdecl* a3)(CYyResourceFile**, File::ReferenceReadBase*), File::ReferenceReadBase* a4, CXiActor* a5, CXiActor* a6)
{
	char* mem = StorageLists::instance->Get(sizeof(CYyReferenceReadWithAttachmentTask), FFXI::Constants::Enums::MEM_MODE::Ex);
	CMoTaskMng::DeleteThisTask = true;

	CYyReferenceReadWithAttachmentTask* refreadtask{ nullptr };
	if (mem)
		refreadtask = new (mem) CYyReferenceReadWithAttachmentTask();

	//refreadtask could be null as per client
	refreadtask->Start(*a2, a3, a4, 1);
	//refreadtask->field_44.SetCaster(a5);
	//refreadtask->field_44.SetTarget(a6);
}

FFXI::CYy::CYyResourceFile::CYyResourceFile()
{
	//nullsub
}

FFXI::CYy::CYyResourceFile::~CYyResourceFile()
{
	//nullsub
}

void FFXI::CYy::CYyResourceFile::StopMovers()
{
	if (!this) return;

	ResourceList v8{}, v9{};
	v8.Clear();
	v8.PrepareFromResource(this, FFXI::Constants::Enums::ResourceType::Generater, 0, -1);
	CYyGenerator* gen = (CYyGenerator*)v8.GetNext(false);
	while (gen) {
		v9.PrepareFromLastCreated(CYyDb::g_pCYyDb->pCMoResourceMng, FFXI::Constants::Enums::ResourceType::Generater, 0, -1);
		CYyGenerator* gen2 = (CYyGenerator*)v9.GetNext(false);
		while (gen2) {
			if ((gen2->PreHeader.field_16 & 4) != 0 &&
				gen2->Header.RPLHead == gen->PreHeader.PointerToThisInRPL) {
				CMoResource::DoTheThing(gen2);
				gen2->KillAll(nullptr);
				gen2->SomeClean3();
				if ((gen2->PreHeader.field_14 & 0x7FFF) == 0) {
					CYyDb::g_pCYyDb->pCMoResourceMng->Unlink(gen2->PreHeader.PointerToThisInRPL);
					v9.PrepareFromLastCreated(CYyDb::g_pCYyDb->pCMoResourceMng, FFXI::Constants::Enums::ResourceType::Generater, 0, -1);
				}
			}
			gen2 = (CYyGenerator*)v9.GetNext(false);
		}
		gen->KillAll(nullptr);
		gen->Unlink(true);
		gen = (CYyGenerator*)v8.GetNext(false);
	}

	v8.PrepareFromResource(this, FFXI::Constants::Enums::ResourceType::Scheduler, 0, -1);
	CYyScheduler* sched = (CYyScheduler*)v8.field_14;
	while (sched != nullptr) {
		CMoTask* task = CYyDb::g_pCYyDb->pCMoTaskMng->field_D50;
		while (task != nullptr) {
			if (task->Param2 == 'Ms\0\0') {
				CMoSchedulerTask* stask = (CMoSchedulerTask*)task;
				CYyScheduler* runner = stask->field_80 ? *stask->field_80 : nullptr;
				if (runner == sched) {
					//std::cout << "STOPMOVE: " << (int)stask << std::endl;
					stask->Destroy();
					sched = (CYyScheduler*)v8.field_14;
					task = CYyDb::g_pCYyDb->pCMoTaskMng->field_D50;
					continue;
				}
			}
			task = task->field_C;
		}
		v8.GetNext(false);
		sched = (CYyScheduler*)v8.field_14;
	}
}
