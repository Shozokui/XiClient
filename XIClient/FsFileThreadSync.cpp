#include "FsFileThreadSync.h"
#include <iostream>
using namespace FFXI::File;

FsFileThreadSync::~FsFileThreadSync() {
	if (this->ErrorQueue) delete this->ErrorQueue;
	if (this->FileQueue) delete this->FileQueue;
	if (this->Semaphore) delete this->Semaphore;
	if (this->fileSleeper) delete this->fileSleeper;
	if (this->mainSleeper) delete this->mainSleeper;
}

FsFileThreadSync::FsFileThreadSync(int p_eventCount) {
	this->FileQueue = new FsFileQueue(p_eventCount);
	this->ErrorQueue = new FsErrorQueue();
	this->Semaphore = new FsSemaphore(1, 1);
	this->fileSleeper = new FsSleeper();
	this->mainSleeper = new FsSleeper();
	this->field_1C = 0;
}

int FFXI::File::FsFileThreadSync::Pause()
{
	return 2 * (this->Semaphore->Wait(INFINITE) >= 0) - 1;
}

int FFXI::File::FsFileThreadSync::Resume()
{
	return 2 * (this->Semaphore->Signal() >= 0) - 1;
}

int FFXI::File::FsFileThreadSync::End()
{
	return 1;
}

int FFXI::File::FsFileThreadSync::mainSleep(DWORD p_ms)
{
	return 2 * (this->mainSleeper->sleep(p_ms) >= 0) - 1;
}

int FFXI::File::FsFileThreadSync::mainAwake()
{
	return 2 * (this->mainSleeper->alarm() >= 0) - 1;
}


int FFXI::File::FsFileThreadSync::fileSleep(DWORD p_ms)
{
	return 2 * (this->fileSleeper->sleep(p_ms) >= 0) - 1;
}

int FFXI::File::FsFileThreadSync::fileAwake()
{
	return 2 * (this->fileSleeper->alarm() >= 0) - 1;
}

int FFXI::File::FsFileThreadSync::ThreadSemaWait()
{
	return 2 * (this->Semaphore->Wait(INFINITE) >= 0) - 1;
}

int FFXI::File::FsFileThreadSync::ThreadSemaSignal()
{
	return 2 * (this->Semaphore->Signal() >= 0) - 1;
}
