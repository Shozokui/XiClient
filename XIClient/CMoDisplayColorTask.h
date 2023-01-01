#pragma once
#include "CMoTask.h"
namespace FFXI {
	namespace CYy {
		class CMoDisplayColorTask : public CMoTask {
		public:
			CMoDisplayColorTask(int, float, char*);
			~CMoDisplayColorTask();
			virtual char OnMove() override final;
			float TimeLeft;
			float TaskDuration;
			short StartColorGreen;
			short StartColorBlue;
			short StartColorRed;
			short StartColorAlpha;
			unsigned char FinalColorRed;
			unsigned char FinalColorBlue;
			unsigned char FinalColorGreen;
			unsigned char FinalColorAlpha;
			int Type;
		};
	}
}