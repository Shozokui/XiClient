#include "XiDateTime.h"
#include "XiZone.h"
#include "Globals.h"
#include "CApp.h"
#include "XiZone.h"
#include "CYyDb.h"
#include "NT_SYS.h"
#include <iostream>

using namespace FFXI;

XiDateTime XiDateTime::current_date_time{};
XiDateTime XiDateTime::last_frame_current_date_time{};
int XiDateTime::delta_tick_count{ 0 };

XiDateTime XiDateTime::current_time{};
float XiDateTime::current_time_t{};
bool XiDateTime::enable_game_timer{ true };
bool XiDateTime::event_flag{ false };

FFXI::XiDateTime::XiDateTime()
{
	this->Time = 0;
}

FFXI::XiDateTime::XiDateTime(int a2)
{
	this->SetTickFrame(a2);
}

void FFXI::XiDateTime::EnableGameTimer()
{
	enable_game_timer = true;
	SysMove();
	if (FFXI::CYy::XiZone::zone != nullptr)
		FFXI::CYy::XiZone::zone->UpdateCombineWeather();
}

void FFXI::XiDateTime::DisableGameTimer()
{
	enable_game_timer = false;
}

void FFXI::XiDateTime::SetGameTime(int a1, int a2)
{
	XiDateTime v3{}, v4{};
	XiDateTime::GetCurrentDateTime(&v4);
	v3.SetTickFrame(v4.Time);
	v3.SetHour(a1);
	v3.SetMinute(a2);
	XiDateTime::SetCurrentDateTime(v3.Time);
}

void FFXI::XiDateTime::GetCurrentDateTime(XiDateTime* a1)
{
	XiDateTime v3{};
	v3.SetTickFrame(XiDateTime::current_date_time.Time);
	a1->Time = v3.Time;
}

void FFXI::XiDateTime::GetCurrentDate(XiDateTime* a1)
{
	XiDateTime v4{};
	int v2 = XiDateTime::current_date_time.Time / 0x4F1A00;
	v4.SetDay(v2);
	a1->Time = v4.Time;

}

void FFXI::XiDateTime::SetCurrentDateTime(int a1)
{
	current_date_time.SetTickFrame(a1);
}

void FFXI::XiDateTime::SetCurrentTime(int a1)
{
	XiDateTime v3{};
	v3.Time = a1;

	int Hour = v3.GetHour();
	int Minute = v3.GetMinute();
	int Something1 = v3.GetSomething1();
	int Something2 = v3.GetSomething2();

	current_date_time.SetHour(Hour);
	current_date_time.SetMinute(Minute);
	current_date_time.SetSomething1(Something1);
	current_date_time.SetSomething2(Something2);
}

void FFXI::XiDateTime::SysMove()
{
	bool updated{ false };
	if (enable_game_timer == true) {
		Network::NT_SYS* sys = FFXI::CYy::CApp::g_pCApp->g_pNT_SYS;
		if (CYyDb::g_pCYyDb->field_9 != 0 && sys->IsActive() == true) {
			int gametime = Network::CNtTimeSys::ntGameTimeGet();
			int gamemonth = Network::CNtTimeSys::ntGameMonthGet(gametime);
			int gameday = Network::CNtTimeSys::ntGameDayGet(gametime);
			int gamehour = Network::CNtTimeSys::ntGameHourGet(gametime);

			int MSec = Network::CNtTimeSys::ntTimeGetMSec();
			unsigned int gameminute = (MSec + 1000 * (gametime % 0x90));

			current_date_time.SetMonth(gamemonth);
			current_date_time.SetDay(gameday);
			current_date_time.SetHour(gamehour);
			current_date_time.SetMinute(gameminute / 0x960);
			current_date_time.SetSomething1((gameminute % 0x960) * 0.016666668);
			current_date_time.SetSomething2((gameminute % 0x28) * 0.016666668);
			updated = true;
			current_time_t = (double)((int)(((int)((double)MSec + (gametime % 0xD80) * 1000.0) % 0x34BC00) * 0.025) % 0x15180) * 0.000011574074;
		}
		else
			current_date_time.UpdateFromCYyDb();
	}
	current_time.SetHour(current_date_time.GetHour());
	current_time.SetMinute(current_date_time.GetMinute());
	current_time.SetSomething1(current_date_time.GetSomething1());
	current_time.SetSomething2(current_date_time.GetSomething2());

	if (updated == false)
		current_time_t = 0.00000019290124 * (double)current_time.Time;

	delta_tick_count = current_date_time.Time - last_frame_current_date_time.Time;
	last_frame_current_date_time.SetTickFrame(current_date_time.Time);
}

void FFXI::XiDateTime::GetWeatherTimeNow(XiDateTime* a1)
{
	if (CYyDb::g_pCYyDb->field_9) {
		if (CYy::XiZone::zone->field_1C8.Time >= 0) {
			*a1 = CYy::XiZone::zone->field_1C8;
			return;
		}
	}

	*a1 = XiDateTime::current_time.Time;
}

bool FFXI::XiDateTime::DecNeck()
{
	long long v2 = CYyDb::g_pCYyDb->CheckTick() * -25.0;
	bool result = (int)(v2 + this->Time) < 0;
	this->Time += v2;
	return result;
}

void FFXI::XiDateTime::UpdateFromCYyDb()
{
	double delta = CYyDb::g_pCYyDb->CheckTick() * 25.0;
	this->Time += delta;
}

void FFXI::XiDateTime::SetTickFrame(int a2)
{
	if (a2 > 0x6F3C9000)
		a2 += 0x6F3C9000 * (0xFFFFFFFF - (a2 - 0x6F3C9001) / 0x6F3C9000u);
	
	this->Time = a2;
}

int FFXI::XiDateTime::GetHour()
{
	return this->Time % 0x4F1A00 / 0x34BC0;
}

int FFXI::XiDateTime::GetMinute()
{
	return this->Time % 0x34BC0 / 0xE10;
}

int FFXI::XiDateTime::GetSomething1()
{
	return this->Time % 0xE10 / 60;
}

int FFXI::XiDateTime::GetSomething2()
{
	return this->Time % 60;
}

void FFXI::XiDateTime::SetMonth(int a2)
{
	this->SetTickFrame(155520000 * a2 + this->Time % 155520000);
}

void FFXI::XiDateTime::SetDay(int a2)
{
	this->SetTickFrame(this->Time % 5184000 + 5184000 * (a2 + 30 * (this->Time / 155520000)));
}

void FFXI::XiDateTime::SetHour(int a2)
{
	this->SetTickFrame(this->Time % 216000 + 216000 * (a2 + 24 * (this->Time / 5184000)));
}

void FFXI::XiDateTime::SetMinute(int a2)
{
	this->SetTickFrame(this->Time % 3600 + 3600 * (a2 + 60 * (this->Time / 216000)));
}

void FFXI::XiDateTime::SetSomething1(int a2)
{
	this->SetTickFrame(this->Time % 60 + 60 * (a2 + 60 * (this->Time / 3600)));
}

void FFXI::XiDateTime::SetSomething2(int a2)
{
	this->SetTickFrame(a2 + 60 * (this->Time / 60));
}
