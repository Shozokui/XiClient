#pragma once
#include "d3dx9math.h"
#include "d3d8to9/d3d8.hpp"
namespace FFXI {
	namespace CYy {
		class ModelSubStruct1 {
		public:
			static float g_mss1_float;
			ModelSubStruct1();
			~ModelSubStruct1();
			void Init(D3DXVECTOR3*);
			void DoTheThing(D3DXVECTOR4*);
			bool PrepareToRender(int*, D3DCOLOR);
			char field_4;
			char field_5;
			char field_6;
			char field_7;
			IDirect3DTexture8* field_8;
			IDirect3DTexture8* field_C;
			int field_10;
			D3DXVECTOR3 field_14;
			D3DXVECTOR3 field_20;
		};
	}
}