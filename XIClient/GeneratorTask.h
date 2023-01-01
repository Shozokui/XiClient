#pragma once
#include "CMoTask.h"
namespace FFXI {
	namespace CYy {
		class CYyGenerator;
		class GeneratorTask : public CMoTask {
		public:
			virtual void VObj2(int*);
			virtual void OptionalMove() override final;
			~GeneratorTask();
			CYyGenerator** field_34;
			CYyGenerator** field_38;
			char field_3C;
			char field_3D;
			char field_3E;
			char field_3F;
		};
	}
}