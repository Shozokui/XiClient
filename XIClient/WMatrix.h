#pragma once
#define WIN32_LEAN_AND_MEAN
#include "d3dx9math.h"
namespace FFXI {
	namespace Math {
		class WMatrix : public D3DXMATRIX {
		public:
			WMatrix() = default;
			WMatrix(bool);
			void Identity();
			bool CheckMatrix();
			void Vec3TransformDrop4(D3DXVECTOR3*, D3DXVECTOR3*);
			void Vec3TransformDrop4Self(D3DXVECTOR3*);
			void Vec3TransformNormal(D3DXVECTOR3*, D3DXVECTOR3*);
			void Vec3TransformNormalSelf(D3DXVECTOR3*);
			void Vec4TransformSelf(D3DXVECTOR4*);
			void Vec4MultiplySelf(D3DXVECTOR4*);
			void Vec4Multiply(D3DXVECTOR4*, D3DXVECTOR4*);
			void RotateX(float);
			void RotateY(float);
			void RotateZ(float);
			void Scale3(D3DXVECTOR3*);
			void MatrixMultiply(WMatrix*);
			void MatrixInvert();
			void SomeCombo(WMatrix*);
			void DoSomething();
			bool SomeDrawCalc(D3DXVECTOR3*);
			bool SomeDrawCalc2(D3DXVECTOR3*);
			void CreateScaling(D3DXVECTOR4*);
			void AddTranslation4(D3DXVECTOR4*);
			void AddTranslation3(D3DXVECTOR3*);
			void __fastcall Vec3TransNorm(D3DXVECTOR3*, float, float, float);
			void RHPerspective(float, float, float, float);
		};
	}
}