#pragma once
#include "FsThread.h"
#include "FsFileEvent.h"
#include "FsFileThreadSync.h"

namespace FFXI {
	namespace File {
		class FsFileThreadManager {
		public:
			virtual ~FsFileThreadManager();
			FsFileThreadManager();
			int Init(int);
			int DoEventNow(FsFileEvent*);
			bool checkFileEvent(FsFileEvent*);
			int addFileEvent(FsFileEvent*);
			static int HandleFileEvent(FsFileEvent);
			static int DoRead(FsFileEvent);
			static int DoGetFileSize(FsFileEvent);
			static int DoGetStat(FsFileEvent);
			FsThread* FsThread;
			FsFileThreadSync* ThreadSync;
			static DWORD CALLBACK threadControlFunction(LPVOID);
		};
	}
}