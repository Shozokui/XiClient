#pragma once
#include <wtypes.h>
namespace FFXI {
	namespace File {
		class FsSemaphore {
		public:
			~FsSemaphore();
			FsSemaphore(LONG, LONG);
			int Wait(DWORD);
			int Signal();
			HANDLE Handle;
		};
	}
}