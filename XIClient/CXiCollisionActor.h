#pragma once
#include "CXiControlActor.h"
namespace FFXI {
	namespace CYy {
		class CXiCollisionActor : public CXiControlActor {
		public:
			static const CYyClass CXiCollisionActorClass;
			virtual const CYyClass* GetRuntimeClass() override;
			virtual char OnMove() override;
			CXiCollisionActor();
			D3DXVECTOR4 field_5C4;
			int field_5D4;
			int field_5D8;
			int field_5DC;
			char field_5E0;
			char field_5E1;
			char field_5E2;
			char field_5E3;
			int field_5E4;
			int field_5E8;
			char field_5EC;
			char field_5ED;
			char field_5EE;
			char field_5EF;
			int field_5F0;
			char field_5F4;
			char field_5F5;
			char field_5F6;
			char field_5F7;
			virtual int VirtActor88() override final;
			virtual char VirtActor92() override final;
			virtual char VirtActor93() override final;
			virtual D3DXVECTOR4* VirtActor101() override;
			virtual char VirtActor242() override final;
			virtual void VirtActor243(char) override final;
		};
	}
}