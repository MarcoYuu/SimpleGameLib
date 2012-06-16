

#include "TestClass.h"

int WINAPI WinMain(
	HINSTANCE hCurInst, HINSTANCE hPrevInst, LPSTR lpsCmdLine, int nCmdShow)
{
	int result =MessageBoxW( 
		NULL, 
		_T("�͂��F320x240\n�������F640x480\n�L�����Z���F1280x960"), 
		_T("�f�o�b�O�p�E�B���h�E�T�C�Y�I��"), 
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