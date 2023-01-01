#pragma once
#include "CMoResource.h"
namespace FFXI {
	namespace CYy {
		class MotionManStaticStruct;
		class CMoMo2 : public CMoResource {
		public:
			CMoMo2();
			virtual ~CMoMo2() = default;
			virtual void Open() override final;
			virtual void Close() override final;
			void CalcMotionData(int, float, float, MotionManStaticStruct*);
			bool BlendZero(int, int, unsigned int, float, MotionManStaticStruct*);
			bool BlendOne(int, int, unsigned int, float, float, MotionManStaticStruct*);
			char Data[12];
		};
	}
}