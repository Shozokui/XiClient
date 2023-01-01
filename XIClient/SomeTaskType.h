#pragma once

#include "CMoTask.h"

namespace FFXI {
	namespace CYy {
		class SomeTaskType : public CMoTask {
		public:
			virtual char OnMove() override final;
			~SomeTaskType();
			int field_34;
		};
	}
}