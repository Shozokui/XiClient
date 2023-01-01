#include "WindowManager.h"
#include "PolVars.h"
#include "RuntimeConfig.h"
#include "CApp.h"

Window::WindowManager Window::WindowManager::window_manager{};

//local vars
const char WindowClass[] = "FFXiClass";
const char WindowName[] = "FINAL FANTASY XI";

LRESULT __stdcall WinMsgHandler(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
    static char byte_10445858{};
    tagRECT v31{}, Rect{};
    WINDOWPLACEMENT wndpl{};
    HANDLE CurrentProcess{};

    if (Msg > WM_WINDOWPOSCHANGING) {
        if (Msg > WM_SYSCOMMAND) {
            if (Msg != WM_PSD_FULLPAGERECT) goto LABEL_61;
            if (FFXI::CYy::CApp::g_pCApp != nullptr && FFXI::CYy::CApp::g_pCApp->field_1D != 0) {
                FFXI::CYy::CApp::g_pCApp->ClientTick();
            }
        }
        else {
            if (Msg != WM_SYSCOMMAND) {
                if (Msg == WM_KEYFIRST) {
                    if (wParam == 16) byte_10445858 = 1;
                    return DefWindowProcA(hWnd, Msg, wParam, lParam);
                }
                if (Msg == WM_KEYUP) {
                    if (wParam == 16) byte_10445858 = 0;
                    return DefWindowProcA(hWnd, Msg, wParam, lParam);
                }
            LABEL_61:
                if (Msg > WM_IME_NOTIFY) {
                    if (Msg != WM_IME_CONTROL) return DefWindowProcA(hWnd, Msg, wParam, lParam);
                }
                else {
                    if (Msg == WM_IME_NOTIFY) {
                        if (wParam != 8
                            && (wParam != 14 || lParam != 5 && lParam)
                            && (FFXI::PolVars::instance.UseImeUi != 1 || wParam != 5 && wParam != 4)) {
                            return 0;
                        }
                        return DefWindowProcA(hWnd, Msg, wParam, lParam);
                    }
                    if (Msg != WM_IME_STARTCOMPOSITION && Msg != WM_IME_ENDCOMPOSITION)
                        return DefWindowProcA(hWnd, Msg, wParam, lParam);
                }
                return 0;
            }
            if (wParam > 0xF090)
            {
                switch (wParam)
                {
                case 0xF100u:
                case 0xF130u:
                case 0xF140u:
                case 0xF150u:
                case 0xF170u:
                case 0xF180u:
                    return 1;
                default:
                    goto LABEL_45;
                }
            }
            if (wParam != 0xF090)
            {
                if (wParam > 0xF040)
                {
                    if (wParam == 0xF050)
                        return 1;
                    if (wParam == 0xF060) {
                        PostQuitMessage(0);
                        return 1;
                    }

                }
                else
                {
                    switch (wParam)
                    {
                    case 0xF040u:
                        return 1;
                    case 1u:
                        if (FFXI::Config::RuntimeConfig::instance.window_mode == FFXI::Config::RuntimeConfig::WindowMode::Fullscreen)
                            return DefWindowProcA(hWnd, Msg, wParam, lParam);
                        wndpl.length = 44;
                        if (GetWindowPlacement(hWnd, &wndpl) && (wndpl.showCmd == 2 || wndpl.showCmd == 3))
                        {
                            ShowWindow(hWnd, 1);
                        }
                        else
                        {
                            GetWindowRect(hWnd, &v31);
                            Rect.right = FFXI::Config::RuntimeConfig::instance.resolution_main.X;
                            Rect.bottom = FFXI::Config::RuntimeConfig::instance.resolution_main.Y;
                            Rect.left = 0;
                            Rect.top = 0;
                            AdjustWindowRectEx(&Rect, Window::WindowManager::window_manager.dwStyle, 0, Window::WindowManager::window_manager.dwExStyle);
                            MoveWindow(hWnd, v31.left, v31.top, Rect.right - Rect.left, Rect.bottom - Rect.top, 1);
                        }
                        return DefWindowProcA(hWnd, Msg, wParam, lParam);
                    case 0xF000u:
                    case 0xF010u:
                        return 1;
                    }
                }
            LABEL_45:
                if (FFXI::Config::RuntimeConfig::instance.window_mode != FFXI::Config::RuntimeConfig::WindowMode::Fullscreen)
                    return DefWindowProcA(hWnd, Msg, wParam, lParam);
                if (wParam != 0xF020 && wParam != 0xF030)
                {
                    if (wParam == 0xF120)
                        return 1;
                    return DefWindowProcA(hWnd, Msg, wParam, lParam);
                }
            }
        }
        return 1;
    }

    if (Msg == WM_WINDOWPOSCHANGING)
    {
        WINDOWPOS* newPos = (WINDOWPOS*)lParam;
        if ((newPos->flags & 1) != 0 || FFXI::Config::RuntimeConfig::instance.window_mode == FFXI::Config::RuntimeConfig::WindowMode::Fullscreen)
            return DefWindowProcA(hWnd, Msg, wParam, lParam);
        int cxframe = GetSystemMetrics(SM_CXFRAME);
        int cyframe = GetSystemMetrics(SM_CYFRAME);
        int cycap = GetSystemMetrics(SM_CYCAPTION);
        int cxmin = GetSystemMetrics(SM_CXMIN);
        if (FFXI::Config::RuntimeConfig::instance.window_mode == FFXI::Config::RuntimeConfig::WindowMode::BorderlessFullscreen)
        {
            cxframe = 0;
            cyframe = 0;
            cycap = 0;
        }
        int v14 = (double)(cxmin - 2 * cxframe) / FFXI::Config::RuntimeConfig::instance.resolution_main.X * FFXI::Config::RuntimeConfig::instance.resolution_main.Y;
        char v16 = 0;
        if (newPos->cy < v14)
        {
            newPos->cx = 2 * cxframe;
            v16 = 1;
            newPos->cy = cycap + v14 + 2 * cyframe;
        }
        if (byte_10445858)
            goto LABEL_22;
        if (v16 != 1)
            return DefWindowProcA(hWnd, Msg, wParam, lParam);
    LABEL_22:
        GetWindowRect(hWnd, &Rect);
        int width = Rect.right - Rect.left;
        int height = Rect.bottom - Rect.top;

        if (width < 0)
            width = newPos->cx - width;
        if (height < 0)
            height = newPos->cy - height;

        if (width < height || v16 == 1) {
            float v25 = (double)(newPos->cy - 2 * cyframe - cycap) / (float)FFXI::Config::RuntimeConfig::instance.resolution_main.Y;
            newPos->cx = (double)FFXI::Config::RuntimeConfig::instance.resolution_main.X * v25 + (double)(2 * cxframe);
        }
        else {
            newPos->cy = (double)(newPos->cx - 2 * cxframe) / (double)FFXI::Config::RuntimeConfig::instance.resolution_main.X * (double)FFXI::Config::RuntimeConfig::instance.resolution_main.Y + (double)(2 * cyframe) + cycap;
        }
        return DefWindowProcA(hWnd, WM_WINDOWPOSCHANGING, wParam, lParam);
    }
    switch (Msg)
    {
    case WM_DESTROY:
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;
        break;
    case WM_SETFOCUS:
        CurrentProcess = GetCurrentProcess();
        if (CurrentProcess) {
            SetPriorityClass(CurrentProcess, 0x20u);
        }
        if (FFXI::CYy::CApp::g_pCApp != nullptr) {
            FFXI::CYy::CApp::g_pCApp->field_1C = 1;
        }
        return DefWindowProcA(hWnd, Msg, wParam, lParam);
    case WM_KILLFOCUS:
        CurrentProcess = GetCurrentProcess();
        if (CurrentProcess != NULL) {
            SetPriorityClass(CurrentProcess, 0x40u);
        }
        if (FFXI::CYy::CApp::g_pCApp != nullptr) {
            FFXI::CYy::CApp::g_pCApp->field_1C = 0;
        }
        return DefWindowProcA(hWnd, Msg, wParam, lParam);
    case WM_GETTEXTLENGTH:
        return 1;
    default:
        goto LABEL_61;
    }
    return 0;
}

HWND Window::WindowManager::CreateXIWindow(LPCSTR lpWindowName, LPCSTR lpClassName, HINSTANCE hInstance, const char* a4, enum class FFXI::Config::RuntimeConfig::WindowMode WindowMode, int xRight, int yBottom)
{
	switch (WindowMode) {
    case FFXI::Config::RuntimeConfig::WindowMode::Windowed:
		this->dwExStyle = WS_EX_OVERLAPPEDWINDOW;
		this->dwStyle = WS_OVERLAPPEDWINDOW;
		break;
	case FFXI::Config::RuntimeConfig::WindowMode::BorderlessWindowed:
		this->dwExStyle = WS_EX_LEFT;
		this->dwStyle = WS_POPUP;
		break;
	case FFXI::Config::RuntimeConfig::WindowMode::BorderlessFullscreen:
		this->dwExStyle = WS_EX_LEFT;
		this->dwStyle = WS_POPUP;
		xRight = GetSystemMetrics(SM_CXSCREEN);
		yBottom = GetSystemMetrics(SM_CYSCREEN);
		break;
	}
	WNDCLASSA WndClass{};
	tagRECT rc{};
	WndClass.style = 0;
	WndClass.lpfnWndProc = WinMsgHandler;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = 0;
	WndClass.hCursor = 0;
	WndClass.hbrBackground = (HBRUSH)6;
	WndClass.lpszMenuName = 0;
	WndClass.lpszClassName = lpClassName;

	if (!RegisterClassA(&WndClass)) return 0;

	SetRect(&rc, 0, 0, xRight, yBottom);
	AdjustWindowRectEx(&rc, this->dwStyle, 0, this->dwExStyle);

	HWND Window = CreateWindowExA(
		this->dwExStyle,
		lpClassName,
		lpWindowName,
		this->dwStyle,
		0x80000000,
		0x80000000,
		rc.right - rc.left,
		rc.bottom - rc.top,
		0,
		0,
		hInstance,
		0
	);

    if (Window == NULL) {
        return NULL;
    }

    if (WindowMode != FFXI::Config::RuntimeConfig::WindowMode::Fullscreen) {
        char LibFileName[260] = { 0 };
        strcpy_s(LibFileName, a4);
        strcat_s(LibFileName, "\\FFXiMain.dll");
        HMENU SystemMenu = GetSystemMenu(Window, false);
        if (SystemMenu) RemoveMenu(SystemMenu, 0xF060u, 0);
        HMODULE LibraryA = LoadLibraryA(LibFileName);
        HINSTANCE v6 = LibraryA;
        if (LibraryA) {
            MENUITEMINFOA mi{};
            char string[256] = { 0 };
            LoadStringA(LibraryA, 0x65, string, 256);
            mi.dwTypeData = string;
            mi.cbSize = 44;
            mi.fMask = 18;
            mi.cch = strlen((LPCSTR)&mi.hbmpItem);
            mi.wID = 1;
            mi.fType = 0;
            if (SystemMenu)	InsertMenuItemA(SystemMenu, 0, 1, &mi);
            HICON IconA = LoadIconA(v6, MAKEINTRESOURCEA(0xCF));
            if (IconA) SendMessage(Window, 0x80u, 0, (LPARAM)IconA);
            FreeLibrary(LibraryA);
        }
        ShowWindow(Window, SW_SHOW);
        UpdateWindow(Window);
    }

	return Window;
}

bool Window::WindowManager::InitWindow(HWND hwnd)
{
    if (hwnd != NULL) {
        Window::WindowManager::window_manager.hWnd = hwnd;
        Window::WindowManager::window_manager.created_own_window = false;
        return true;
    }

    Window::WindowManager::window_manager.hWnd = Window::WindowManager::window_manager.CreateXIWindow(
        WindowName,
        WindowClass,
        FFXI::PolVars::instance.hmod,
        FFXI::PolVars::instance.FFXIDirectory,
        FFXI::Config::RuntimeConfig::instance.window_mode,
        FFXI::Config::RuntimeConfig::instance.resolution_main.X,
        FFXI::Config::RuntimeConfig::instance.resolution_main.Y
    );

    Window::WindowManager::window_manager.created_own_window = true;
    return Window::WindowManager::window_manager.hWnd != NULL;
}

void Window::WindowManager::UninitWindow()
{
    if (Window::WindowManager::window_manager.created_own_window == false) {
        return;
    }

    if (Window::WindowManager::window_manager.hWnd != NULL) {
        DestroyWindow(Window::WindowManager::window_manager.hWnd);
        Window::WindowManager::window_manager.hWnd = nullptr;
    }
    UnregisterClassA(WindowClass, FFXI::PolVars::instance.hmod);
    tagMSG Msg{};
    while (PeekMessageA(&Msg, 0, 0, 0, 1));
}
