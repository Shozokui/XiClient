#include "XIClient.h"
#include "streams.h"
#include <tuple>
#include <functional>
#include <vector>
#include <filesystem>
#include <VersionHelpers.h>
#include "XIFileManager.h"
#include "GlobalStruct.h"
#include "PolVars.h"
#include "SoundMng.h"
#include "WindowManager.h"
#include "FFXIDX.h"
#include "CommandLineConfig.h"
#include "RuntimeConfig.h"
#include "Globals.h"
#include "StorageLists.h"
#include "CDx.h"
#include "TkManager.h"
#include "CApp.h"
#include "CXITimerLow.h"

//Required by DirectShow
CFactoryTemplate* g_Templates;
int g_cTemplates;
extern HINSTANCE g_hInst;

enum class XIClient::init_result {
	OK = 0,
	InvalidEnvironment,
	ConfigNotFound,
	ConfigParseError,
	ConfigInstallDirMissing,
	InstallDirNotExist,
	InstallDirInvalid,
	CreateWindowFailure,
	DirectXFailure,
	SoundFailure,
	COMFailure,
	MemoryAllocationFailure
};

bool InitIME()
{
	if (FFXI::PolVars::instance.UseImeUi == 1) {
		if (true) { //LoadImeUIDll(Window, Globals::g_polVars.FFXIDirectory)
			if (FFXI::PolVars::instance.UseImeUi == 1) {
				if (true) { //LoadImeUIDll2(Window, Globals::g_polVars.FFXIDirectory)
					if (FFXI::PolVars::instance.UseImeUi == 1) {
						//ImeUi_Setstate(1);
					}
				}
				else FFXI::PolVars::instance.UseImeUi = 0;
			}
		}
		else FFXI::PolVars::instance.UseImeUi = 0;
	}

	return true;
}

void UninitializeIME()
{
	//sub //TODO
}

void ParseCommandLineArgs() {
	FFXI::PolVars::ParseCLA();
	FFXI::SoundMng::ParseCLA();
}

bool CheckValidWindowsEnvironment()
{
	if (IsWindowsXPOrGreater() == true) {
		HMODULE LibraryA = LoadLibraryA("dpnhpast.dll");
		if (LibraryA != nullptr) {
			FreeLibrary(LibraryA);
			return true;
		}
	}

	return false;
}

int XIClient::StartMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, HWND hwnd)
{
	//required by DirectShow
	g_hInst = hInstance;
	FFXI::PolVars::instance.hmod = hInstance;

	//value used by client on error
	int return_value = 0x88770000;

	//Initialize FFXI
	init_result result = Initialize(hwnd, lpCmdLine);
	if (result == init_result::OK) {
		return_value = FFXI::CYy::CApp::g_pCApp->Execute();
	}
	else {
		MessageBox(NULL, GetInitializeErrorString(result), L"Error", MB_OK | MB_ICONERROR);
	}

	Uninitialize();
	return return_value;
}

void SetProcessPriority(int priority) {
	HANDLE	CurrentProcess = GetCurrentProcess();
	if (CurrentProcess != NULL)
	{
		switch (priority) {
		case 0:
			SetPriorityClass(CurrentProcess, IDLE_PRIORITY_CLASS);
			break;
		case 2:
			SetPriorityClass(CurrentProcess, HIGH_PRIORITY_CLASS);
			break;
		default:
			SetPriorityClass(CurrentProcess, NORMAL_PRIORITY_CLASS);
			break;
		}
	}
}

XIClient::init_result XIClient::Initialize(HWND hwnd, LPSTR CLA)
{
	if (CheckValidWindowsEnvironment() == false) {
		return init_result::InvalidEnvironment;
	}

	if (FFXI::Config::CommandLineConfig::Init(CLA) == false) {
		return init_result::MemoryAllocationFailure;
	}

	wchar_t exe_dir[1024];
	GetModuleFileName(NULL, exe_dir, sizeof exe_dir);
	std::filesystem::path exe_path = exe_dir;
	std::filesystem::path config_path = exe_path.parent_path() / "config.toml";
	int rtconf_init_result = FFXI::Config::RuntimeConfig::Initialize(config_path);
	if (rtconf_init_result == -1) {
		//Config file not found
		return init_result::ConfigNotFound;
	}
	else if (rtconf_init_result == -2) {
		//Error parsing config file
		return init_result::ConfigParseError;
	}
	else if (rtconf_init_result == -3) {
		//The config file did not contain an FFXI install dir
		return init_result::ConfigInstallDirMissing;
	}

	if (std::filesystem::exists(FFXI::Config::RuntimeConfig::instance.ffxi_install_path) == false) {
		return init_result::InstallDirNotExist;
	}

	std::filesystem::current_path(FFXI::Config::RuntimeConfig::instance.ffxi_install_path);
	
	//This allows for parsing of CLA before initialization
	ParseCommandLineArgs();

	FFXI::File::XIFileManager::g_pXIFileManager = nullptr;
	FFXI::GlobalStruct::g_GlobalStruct.RegionCode = FFXI::PolVars::instance.RegionCode;
	FFXI::GlobalStruct::g_GlobalStruct.phkResult = FFXI::PolVars::instance.phkResult;

	//p_GlobalStruct->field_18 ^= ((unsigned __int8)p_GlobalStruct->field_18 ^ (unsigned __int8)((_BYTE)dword_1044581C << 7)) & 0x80;
	//p_GlobalStruct->field_18 ^= ((unsigned __int8)p_GlobalStruct->field_18 ^ (unsigned __int8)((_BYTE)dword_1044581C << 7)) & 0x80;
	//Some debug value set here. It has been omitted.

	//TEMP
	FFXI::Config::RegistryConfig::g_pOptions = new FFXI::Config::RegistryConfig();
	if (FFXI::File::XIFileManager::fsInitFileSystem(FFXI::Config::RegistryConfig::g_pOptions->Other.field_1C, FFXI::Config::RegistryConfig::g_pOptions->Main.ScreenshotPath) == false) {
		//file system init fails if patch.ver isn't found in the base dir
		return init_result::InstallDirInvalid;
	}

	if (Window::WindowManager::InitWindow(hwnd) == false) {
		return init_result::CreateWindowFailure;
	}

	//sub //TODO
	if (InitIME() == false) {
		
	}

	if (FFXI::FFXIDX::InitializeDX(Window::WindowManager::window_manager.hWnd) != 0) {
		return init_result::DirectXFailure;
	}

	if (FFXI::Config::RuntimeConfig::instance.sound_enable == true
		&& FFXI::SoundMng::InitSound(Window::WindowManager::window_manager.hWnd) == false) {
		return init_result::SoundFailure;
	}

	//sub_10002400();
	SetProcessPriority(FFXI::Config::RuntimeConfig::instance.process_priority);
	_control87(_MCW_EM, _MCW_EM);

	if (CoInitialize(nullptr) < S_OK) {
		return init_result::COMFailure;
	}

	if (FFXI::StorageLists::Initialize() == false) {
		return init_result::MemoryAllocationFailure;
	}

	FFXI::CTk::TkManager::g_CTkMenuMng.Init();
	FFXI::CTk::TkManager::g_CTkMenuDataList.Init();

	if (FFXI::CYy::CDx::Initialize(FFXI::FFXIDX::g_pFFXIDX->DXInterface, FFXI::FFXIDX::g_pFFXIDX->DXDevice) == false) {
		return init_result::MemoryAllocationFailure;
	}

	if (FFXI::CYy::CApp::Initialize(Window::WindowManager::window_manager.hWnd) == false) {
		return init_result::MemoryAllocationFailure;
	}
	//not sure if these timers are used
	//for (int i = 0; i < sizeof(Globals::g_apLowTimers) / sizeof(Globals::g_apLowTimers[0]); ++i)
	//	Globals::g_apLowTimers[i] = FFXI::CYy::CXITimerLow::GetNew();

	return init_result::OK;
}

LPCWSTR XIClient::GetInitializeErrorString(init_result result)
{
	switch (result) {
	case init_result::OK:
		return L"No error";
	case init_result::InvalidEnvironment:
		return L"Unable to launch. WinXP+ required OR cannot find dpnhpast.dll";
	case init_result::ConfigNotFound:
		return L"Configuration file not found";
	case init_result::ConfigParseError:
		return L"Could not parse configuration file";
	case init_result::ConfigInstallDirMissing:
		return L"The configuration file is missing [Install] dir";
	case init_result::InstallDirNotExist:
		return L"The supplied FFXI install directory does not exist";
	case init_result::InstallDirInvalid:
		return L"The supplied FFXI install directory exists, but does not contain a valid FFXI install";
	case init_result::CreateWindowFailure:
		return L"Unable to create window";
	case init_result::DirectXFailure:
		return L"DirectX failed to initialize";
	case init_result::SoundFailure:
		return L"DirectSound failed to initialize";
	case init_result::COMFailure:
		return L"CoInitialize failed";
	case init_result::MemoryAllocationFailure:
		return L"Failed to allocate sufficient memory";
	default:
		return L"Unhandeled error";
	}
}

void XIClient::Uninitialize()
{
	//not sure if these timers are used
	//for (int i = 0; i < sizeof(Globals::g_apLowTimers) / sizeof(Globals::g_apLowTimers[0]); ++i) {
	//	if (Globals::g_apLowTimers[i]) {
	//		delete Globals::g_apLowTimers[i];
	//		Globals::g_apLowTimers[i] = 0;
	//	}
	//}
	FFXI::CYy::CApp::Uninitialize();
	FFXI::CYy::CDx::Uninitialize();
	FFXI::CTk::TkManager::g_CTkMenuDataList.~CTkMsbDataList();
	FFXI::CTk::TkManager::g_CTkMenuMng.DeleteMenuAll();
	FFXI::StorageLists::Uninitialize();
	CoUninitialize();
	FFXI::SoundMng::UninitSound();
	FFXI::FFXIDX::UninitializeDX();
	UninitializeIME();
	Window::WindowManager::UninitWindow();
	FFXI::File::XIFileManager::fsUninitFileSystem();
	FFXI::Config::CommandLineConfig::Uninit();
}