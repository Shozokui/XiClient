#pragma once
#include "WMatrix.h"
namespace FFXI {
	namespace Math {
		class KzFQuat : public D3DXQUATERNION {
		public:
			KzFQuat();
			void Init();
			void SetQuatFromVector(D3DXVECTOR3*, D3DXVECTOR3*, float);
			void GetMatrix(WMatrix*);
			void Normalize();
			void Multiply(KzFQuat*, KzFQuat*);
			void DoTheThing(D3DXVECTOR3*, D3DXVECTOR3*, float);
			void lerp(KzFQuat*, KzFQuat*, float);
		};
	}
}