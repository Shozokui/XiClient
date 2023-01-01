#include "Task.h"

using namespace FFXI;

FFXI::Task::Task()
{
	this->field_4 = 1;
	this->field_5 = 0;
	this->field_6 = 0;
	this->ThreadHandle = nullptr;
	this->ThreadID = 0;
}

FFXI::Task::~Task()
{
	if (this->ThreadHandle) {
		while (WaitForSingleObject(this->ThreadHandle, 0) == WAIT_TIMEOUT)
			Sleep(1u);
		CloseHandle(this->ThreadHandle);
	}
	this->field_4 = 0;
	this->field_5 = 0;
	this->ThreadHandle = nullptr;
	this->ThreadID = 0;
}

void FFXI::Task::Init(LPTHREAD_START_ROUTINE a1, LPVOID a2, int a3)
{
	this->ThreadHandle = CreateThread(NULL, 0, a1, a2, CREATE_SUSPENDED, &this->ThreadID);
	if (this->ThreadHandle) {
		this->field_4 = 1;
		this->field_5 = 0;
	}
}
