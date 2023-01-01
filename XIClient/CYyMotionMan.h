#pragma once
#include "CYyMotionQueList.h"
namespace FFXI {
	namespace CYy {
		class CXiActor;
		class MotionManStaticStruct;
		class CMoMo2;
		class CYyMotionMan {
		public:
			static MotionManStaticStruct g_mmss_arr[256];
			void AppendSync(CXiActor*, CMoMo2**, float, float, int, float, float, int, float, float);
			void Append(CXiActor*, CMoMo2**, float, float, int, float, float, int, float, float);
			
			bool SetStatics();
			CYyMotionQueList motions[7];
		};
	}
}