#pragma once
#include "CMoTask.h"
#include "OTStruct.h"
namespace FFXI {
	namespace CYy {
		class CMoOtTask : public CMoTask {
		public:
			static const CYyClass CMoOtTaskClass;
			virtual const CYyClass* GetRuntimeClass() override;
			virtual void VirtOt1();
			CMoOtTask();
			virtual ~CMoOtTask() = default;
			OTStruct field_34;
		};
	}
}