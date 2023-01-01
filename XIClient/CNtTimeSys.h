#pragma once
#include <time.h>
#include <wtypes.h>
namespace FFXI {
	namespace Network {
		class CNtTimeSys {
		public:
			DWORD field_0;
			static int gtime;
			static void UpdateTime();
			static void ntTimeSet(int, int);
			static int ntTimeNowGetSec();
			static int ntTimeGetSec();
			static unsigned int ntTimeNowGetMSec();
			static unsigned int ntTimeGetMSec();

			static void ntGameTimeSet(int);
			static int ntGameTimeGet();
			static int ntGameMonthGet(int);
			static int ntGameDayGet(int);
			static int ntGameHourGet(int);
			static bool ntTimeProc();
			int field_4;
			int field_8;
			int field_C;
			int field_10;
			int field_14;
			int field_18[10];
			char field_40[10];
			short field_4A;
			int field_4C;
			int field_50;
			int field_54;
		};
	}
}