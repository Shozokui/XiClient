#pragma once
#include <time.h>
#include <sys/types.h>
namespace FFXI {
	namespace File {
		class FileStatTime {
		public:
			char field_0;
			char Second;
			char Minute;
			char Hour;
			char Day;
			char Month;
			short Year;
		};
		class XiFileStat {
		public:
			XiFileStat();
			void setSize(off_t);
			void setCreation(time_t);
			void setLastAccess(time_t);
			void setLastModify(time_t);
			int FileSize;
			FileStatTime CreationTime;
			FileStatTime LastAccess;
			FileStatTime LastModify;
		};
	}
}