#pragma once
#include "StorageListObject.h"
#include <wtypes.h>

namespace FFXI {
	class Task;
	namespace CYy {
		class TaskBuff : public StorageListObject {
		public:
			const static int BuffSize = 8;
			TaskBuff();
			~TaskBuff();
			void AddTask(LPTHREAD_START_ROUTINE, LPVOID, int);
			bool Check();
			int task_wait(int*);
			char field_4;
			char field_5;
			char field_6;
			char field_7;
			int TaskCount;
			Task* Tasks[BuffSize];
		};
	}
}