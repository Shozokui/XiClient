#pragma once
#include "XiColorEnv.h"
#include "XiWorldEnv.h"
namespace FFXI {
	class XiWeather {
	public:
		static const int* WeatherTables[2];
		static XiWeather default_weather;
		static void SetDefault(XiWeather*);
		XiWeather();
		~XiWeather();
		void Combine(XiWeather*, XiWeather*, float);
		int field_0;
		int field_4;
		int field_8;
		XiColorEnv env1;
		XiColorEnv env2;
		XiWorldEnv world;
	};
}