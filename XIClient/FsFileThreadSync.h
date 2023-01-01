#pragma once
#include "FsFileQueue.h"
#include "FsErrorQueue.h"
#include "FsSemaphore.h"
#include "FsSleeper.h"
#include <wtypes.h>
namespace FFXI {
	namespace File {
		class FsFileThreadSync {
		public:
			virtual ~FsFileThreadSync();
			FsFileThreadSync(int);
			int Pause();
			int Resume();
			int End();
			int mainSleep(DWORD);
			int mainAwake();
			int fileSleep(DWORD);
			int fileAwake();
			int ThreadSemaWait();
			int ThreadSemaSignal();
			FsFileQueue* FileQueue;
			FsErrorQueue* ErrorQueue;
			int field_C;
			FsSleeper* fileSleeper;
			FsSleeper* mainSleeper;
			FsSemaphore* Semaphore;
			int field_1C;
		};
	}
}