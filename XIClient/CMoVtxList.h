#pragma once
#include "d3dx9math.h"
namespace FFXI {
	namespace CYy { class CMoSpline; }
	class CMoVtxList {
	public:
		void GetSplinePos(D3DXVECTOR3*, float);
		void RemoveRef();
		float* GetFloatPtr(int);
		FFXI::CYy::CMoSpline* GetSpline();
		int field_0;
		FFXI::CYy::CMoSpline* spline;
		int field_8;
	};
}