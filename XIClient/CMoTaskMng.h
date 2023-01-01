#pragma once
#include "StorageListObject.h"
#include "CMoTask.h"
namespace FFXI {
	namespace CYy {
		class CMoDisplayColorTask;
		class CMoTaskMng : public StorageListObject {
		public:
			const static CYyClass CMoTaskMngClass;
			virtual const CYyClass* GetRuntimeClass() override final;
			~CMoTaskMng();
			CMoTaskMng();
			static void TaskCallback(CMoTask*);
			static bool DontInitThisTask;
			static bool DeleteThisTask;
			static CMoDisplayColorTask* current_display_color_task_f;
			static CMoDisplayColorTask* current_display_color_task_51;
			static CMoDisplayColorTask* current_display_color_task_72;
			CMoTask* SomeShift(void(__cdecl*)(CMoTask*));
			CMoTask* SomeShift2(CMoTask*);
			CMoTask* SomeShift3(CMoTask*);
			CMoTask* SomeShift4(CMoTask*, int);
			void DoSomething(CMoTask*);
			void DoTasks();
			void InitStackTask(void(__cdecl*)(CMoTask*), int);
			CMoTask* FindInQueue(int, int);
			CMoTask Tasks[64];
			CMoTask* TaskPointers[16];
			CMoTask* field_D44;
			int field_D48;
			CMoTask* field_D4C;
			CMoTask* field_D50;
			CMoTask* field_D54;
			CMoTask* field_D58;
			CMoTask* field_D5C;
			char field_D60;
			char field_D61;
			char field_D62;
			char field_D63;
		};
	}
}