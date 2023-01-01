#include "App.h"
#include "nfd/include/nfd.h"
#include "imgui/imgui_internal.h"
#include "imgui/backends/imgui_impl_dx9.h"
#include "imgui/backends/imgui_impl_win32.h"
#include <d3d9.h>

#include "XIDirMng.h"
#include "Event.h"
#include "Client.h"

#ifndef WM_DPICHANGED
#define WM_DPICHANGED 0x02E0 // From Windows SDK 8.1+ headers
#endif

//Forward declaration from ImGui backend
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#pragma region Local Variables
LPDIRECT3D9              g_pD3D = NULL;
LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
D3DPRESENT_PARAMETERS    g_d3dpp = {};
HWND                     hwnd = NULL;
WNDCLASSEXW              wc = {};

DATio::IO::XIDirMng      dir_mng = {};
DATio::UI::EventWindow   win_event = {};
DATio::UI::ClientWindow  win_client = {};

//enable flags
bool enable_menubar_view{ true };
#pragma endregion

#pragma region Cleanup
void CleanupDeviceD3D()
{
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
    if (g_pD3D) { g_pD3D->Release(); g_pD3D = NULL; }
}

void ResetDevice()
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
    HRESULT hr = g_pd3dDevice->Reset(&g_d3dpp);
    if (hr == D3DERR_INVALIDCALL)
        IM_ASSERT(0);
    ImGui_ImplDX9_CreateDeviceObjects();
}

void End()
{
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
}
#pragma endregion

//Windows message handler
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
        {
            g_d3dpp.BackBufferWidth = LOWORD(lParam);
            g_d3dpp.BackBufferHeight = HIWORD(lParam);
            ResetDevice();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    case WM_DPICHANGED:
        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DpiEnableScaleViewports)
        {
            //const int dpi = HIWORD(wParam);
            //printf("WM_DPICHANGED to %d (%.0f%%)\n", dpi, (float)dpi / 96.0f * 100.0f);
            const RECT* suggested_rect = (RECT*)lParam;
            ::SetWindowPos(hWnd, NULL, suggested_rect->left, suggested_rect->top, suggested_rect->right - suggested_rect->left, suggested_rect->bottom - suggested_rect->top, SWP_NOZORDER | SWP_NOACTIVATE);
        }
        break;
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}

#pragma region Initialization

bool CreateDeviceD3D(HWND hWnd)
{
    if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
        return false;

    // Create the D3DDevice
    ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
    g_d3dpp.Windowed = TRUE;
    g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN; // Need to use an explicit format with alpha if needing per-pixel alpha composition.
    g_d3dpp.EnableAutoDepthStencil = TRUE;
    g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;           // Present with vsync
    //g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;   // Present without vsync, maximum unthrottled framerate
    if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0)
        return false;

    return true;
}

bool DATio::UI::App::Initialize()
{
    // Create application window
    //ImGui_ImplWin32_EnableDpiAwareness();
    wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, L"DATio", NULL };
    ::RegisterClassExW(&wc);
    hwnd = ::CreateWindowW(wc.lpszClassName, L"DATio", WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, NULL, NULL, wc.hInstance, NULL);

    // Initialize Direct3D
    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return false;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    return true;
}
#pragma endregion

#pragma region Main Menu
//Helper funcs for Render Main Menu
void FileMenu_Open() {
    nfdnchar_t* outPath = NULL;
    nfdresult_t result = NFD_PickFolderN(&outPath, NULL);
    
    if (result == NFD_OKAY) {
        DATio::IO::XIDirMng::result_code set_result = dir_mng.TrySetHead(outPath);
        enable_menubar_view = set_result == DATio::IO::XIDirMng::result_code::OK;

        if (set_result != DATio::IO::XIDirMng::result_code::OK) {
            std::wstring error_message{};

            switch (set_result) {
            case DATio::IO::XIDirMng::result_code::BASE_VTABLE_MISSING:
                error_message = L"The selected directory does not contain VTABLE.DAT";
                break;
            case DATio::IO::XIDirMng::result_code::BASE_VTABLE_BLANK:
                error_message = L"The selected directory contains a blank VTABLE.DAT";
                break;
            case DATio::IO::XIDirMng::result_code::EXPANSION_VTABLE_SIZE_MISMATCH:
                error_message = L"An expansion VTABLE filesize does not match base VTABLE filesize";
                break;
            case DATio::IO::XIDirMng::result_code::BASE_FTABLE_MISSING:
                error_message = L"The selected directory does not contain FTABLE.DAT";
                break;
            case DATio::IO::XIDirMng::result_code::BASE_FTABLE_BLANK:
                error_message = L"The selected directory contains a blank FTABLE.DAT";
                break;
            case DATio::IO::XIDirMng::result_code::EXPANSION_FTABLE_SIZE_MISMATCH:
                error_message = L"An expansion FTABLE filesize does not match base FTABLE filesize";
                break;
            case DATio::IO::XIDirMng::result_code::BASE_VTABLE_FTABLE_SIZE_MISMATCH:
                error_message = L"The base FTABLE filesize does not match the base VTABLE filesize";
                break;
            case DATio::IO::XIDirMng::result_code::FILESYSTEM_FAILURE:
                error_message = L"An unspecified file system error has occurred";
                break;
            default:
                error_message = L"An unspecified error has occurred";
                break;
            }

            MessageBoxW(hwnd, error_message.c_str(), L"Error", MB_OK);
        }

        NFD_FreePathN(outPath);
    }
}


void RenderFileMenu() {
    if (ImGui::BeginMenu("File", true) == true) {
        if (ImGui::MenuItem("Open", "") == true) { FileMenu_Open(); }
        ImGui::EndMenu();
    }
}


void RenderViewMenu() {
    if (ImGui::BeginMenu("View", enable_menubar_view) == true) {
        if (ImGui::MenuItem("Event", "", &win_event.visible) == true) { }
        if (ImGui::MenuItem("Client", "", &win_client.visible) == true) { }
        ImGui::EndMenu();
    }
}
void RenderMainMenu()
{
    if (ImGui::BeginMainMenuBar() == true) {
        RenderFileMenu();
        RenderViewMenu();
        ImGui::EndMainMenuBar();
    }
}
#pragma endregion

void RenderMainStatusBar()
{
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar;
    float height = ImGui::GetFrameHeight();

    if (ImGui::BeginViewportSideBar("##MainStatusBar", NULL, ImGuiDir_Down, height, window_flags)) {
        if (ImGui::BeginMenuBar()) {
            std::string StatusText = "Directory: " + dir_mng.GetHead().string();
            ImGui::Text(StatusText.c_str());
            ImGui::EndMenuBar();
        }
    }

    ImGui::End();
}

//Main loop
void DATio::UI::App::Begin()
{
    // Our state
    bool show_demo_window = true;

    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    // Main loop
    bool done = false;
    while (!done)
    {
        // Poll and handle messages (inputs, window resize, etc.)
        // See the WndProc() function below for our to dispatch events to the Win32 backend.
        MSG msg;
        while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        // Start the Dear ImGui frame
        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        //Enable the Dockspace
        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

        //Draw the main menu
        RenderMainMenu();

        //Draw the status bar
        RenderMainStatusBar();

        //Draw any open views
        if (win_event.visible == true) { win_event.Render(); }
        if (win_client.visible == true) { win_client.Render(); }

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // Rendering
        ImGui::EndFrame();
        g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
        
        if (g_pd3dDevice->BeginScene() >= 0)
        {
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            g_pd3dDevice->EndScene();
        }

        // Update and Render additional Platform Windows
        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }

        HRESULT result = g_pd3dDevice->Present(NULL, NULL, NULL, NULL);

        // Handle loss of D3D9 device
        if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
            ResetDevice();
    }
}