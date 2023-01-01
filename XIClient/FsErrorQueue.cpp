#include "FsErrorQueue.h"
#include "FsSemaphore.h"
#include <cstring>
using namespace FFXI::File;

FFXI::File::FsErrorQueue::~FsErrorQueue()
{
	this->Semaphore->Signal();
	if (this->Semaphore) {
		delete this->Semaphore;
		this->Semaphore = nullptr;
	}
}

FFXI::File::FsErrorQueue::FsErrorQueue()
{
	std::memset(this->Errors, 0, sizeof(this->Errors));
	this->field_88 = 0;
	this->NextFreeIndex = 0;
	this->ErrorCount = 0;
	this->Semaphore = new FsSemaphore(1, 1);
}

int FFXI::File::FsErrorQueue::AddError(int p_error)
{
	this->Semaphore->Wait(INFINITE);
	if (this->ErrorCount + 1 >= 32)
		return this->Semaphore->Signal();
	this->Errors[this->NextFreeIndex] = p_error;
	this->NextFreeIndex += 1;
	if (this->NextFreeIndex == 32)
		this->NextFreeIndex = 0;
	this->ErrorCount += 1;
	return this->Semaphore->Signal();
}
