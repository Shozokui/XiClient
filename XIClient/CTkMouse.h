#pragma once
#include <wtypes.h>
namespace FFXI {
	namespace CTk {
		class _49SubList;
		class CTkMenuCtrlData;
		class CTkMouse {
		public:
			static int SomeFlag;
			CTkMouse();
			~CTkMouse();
			void ResetMouseMoveCenter();
			void UpdateState();
			void OnLButtonDown();
			void OnRButtonDown();
			void OnLButtonUp();
			void OnRButtonUp();
			void OnMButtonDown();
			void OnMButtonUp();
			void OnTrgButtons(bool);
			void RequestJumpWindow();
			void CheckActiveWindow();
			void SetMousePoint(tagPOINT*, float);
			void DrawCursor();
			void SetDrawStatus(unsigned char);
			void OnCalc();
			void OnCalcHelper1();
			void OnCalcHelper2();
			void OnMouseMove(tagPOINT*, short);
			void UpdatePos(tagPOINT*, short);
			float GetMoveH();
			float GetMoveV();
			bool CheckMouse();
			CTkMenuCtrlData* field_0;
			tagPOINT field_4;
			int field_C;
			int field_10;
			char field_14;
			char field_15;
			char field_16;
			char field_17;
			char field_18;
			char field_19;
			char field_1A;
			char field_1B;
			tagPOINT field_1C[3];
			char field_34[3];
			char field_37;
			int field_38;
			int field_3C;
			_49SubList*** field_40;
			short field_44;
			char field_46;
			char field_47;
			float field_48;
			unsigned char field_4C;
			unsigned char field_4D;
			char field_4E;
			char field_4F;
			char field_50;
			char field_51;
			short field_52;
			char field_54;
			char field_55;
			char field_56;
			char field_57;
			int field_58;
			int field_5C;
			int field_60;
			int field_64;
			int field_68;
			char field_6C;
			char field_6D;
			char field_6E;
			char field_6F;
			float field_70;
			char field_74;
			char field_75;
			char field_76;
			char field_77;
			float field_78;
			float field_7C;
			float field_80;
			__int16 field_84;
			char field_86;
			char field_87;
			char field_88;
			char field_89;
			char field_8A;
			char field_8B;
			int CamPointX;
			int CamPointY;
			float field_94;
			float field_98;
			int field_9C;
			char field_A0;
			char field_A1;
			short field_A2;
			short field_A4;
			short field_A6;
			float field_A8;
			char field_AC;
			char field_AD;
			char field_AE;
			char field_AF;
		};
	}
}