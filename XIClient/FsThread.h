#pragma once
#include <wtypes.h>
namespace FFXI {
	namespace File {
		class FsFileThreadSync;
		class FsFileThreadManager;
		class FsThread {
		public:
			virtual ~FsThread();
			virtual int Start();
			FsThread(LPTHREAD_START_ROUTINE, FsFileThreadSync*);
			static void ExitSelf();
			HANDLE Handle;
			DWORD ThreadID;
			LPTHREAD_START_ROUTINE ThreadControlFunction;
			FsFileThreadSync* ThreadSync;
			int field_14;
		};
	}
}