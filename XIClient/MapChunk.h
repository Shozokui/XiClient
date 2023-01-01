#pragma once
#include "WMatrix.h"
namespace FFXI {
	namespace CYy {
		class CMoKeyframe;
		class MmbListNode;
		class XiArea;
		class MapChunk {
		public:
			void DoTransNorms();
			int field_0;
			int field_4;
			char field_8;
			char field_9;
			char field_A;
			char field_B;
			char field_C;
			char field_D;
			char field_E;
			char field_F;
			MmbListNode* field_10;
			MmbListNode* field_14;
			MmbListNode* field_18;
			D3DXVECTOR3 Translation;
			D3DXVECTOR3 Rotation;
			D3DXVECTOR3 Scaling;
			FFXI::Math::WMatrix field_40;
			FFXI::Math::WMatrix TransRotScaler;
			int field_C0;
			int field_C4;
			float field_C8;
			float field_CC;
			float field_D0;
			short field_D4;
			short field_D6;
			char* field_D8;
			int SomeFourCC;
			XiArea* field_E0;
			int field_E4;
			int field_E8;
			int field_EC;
			int field_F0;
			D3DXVECTOR3 field_F4[8];
			short field_154;
			short field_156;
			float field_158;
			float field_15C;
			CMoKeyframe** field_160;
			int field_164;
			int field_168;
		};
	}
}