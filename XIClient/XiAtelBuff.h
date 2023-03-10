#pragma once
#include "CYyObject.h"
#include "d3dx9math.h"
namespace FFXI {
	namespace CYy {
		class CXiActor;
		class CXiControlActor;
		class XiAtelBuff : public CYyObject{
		public:
			//sub //todo maybe move this elsewhere
			static XiAtelBuff* AtelBuffers[0x900];
			XiAtelBuff();
			void Reset(int);
			void MotionSetChg();
			void IdleDefMotionReset();
			unsigned char GetSomeFlag();
			bool TestSomething(int);
			bool Test74Over700();
			void SetName(char*);
			bool CheckStatus();
			bool CheckGameStatus5(int);
			bool CheckGameStatus85(int);
			void CopyAllSPos();
			void CopyAllPos();
			CXiActor* GetSomeActor();
			D3DXVECTOR4 field_4;
			D3DXVECTOR4 field_14;
			D3DXVECTOR4 field_24;
			D3DXVECTOR4 field_34;
			int field_44;
			int field_48;
			int field_4C;
			int field_50;
			int field_54;
			int field_58;
			int field_5C;
			int field_60;
			int field_64;
			int field_68;
			int field_6C;
			int field_70;
			int GlobalBufferIndex;
			int field_78;
			char Name[25];
			char field_95;
			char field_96;
			char field_97;
			float field_98;
			float field_9C;
			CXiActor* Actor;
			CXiActor* ActorPointers[12];
			int field_D4;
			int field_D8;
			int field_DC;
			int field_E0;
			int field_E4;
			int field_E8;
			char field_EC;
			char field_ED;
			char ActorType;
			unsigned char RaceGender;
			int field_F0;
			__int16 field_F4;
			char field_F6;
			char field_F7;
			int field_F8;
			unsigned short Equipment[16];
			__int16 field_11C;
			__int16 field_11E;
			int field_120;
			unsigned int field_124;
			unsigned int field_128;
			unsigned int field_12C;
			int field_130;
			int field_134;
			int field_138;
			int field_13C;
			char field_140;
			char field_141;
			__int16 field_142;
			__int16 field_144;
			__int16 field_146;
			__int16 field_148;
			__int16 field_14A;
			__int16 field_14C;
			__int16 field_14E;
			int field_150;
			int field_154;
			float field_158;
			int field_15C;
			__int16 field_160;
			__int16 field_162;
			int field_164;
			int field_168;
			int GameStatus;
			int field_170;
			int field_174;
			int field_178;
			int field_17C;
			int field_180;
			int field_184;
			int field_188;
			int field_18C[5];
			int field_1A0[5];
			__int16 field_1B4;
			__int16 field_1B6;
			char field_1B8;
			unsigned char field_1B9;
			char field_1BA;
			char field_1BB;
			__int16 field_1BC;
			__int16 field_1BE;
			int field_1C0;
			int field_1C4;
			int field_1C8;
			int field_1CC;
			int field_1D0;
			__int16 field_1D4;
			__int16 field_1D6;
			int field_1D8;
			int field_1DC;
			int field_1E0;
			int field_1E4;
			int field_1E8;
			int field_1EC;
			int field_1F0;
			unsigned short field_1F4;
			short field_1F6;
			short field_1F8;
			char field_1FA;
			char field_1FB;
			char field_1FC;
			char field_1FD;
			short field_1FE;
			float field_200;
			int field_204;
			int field_208;
			short field_20C;
			short field_20E;
			int field_210;
			int field_214;
			int field_218;
			int field_21C;
			int field_220;
			int field_224;
			int field_228;
			int field_22C;
			int field_230;
			int field_234;
			int field_238;
			int field_23C;
			int field_240;
			int field_244;
			char field_248;
			char field_249;
			char field_24A;
			char field_24B;
			char MonsterNumber;
			char field_24D;
			short field_24E[9];
			int field_260;
			int field_264;
			int field_268;
			int field_26C;
			int field_270;
			int field_274;
			int field_278;
			int field_27C;
			int field_280;
			int field_284;
			int field_288;
			int field_28C;
			int field_290;
			int field_294;
			int field_298;
			short field_29C;
			short field_29E;
			short field_2A0;
			short field_2A2;
			int field_2A4;
		};
	}
}