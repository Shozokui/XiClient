#pragma once
#include "WMatrix.h"
namespace FFXI {
	namespace CYy {
		class RidStruct {
		public:
			FFXI::Math::WMatrix field_0;
			int field_40;
			int field_44;
			int field_48;
			D3DXVECTOR3 field_4C[8];
			D3DXVECTOR3 Position;
			float field_B8;
			float field_BC;
			int field_C0;
			int field_C4;
			int field_C8;
			int field_CC;
			int field_D0;
			int field_D4;
			int* field_D8;
			int field_DC;
			int FourCC;
			int field_E4;
			int field_E8;
			int field_EC;
			int field_F0;
		};
	}
}