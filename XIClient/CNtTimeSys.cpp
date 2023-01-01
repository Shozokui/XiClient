#include "CNtTimeSys.h"
#include "Globals.h"
#include "CApp.h"
#include "NT_SYS.h"
using namespace FFXI::Network;

int CNtTimeSys::gtime{ 0 };

int checkTime(int a1, int a2, int a3) {
	if (a3 >= a2)
		return (a3 - a2);

	return (a3 + a1 - a2);
}
void CNtTimeSys::ntTimeSet(int a2, int a3)
{
	CNtTimeSys* pTimeSys = &FFXI::CYy::CApp::g_pCApp->g_pNT_SYS->g_TimeSys;
	pTimeSys->field_C = a2;
	pTimeSys->field_10 = a3;
	pTimeSys->field_14 = 0;
	pTimeSys->field_0 = timeGetTime();

	for (int i = 0; i < 10; ++i) {
		pTimeSys->field_18[i] = 0;
		pTimeSys->field_40[i] = 0;
	}
}

void CNtTimeSys::ntGameTimeSet(int a1)
{
	gtime = a1 - FFXI::CYy::CApp::g_pCApp->g_pNT_SYS->g_TimeSys.ntTimeNowGetSec();
}

int CNtTimeSys::ntGameTimeGet()
{
	return gtime + ntTimeNowGetSec();
}

int CNtTimeSys::ntGameMonthGet(int a1)
{
	return a1 % 0x12FC00 / 0x19500;
}

int CNtTimeSys::ntGameDayGet(int a1)
{
	return a1 % 0x19500 / 0xD80;
}

int CNtTimeSys::ntGameHourGet(int a1)
{
	return a1 % 0xD80 / 0x90;
}

int CNtTimeSys::ntTimeNowGetSec()
{
	UpdateTime();
	return ntTimeGetSec();
}

void CNtTimeSys::UpdateTime()
{
	CNtTimeSys* pTimeSys = &FFXI::CYy::CApp::g_pCApp->g_pNT_SYS->g_TimeSys;

	int timeNow = timeGetTime();
	int v3 = checkTime(-1, pTimeSys->field_0, timeNow);
	pTimeSys->field_0 = timeNow;
	
	if (v3 < 0) return;

	if (pTimeSys->field_14 && v3 > 20) {
		int v6 = pTimeSys->field_14;
		if (pTimeSys->field_14 >= 0) {
			if (pTimeSys->field_14 > 20)
				v6 = 20;
			if (v6 > v3)
				v6 = v3;
		}
		else {
			if (pTimeSys->field_14 < -20)
				v6 = -20;
			if (v6 < -v3)
				v6 = -v3;
		}
		v3 += v6;
		pTimeSys->field_14 -= v6;
	}
	pTimeSys->field_10 += v3;
	while (pTimeSys->field_10 >= 1000) {
		++pTimeSys->field_C;
		pTimeSys->field_10 -= 1000;
	}
}

int CNtTimeSys::ntTimeGetSec()
{
	return FFXI::CYy::CApp::g_pCApp->g_pNT_SYS->g_TimeSys.field_C;
}

unsigned int CNtTimeSys::ntTimeNowGetMSec()
{
	UpdateTime();
	return ntTimeGetMSec();
}

unsigned int CNtTimeSys::ntTimeGetMSec()
{
	return FFXI::CYy::CApp::g_pCApp->g_pNT_SYS->g_TimeSys.field_10;
}

bool CNtTimeSys::ntTimeProc()
{
	CNtTimeSys* pTimeSys = &FFXI::CYy::CApp::g_pCApp->g_pNT_SYS->g_TimeSys;
	UpdateTime();
	pTimeSys->field_8 = checkTime(1000, pTimeSys->field_4, pTimeSys->field_10);
	pTimeSys->field_4 = pTimeSys->field_10;
	pTimeSys->field_54 = 1;
	return true;
}
