#pragma once
#include <cstdio>
#include <iostream>
namespace FFXI {
	namespace File {
		class XiFileStat;
		class XIFileBase {
		public:
			enum class XI_OPENMODE {
				Read = 0,
				Write = 1,
				Append = 2
			};
			enum class XI_SEEKMODE {
				Set = SEEK_SET,
				Cur = SEEK_CUR,
				End = SEEK_END
			};
			static int Open(FILE**, char*, XI_OPENMODE);
			static int Read(FILE*, char*, int);
			static int Seek(FILE*, int, XI_SEEKMODE);
			static int Close(FILE*);
			static off_t GetFileSize(char*);
			static int GetFileState(char*, XiFileStat*);
			static void CreatePath(char*);
		};
	}
}