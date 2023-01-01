#pragma once
#include <wtypes.h>
#include "RuntimeConfig.h"
namespace Window {
	class WindowManager {
	private:
		bool created_own_window;
		HWND CreateXIWindow(LPCSTR, LPCSTR, HINSTANCE, const char*, enum class FFXI::Config::RuntimeConfig::WindowMode, int, int);
	public:
		static WindowManager window_manager;
		static bool InitWindow(HWND);
		static void UninitWindow();
		DWORD dwExStyle;
		DWORD dwStyle;
		HWND hWnd;
	};
}