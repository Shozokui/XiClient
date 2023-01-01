#pragma once
#include "CMoResource.h"
#include "CMoAttachments.h"
namespace FFXI {
	namespace CYy {
		class CMoSchedulerTask;
		class CXiActor;
		class CYyScheduler : public CMoResource {
		public:
			CYyScheduler();
			virtual ~CYyScheduler();
			virtual void ConstructFromData(char*) override final;
			virtual void Open() override final;
			virtual void Close() override final;
			CMoSchedulerTask* CreateStartTask(CXiActor*, CXiActor*, int, void*);
			CMoSchedulerTask* CreateStartTask(CXiActor*, CXiActor*, CMoSchedulerTask*);
			CMoSchedulerTask* Execute(int, void*);
			CMoSchedulerTask* Execute(CXiActor*, CXiActor*, int, void*);
			void CalcTotalFrame();
			bool IsLoopScheduler();
			char* FindFirstTag(int);
			char* FindFirstTag2(int);
			void Stop();
			CMoSchedulerTask* CreateSchedulerTask(CXiActor*, CXiActor*, void*, int);
			CMoAttachments Attachments;
			int field_70;
			int field_74;
			int field_78;
			unsigned int field_7C;
			int field_80;
			int field_84;
			int field_88;
			int field_8C;
			int field_90;
			int field_94;
		};
	}
}