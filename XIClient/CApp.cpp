#include "Globals.h"
#include "CApp.h"
#include "InputMng.h"
#include "CYyDb.h"
#include "RegistryConfig.h"
#include "CDx.h"
#include "XIFileManager.h"
#include "CEnv.h"
#include "RuntimeConfig.h"
#include <iostream>
#include <commdlg.h>
#include <timeapi.h>

using namespace FFXI::CYy;

CApp* FFXI::CYy::CApp::g_pCApp{ nullptr };
UINT FFXI::CYy::CApp::period{};

const CYyClass CApp::CAppClass{
	"CApp", sizeof(CApp), &CYyObject::CYyObjectClass
};

FFXI::Input::InputMng* CApp::g_pInputMng{ nullptr };
int CApp::AppState{ 0 };

FFXI::Network::NT_SYS CApp::g_NT_SYS{};
FFXI::Network::NT_SYS* CApp::g_pNT_SYS{ &CApp::g_NT_SYS };

MMRESULT InitPeriod() {
	MMRESULT result; // eax
	UINT wPeriodMin; // eax
	UINT v2; // ecx
	struct timecaps_tag ptc; // [esp+0h] [ebp-8h] BYREF

	FFXI::CYy::CApp::period = 0;
	result = timeGetDevCaps(&ptc, 8u);
	if (result != MMSYSERR_NOERROR) {
		return result;
	}
		
	wPeriodMin = ptc.wPeriodMin;
	v2 = ptc.wPeriodMin;
	if (ptc.wPeriodMin <= 1)
		v2 = 1;
	if (v2 >= ptc.wPeriodMax)
	{
		wPeriodMin = ptc.wPeriodMax;
	}
	else if (ptc.wPeriodMin <= 1)
	{
		wPeriodMin = 1;
	}

	FFXI::CYy::CApp::period = wPeriodMin;
	timeBeginPeriod(wPeriodMin);
	return MMSYSERR_NOERROR;
}
bool FFXI::CYy::CApp::Initialize(HWND hwnd)
{
	//CApp init
	char* mem = StorageLists::instance->Get(sizeof(CYy::CApp), Constants::Enums::MEM_MODE::Ex);
	if (mem == nullptr) {
		return false;
	}

	FFXI::CYy::CApp::g_pCApp = new (mem) CYy::CApp(
			FFXI::PolVars::instance.FFXIDirectory,
			FFXI::PolVars::instance.hmod,
			hwnd,
			FFXI::PolVars::instance.DateString,
			FFXI::PolVars::instance.TimeString
		);

	InitPeriod();

	return FFXI::CYy::CApp::g_pCApp != nullptr;
}

void FFXI::CYy::CApp::Uninitialize()
{
	if (FFXI::CYy::CApp::period) {
		timeEndPeriod(FFXI::CYy::CApp::period);
		FFXI::CYy::CApp::period = 0;
	}

	if (FFXI::CYy::CApp::g_pCApp) {
		delete FFXI::CYy::CApp::g_pCApp;
		FFXI::CYy::CApp::g_pCApp = nullptr;
	}
}

const CYyClass* CApp::GetRuntimeClass()
{
	return &CAppClass;
}

CApp::~CApp() {
	
	if (CYyDb::g_pCYyDb) {
		CYyDb::g_pCYyDb->Clean();
		delete CYyDb::g_pCYyDb;
		CYyDb::g_pCYyDb = nullptr;
	}

	if (GetAppState() != 1 && CApp::g_pNT_SYS)
		CApp::g_pNT_SYS->End();

	if (CApp::g_pInputMng) {
		delete CApp::g_pInputMng;
		CApp::g_pInputMng = nullptr;
	}
	if (this->SomeDateString)
	{
		delete[] this->SomeDateString;
		this->SomeDateString = nullptr;
	}
	if (this->SomeTimeString)
	{
		delete[] this->SomeTimeString;
		this->SomeTimeString = nullptr;
	}
	if (this->VersionString)
	{
		delete[] this->VersionString;
		this->VersionString = nullptr;
	}

	if (Globals::g_pCenv) {
		delete Globals::g_pCenv;
		Globals::g_pCenv = nullptr;
	}

	if (Globals::g_hhk) {
		UnhookWindowsHookEx(Globals::g_hhk);
		Globals::g_hhk = nullptr;
	}

	if (FFXI::Config::RuntimeConfig::instance.window_mode != FFXI::Config::RuntimeConfig::WindowMode::Fullscreen)
		this->NonFullscreenClean();
}

LRESULT CALLBACK fn(int code, WPARAM wParam, LPARAM lParam) {
	if ((code != HC_ACTION) || (FFXI::Config::RuntimeConfig::instance.window_mode != FFXI::Config::RuntimeConfig::WindowMode::Fullscreen))
		return CallNextHookEx(Globals::g_hhk, code, wParam, lParam);

	int AsyncKeyState = GetAsyncKeyState(VK_CONTROL);
	int v5 = AsyncKeyState >> 15;

	KBDLLHOOKSTRUCT* kbHook = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);
	DWORD vkCode = kbHook->vkCode;
	DWORD flags = kbHook->flags;
	if (vkCode == VK_LWIN) return 1;
	if (vkCode == VK_RWIN) return 1;
	if (vkCode == VK_ESCAPE && v5) return 1;
	if (vkCode == VK_TAB && (flags & LLKHF_ALTDOWN) != 0) return 1;
	if (vkCode == VK_ESCAPE && (flags & LLKHF_ALTDOWN) != 0) return 1;
	if (vkCode >= VK_BROWSER_BACK && vkCode <= VK_BROWSER_HOME) return 1;
	if (vkCode >= VK_MEDIA_NEXT_TRACK && vkCode <= VK_MEDIA_PLAY_PAUSE) return 1;
	if (vkCode >= VK_LAUNCH_MAIL && vkCode <= VK_LAUNCH_APP2) return 1;
	if (vkCode >= 0xB8 && vkCode <= 0xB9) return 1;
	if (vkCode == VK_APPS) return 1;
	if (vkCode == 94) return 1;
	if (vkCode == VK_SLEEP) return 1;
	if (vkCode != VK_F5) return CallNextHookEx(Globals::g_hhk, code, wParam, lParam);
	return 1;
}

char VersionError[] = "Version Error!!!!\0";
CApp::CApp(char* p_FFXIDir, HINSTANCE p_hMod, HWND p_hWnd, char* p_DateString, char* p_TimeString)
{
	this->field_1D = 0;
	this->field_1C = 1;
	size_t len = strlen(p_DateString) + 1;
	this->SomeDateString = new char[len];
	strcpy_s(this->SomeDateString, len, p_DateString);
	len = strlen(p_TimeString) + 1;
	this->SomeTimeString = new char[len];
	strcpy_s(this->SomeTimeString, len, p_TimeString);
	#if not defined _DEBUG
		Globals::g_hhk = SetWindowsHookExA(WH_KEYBOARD_LL, fn, p_hMod, 0);
	#endif
	char* VersionString = FFXI::File::XIFileManager::g_pXIFileManager->GetVersionString();
	if (!VersionString) VersionString = VersionError;
	int newlen = strlen(VersionString) + 1;
	char* v10 = new char[newlen];
	this->VersionString = v10;
	strcpy_s(v10, newlen, VersionString);
	this->FFXIDirectory = p_FFXIDir;
	this->hmod = p_hMod;
	this->hWnd = p_hWnd;
	
	CYyDb::g_pCYyDb = nullptr;
	FFXI::GlobalStruct::g_GlobalStruct.field_0 = 0;
	
	FFXI::GlobalStruct::g_GlobalStruct.ClientExpansions |= 1u;

	char* mem = StorageLists::instance->Get(sizeof(CEnv), Constants::Enums::MEM_MODE::Ex);
	if (mem)
		Globals::g_pCenv = new (mem) CEnv();

	if (FFXI::Config::RuntimeConfig::instance.window_mode != FFXI::Config::RuntimeConfig::WindowMode::Fullscreen)
		this->NonFullscreenStuff(this->hWnd);

}

bool FFXI::CYy::CApp::InitInput(HINSTANCE a1)
{
	CApp::g_pInputMng = new Input::InputMng();

	if (CApp::g_pInputMng) 
		return CApp::g_pInputMng->Init(a1);
	
	return false;
}

int FFXI::CYy::CApp::GetAppState()
{
	return CApp::AppState;
}

void FFXI::CYy::CApp::SetAppState(int a1)
{
	CApp::AppState = a1;
}

int FullscreenStuff1(HWND p_hWnd) {
	HWND ForegroundWindow{ 0 }; // eax
	DWORD WindowThreadProcessId{ 0 }; // esi
	DWORD v5{ 0 };
	DWORD pvParam{ 0 };
	if (FFXI::Config::RuntimeConfig::instance.window_mode != FFXI::Config::RuntimeConfig::WindowMode::Fullscreen)
		return (int)FFXI::Config::RuntimeConfig::instance.window_mode;

	ForegroundWindow = GetForegroundWindow();
	WindowThreadProcessId = GetWindowThreadProcessId(ForegroundWindow, 0);
	v5 = GetWindowThreadProcessId(p_hWnd, 0);
	AttachThreadInput(v5, WindowThreadProcessId, 1);
	SystemParametersInfoA(SPI_GETFOREGROUNDLOCKTIMEOUT, 0, &pvParam, 0);
	SystemParametersInfoA(SPI_SETFOREGROUNDLOCKTIMEOUT, 0, 0, 0);
	SetForegroundWindow(p_hWnd);
	SystemParametersInfoA(SPI_SETFOREGROUNDLOCKTIMEOUT, 0, &pvParam, 0);
	return AttachThreadInput(v5, WindowThreadProcessId, 0);

}
int FFXI::CYy::CApp::ClientTick()
{
	unsigned int v0 = _control87(0, 0);
	_control87(0x8001Fu, 0x8001Fu);
	if (CYyDb::g_pCYyDb->MainFlow()) {
		_control87(v0, 0x8001Fu);
		if (this->g_pNT_SYS != nullptr) {
			this->g_pNT_SYS->Proc();
		}
		return 1;
	}
	_control87(v0, 0x8001Fu);
	return 0;
}
int CApp::Execute() {
	tagMSG Msg{}, v5{};
	char v6{ 0 };
	FullscreenStuff1(this->hWnd);
	FFXI::CYy::CDx::instance->CheckDeviceCaps();
	if (InitInput(this->hmod)) {
		GetWindowRect(this->hWnd, &this->Rect);
		CYyDb::g_pCYyDb = new CYyDb();
		if (CYyDb::g_pCYyDb) {
			CYyDb::g_pCYyDb->InitSubState(0);
			do {
				if (!CYyDb::g_pCYyDb->Init(&v6))
					goto LABEL_23;
			} while (v6 != 1);
			this->field_1D = 1;
		LABEL_9:
			FullscreenStuff1(this->hWnd);
			if (!FFXI::CYy::CApp::g_pCApp->ClientTick())
				goto LABEL_23;

			while (true)
			{
				do
				{
					if (!PeekMessageA(&Msg, 0, 0, 0, 1u) || Msg.message == WM_PSD_FULLPAGERECT)
					{
						if (Msg.message == WM_QUIT)
							goto LABEL_23;
						goto LABEL_9;
					}
					if (Msg.message == WM_QUIT)
						goto LABEL_23;
				} while (Msg.message == WM_IME_SETCONTEXT || Msg.message == WM_ACTIVATEAPP);
				if (Msg.message == WM_SYSKEYUP)
				{
					if (Msg.wParam == VK_SNAPSHOT)
						goto LABEL_17;
				LABEL_21:
					CApp::WinMsgHandler(&Msg);
					TranslateMessage(&Msg);
					DispatchMessageA(&Msg);
				}
				else
				{
					if (Msg.message != WM_KEYUP || Msg.wParam != VK_SNAPSHOT)
						goto LABEL_21;
				LABEL_17:
					;
					//sub_10001CC0(this, &Read);
				}
			}
			
		}
	}
LABEL_23:
	this->field_1D = 0;
	return 0;
}

DWORD CALLBACK StartAddress(LPVOID p_param) {
	CApp* app = static_cast<CApp*>(p_param);
	while (!app->field_3D) {
		PostMessageA(app->field_40, WM_PSD_FULLPAGERECT, NULL, NULL);
		Sleep(0x14Du);
	}
	app->field_3C = 0;
	return 0;
}
void FFXI::CYy::CApp::NonFullscreenStuff(HWND p_hWnd)
{
	this->field_3D = 0;
	this->field_3C = 1;
	this->field_40 = p_hWnd;
	this->field_38 = CreateThread(NULL, 0, StartAddress, this, CREATE_SUSPENDED, NULL);
	if (this->field_38)
		ResumeThread(this->field_38);

}

void FFXI::CYy::CApp::NonFullscreenClean()
{
	this->field_3D = 1;
	if (!this->field_38) return;
	while (this->field_3C == 1)
		Sleep(1u);
	CloseHandle(this->field_38);
	this->field_38 = nullptr;
}
