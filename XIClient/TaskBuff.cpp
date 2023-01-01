#include "TaskBuff.h"
#include "Task.h"
#include <string>

using namespace FFXI::CYy;

TaskBuff::TaskBuff()
{
	this->TaskCount = 0;
	memset(&this->Tasks, 0, sizeof(this->Tasks));
	this->field_4 = 1;
}

TaskBuff::~TaskBuff()
{
	this->field_4 = 0;
	for (int i = 0; i < TaskBuff::BuffSize; ++i) {
		Task* task = this->Tasks[i];
		if (task) {
			if (task->ThreadHandle)
				task->field_6 = 1;
			if (task) {
				delete task;
				task = nullptr;
			}
		}
	}
}

void TaskBuff::AddTask(LPTHREAD_START_ROUTINE a2, LPVOID a3, int a4)
{
	if (this->TaskCount < TaskBuff::BuffSize) {
		Task* newTask = new Task();
		if (newTask) {
			newTask->Init(a2, a3, a4);
			this->Tasks[this->TaskCount++] = newTask;
		}
	}
	else {
		;//Thread Num Over!!
	}
}

bool FFXI::CYy::TaskBuff::Check()
{
	if (!this->TaskCount)
		return true;

	Task* task = this->Tasks[this->TaskCount - 1];
	if (!task->ThreadHandle)
		return task->field_6;

	if (task->field_5 != 1)
		return task->field_6;

	task->field_5 = 0;

	do {
		if (task->field_6)
			break;
		Sleep(1u);
	} while (!task->field_5);
	return task->field_6;
}

int FFXI::CYy::TaskBuff::task_wait(int* a1)
{
	bool check = Check();
	if (a1) {
		if (check)
			return -1;
		while (*a1 != 1) {
			if (Check())
				return -1;
		}
	}
	else if (check) {
		return -1;
	}
	return 0;
}
