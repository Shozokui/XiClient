#pragma once
#include "CKaTarget.h"

namespace FFXI {
	namespace CTk {
		class CTkInputCtrl {
		public:
			CTkInputCtrl();
			void Init();
			void ResetAllFlag();
			void ResetCallback();
			void SetSomeCoords(int, int);
			CKaTarget field_0;
			CKaTarget field_28;
			char field_50;
			char field_51;
			char field_52;
			char field_53;
			char field_54;
			char field_55;
			char field_56;
			char field_57;
			char field_58;
			char field_59;
			char field_5A;
			char field_5B;
			int field_5C;
			int field_60;
			int field_64;
			short field_68;
			short field_6A;
			int field_6C;
			int field_70;
			char field_74;
			char field_75;
			char field_76;
			char field_77;
			int field_78;
			int field_7C;
			int field_80;
			int field_84;
			int field_88;
			float field_8C;
			float field_90;
			int field_94;
			int field_98;
			char field_9C;
			char field_9D;
			char field_9E;
			char field_9F;
			int field_A0;
			int field_A4;
			int field_A8;
			int field_AC;
			int field_B0;
			int field_B4;
			int field_B8;
			int field_BC;
			int field_C0;
			int field_C4;
			void(__cdecl* CallbackReset)(int, int);
			int field_CC;
			int field_D0;
		};
	}
}