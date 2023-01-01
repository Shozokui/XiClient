#pragma once
#define WIN32_LEAN_AND_MEAN
#include "CNtGame.h"
#include "CNtDebug.h"
#include "CNtTimeSys.h"
#include "CNtThreadSys.h"
#include "CNtUdpSys.h"
#include "CGcMainSys.h"

namespace FFXI {
	namespace Network {
		class NT_SYS_BASE {
		public:
			static CNtGame g_Game;
			static CNtGame* pGame;
			static CNtDebug g_Debug;
			static CNtDebug* pDebug;
			static CNtTimeSys g_TimeSys;
			static CNtTimeSys* pTimeSys;
			static CNtThreadSys g_ThreadSys;
			static CNtThreadSys* pThreadSys;
			static CNtUdpSys g_UdpSys;
			static CNtUdpSys* pUdpSys;
			static CGcMainSys g_GcMainSys;
			static CGcMainSys* pGcMainSys;
			static unsigned int StrToIp(const char*);
			static bool ntIpgethostname(void*);
			bool IsActive();
			bool Init();
			bool ntGameInit();
			bool ntGameEnd();
			bool gcMainWorkInit(int, int, short, char*, char*, char*, int);
			void ntTimeInit();
			void ntThreadInit();
			void ntGameProc2();
			int ntIpInit();
			int ntUdpInit();
			void ntUdpEnd();
			void End();
			char field_0;
			char field_1;
			char field_2;
			char field_3;

			class NTGameInitStruct {
			public:
				NTGameInitStruct();
				int field_0;
				int ZoneNumber;
				int WorldNumber;
				int CliMode;
				int field_10;
				int field_14;
				int field_18;
				char WorldLocallyHosted;
				char field_1D;
				short WorldPort;
				unsigned short CenterPort;
				short field_22;
				int CenterIP;
				int WorldIP;
				int MyIP;
				char Name[0x50];
				char Password[0x50];
				int pcnt;
				short ClientPort;
				short Polcon;
				short DnsMode;
				short field_DA;
				int field_DC;
			};
		};
	}
}