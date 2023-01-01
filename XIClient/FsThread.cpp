#include "Globals.h"
#include "FsThread.h"
#include "FsFileThreadManager.h"
#include "FsFileThreadSync.h"
#include <iostream>
using namespace FFXI::File;

FsThread::~FsThread() {
	if (!this->field_14) CloseHandle(this->Handle);
}

int FFXI::File::FsThread::Start()
{
	HANDLE Thread = CreateThread(NULL, 0, this->ThreadControlFunction, this->ThreadSync, CREATE_SUSPENDED, &this->ThreadID);
	if (Thread) {
		ResumeThread(Thread);
		this->field_14 = 0;
	}
	else {
		this->field_14 = 1;
		return -1;
	}
	return 0;
}

FsThread::FsThread(LPTHREAD_START_ROUTINE p_func, FsFileThreadSync* p_sync) {
	this->ThreadControlFunction = p_func;
	this->ThreadSync = p_sync;
	this->field_14 = 1;
	this->ThreadID = -1;
	this->Handle = nullptr;
}

void FFXI::File::FsThread::ExitSelf()
{
	ExitThread(0x1234);
}
