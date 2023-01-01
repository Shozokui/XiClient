#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#ifdef XIClient_EXPORTS
#define XIClient_API __declspec(dllexport)
#else
#define XIClient_API __declspec(dllimport)
#endif

static class XIClient {
protected:
	//Prevent instantiation. XIClient makes heavy use of static variables, so users
	//should not try to use multiple XIClients in the same application
	XIClient() {}
public:
	//Starts the standard XIClient
	XIClient_API int static StartMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, HWND hwnd);
	
	enum class init_result;
	//The following methods should only be used if you are rolling
	//a custom implementation of XiClient. ie, in a tool
	XIClient_API static init_result Initialize(HWND, LPSTR);
	XIClient_API static LPCWSTR GetInitializeErrorString(init_result);
	XIClient_API static void Uninitialize();
};

