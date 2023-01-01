#include "InputMng.h"
#include "RegistryConfig.h"
#include "TkManager.h"
#include "CTkMouse.h"
#include "Globals.h"
#include "CDx.h"
#include "CApp.h"
#include "CYyDb.h"
#include <iostream>
#include <imm.h>
#include "RuntimeConfig.h"

using namespace FFXI::Input;

FsTextInput* InputMng::g_pFsTextInput{ nullptr };
FsKeyboard InputMng::g_FsKeyboard{};

bool InputMng::s_isEnableShortcut{ true };
bool InputMng::KeyboardReacquire{ false };
bool InputMng::MouseReacquire{ false };
bool InputMng::UseSoftwareMouse{ false };
bool InputMng::s_bResetFlag{ false };
int InputMng::globalLevel{ 0xFFFF };

struct KeyboardLayoutEntry {
	int ID;
	const char Name[32] = { 0 };
};

KeyboardLayoutEntry KeyboardLayouts[] = {
	{0, "Microsoft IME 95 (Japanese)"},
	{1, "Microsoft IME 97 (Japanese)"},
	{2, "Microsoft IME 98 (Japanese)"},
	{3, "Microsoft IME 2000 (Japanese)"},
	{4, "ATOK10"},
	{5, "ATOK11"},
	{6, "ATOK12"},
	{7, "ATOK13"},
	{8, "ATOK14"},
	{9, "ATOK15"},
	{10, "ATOK16"},
	{12, "Microsoft IME Standard"},
	{13, Globals::NullString},
	{15, Globals::NullString}
};

InputMng::InputMng() {
	this->PadInterface = nullptr;
	this->field_C99 = 0;
	this->DIMouseDevice = nullptr;
	this->DIKeyboardDevice = nullptr;
	memset(this->field_398, 0, sizeof(this->field_398));
	this->RepeatInputDelay = 16;
	this->DeviceObjectDataCount = 0;
	this->field_C94 = 0;
	this->MouseDeviceDataCount = 0;
	memset(this->KeyboardDeviceState, 0, sizeof(this->KeyboardDeviceState));
	memset(this->field_198, 0, sizeof(this->field_198));
	this->field_DAC = 0;
	this->field_DB0 = 0;
	this->field_DB4 = 0;
	this->field_C98 = 0;
	this->field_DA0 = 0;
	this->XInputDLLFound = 0;
	this->field_DBC = 0;
	this->field_DC0 = 0;
	this->SoftwareMouseState.lX = 0;
	this->SoftwareMouseState.lY = 0;
	this->SoftwareMouseState.lZ = 0;
	memset(this->SoftwareMouseState.rgbButtons, 0, sizeof(this->SoftwareMouseState.rgbButtons));
}

FFXI::Input::InputMng::~InputMng()
{

	if (this->DIMouseDevice) {
		this->DIMouseDevice->Unacquire();
		this->DIMouseDevice->Release();
		this->DIMouseDevice = nullptr;
	}

	if (this->DIKeyboardDevice) {
		this->DIKeyboardDevice->Unacquire();
		this->DIKeyboardDevice->Release();
		this->DIKeyboardDevice = nullptr;
	}

	if (this->DI8Interface) {
		this->DI8Interface->Release();
		this->DI8Interface = nullptr;
	}
	
	FFXI::CYy::CApp::g_pCApp->g_pInputMng = nullptr;
}

bool FFXI::Input::InputMng::isShiftDown()
{
	if (isKeyboard(0x2A, 4))
		return true;

	if (isKeyboard(0x36, 4))
		return true;

	return false;
}

bool FFXI::Input::InputMng::isShiftDown2()
{
	return InputMng::g_FsKeyboard.CheckFlag4(0x2Au) || InputMng::g_FsKeyboard.CheckFlag4(0x36u);
}

bool FFXI::Input::InputMng::isKeyboard(int a1, int a2)
{
	if (g_pFsTextInput)
		throw "NOT IMPLEMENTED";

	switch (a2) {
	case 0:
		return InputMng::g_FsKeyboard.rept(a1);
	case 1:
		return InputMng::g_FsKeyboard.just(a1);
	case 2:
		return InputMng::g_FsKeyboard.reptoff(a1);
	case 3:
		return InputMng::g_FsKeyboard.justoff(a1);
	case 4:
		return InputMng::g_FsKeyboard.CheckFlag4(a1);
	default:
		return false;
	}
	return false;
}

bool FFXI::Input::InputMng::isEnableShortcut()
{
	return s_isEnableShortcut;
}

void FFXI::Input::InputMng::setEnableShortcut(bool a1)
{
	s_isEnableShortcut = a1;
}

bool FFXI::Input::InputMng::CanIGetKey(int a1)
{
	return (a1 & globalLevel) != 0;
}

void FFXI::Input::InputMng::SetKeyLevel(int a1)
{
	globalLevel = a1;
}

bool FFXI::Input::InputMng::Init(HINSTANCE a1)
{
	HRESULT result{};
	result = DirectInput8Create(a1, DIRECTINPUT_VERSION, IID_IDirectInput8A, (LPVOID*)&this->DI8Interface, NULL);
	if (result < DI_OK) {

		char Message[256];
		sprintf_s(Message, 256, "[Fatal Error]: DirectInput8Create returned %d", result);
		std::cerr << Message << std::endl;

		return false;
	}
		
	if (!this->InitKeyboard()) {
		std::cerr << "[Fatal Error]: InitKeyboard fail" << std::endl;
		return false;
	}

	if (!this->InitMouse()) {
		std::cerr << "[Fatal Error]: InitMouse fail" << std::endl;
		return false;
	}

	//sub //todo pad
	this->CheckKeyboardLayout();

	if (FFXI::Config::RuntimeConfig::instance.window_mode != FFXI::Config::RuntimeConfig::WindowMode::Fullscreen)
		return true;

	if (!this->DIMouseDevice)
		return true;

	if (!UseSoftwareMouse)
		return true;

	DIMOUSESTATE v9{};
	if (this->DIMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), &v9))
		return true;

	this->SoftwareMouseState = v9;
	return true;
}

bool FFXI::Input::InputMng::InitKeyboard()
{
	HRESULT result{};
	char Message[256];
	result = this->DI8Interface->CreateDevice(GUID_SysKeyboard, &this->DIKeyboardDevice, NULL);
	if (result < DI_OK) {
		sprintf_s(Message, 256, "[Fatal Error]: DI8Interface->CreateDevice returned %d", result);
		std::cerr << Message << std::endl;
		return false;
	}

	DIPROPDWORD v5{};
	v5.diph.dwSize = sizeof(DIPROPDWORD);
	v5.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	v5.diph.dwObj = 0;
	v5.diph.dwHow = DIPH_DEVICE;
	v5.dwData = 320;

	result = this->DIKeyboardDevice->SetProperty(DIPROP_BUFFERSIZE, &v5.diph);
	if (result < DI_OK) {
		sprintf_s(Message, 256, "[Fatal Error]: DIKeyboardDevice->SetProperty returned %d", result);
		std::cerr << Message << std::endl;
		return false;
	}

	result = this->DIKeyboardDevice->SetDataFormat(&c_dfDIKeyboard);
	if (result < DI_OK) {
		sprintf_s(Message, 256, "[Fatal Error]: DIKeyboardDevice->SetDataFormat returned %d", result);
		std::cerr << Message << std::endl;
		return false;
	}

	DWORD flags = DISCL_FOREGROUND | DISCL_NONEXCLUSIVE;
	if (FFXI::Config::RuntimeConfig::instance.window_mode == FFXI::Config::RuntimeConfig::WindowMode::Fullscreen)
		flags |= DISCL_NOWINKEY;

	result = this->DIKeyboardDevice->SetCooperativeLevel(FFXI::CYy::CApp::g_pCApp->hWnd, flags);
	if (result < DI_OK) {
		sprintf_s(Message, 256, "[Fatal Error]: DIKeyboardDevice->SetCooperativeLevel returned %d", result);
		std::cerr << Message << std::endl;
		return false;
	}

	this->DIKeyboardDevice->Acquire();
	return true;
}

bool FFXI::Input::InputMng::InitMouse()
{
	char Message[256];
	if (FFXI::Config::RuntimeConfig::instance.window_mode != FFXI::Config::RuntimeConfig::WindowMode::Fullscreen) {
		UseSoftwareMouse = false;
		return true;
	}

	HRESULT result{};
	result = this->DI8Interface->CreateDevice(GUID_SysMouse, &this->DIMouseDevice, NULL);
	if (result < DI_OK) {
		sprintf_s(Message, 256, "[Fatal Error]: DI8Interface->CreateDevice returned %d", result);
		std::cerr << Message << std::endl;
		return false;
	}

	result = this->DIMouseDevice->SetDataFormat(&c_dfDIMouse);
	if (result < DI_OK) {
		sprintf_s(Message, 256, "[Fatal Error]: DIMouseDevice->SetDataFormat returned %d", result);
		std::cerr << Message << std::endl;
		return false;
	}

	if (Config::MainRegistryConfig::HardwareMouse)
		FFXI::CYy::CDx::instance->ShowCursor(true);

	if (Config::MainRegistryConfig::HardwareMouse) {
		UseSoftwareMouse = false;

		if (this->DIMouseDevice) {
			this->DIMouseDevice->Release();
			this->DIMouseDevice = nullptr;
		}
		
		return true;
	}
	
	UseSoftwareMouse = true;

	DIPROPDWORD v5{};
	v5.diph.dwSize = sizeof(DIPROPDWORD);
	v5.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	v5.diph.dwHow = DIPH_DEVICE;
	v5.diph.dwObj = 0;
	v5.dwData = 0;
	
	this->DIMouseDevice->SetProperty(DIPROP_AXISMODE, &v5.diph);
	v5.dwData = 320;
	this->DIMouseDevice->SetProperty(DIPROP_BUFFERSIZE, &v5.diph);
	
	result = this->DIMouseDevice->SetCooperativeLevel(FFXI::CYy::CApp::g_pCApp->hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (result < DI_OK) {
		sprintf_s(Message, 256, "[Fatal Error]: DIMouseDevice->SetCooperativeLevel returned %d", result);
		std::cerr << Message << std::endl;
		return false;
	}

	this->DIMouseDevice->Acquire();
	return true;
}

void FFXI::Input::InputMng::CheckMouse()
{
	if (!this->DIMouseDevice || !UseSoftwareMouse) return;

	this->MouseDeviceDataCount = sizeof(this->MouseDeviceData) / sizeof(DIDEVICEOBJECTDATA);
	HRESULT result = this->DIMouseDevice->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), this->MouseDeviceData, &this->MouseDeviceDataCount, 0);
	
	if (result == 1) {
		if (MouseReacquire) {
			this->DIMouseDevice->Acquire();
		}
		else {
			MouseReacquire = true;
			this->DIMouseDevice->Release();
			this->InitMouse();
		}
	}

	if (result != DI_OK) {
		this->MouseDeviceDataCount = 0;
		memset(this->MouseDeviceData, 0, sizeof(this->MouseDeviceData));
	}
	else
		MouseReacquire = false;
}

bool FFXI::Input::InputMng::CheckThing(int a2)
{
	if (a2)
		return InputMng::g_FsKeyboard.CheckFlag4(a2);
	if (this->field_C94 || this->DeviceObjectDataCount)
		return true;
	
	return CTk::TkManager::g_pCTkMouse->field_60 != 0;
}

void FFXI::Input::InputMng::OnCalc()
{
}

void FFXI::Input::InputMng::CheckKeyboardLayout()
{
	char szDescription[0x100];
	memset(szDescription, 0, 0x100);
	HKL keyboardLayout = GetKeyboardLayout(0);
	ImmGetDescriptionA(keyboardLayout, szDescription, sizeof(szDescription));

	KeyboardLayoutEntry* layout = KeyboardLayouts;
	while (strcmp(szDescription, layout->Name)) {
		++layout;
		if (layout->ID >= 15) {
			this->KeyboardLayout = 14;
			return;
		}
	}

	this->KeyboardLayout = layout->ID;
}

void FFXI::Input::InputMng::Process()
{

	int v136 = CYyDb::g_pCYyDb->CheckTick();
	float v140 = (float)v136;
	if (this->DIKeyboardDevice) {
		HRESULT result = this->DIKeyboardDevice->GetDeviceState(sizeof(this->KeyboardDeviceState), this->KeyboardDeviceState);
		if (result == DIERR_ACQUIRED || result == DIERR_NOTACQUIRED || result == DIERR_INPUTLOST) {
			if (InputMng::KeyboardReacquire)
				this->DIKeyboardDevice->Acquire();
			else {
				InputMng::KeyboardReacquire = true;
				this->DIKeyboardDevice->Release();
				this->InitKeyboard();
			}
			memset(this->KeyboardDeviceState, 0, sizeof(this->KeyboardDeviceState));
		}
		else if (result != DI_OK)
			memset(this->KeyboardDeviceState, 0, sizeof(this->KeyboardDeviceState));
		else
			InputMng::KeyboardReacquire = false;

		this->DeviceObjectDataCount = 16;
		result = this->DIKeyboardDevice->GetDeviceData(sizeof(this->DeviceObjectData), this->DeviceObjectData, &this->DeviceObjectDataCount, 0);
		
		if (result == DI_OK)
			InputMng::KeyboardReacquire = false;
		else if (result == DIERR_ACQUIRED || result == DIERR_NOTACQUIRED || result == DIERR_INPUTLOST) {
			if (InputMng::KeyboardReacquire)
				this->DIKeyboardDevice->Acquire();
			else {
				InputMng::KeyboardReacquire = true;
				this->DIKeyboardDevice->Release();
				this->InitKeyboard();
			}
			memset(this->DeviceObjectData, 0, sizeof(this->DeviceObjectData));
			this->DeviceObjectDataCount = 0;
		}
		else
			this->DeviceObjectDataCount = 0;

		//sub //TODO
		for (int i = 0; i < this->DeviceObjectDataCount; ++i) {
			DIDEVICEOBJECTDATA* objectData = this->DeviceObjectData + i;
			if (objectData->dwOfs < 0x100 && (char)(objectData->dwData & 0xFF) < 0) {
				if (FFXI::GlobalStruct::g_GlobalStruct.RegionCode == 2)
					objectData->dwOfs = 0;//sub
				else if (FFXI::GlobalStruct::g_GlobalStruct.RegionCode == 3)
					objectData->dwOfs = 0;//sub
			}

		}

		//sub //TODO
		for (int i = 0; i < sizeof(this->KeyboardDeviceState); ++i) {
			int v12 = i;
			if (FFXI::GlobalStruct::g_GlobalStruct.RegionCode == 2)
				v12 = 0;//sub
			else if (FFXI::GlobalStruct::g_GlobalStruct.RegionCode == 3)
				v12 = 0;//sub
			this->field_198[v12] = this->KeyboardDeviceState[i];
		}
		

		char v145[0x100];
		memset(v145, 0, 0x100);

		for (int i = 0; i < this->DeviceObjectDataCount; ++i) {
			DIDEVICEOBJECTDATA* objectData = this->DeviceObjectData + i;
			if (objectData->dwOfs < 0x100 && (char)(objectData->dwData) < 0)
				v145[objectData->dwOfs] = 1;
		}

		for (int v15 = 0; v15 < 256; ++v15) {
			if (this->field_198[v15] < 0 || v145[v15]) {
				if (InputMng::g_FsKeyboard.CheckFlag4(v15)) {
					InputMng::g_FsKeyboard.ClearFlags821(v15);
					this->field_398[v15] += v140;
					if (this->field_398[v15] > this->RepeatInputDelay) {
						InputMng::g_FsKeyboard.SetFlag1(v15);
						this->field_398[v15] = (double)this->RepeatInputDelay - 2.0;
					}
				}
				else {
					InputMng::g_FsKeyboard.SetFlag4(v15);
					InputMng::g_FsKeyboard.SetFlag2(v15);
					InputMng::g_FsKeyboard.SetFlag1(v15);
					this->field_398[v15] = 0;
					//sub //todo KeyboardConfigMenu
				}
			}
			else {	
				if (InputMng::g_FsKeyboard.CheckFlag4(v15)) {
					InputMng::g_FsKeyboard.ClearFlags(v15);
					InputMng::g_FsKeyboard.SetFlag8(v15);
				}
				else {
					InputMng::g_FsKeyboard.ClearFlags(v15);
				}
					
				this->field_398[v15] = 0;
			}
		}

		if (InputMng::g_FsKeyboard.just(0x1Cu) || InputMng::g_FsKeyboard.just(0x9C)) {
			//sub //todo
			if (InputMng::g_pFsTextInput) {//some func
				throw "NOT IMPLEMENTED";
				InputMng::g_FsKeyboard.ClearFlags821(0x1Cu);
				InputMng::g_FsKeyboard.ClearFlags821(0x9Cu);
			}
		}
		else if (InputMng::g_FsKeyboard.just(1u) && InputMng::g_pFsTextInput) {
			//sub //todo
			throw "NOT IMPLEMENTED";
			InputMng::g_FsKeyboard.ClearFlags821(1u);
		}

		GetKeyboardState(this->field_298);
		if ((this->field_298[0x90] & 1) != 0)
			InputMng::g_FsKeyboard.SetFlag10(0x45u);
		if ((this->field_298[0xA4] & 1) != 0)
			InputMng::g_FsKeyboard.SetFlag10(0x38u);
		if ((this->field_298[0xA5] & 1) != 0)
			InputMng::g_FsKeyboard.SetFlag10(0xB8u);
		if ((this->field_298[0xA0] & 1) != 0)
			InputMng::g_FsKeyboard.SetFlag10(0x2Au);
		if ((this->field_298[0xA1] & 1) != 0)
			InputMng::g_FsKeyboard.SetFlag10(0x36u);
		if ((this->field_298[0xA2] & 1) != 0)
			InputMng::g_FsKeyboard.SetFlag10(0x1Du);
		if ((this->field_298[0xA3] & 1) != 0)
			InputMng::g_FsKeyboard.SetFlag10(0x9Du);
	}

	if (this->DIMouseDevice && UseSoftwareMouse) {
		DIMOUSESTATE state{};
		if (this->DIMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), &state) == DI_OK) {
			MouseReacquire = false;
			if (state.lX != this->SoftwareMouseState.lX
				|| state.lY != this->SoftwareMouseState.lY
				|| state.lZ != this->SoftwareMouseState.lZ) {
				tagPOINT v134{};
				v134.x = state.lX - this->SoftwareMouseState.lX;
				v134.y = state.lY - this->SoftwareMouseState.lY;
				short v995 = ((short)state.lZ - this->SoftwareMouseState.lZ) / 60;
				CTk::TkManager::g_pCTkMouse->OnMouseMove(&v134, v995);
				this->SoftwareMouseState.lX = state.lX;
				this->SoftwareMouseState.lY = state.lY;
				this->SoftwareMouseState.lZ = state.lZ;
				memcpy(this->SoftwareMouseState.rgbButtons, state.rgbButtons, sizeof(state.rgbButtons));
			}
		}
		this->CheckMouse();
	}
}

void FFXI::Input::InputMng::KeyProcessed()
{
	InputMng::s_bResetFlag = true;
}

void FFXI::Input::InputMng::ResetVirtualKey()
{
	InputMng::s_bResetFlag = false;
}

bool FFXI::Input::InputMng::GetKey(Constants::Enums::KEYTYPE a1, Constants::Enums::TRGTYPE a2)
{
	return GetKey(62, a1, a2, -1);
}

bool FFXI::Input::InputMng::GetKey(int a1, Constants::Enums::KEYTYPE a2, Constants::Enums::TRGTYPE a3, int a4)
{
	if (s_bResetFlag && a3 != 4)
		return false;

	if (a2 == -1)
		return false;

	short v5 = keykind[a2];

	if (!v5) {
		throw "NOT IMPLEMENTED";
		return false;
	}
	int v16 = 0;
	int(__cdecl * func)(FFXI::Constants::Enums::TRGTYPE) { nullptr };
	if (a4 == -1) {
		func = IsKeyTable[2 * a2];
		if (func)
			v16 = func(a3);
	}
	else if (a4 != 1) {
		func = IsKeyTable[2 * a2 + a4];
		if (func)
			v16 = func(a3);
	}

	int v8 = 0;
	int v12 = 0;
	while (!v16) {
		int v9 = v8 + 3 * a2;
		short v10 = isKeyAssinTable[v9];
		if (v10) {
			v12 = 1;
			if ((v10 & 0x4000) != 0) {
				v12 = 0;
				throw "NOT SUPPORTED";
			}
			if ((v10 & 0x8000) != 0 && v12) {
				v12 = 0;
				if (!isCtrlDown() && !isAltDown())
					v12 = 1;
			}
			if ((v10 & 0x100) != 0 && v12)
				v12 = isShiftDown();
			if ((v10 & 0x200) != 0 && v12)
				v12 = isCtrlDown();
			if ((v10 & 0x400) != 0 && v12)
				v12 = isAltDown();
			if ((v10 & 0x800) != 0 && v12) {
				v12 = 0;
				if (isCtrlDown() && isAltDown())
					v12 = 1;
			}
			if ((v10 & 0x1000) != 0 && v12) {
				if (isCtrlDown() || isAltDown()) {
					if (isEnableShortcut())
						v12 = 1;
				}
			}
			if (v12) {
				if ((v10 & 0x2000) == 0 || !isShiftDown()) {
					switch (v10 & 0xFF) {
						case 0x1D:
							if (a3 == 4)
								v16 = isCtrlDown2();
							break;
						case 0x2A:
							if (a3 == 4) 
								v16 = isShiftDown2();
							break;
						case 0x38:
							if (a3 == 4) 
								v16 = isAltDown2();
							break;
						default:
							v16 = isKeyboard(v10 & 0xFF, a3);
							break;
					}
				}
			}
		}
		if (++v8 >= 3)
			break;
	}

	if (!v16)
		return false;

	switch (a2) {
	case 0x76:
	case 0x77:
	case 0x89:
	case 0x8A:
		return v16;
	case Constants::Enums::KEYTYPE::KEYTYPE_MOUSE_LBUTTON:
	case Constants::Enums::KEYTYPE::KEYTYPE_MOUSE_RBUTTON:
	case Constants::Enums::KEYTYPE::KEYTYPE_MOUSE_MBUTTON:
		CTk::TkManager::g_pCTkMouse->OnTrgButtons(true);
		return true;
	default:
		CTk::TkManager::g_pCTkMouse->OnTrgButtons(false);
		return v16;
	}
	return false;
}

bool FFXI::Input::InputMng::isCtrlDown()
{
	if (InputMng::g_FsKeyboard.CheckFlag4(0x1Du))
		return true;
	if (InputMng::g_FsKeyboard.CheckFlag4(0x9Du))
		return true;

	return false;
}

bool FFXI::Input::InputMng::isCtrlDown2()
{
	return InputMng::g_FsKeyboard.CheckFlag4(0x1Du) || InputMng::g_FsKeyboard.CheckFlag4(0x9Du);
}

bool FFXI::Input::InputMng::isAltDown()
{
	if (InputMng::g_FsKeyboard.CheckFlag4(0xB8u))
		return true;
	if (InputMng::g_FsKeyboard.CheckFlag4(0x38u))
		return true;

	return false;
}

bool FFXI::Input::InputMng::isAltDown2()
{
	return InputMng::g_FsKeyboard.CheckFlag4(0xB8u) || InputMng::g_FsKeyboard.CheckFlag4(0x38u);
}

unsigned short InputMng::isKeyAssinTable[540] = {
	0x0C8, 0x0, 0x0, 0x0D0, 0x0, 0x0, 0x0CD, 0x0, 0x0, 0x0CB, 0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1015, 0x1C, 0x9C, 0x1031,
	0x8001, 0x0, 0x102F, 0x0C04C, 0x0, 0x1021, 0x804E, 0x0, 0x0, 0x0, 0x0,
	0x80C5, 0x0, 0x0, 0x80C5, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
	0x100C, 0x804A, 0x0, 0x100C, 0x804A, 0x0, 0x0, 0x0, 0x0, 0x101B, 0x0,
	0x0, 0x0, 0x8047, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x20C8, 0x0, 0x0, 0x20D0,
	0x0, 0x0, 0x20CD, 0x0, 0x0, 0x20CB, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0C037, 0x0,
	0x0, 0x80C8, 0x0, 0x0, 0x80D0, 0x0, 0x0, 0x80CD, 0x0, 0x0, 0x80CB, 0x0,
	0x0, 0x802A, 0x0, 0x0, 0x1012, 0x0, 0x0, 0x1013, 0x0, 0x0, 0x1014, 0x0,
	0x0, 0x1017, 0x0, 0x0, 0x1019, 0x0, 0x0, 0x101E, 0x0, 0x0, 0x101F, 0x0,
	0x0, 0x1023, 0x0, 0x0, 0x1026, 0x0, 0x0, 0x102E, 0x0, 0x0, 0x102F, 0x0,
	0x0, 0x1032, 0x0, 0x0, 0x1015, 0x101C, 0x109C, 0x109C, 0x0, 0x0, 0x0C037,
	0x0, 0x0, 0x102C, 0x0C0B5, 0x0, 0x803B, 0x0, 0x0, 0x803C, 0x0, 0x0, 0x803D,
	0x0, 0x0, 0x803E, 0x0, 0x0, 0x803F, 0x0, 0x0, 0x8040, 0x0, 0x0, 0x8041,
	0x0, 0x0, 0x8042, 0x0, 0x0, 0x8043, 0x0, 0x0, 0x1034, 0x0C0C9, 0x0C049,
	0x1033, 0x0C0D1, 0x0C051, 0x80C7, 0x0, 0x0, 0x8044, 0x0, 0x0, 0x8057,
	0x0, 0x0, 0x8058, 0x0, 0x0, 0x101A, 0x80B7, 0x80B7, 0x101B, 0x8046,
	0x0, 0x0, 0x0, 0x0, 0x110B, 0x0, 0x0, 0x1102, 0x0, 0x0, 0x1103, 0x0, 0x0, 0x1104,
	0x0, 0x0, 0x1105, 0x0, 0x0, 0x1106, 0x0, 0x0, 0x1107, 0x0, 0x0, 0x1108,
	0x0, 0x0, 0x1109, 0x0, 0x0, 0x110A, 0x0, 0x0, 0x40B, 0x0, 0x0, 0x402, 0x0,
	0x0, 0x403, 0x0, 0x0, 0x404, 0x0, 0x0, 0x405, 0x0, 0x0, 0x406, 0x0, 0x0, 0x407,
	0x0, 0x0, 0x408, 0x0, 0x0, 0x409, 0x0, 0x0, 0x40A, 0x0, 0x0, 0x20B, 0x0, 0x0,
	0x202, 0x0, 0x0, 0x203, 0x0, 0x0, 0x204, 0x0, 0x0, 0x205, 0x0, 0x0, 0x206,
	0x0, 0x0, 0x207, 0x0, 0x0, 0x208, 0x0, 0x0, 0x209, 0x0, 0x0, 0x20A, 0x0, 0x0,
	0x21D, 0x0, 0x0, 0x438, 0x0, 0x0, 0x1021, 0x804E, 0x0, 0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x102F, 0x804C, 0x0, 0x8037, 0x0, 0x0,
	0x1024, 0x0, 0x0, 0x1011, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x802A, 0x0,
	0x0, 0x200F, 0x2052, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x80B5,
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x804A, 0x0,
	0x0, 0x0, 0x0, 0x0, 0x1015, 0x801C, 0x809C, 0x1031, 0x8001, 0x0, 0x1015,
	0x801C, 0x809C, 0x1031, 0x8001, 0x0, 0x1015, 0x1C, 0x9C, 0x1031,
	0x1, 0x0, 0x1031, 0x1, 0x0, 0x0, 0x0, 0x0, 0x10F, 0x152, 0x0, 0x913, 0x0,
	0x0, 0x930, 0x0, 0x0, 0x438, 0x0, 0x0, 0x920, 0x0, 0x0, 0x12A, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0, 0x0, 0x1030, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0, 0x1034, 0x0C0C9, 0x0C049, 0x1033, 0x0C0D1, 0x0C051,
	0x20, 0x0, 0x0, 0x102F, 0x0C04C, 0x0, 0x0, 0x0, 0x0, 0x8010, 0x0, 0x0, 0x8012,
	0x0, 0x0, 0x242, 0x0, 0x0, 0x442, 0x0, 0x0, 0x1016, 0x0, 0x0, 0x102D, 0x0,
	0x0, 0x1000, 0x0, 0x0, 0x10C8, 0x0, 0x0, 0x10D0, 0x0, 0x0, 0x10CD, 0x0,
	0x0, 0x10CB, 0x0, 0x00
};

short InputMng::keykind[180] =
{
	 -1, -1, -1, -1, 8, 8, 16, 16, 4, 4, 1, 1,
	1, -1, -1, -1, 16, -1, 4, 4, 2, 2, 2, -1,
	4, 4, -1, 8, 8, -1, 62, 62, -1, -1, 2, 0,
	62, 62, 62, 1, 1, 1, 1, 62, 62, 62, 62, 62, 8, 64, 64,
	64, 64, 62, -1, 62, 62, -1, 62, -1, 62, -1,
	62, -1, -1, -1, -1, -1, 8, 8, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 16, 16, -1, 1, -1, -1, 62,
	62, -1, -1, -1, -1, -1, -1, -1,
	- 1, -1, -1, -1, -1, -1, -1, -1,
	- 1, -1, -1, -1, -1, -1, -1, -1,
	- 1, -1, -1, -1, -1, -1, -1, -1,
	- 1, -1, -1, 4, 4, 4, 4, 4, 4, -1, -1,
	- 1, -1, -1, 1, 62, 62, -1, -1, -1,
	- 1, 62, 62, 64, 64, 64, 64, 1, 1, 62, 62, 1, 62,
	1, -1, -1, -1, -1, -1, 4, 4, -1, 8,
	8, 8, 8, 4, 4, -1, 64, -1, -1, -1, 1, 1
};

