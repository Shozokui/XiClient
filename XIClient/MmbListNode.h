#pragma once
#define WIN32_LEAN_AND_MEAN
#include "d3dx9math.h"
namespace FFXI {
	namespace CYy {
		class MmbStruct;
		class MmbListNode {
		public:
			MmbListNode* NextNode;
			int* data;
			char name[0x10];
			int field_18;
			int field_1C;
			int VertexFVF;
			int field_24;
			int field_28;
			D3DPRIMITIVETYPE field_2C;
			int field_30;
			int field_34;
			int field_38;
			int field_3C;
			int field_40;
			int field_44;
			float field_48[6];
			MmbStruct* field_60;
			int field_64;
			int field_68;
			float field_6C;
			D3DXVECTOR3 field_70[8];
			int field_D0;
			float field_D4;
			int field_D8;
			char field_DC;
			char field_DD;
			char field_DE;
			char field_DF;
			D3DXVECTOR4 field_E0;
		};
	}
}