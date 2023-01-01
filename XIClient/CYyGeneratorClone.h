#pragma once
#include "CYyGenerator.h"
namespace FFXI {
	namespace CYy {
		class CYyGeneratorClone : public CYyGenerator{
		public:
			static const CYyClass CYyGeneratorCloneClass;
			virtual const CYyClass* GetRuntimeClass() override final;
			virtual void VObj2(int*) override final;
			virtual const CYyClass* AttachedTo() override final;
			virtual bool VirtAttach3() override final;
			virtual bool VirtAttach4() override final;
			virtual CYyObject* GetCYyObject() override final;
			CYyGeneratorClone() = default;
			virtual ~CYyGeneratorClone();
			void DeactivateClone();
			int field_F0;
			CMoResource** field_F4;
			int field_F8;
		};
	}
}