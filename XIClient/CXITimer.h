#pragma once
#include "StorageListObject.h"

namespace FFXI {
	namespace CYy {
		class CXITimer : public StorageListObject {
		public:
			const static CYyClass CXITimerClass;
			virtual const CYyClass* GetRuntimeClass() override;
			virtual void TimerVirt1();
			virtual int TimerVirt2() = 0;
			virtual double TimerVirt3() = 0;
			virtual double TimerVirt4() = 0;
			virtual double TimerVirt5() = 0;
			virtual double TimerVirt6();
			virtual float TimerVirt7(float);
			virtual ~CXITimer();
			CXITimer();
			float field_4{ 0 };
			float field_8{ 0 };
		};
	}
}