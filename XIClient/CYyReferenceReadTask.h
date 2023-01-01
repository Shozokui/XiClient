#pragma once
#include "CMoTask.h"

namespace FFXI {
	namespace File { class ReferenceReadBase; }
	namespace CYy {
		class CYyResourceFile;
		class CYyReferenceReadTask : public CMoTask {
		public:
			virtual void VObj2(int*) override final;
			virtual char OnMove() override final;
			CYyReferenceReadTask();
			virtual ~CYyReferenceReadTask();
			void Start(CYyResourceFile**, void(__cdecl*)(CYyResourceFile**, File::ReferenceReadBase*), File::ReferenceReadBase*, char);
			CYyResourceFile** ResourceFile;
			void(__cdecl* Callback)(CYyResourceFile**, File::ReferenceReadBase*);
			File::ReferenceReadBase* Context;
			char field_40;
			char field_41;
			char field_42;
			char field_43;
		};
	}
}