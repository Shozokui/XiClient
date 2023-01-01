#pragma once
#include "CXiSkeletonActor.h"
#include "XiAtelBuff.h"
#include "d3dx9math.h"
namespace FFXI {
	namespace CYy {
		class XiDancerActorPara;
		class CXiDancerActor : public CXiSkeletonActor {
		public:
			static const CYyClass CXiDancerActorClass;
			virtual const CYyClass* GetRuntimeClass() override;
			CXiDancerActor(D3DXVECTOR4*, D3DXVECTOR4*, XiDancerActorPara*, int);
			void AtelBuffSet(D3DXVECTOR4*, D3DXVECTOR4*, XiDancerActorPara*, int);
			void AtelBuffInit(XiAtelBuff*, D3DXVECTOR4*, D3DXVECTOR4*, XiDancerActorPara*);
			XiAtelBuff field_A0C;
			virtual void OnDraw() override final;
			virtual char OnMove() override final;
		};
	}
}