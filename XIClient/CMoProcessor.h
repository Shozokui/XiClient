#pragma once
#include "CYyObject.h"

namespace FFXI {
	namespace CYy {
		class CMoProcessor : public CYyObject {
		protected:
			CMoProcessor() = default;
		public:
			const static CYyClass CMoProcessorClass;
			const CYyClass* GetRuntimeClass() override;
			int field_4;
			int field_8;
			int field_C;
		};
	}
}