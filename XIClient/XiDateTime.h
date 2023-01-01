#pragma once

namespace FFXI {
	class XiDateTime {
	public:
		XiDateTime();
		XiDateTime(int);
		static void EnableGameTimer();
		static void DisableGameTimer();
		static void SetGameTime(int, int);
		static void GetCurrentDateTime(XiDateTime*);
		static void GetCurrentDate(XiDateTime*);
		static void SetCurrentDateTime(int);
		static void SetCurrentTime(int);
		static void SysMove();
		static void GetWeatherTimeNow(XiDateTime*);
		static XiDateTime current_date_time;
		static XiDateTime last_frame_current_date_time;
		static int delta_tick_count;

		static XiDateTime current_time;
		static float current_time_t;
		static bool enable_game_timer;
		static bool event_flag;
		bool DecNeck();
		void UpdateFromCYyDb();
		void SetTickFrame(int);
		int GetHour();
		int GetMinute();
		int GetSomething1();
		int GetSomething2();

		void SetMonth(int);
		void SetDay(int);
		void SetHour(int);
		void SetMinute(int);
		void SetSomething1(int);
		void SetSomething2(int);
		int Time;
	};
}