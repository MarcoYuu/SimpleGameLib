#include <windows.h>
#include "TestClass.h"

int WINAPI WinMain(
	HINSTANCE hCurInst, HINSTANCE hPrevInst, LPSTR lpsCmdLine, int nCmdShow)
{
	TestClass app(WS_320x240);
	app.run();

	return 0;
}