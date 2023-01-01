#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "App.h"
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if (DATio::UI::App::Initialize() == true) {
		DATio::UI::App::Begin();
	}
	return 1;
}