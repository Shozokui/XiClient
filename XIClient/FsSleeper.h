#pragma once
#include <wtypes.h>
namespace FFXI {
	namespace File {
		class FsSleeper {
		public:
			virtual ~FsSleeper();
			FsSleeper();
			int sleep(DWORD);
			int alarm();
			HANDLE Handle;
		};
	}
}