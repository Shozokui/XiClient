#pragma once
#include "WeatherResource.h"
namespace FFXI {
	namespace CYy {
		class XiZone;
		class WeatherResources {
		public:
			void LoadAll(XiZone*);
			void Load(int, XiZone*);
			CMoKeyframe** GetRandKeyframe(int);
			int field_0;
			int Index;
			WeatherResource Resources[32];
		};
	}
}