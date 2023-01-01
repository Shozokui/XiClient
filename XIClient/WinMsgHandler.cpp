#include "Globals.h"
#include "CApp.h"
#include "InputMng.h"
#include "CTkMouse.h"
#include "TkManager.h"
#include "CEnv.h"
#include <Windowsx.h>
#include "RegistryConfig.h"
#include "CTkMenuMng.h"
#include "CFsConf6Win.h"
#include <iostream>
#include "RuntimeConfig.h"
using namespace FFXI::CYy;
using namespace FFXI::CTk;
void HandleMouseMove(MSG* msg) {
	FFXI::CTk::CTkMouse* mouse = TkManager::g_pCTkMouse;
	if (!mouse) return;
	short param = (short)(msg->wParam >> 16);
	mouse->field_84 = (unsigned short)param;
	short v4 = param / 120;
	tagPOINT currentPos = mouse->field_4;
	int v7{}, v8{};

	tagPOINT rawPos = { GET_X_LPARAM(msg->lParam), GET_Y_LPARAM(msg->lParam) };

	int maxMouseX{}, maxMouseY{};
	if (FFXI::Config::RuntimeConfig::instance.window_mode == FFXI::Config::RuntimeConfig::WindowMode::Fullscreen) {
		maxMouseX = FFXI::Config::RuntimeConfig::instance.resolution_main.X;
		maxMouseY = FFXI::Config::RuntimeConfig::instance.resolution_main.Y;
	}
	else if (FFXI::Config::RuntimeConfig::instance.window_mode == FFXI::Config::RuntimeConfig::WindowMode::Windowed) {
		int cxframe = GetSystemMetrics(SM_CXFRAME);
		int cyframe = GetSystemMetrics(SM_CYFRAME);
		int cycap = GetSystemMetrics(SM_CYCAPTION);

		tagRECT Rect{};
		WINDOWINFO info{};
		GetWindowRect(FFXI::CYy::CApp::g_pCApp->hWnd, &Rect);
		GetWindowInfo(FFXI::CYy::CApp::g_pCApp->hWnd, &info);
		Rect.left += info.cxWindowBorders;
		Rect.right -= info.cxWindowBorders + cxframe;
		Rect.bottom -= info.cyWindowBorders + cyframe;
		Rect.top += info.cyWindowBorders + cycap;
		maxMouseX = (Rect.right - Rect.left);
		maxMouseY = (Rect.bottom - Rect.top);
	}
	else {
		tagRECT Rect{};
		GetWindowRect(FFXI::CYy::CApp::g_pCApp->hWnd, &Rect);
		maxMouseX = Rect.right - Rect.left;
		maxMouseY = Rect.bottom - Rect.top;
	}

	double adjustedX = (double)rawPos.x / maxMouseX * (TkManager::g_CTkMenuMng.UIXRes - TkManager::g_CTkMenuMng.field_7C);
	double adjustedY = (double)rawPos.y / maxMouseY * (TkManager::g_CTkMenuMng.UIYRes - TkManager::g_CTkMenuMng.field_7E);

	tagPOINT posDiff = { (LONG)adjustedX - currentPos.x, (LONG)adjustedY - currentPos.y };
	if (v4 || abs(posDiff.x) > 2 || abs(posDiff.y) > 2) {
		if (CFsConf6Win::Check())
			v4 = 0;
		mouse->OnMouseMove(&posDiff, v4);
		mouse->SetDrawStatus(1);
	}
}
void CApp::WinMsgHandler(MSG* msg) {
	if (msg->message > WM_IME_COMPOSITION) {
		switch (msg->message) {
		case WM_IME_SETCONTEXT:
			return;
		case WM_IME_NOTIFY:
			//sub //TODO fsTextInput stuff
			goto LABEL_39;
		case WM_IME_CONTROL:
			//nullsub
			goto LABEL_39;
		default:
			goto LABEL_39;
		}
	}
	else {
		if (msg->message == WM_IME_COMPOSITION) {
			//sub //todo fsTextInput stuff
		}
		else if (msg->message > WM_CHAR) {
			if (msg->message == WM_SYSKEYUP) {
				//sub //todo TextInput stuff
			}
			else if (msg->message == WM_IME_STARTCOMPOSITION) {
				//nullsub
			}
		}
		else {
			switch (msg->message) {
			case WM_CHAR:
				if (CApp::g_pInputMng)
					;//sub //TODO
				break;
			case WM_ACTIVATE:
				if (CApp::g_pInputMng)
					;//sub //TODO
				break;
			case WM_KEYDOWN:
				if (Globals::g_pCenv) {
					if (msg->wParam == VK_PROCESSKEY) {
						if (FFXI::CYy::CApp::g_pCApp->hWnd && ImmGetVirtualKey(FFXI::CYy::CApp::g_pCApp->hWnd) == 0xFF)
							Globals::g_pCenv->field_14 = 1;
					}
					else if (msg->wParam == 0xFF) {
						Globals::g_pCenv->field_14 = 1;
					}
				}
				//sub //todo TextInput stuff
				break;
			}
		}
	LABEL_39:
		if (!CApp::g_pInputMng->UseSoftwareMouse) {
			switch (msg->message) {
			case WM_MOUSEMOVE:
				HandleMouseMove(msg);
				break;
			case WM_LBUTTONDOWN:
				CTk::TkManager::g_pCTkMouse->OnLButtonDown();
				break;
			case WM_LBUTTONUP:
				CTk::TkManager::g_pCTkMouse->OnLButtonUp();
				break;
			case WM_RBUTTONDOWN:
				CTk::TkManager::g_pCTkMouse->OnRButtonDown();
				break;
			case WM_RBUTTONUP:
				CTk::TkManager::g_pCTkMouse->OnRButtonUp();
				break;
			case WM_MBUTTONDOWN:
				//sub todo
				break;
			case WM_MBUTTONUP:
				//sub todo
				break;
			case WM_MOUSEWHEEL:
				if (FFXI::CTk::CFsConf6Win::Check()) {
					if (!CTk::TkManager::g_CTkMenuMng.HaveActiveWindow())
					{
						tagPOINT v2{ 0, 0 };
						short s = (short)(msg->wParam >> 16);
						CTk::TkManager::g_pCTkMouse->UpdatePos(&v2, s / 120);
					}
				}
				else {
					HandleMouseMove(msg);
				}
				break;
			default:
				return;
			}

		}
	}
}