#include "../XIClient/XIClient.h"

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nShowCmd)
{
	XIClient::StartMain(hInstance, hPrevInstance, lpCmdLine, NULL);
	return 1;
}