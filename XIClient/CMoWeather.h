#pragma once
#include "CMoResource.h"
#include "XiWeather.h"
namespace FFXI {
	namespace CYy {
		class CMoWeather : public CMoResource {
		public:
			virtual void Open() override final;
			CMoWeather();
			virtual ~CMoWeather() = default;
			XiWeather field_30;
		};
	}
}