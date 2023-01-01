#pragma once
#include "CMoTask.h"
namespace FFXI {
	namespace CYy {
		class CMoTextureDisplayTask : public CMoTask {
		public:
			static CMoTextureDisplayTask* CurrentTextureDisplayTask;
			static float progress;
			CMoTextureDisplayTask(CMoTask*);
			~CMoTextureDisplayTask();
			virtual char OnMove() override final;
			float TimeRemaining;
			float Duration;
		};
	}
}