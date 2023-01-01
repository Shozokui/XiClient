#include "CMoSchedulerTask.h"
#include "Globals.h"
#include "StorageLists.h"
#include "CYyDb.h"
#include "CEnv.h"
#include "CDx.h"
#include "CMoTaskMng.h"
#include "CMoCameraTask.h"
#include "CMoCamera.h"
#include "CYyScheduler.h"
#include "CMoResourceMng.h"
#include "Strings.h"
#include "XiDateTime.h"
#include "CMoDisplayColorTask.h"
#include "CMoTextureDisplayTask.h"
#include "CMoDx.h"
#include "CYyGenerator.h"
#include "CYyGeneratorClone.h"
#include "XiZone.h"
#include <iostream>
#include "CYySepRes.h"
#include "CXiActor.h"
#include "CXiDollActor.h"
using namespace FFXI::CYy;

const CYyClass CMoSchedulerTask::CMoSchedulerTaskClass{
	"CMoSchedulerTask", sizeof(CMoSchedulerTask), &CMoTask::CMoTaskClass
};
const CYyClass* FFXI::CYy::CMoSchedulerTask::GetRuntimeClass()
{
	return &CMoSchedulerTaskClass;

}
//LOCAL FUNCS
char* GetNextTagLocation(char* a1) {
	if (*a1)
		return a1 + ((*(int*)a1 >> 6) & 0x7C);

	return nullptr;
}

void** GetDword3(char* a1, void** a2) {
	*a2 = *(void**)(a1 + 12);
	return a2;
}
//~LOCAL FUNCS
char FFXI::CYy::CMoSchedulerTask::OnMove()
{
	if (this->field_145) {
		delete this;
		return true;
	}

	if (!CMoResourceMng::CheckResourceThing((CMoResource***)&this->field_80))
		return false;

	double v3{};
	if (this->field_140) 
		v3 = (double)XiDateTime::delta_tick_count;
	else 
		v3 = CYyDb::g_pCYyDb->g_pCMoDx->field_EB0;
	

	this->field_98 -= v3;
	this->field_94 += v3;

	//std::cout << "DURATION REMAINING: " << this->field_98 << std::endl;
	float v9{ 0.0 };
	while (true) {
		if (this->field_98 < 0.0) {
			v9 = -this->field_98;
			this->field_98 = 0.0;
		}
		if (this->field_98 == 0.0) {
			if (this->field_88) {
				while (true) {
					this->field_98 += (double)*((unsigned short*)this->field_88 + 2) * this->field_9C;
					int result = this->ExecuteTag();
					if (result == 1)
						return true;
					if (result == 2)
						return false;
					if (*this->field_88) {
						int size = (*(int*)this->field_88 >> 8) & 0x1F;
						char* v8 = this->field_88 + 4 * size;
						this->SetNextTag(v8);
					}
					else
						this->SetNextTag(nullptr);

					if (this->field_98 != 0.0) {
						this->field_98 -= v9;
						break;
					}
					if (!this->field_88)
						break;
				}
			}
			else {
				if (this->CheckSomething())
					this->field_145 = 1;
return false;
			}
		}

		if (this->field_98 >= 0.0)
			return false;
	}
}

void FFXI::CYy::CMoSchedulerTask::VTask4(CMoTask* a2)
{
	CMoSchedulerTask* sched = (CMoSchedulerTask*)a2;
	sched->InitSomething();
	sched->CMoAttachments::Init();
	CXiActor* caster = this->CMoAttachments::GetMasterCaster();
	CXiActor* target = this->CMoAttachments::GetMasterTarget();
	sched->CMoAttachments::SetCaster(caster);
	sched->CMoAttachments::SetTarget(target);
	(*sched->field_80)->DoTheThing;
	sched->Param1 = 0;

	if (caster != nullptr) {
		caster->VirtActor80();
	}
	if (target != nullptr) {
		target->VirtActor80();
	}

	if (this->field_90 != nullptr) {
		exit(0x10056E53);
	}
}

const CYyClass* FFXI::CYy::CMoSchedulerTask::AttachedTo()
{
	return this->GetRuntimeClass();
}

bool FFXI::CYy::CMoSchedulerTask::VirtAttach3()
{
	if (this != nullptr) {
		delete this;
	}
	return true;
}

bool FFXI::CYy::CMoSchedulerTask::VirtAttach4()
{
	if (this != nullptr) {
		delete this;
	}
	return true;
}

CYyObject* FFXI::CYy::CMoSchedulerTask::GetCYyObject()
{
	return this;
}

FFXI::CYy::CMoSchedulerTask::CMoSchedulerTask(CYyScheduler* a2, unsigned int a3, void* a4)
{
	//something with field_b4
	//sub //TODO
	this->InitSomething();
	this->StartInit(a2, a3, a4);
	this->DoSomething();
}

FFXI::CYy::CMoSchedulerTask::~CMoSchedulerTask()
{
	CXiActor* caster = this->CMoAttachments::GetMasterCaster();
	CXiActor* target = this->CMoAttachments::GetMasterTarget();
	if (caster != nullptr) {
		caster->VirtActor81();
	}
	if (target != nullptr) {
		target->VirtActor81();
	}
	for (CMoTask* i = CYyDb::g_pCYyDb->pCMoTaskMng->field_D50; i; i = i->field_C) {
		if (i->Param2 == 'Ms\0\0' && ((CMoSchedulerTask*)i)->ParentTask == this)
			((CMoSchedulerTask*)i)->ParentTask = nullptr;
	}

	if (this->ParentTask)
		this->ParentTask->SomeRecursive2();

	if (this->field_90 != nullptr)
		exit(0x10056587);

	(*this->field_80)->SomeClean3();
}

void FFXI::CYy::CMoSchedulerTask::StartInit(CYyScheduler* a2, unsigned int a3, void* a4)
{
	this->field_90 = 0;
	this->field_140 = 0;
	this->field_AC = 0;

	//sub //todo //setstatus
	this->field_80 = (CYyScheduler**)a2->PreHeader.PointerToThisInRPL;
	if (((*this->field_80)->Header.Type_Size & 0x7F) == 0x3D) {
		int a = 1;
	}
	if (a3 == 0)
		this->field_9C = 1.0;
	else if (a3 == 1) {
		this->field_141 = 1;
		this->field_9C = 1.0;
	}
	else {
		this->field_9C = (double)a3 / (double)a2->field_7C;
	}
	this->field_A0 = 0;
	this->ParentTask = nullptr;
	this->field_94 = 0;
	this->field_98 = 0;
	this->field_88 = (char*)a2->field_74;
}

void FFXI::CYy::CMoSchedulerTask::InitSomething()
{
	this->field_141 = 0;
	this->field_143 = 0;
	this->field_144 = 0;
	this->field_140 = 0;
	this->field_145 = 0;
	this->field_146 = 0;
}

void FFXI::CYy::CMoSchedulerTask::DoSomething()
{
	this->DoSomethingManager();
}

void FFXI::CYy::CMoSchedulerTask::Execute()
{
	std::cout << "Executing Scheduler Task: ";
	char* ok = this->field_88;
	while (ok) {
		if (*ok) {
			std::cout << std::hex << (int)*ok << " ";
			int size = (*(int*)ok >> 8) & 0x1F;
ok += 4 * size;
		}
		else
		ok = nullptr;
	}
	std::cout << std::endl;
	CMoTask* v1 = CYyDb::g_pCYyDb->pCMoTaskMng->field_D4C;
	CYyDb::g_pCYyDb->pCMoTaskMng->field_D4C = this;
	this->OnMove();
	CYyDb::g_pCYyDb->pCMoTaskMng->field_D4C = v1;
}

void FFXI::CYy::CMoSchedulerTask::SetCaster(CXiActor* a2)
{
	CXiActor* caster = this->CMoAttachments::GetMasterCaster();
	if (caster != nullptr) {
		caster->VirtActor81();
	}

	this->CMoAttachments::SetCaster(a2);
	caster = this->CMoAttachments::GetMasterCaster();
	if (caster != nullptr) {
		caster->VirtActor80();
	}
}

void FFXI::CYy::CMoSchedulerTask::SetTarget(CXiActor* a2)
{
	CXiActor* target = this->CMoAttachments::GetMasterTarget();
	if (target != nullptr) {
		target->VirtActor81();
	}

	this->CMoAttachments::SetTarget(a2);
	target = this->CMoAttachments::GetMasterTarget();
	if (target != nullptr) {
		target->VirtActor80();
	}
}

int FFXI::CYy::CMoSchedulerTask::ExecuteTag()
{
	CMoResource** unused{ nullptr };
	unsigned char TagCode = *(unsigned char*)this->field_88;
	char* mem = { nullptr };
	CMoDisplayColorTask* colortask{ nullptr };
	switch (TagCode) {
	case 0x0:
	case 0x1:
		return false;
	case 0x02: {
		CYyGenerator** pgen = *(CYyGenerator***)(this->field_88 + 12);

		if (pgen == nullptr || *pgen == nullptr) {
			int fourcc = *(int*)(this->field_88 + 8);
			if (this->field_146 == 1) {
				if (fourcc == 'lloc' || fourcc == '30ds' || fourcc == '10ds') {
					return 0;
				}
			}

			(*this->field_80)->FindResourceUnder((CMoResource***)&pgen, FFXI::Constants::Enums::ResourceType::Generater, fourcc);
			if (pgen == nullptr || *pgen == nullptr) {
				return 0;
			}
			*(CYyGenerator***)(this->field_88 + 12) = pgen;
		}

		CYyGenerator* gen = *pgen;
		void* model = gen->GetModel();
		int v234 = 0;
		char* cc = (char*)gen->Script2;
		if (cc[40] != 0 || model == nullptr) {
			if (gen->GetModelType() == FFXI::Constants::Enums::ResourceType::Sep)
				v234 = 1;
		}
		else {
			CYyObject* obj = (CYyObject*)model;
			if (obj->IsKindOf(&CYySepRes::CYySepResClass)) {
				CYySepRes* sep = (CYySepRes*)obj;
				v234 = sep->field_3C >> 31;
			}
			else
				v234 = gen->IsNever();
		}

		bool cf = gen->CMoAttachments::CheckFlags();
		if (cf == true) {
			CXiActor* caster = this->CMoAttachments::GetCasterCheckDoll();
			if (caster != nullptr) {
				if ((caster->field_B2 & 2) != 0)
					return 0;
			}
			CXiActor* target = this->CMoAttachments::GetTargetCheckDoll();
			if (target != nullptr) {
				if ((target->field_B2 & 2) != 0)
					return 0;
			}
		}

		if (cf == true || v234 == 0) {
			if (((gen->flags >> 12) & 1) != 0) {
				gen->DeleteClone(this->GetCaster(), this->GetTarget(), 1, nullptr);
			}

			if (this->field_90 != nullptr) {
				//sub //TODO
				exit(0x10058B27);
			}

			CYyGeneratorClone* clone = gen->Clone(this->GetCaster(), this->GetTarget(), 1);
			if (clone != nullptr) {
				if (gen->CMoAttachments::CheckFlags() == true) {
					clone->CMoAttachments::field_34 = this;
				}

				if (gen->IsNever() == false) {
					if (((clone->flags >> 12) & 1) == 0) {
						float v842 = this->GetMaybeDuration();
						if (v842 == 0.0) {
							v842 = 1.0;
						}

						clone->field_D0 = v842;
					}
				}
			}

		}
		else {
			gen->CMoAttachments::SetCaster(this->GetCaster());
			gen->CMoAttachments::SetTarget(this->GetTarget());
			gen->Activate();
			if (gen->GetModelType() == FFXI::Constants::Enums::ResourceType::Pointlightprog) {
				gen->Idle(nullptr);
			}
		}
		return 0;
	}
	case 0x03:
		//FALLTHROUGH
	case 0x73:
	{
		CXiActor* caster = this->GetMasterCaster();
		CXiActor* target = this->GetMasterTarget();
		CYyScheduler** scheduler = Globals::Get<CYyScheduler**>(this->field_88, 12);
		if (scheduler) {
			if (TagCode == 0x73)
				(*scheduler)->CreateStartTask(caster, target, 1, this->field_90);
			else
				(*scheduler)->CreateStartTask(caster, target, this);
			
		}
		else {
			if (this->field_80) {
				int fourCC = Globals::Get<int>(this->field_88, 8);
				scheduler = *(CYyScheduler***)(*this->field_80)->FindResourceUnder(&unused, Constants::Enums::ResourceType::Scheduler, fourCC);
				std::cout << "Starting new sched task: ";
				for (int z = 0; z < 4; ++z) {
					std::cout << (char)fourCC;
					fourCC >>= 8;
				}

				std::cout << std::endl;
			}

			if (!scheduler || !*scheduler) return false;


			Globals::Set(this->field_88, 12, scheduler);

			CYyScheduler* newsched = *scheduler;
			if ((newsched->PreHeader.field_16 & 8) == 0) 
				newsched->Open();

			if (TagCode == 0x73)
				newsched->CreateStartTask(caster, target, 1, nullptr);
			else
				newsched->CreateStartTask(caster, target, this);
		}
	}
		return 0;
	case 0x4:
	//FALLTHROUGH
	case 0x81: {
		CMoCamera** camera = Globals::Get<CMoCamera**>(this->field_88, 12);
		if (camera == nullptr) {
			int fourcc = Globals::Get<int>(this->field_88, 8);
			CYyScheduler* sched = *this->field_80;
			CMoResource** unused{ nullptr };
			sched->FindResourceUnder((CMoResource***) &camera, Constants::Enums::ResourceType::Camera, fourcc);
			Globals::Set(this->field_88, 12, camera);
			if (CMoResourceMng::CheckResourceThing((CMoResource***)&camera) == false)
				return 0;
		}
		
		CXiActor* target = this->GetTarget();
		CXiActor* caster = this->GetCaster();

		double v290 = this->GetMaybeDuration();
		CMoCameraTask* camtask = (*camera)->Execute(v290, caster, target);
		if (camtask)
			camtask->DoIt(this);
	}
		return 0;
	case 0xF:
		mem = StorageLists::instance->Get(sizeof(CMoDisplayColorTask), Constants::Enums::MEM_MODE::Ex);
		if (mem != nullptr) {
			CMoTaskMng::DeleteThisTask = true;
			colortask = new (mem) CMoDisplayColorTask(0xF, this->field_9C, this->field_88);
		}
		return 0;
	case 0x10:
		//This is a scene cut with a fade transition
		mem = StorageLists::instance->Get(sizeof(CMoTextureDisplayTask), Constants::Enums::MEM_MODE::Ex);
		CMoTaskMng::DeleteThisTask = true;
		if (mem != nullptr) {
			CMoTextureDisplayTask* tdt = new (mem) CMoTextureDisplayTask(this);
			if (CMoTextureDisplayTask::CurrentTextureDisplayTask != nullptr) {
				delete CMoTextureDisplayTask::CurrentTextureDisplayTask;
			}
			CYyDb::TextureDisplayTaskArr[2] = nullptr;
			CYyDb::TextureDisplayTaskArr[0] = FFXI::CYy::CDx::instance->CreateTexture(
				CYyDb::GetBackgroundXRes() >> 1,
				CYyDb::GetBackgroundYRes() >> 1,
				CYyDb::TextureDisplayTaskArr + 2,
				1, D3DPOOL_DEFAULT);

			if (Globals::g_pCenv->field_18 != 0) {
				CYyDb::TextureDisplayTaskArr[3] = nullptr;
				CYyDb::TextureDisplayTaskArr[1] = FFXI::CYy::CDx::instance->CreateTexture(
					CYyDb::GetBackgroundXRes() >> 1,
					CYyDb::GetBackgroundYRes() >> 1,
					CYyDb::TextureDisplayTaskArr + 3,
					1, D3DPOOL_DEFAULT);
			}

			if (CYyDb::TextureDisplayTaskArr[0] != nullptr) {
				CYyDb::TextureDisplayState = 1;
				CMoTextureDisplayTask::progress = 1.0;
				CMoTextureDisplayTask::CurrentTextureDisplayTask = tdt;
				unsigned short duration = *((unsigned short*)this->field_88 + 3);
				tdt->TimeRemaining = duration * this->field_9C;
				tdt->Duration = tdt->TimeRemaining;
			}
			else {
				tdt->TimeRemaining = 0.0;
				tdt->Duration = 1.0;
			}
		}
		return 0;
	case 0x3B:
	{
		CYyScheduler** sched = Globals::Get<CYyScheduler**>(this->field_88, 12);
		CMoSchedulerTask* newTask{ nullptr };
		if (sched == nullptr) {
			int fourCC = Globals::Get<int>(this->field_88, 8);
			(*this->field_80)->FindResourceUnder((CMoResource***)&sched, FFXI::Constants::Enums::ResourceType::Scheduler, fourCC);
			Globals::Set(this->field_88, 12, sched);
			if (sched == nullptr) {
				char* v759 = GetNextTagLocation(this->field_88);
				this->SetNextTag(v759);
				return 2;
			}
			newTask = (*sched)->Execute(this->GetCaster(), this->GetTarget(), 0, this->field_90);
		}
		else {
			newTask = (*sched)->CreateStartTask(this->GetCaster(), this->GetTarget(), this);
		}
		newTask->SetParentTask(this);
		this->SomeRecursive();
		char* v759 = GetNextTagLocation(this->field_88);
		this->SetNextTag(v759);
	}
	return 2;
	case 0x3D:
		//FALLTHROUGH
	case 0xAF:
	{
		//3D is a base game area, AF requires an expack
		char* v874{ nullptr };
		char* v731 = GetNextTagLocation(this->field_88);

		//There are a number of 3b options (which area scene.) This counts them
		int count3B{ 0 };
		while (v731) {
			//0x3E Denotes the end of the option list
			if (*v731 == 0x3E) {
				v874 = v731;
				break;
			}
			++count3B;
			v731 = GetNextTagLocation(v731);
		}

		//Make sure a 3E was found
		if (v874 == nullptr)
			return 0;

		//Check they have required expack installed & registered
		int v732{ 0 };
		if (TagCode == 0xAF)
			v732 = Globals::Get<int>(this->field_88, 8);
		
		//-------------------------------------------------------------

		if (!FFXI::GlobalStruct::g_GlobalStruct.Check14Flag(v732) || !FFXI::GlobalStruct::g_GlobalStruct.Check10Flag(v732))
			count3B = 0;
		
		char* v733 = GetNextTagLocation(this->field_88);

		//Change task duration
		float* edi = this->GetFloatPtr();
		*edi -= this->GetDouble();
		this->field_88 = v874;
		*edi += this->GetDouble();

		if (count3B <= 0) 
			return 0;

		//Pick a "random" 3b to load
		unsigned int v740 = CYyDb::g_pCYyDb->pCMoResourceMng->rng.uirand(count3B);
		while (v740) {
			v733 = GetNextTagLocation(v733);
			--v740;
		}
		this->field_88 = v733;
		int result = this->ExecuteTag();

		//Skip to the 3E
		this->field_88 = v874;
		return result;
	}
	case 0x51:
		mem = StorageLists::instance->Get(sizeof(CMoDisplayColorTask), Constants::Enums::MEM_MODE::Ex);
		if (mem) {
			CMoTaskMng::DeleteThisTask = true;
			colortask = new (mem) CMoDisplayColorTask(0x51, this->field_9C, this->field_88);
		}
		return 0;
	case 0x52:
	{
		XiDateTime v863{}, v864{}, v865{};
		int* intData = (int*)this->field_88;

		this->field_140 = 1;

		v865.SetTickFrame(intData[2]);
		v864.SetTickFrame(intData[3]);

		v863.Time = XiDateTime::current_time.Time;
		XiDateTime dt(0x4F1A00);
		if (v865.Time == v864.Time) {
			v864.SetTickFrame(v864.Time + dt.Time);
		}

		v863.SetTickFrame(v863.Time - v865.Time);
		v864.SetTickFrame(v864.Time - v865.Time);

		if (v864.Time < 0) {
			v864.SetTickFrame(v864.Time + dt.Time);
		}

		if (v863.Time < 0) {
			v863.SetTickFrame(v863.Time + dt.Time);
		}
		if (v863.Time < v864.Time) {
			XiDateTime v862(v863.Time % intData[4]);
			if (v862.Time >= intData[5]) {
				XiDateTime v861{}, arg0(v863.Time + intData[5]);
				v861.SetTickFrame(arg0.Time);
				v861.SetTickFrame(v861.Time % intData[4]);
				if (v861.Time >= intData[5]) {
					this->field_98 = intData[4] - v862.Time;
				}
				else {
					CMoTask* clone = this->Clone(sizeof(CMoSchedulerTask));
					CMoSchedulerTask* sclone = (CMoSchedulerTask*)clone;
					sclone->field_88 = nullptr;
					if ((intData[0] & 0xFF) != 0) {
						sclone->field_88 = this->field_88 + 4 * ((intData[0] >> 8) & 0x1F);
						this->field_98 = intData[4] - v862.Time;
					}
						
				}
			}
			else {
				CMoTask* clone = this->Clone(sizeof(CMoSchedulerTask));
				CMoSchedulerTask* sclone = (CMoSchedulerTask*)clone;
				sclone->field_88 = nullptr;
				if ((intData[0] & 0xFF) != 0) {
					sclone->field_88 = this->field_88 + 4 * ((intData[0] >> 8) & 0x1F);
					this->field_98 = intData[4] - v862.Time;
				}
			}
		}
		else {
			dt.SetTickFrame(dt.Time - v863.Time);
			this->field_98 = dt.Time;
		}
		return 2;
	}
	case 0x72:
		mem = StorageLists::instance->Get(sizeof(CMoDisplayColorTask), Constants::Enums::MEM_MODE::Ex);
		if (mem) {
			CMoTaskMng::DeleteThisTask = true;
			colortask = new (mem) CMoDisplayColorTask(0x72, this->field_9C, this->field_88);
		}
		return 0;
	case 0x7B:
		if (XiZone::zone != nullptr) {
			int zoneID = *(int*)(this->field_88 + 8);
			XiZone::Open(zoneID);
		}
			
		return 0;
	case 0x7C:
		if (XiZone::zone) {
			XiDateTime v860{}, v858(0);
			int frame = *(short*)(this->field_88 + 6);
			v860.SetTickFrame(frame);
			int fourcc = *(int*)(this->field_88 + 8);
			XiZone::zone->SetWeather(fourcc, &v860, v858.Time);
		}
	return 0;
	case 0x7D: {
		int newTime = *(int*)(this->field_88 + 8);
		XiDateTime::SetCurrentTime(newTime);
	}
		return 0;
	default:
		std::cout << "TAGCODE: " << std::hex << (int)TagCode << std::endl;
		return 0;
	}
}

void FFXI::CYy::CMoSchedulerTask::SetNextTag(char* a2)
{
	this->field_88 = a2;
	if (a2 && !*a2 && this->field_A0 != 0.0)
		this->field_98 -= this->field_A0;
}

void FFXI::CYy::CMoSchedulerTask::SetParentTask(CMoSchedulerTask* a2)
{
	this->ParentTask = a2;
}

bool FFXI::CYy::CMoSchedulerTask::CheckSomething()
{
	bool result = true;
	CYyScheduler* sched{ nullptr };
	if (this->field_80)
		sched = *this->field_80;
	
	unsigned char* byteData = (unsigned char*)(sched->field_78);
	while (byteData) {
		if (*byteData == 1) {
			this->StartInit(sched, 0, nullptr);
			result = false;
		}
		if (*byteData) {
			int size = (*(int*)byteData >> 8) & 0x1F;
			byteData += 4 * size;
		}
		else
			byteData = nullptr;

	}
	if (!this->field_141) 
		return result;

	this->StartInit(sched, 0, nullptr);
	return false;
}

CXiActor* FFXI::CYy::CMoSchedulerTask::GetMasterCaster()
{
	CXiActor* caster = this->CMoAttachments::GetMasterCaster();
	if (caster == nullptr)
		return nullptr;

	if (caster->IsKindOf(&CXiDollActor::CXiDollActorClass) == false)
		return caster;

	//Doll Actor stuff
	//sub //TODO
	exit(0x100617E2);
}

CXiActor* FFXI::CYy::CMoSchedulerTask::GetMasterTarget()
{
	CXiActor* target = this->CMoAttachments::GetMasterTarget();
	if (target == nullptr)
		return nullptr;

	if (target->IsKindOf(&CXiDollActor::CXiDollActorClass) == false)
		return target;

	//Doll Actor stuff
	//sub //TODO
	exit(0x10061848);
}

float* FFXI::CYy::CMoSchedulerTask::GetFloatPtr()
{
	return &this->field_98;
}

double FFXI::CYy::CMoSchedulerTask::GetDouble()
{
	return (double)*((short*)this->field_88 + 2) * this->field_9C;
}

double FFXI::CYy::CMoSchedulerTask::GetMaybeDuration()
{
	short value = Globals::Get<short>(this->field_88, 6);
	return (double)value * this->field_9C;
}

CXiActor* FFXI::CYy::CMoSchedulerTask::GetTarget()
{
	return this->CMoAttachments::GetMasterTarget();
}

CXiActor* FFXI::CYy::CMoSchedulerTask::GetCaster()
{
	return this->CMoAttachments::GetMasterCaster();
}
