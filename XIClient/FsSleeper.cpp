#include "Globals.h"
#include "FsSleeper.h"

using namespace FFXI::File;

FsSleeper::~FsSleeper() {
	SetEvent(this->Handle);
	CloseHandle(this->Handle);
}

FsSleeper::FsSleeper() {
	this->Handle = CreateEvent(NULL, false, true, NULL);
}

int FsSleeper::sleep(DWORD p_ms) {
	if (ResetEvent(this->Handle))
		return 2 * (WaitForSingleObject(this->Handle, p_ms) != WAIT_FAILED) - 1;
	return -1;
}

int FFXI::File::FsSleeper::alarm()
{
	return SetEvent(this->Handle) ? 1 : -1;
}
