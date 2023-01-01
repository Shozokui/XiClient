#pragma once
#include "StorageListObject.h"
namespace FFXI {
	namespace CYy {
		class CMoTask : public StorageListObject {
		public:
			const static CYyClass CMoTaskClass;
			virtual const CYyClass* GetRuntimeClass() override;
			virtual void VTask1();
			virtual char OnMove();
			virtual void OptionalMove();
			virtual void VTask4(CMoTask*);
			CMoTask();
			virtual ~CMoTask();
			void Reset();
			void Destroy();
			void Clean(int);
			void DoSomething(CMoTask*);
			void DoSomethingManager();
			void RecurseDelete();
			int SomeRecursive();
			void SomeRecursive2();
			void DoSomething2();
			void DoSomething3();
			void TaskMngShift4(int);
			void DoIt(CMoTask*);
			void DoItHelper(CMoTask*);
			CMoTask* Clone(unsigned int);

			void(__cdecl* Callback)(CMoTask*);
			CMoTask* field_8;
			CMoTask* field_C;
			CMoTask* field_10;
			int field_14;
			int Param1;
			int Param2;
			char field_20;
			char field_21;
			__int16 field_22;
			CMoTask* field_24;
			CMoTask* field_28;
			int field_2C;
			char field_30;
			char field_31;
			char field_32;
			char field_33;
		};
	}
}