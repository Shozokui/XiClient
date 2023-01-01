#pragma once
#define WIN32_LEAN_AND_MEAN
#include "d3d8to9/d3d8.hpp"
#include "WMatrix.h"
namespace FFXI {
	namespace CYy {
		class CXiOpening;  class CYyCamMng2; class CMoResource; class CMoResourceMng; class CYyResourceFile;
		class CMoTaskMng; class CYyTexMng; class CYyVbMng; class CYyVb; class SomeTaskType; class CXITimerLow; 
		class CTsZoneMap; class CMoDx; class BaseProcessor; class CXiActorDraw;
	}
	namespace CTk {
		class FsMenuDraw;
	}
	namespace Config { class FsConfig; }
	class CTkMsgWinData; class UnknownClass;
	class CYyDb {
		void FsGameLobbyStart();
		void HandleState();
		void OnMove();
		void CheckMoveHandle();
	public:
		static CYyDb* g_pCYyDb;
		~CYyDb();
		CYyDb();
		void InitSubState(short);
		void CountdownTillClose();
		void IncrementSubState();
		char Init(char*);
		void Clean();
		void SomeCalc1();
		char MainFlow();
		bool PhaseInit();
		bool OpeningMovieIsFinished();
		static unsigned short GetBackgroundYRes();
		static unsigned short GetBackgroundXRes();
		static void SetField2FC(float);
		static void SetField300(float);
		static void SetField314(float);
		static void SetField318(float);
		float CheckTick();
		void SetFarColor(unsigned char, unsigned char, unsigned char);
		void SetBackColor(unsigned char, unsigned char, unsigned char);
		void Set324(int);
		void SetField2F8(float);
		void SetField328(int);
		void SetField334(int);
		void SetField338(int);
		void SetView(FFXI::Math::WMatrix*);
		void LinkCameraManager(CYy::CYyCamMng2*);
		static IDirect3DTexture8* SomeTexture1;
		static IDirect3DTexture8* SomeTexture2;
		static IDirect3DTexture8* SomeTexturesKindaLikeCApp2[2];
		static IDirect3DTexture8* TextureDisplayTaskArr[4];
		static int TextureDisplayState;
		static int TextureDisplaySubState[2];
		static Config::FsConfig* g_pFsConfig;
		static CYy::CMoTaskMng* pCMoTaskMng;
		static CYy::CMoResourceMng* pCMoResourceMng;
		static CYy::SomeTaskType* pSomeTaskType;
		static CYy::CYyTexMng* pCYyTexMng;
		static CYy::CYyVbMng* pCYyVbMng;
		static CYy::CYyVb* pCYyVb;
		static CYy::BaseProcessor* pMoProcessor;
		static CYy::CYyResourceFile** ResFile_MENUMissionQuest;
		static CYy::CYyResourceFile** ResFile_MENUUnk1;
		static CYy::CYyResourceFile** ResFile_TEXGeneral;
		static CYy::CYyResourceFile** ResFile_TEXIcons1;
		static CYy::CYyResourceFile** ResFile_TEXIcons2;
		static CYy::CMoResource** Res_Unk1;
		static void* SomeStatic;
		static CTkMsgWinData* MsgWinData1;
		static CTkMsgWinData* MsgWinData2;
		static UnknownClass* UnknownClass;
		static CYy::CTsZoneMap* g_pTsZoneMap;
		static CYy::CXiActorDraw* pCXiActorDraw;
		static int SomeState;
		static int SomeState2;
		static int SomeState3;
		static int SomeState4;
		static unsigned char SomeByte;
		static int SomeFlag;
		static float StaticFloat;
		static float FloatArray[4];
		static int FloatArrayIndex;
		static WORD TimerSeconds;
		static CYy::CXiOpening* XiOpening;
		static bool PatchingComplete;
		static CYy::CYyResourceFile** RES_1;
		static CTk::FsMenuDraw* g_pFsMenuDraw;
		static int PhaseInitCountdown;
		static CYy::CMoDx* g_pCMoDx;
		short SubState;
		short field_6;
		char field_8;
		char field_9;
		char field_A;
		char UseMIPMapping;
		char field_C;
		char field_D;
		char field_E;
		char field_F;
		unsigned short AppWidth;
		unsigned short AppHeight;
		unsigned short UIXRes;
		unsigned short UIYRes;
		unsigned short BackgroundXRes;
		unsigned short BackgroundYRes;
		CYy::CXITimerLow* Timer1;
		CYy::CXITimerLow* Timer2;
		int CloseTimer;
		float field_28;
		float field_2C;
		int FPSDivisor;
		int field_34;
		int field_38;
		int field_3C;
		int field_40;
		int MainState;
		int field_48;
		int field_4C;
		CYy::CYyCamMng2* CameraManager;
		FFXI::Math::WMatrix field_54;
		FFXI::Math::WMatrix field_94;
		FFXI::Math::WMatrix field_D4;
		FFXI::Math::WMatrix field_114;
		FFXI::Math::WMatrix field_154;
		FFXI::Math::WMatrix field_194;
		FFXI::Math::WMatrix field_1D4;
		FFXI::Math::WMatrix field_214;
		FFXI::Math::WMatrix field_254;
		FFXI::Math::WMatrix field_294;
		float field_2D4;
		float field_2D8;
		float field_2DC;
		float field_2E0;
		float field_2E4;
		float field_2E8;
		float field_2EC;
		float field_2F0;
		float field_2F4;
		float field_2F8;
		float field_2FC;
		float field_300;
		D3DCOLOR FarColor;
		D3DCOLOR BackColor;
		char field_30C;
		char field_30D;
		char field_30E;
		char field_30F;
		char field_310;
		char field_311;
		char field_312;
		char field_313;
		float field_314;
		float field_318;
		D3DCOLOR field_31C;
		D3DCOLOR field_320;
		int field_324;
		int field_328;
		unsigned char field_32C;
		unsigned char field_32D;
		unsigned char field_32E;
		unsigned char field_32F;
		float field_330;
		D3DCOLOR field_334;
		int field_338;
	};
}