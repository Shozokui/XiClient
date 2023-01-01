#pragma once
#include "CXITimer.h"
#include <wtypes.h>
namespace FFXI {
	namespace CYy {
		class CXITimerLow : public CXITimer {
		public:
			const static CYyClass CXITimerLowClass;
			virtual const CYyClass* GetRuntimeClass() override final;
			~CXITimerLow();
			virtual int TimerVirt2() override final;
			virtual double TimerVirt3() override final;
			virtual double TimerVirt4() override final;
			virtual double TimerVirt5() override final;
			CXITimerLow();
			static CXITimerLow* GetNew();
			DWORD field_C{ 0 };
			char field_10{ 0 };
			char field_11{ 0 };
			char field_12{ 0 };
			char field_13{ 0 };
			int field_14{ 0 };
			int field_18{ 0 };
			int field_1C{ 0 };
		};
	}
}