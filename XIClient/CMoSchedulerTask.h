#pragma once
#include "CMoTask.h"
#include "CMoAttachments.h"
#include "CMoVariant.h"
namespace FFXI {
	namespace CYy {
		class CYyScheduler;
		class CXiActor;
		class CMoSchedulerTask : public CMoTask, public CMoAttachments {
		public:
			const static CYyClass CMoSchedulerTaskClass;
			virtual const CYyClass* GetRuntimeClass() override final;
			virtual char OnMove() override final;
			virtual void VTask4(CMoTask*) override final;
			virtual const CYyClass* AttachedTo() override final;
			virtual bool VirtAttach3() override final;
			virtual bool VirtAttach4() override final;
			virtual CYyObject* GetCYyObject() override final;
			CMoSchedulerTask(CYyScheduler*, unsigned int, void*);
			virtual ~CMoSchedulerTask();
			void StartInit(CYyScheduler*, unsigned int, void*);
			void InitSomething();
			void DoSomething();
			void Execute();
			void SetCaster(CXiActor*);
			void SetTarget(CXiActor*);
			int ExecuteTag();
			void SetNextTag(char*);
			void SetParentTask(CMoSchedulerTask*);
			bool CheckSomething();
			CXiActor* GetMasterCaster();
			CXiActor* GetMasterTarget();
			float* GetFloatPtr();
			double GetDouble();
			double GetMaybeDuration();
			CXiActor* GetTarget();
			CXiActor* GetCaster();
			int field_74;
			int field_78;
			int field_7C;
			CYyScheduler** field_80;
			CMoSchedulerTask* ParentTask;
			char* field_88;
			char* field_8C;
			void* field_90;
			float field_94;
			float field_98;
			float field_9C;
			float field_A0;
			int field_A4;
			int field_A8;
			int field_AC;
			int field_B0;
			int field_B4;
			int field_B8;
			int field_BC;
			CMoVariant Variants[16];
			char field_140;
			char field_141;
			char field_142;
			char field_143;
			char field_144;
			char field_145;
			char field_14A;
			char field_14B;
			//146 out of order for alignment
			int field_146;
		};
	}
}