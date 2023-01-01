#pragma once

namespace FFXI {
	namespace File {
		class FsSemaphore;
		class FsErrorQueue {
		public:
			virtual ~FsErrorQueue();
			FsErrorQueue();
			int AddError(int);
			int Errors[32];
			int ErrorCount;
			int field_88;
			int NextFreeIndex;
			FsSemaphore* Semaphore;
		};
	}
}