#pragma once
#include <wtypes.h>

namespace FFXI {
	class Task {
	public:
		Task();
		~Task();
		void Init(LPTHREAD_START_ROUTINE, LPVOID, int);
		char field_4;
		char field_5;
		char field_6;
		char field_7;
		HANDLE ThreadHandle;
		DWORD ThreadID;
	};
}