#pragma once
#define WIN32_LEAN_AND_MEAN
#include "CIwMenu.h"
#include "d3d8to9/d3d8.hpp"
#include "d3dx9math.h"
namespace FFXI {
	namespace CYy { class CXiDancerActor; }
	namespace CTk {
		class IwCharInfo {
		public:
			char AreaName[16];
			char CharName[16];
			char WorldName[16];
			int CharIndex;
			int Exists;
			char field_38;
			char field_39;
			char field_3A;
			char field_3B;
			int World;
			int RaceGender;
			int MainJob;
			int FaceNo;
			int TownNo;
			int Hair;
			int Size;
			int MainJobLevel;
			int RenameFlag;
			int field_60;
			short Equip[8];
			CYy::CXiDancerActor* field_74;
			D3DXVECTOR4 field_78;
			D3DXVECTOR4 field_88;
			D3DXVECTOR4 field_98;
			char field_A8;
			char field_A9;
			char field_AA;
			char field_AB;
		};
		class CIwSelectMenu : public CIwMenu {
		public:
			static int SelectedCharacter;
			static bool Flag;
			static int g_userfile;
			CIwSelectMenu();
			virtual void OnInitialUpdatePrimitive() override final;
			virtual void OnDrawPrimitive() override final;
			virtual void OnUpdatePrimitive() override final;
			virtual void OnKeyDown(short, short) override final;
			virtual void OnDrawCalc(bool) override final;
			void CamMove();
			int field_14;
			int field_18;
			int field_1C;
			int ContentIDCount;
			int CharacterCount;
			int field_28;
			int field_2C;
			char field_30;
			char field_31;
			char field_32;
			char field_33;
			IwCharInfo charInfo[16];
			int field_AF4;
			double field_AF8[2];
			D3DXVECTOR4 field_B08;
			D3DXVECTOR4 field_B18;
			D3DXVECTOR4 field_B28;
			D3DXVECTOR4 field_B38[3];
			D3DXVECTOR4 field_B68[6];
		};
	}
}