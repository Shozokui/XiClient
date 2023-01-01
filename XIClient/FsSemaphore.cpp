#include "Globals.h"
#include "FsSemaphore.h"
using namespace FFXI::File;

FsSemaphore::~FsSemaphore() {
	ReleaseSemaphore(this->Handle, 1, NULL);
	CloseHandle(this->Handle);
}

FsSemaphore::FsSemaphore(LONG p_initial, LONG p_max) {
	this->Handle = CreateSemaphoreA(NULL, p_initial, p_max, NULL);
	if (!this->Handle) GetLastError();
}

int FsSemaphore::Wait(DWORD p_milliseconds) {
	return 2 * (WaitForSingleObject(this->Handle, p_milliseconds) != WAIT_FAILED) - 1;
}

int FFXI::File::FsSemaphore::Signal()
{
	return ReleaseSemaphore(this->Handle, 1, NULL) ? 1 : -1;
}
