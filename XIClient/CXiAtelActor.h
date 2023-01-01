#pragma once
#include "CXiActor.h"
namespace FFXI {
	namespace CYy {
		class CXiAtelActor : public CXiActor {
		public:
			static const CYyClass CXiAtelActorClass;
			virtual const CYyClass* GetRuntimeClass() override;
			virtual char OnMove() override;
			D3DXVECTOR4 field_C4;
		};
	}
}