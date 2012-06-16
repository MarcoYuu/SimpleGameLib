

#include "TestClass.h"

int WINAPI WinMain(
	HINSTANCE hCurInst, HINSTANCE hPrevInst, LPSTR lpsCmdLine, int nCmdShow)
{
	int result =MessageBoxW( 
		NULL, 
		_T("はい：320x240\nいいえ：640x480\nキャンセル：1280x960"), 
		_T("デバッグ用ウィンドウサイズ選択"), 
		MB_YESNOCANCEL );

	BackBufferSize size;
	switch(result){
	case IDYES:		size =WS_320x240;break;
	case IDNO:		size =WS_640x480;break;
	case IDCANCEL:	size =WS_1280x960;break;
	}

	TestClass app(size);
	app.run();

	return 0;
}