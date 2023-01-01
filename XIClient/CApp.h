#pragma once
#include "StorageListObject.h"
#include <wtypes.h>
namespace FFXI {
	namespace Input { class InputMng; }
	namespace Network { class NT_SYS; }
	namespace CYy {
		class CApp : public StorageListObject {
		public:
			static CApp* g_pCApp;
			static bool Initialize(HWND);
			static void Uninitialize();
			const static CYyClass CAppClass;
			virtual const CYyClass* GetRuntimeClass() override final;
			CApp(char*, HINSTANCE, HWND, char*, char*);
			~CApp();
			static void WinMsgHandler(MSG* msg);
			static bool InitInput(HINSTANCE);
			static int GetAppState();
			static void SetAppState(int);
			static Input::InputMng* g_pInputMng;
			static int AppState;
			static Network::NT_SYS* g_pNT_SYS;
			static Network::NT_SYS g_NT_SYS;
			static UINT period;
			int ClientTick();
			int Execute();
			void NonFullscreenStuff(HWND);
			void NonFullscreenClean();
			char* SomeDateString{nullptr};
			char* SomeTimeString{nullptr};
			char* VersionString{nullptr};
			char* FFXIDirectory{nullptr};
			HINSTANCE hmod{};
			HWND hWnd{};
			char field_1C{0};
			char field_1D{0};
			char field_1E{0};
			char field_1F{0};
			RECT Rect{};
			char field_30{0};
			char field_31{0};
			char field_32{0};
			char field_33{0};
			char field_34{0};
			char field_35{0};
			char field_36{0};
			char field_37{0};
			HANDLE field_38{nullptr};
			char field_3C{0};
			char field_3D{0};
			char field_3E{0};
			char field_3F{0};
			HWND field_40{};
		};
	}
}