#pragma once
#include "d3dx9math.h"
namespace FFXI {
	class KO_Path {
	public:
		void GetDirection(D3DXVECTOR3*, D3DXVECTOR3*);
		int field_0;
		int field_4;
		int field_8;
		int field_C;
		float* field_10;
		int* field_14;
		int field_18;
		int field_1C;
	};
}