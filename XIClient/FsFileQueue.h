#pragma once

namespace FFXI {
	namespace File {
		class FsFileEvent;
		class FsSemaphore;
		class FsFileQueue {
		public:
			~FsFileQueue();
			FsFileQueue(int);
			bool hasFileEvent();
			bool isInQueue(FsFileEvent*);
			int addFileEvent(FsFileEvent*);
			FsFileEvent* startNextEvent(FsFileEvent*);
			int endNextEvent();
			int cancelNextEvent();

			//Buffer size is set in ctor
			FsFileEvent* EventBuffer;
			int BufferSize;
			int FileCount;
			int BufferReadPos;
			int BufferWritePos;
			char field_18;
			char field_19;
			char field_1A;
			char field_1B;
			FsSemaphore* Semaphore;
		};
	}
}