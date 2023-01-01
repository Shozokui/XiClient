#pragma once
#include "CMoTask.h"
namespace FFXI {
	namespace CYy {
		class CXiActorDraw : public CMoTask {
		public:
			static const CYyClass CXiActorDrawClass;
			virtual const CYyClass* GetRuntimeClass() override;
			virtual char OnMove() override final;
		};
	}
}