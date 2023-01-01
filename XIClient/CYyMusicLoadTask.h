#pragma once
#include "CMoTask.h"
namespace FFXI {
	namespace CYy {
		class CYyMusicLoadTask : public CMoTask {
		public:
			const static CYyClass CYyMusicLoadTaskClass;
			virtual const CYyClass* GetRuntimeClass() override final;
			virtual void VObj2(int*) override final;
			virtual char OnMove() override final;
			char field_34;
			char field_35;
			char field_36;
			char field_37;
			int field_38;
			int field_3C;
			int field_40;
			int field_44;
			int field_48;
			int field_4C;
			int field_50;
			char field_54;
			char field_55;
			char field_56;
			char field_57;
			int field_58;
		};
	}
}