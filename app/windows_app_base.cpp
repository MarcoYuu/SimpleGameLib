// BasewindowsApp.cpp
// アプリケーションクラス実装
//
// 0.2

#pragma warning(disable:4290)

#include <cassert>
#include <stdexcept>
#include "windows_app_base.h"
#include "../other/utility.h"

// 自分のライブラリの名前空間
namespace yuu
{
// アプリケーション
namespace app
{
//--------------------------------------------------------------------------------
//ウィンドウの基底クラス
//--------------------------------------------------------------------------------
WindowBase::WindowBase(tstring Name, int Width, int Height, DWORD style, bool consider_frame)
	: m_hWnd(NULL)
	, m_name(Name)
	, m_title(Name)
	, m_width(Width)
	, m_height(Height)
{
	if(!initWindowClass())
		throw std::runtime_error("ウィンドウの登録に失敗しました");
	if(!initWindowInstance(style, consider_frame))
		throw std::runtime_error("ウィンドウ作成に失敗しました");

	//自身のハンドルをプロパティに追加
	::SetProp(m_hWnd, L"THIS_PTR", static_cast<HANDLE>(this));
}
Window WindowBase::create(tstring Name, int Width, int Height, DWORD style, bool consider_frame)
{
	return Window(new WindowBase(Name, Width, Height, style, consider_frame));
}

bool WindowBase::initWindowClass()
{
	WNDCLASSEX wc;

	//ウィンドウクラスのパラメータ
	wc.cbSize			= sizeof(WNDCLASSEX);
	wc.style			= CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc		= static_cast<WNDPROC>(dummyProcedure);	//ダミープロシージャ名
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= ::GetModuleHandle(0);
	wc.hIcon			= static_cast<HICON>(LoadImage(NULL, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED));
	wc.hCursor			= static_cast<HCURSOR>(LoadImage(NULL, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED));
	wc.hbrBackground	= static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= m_name.c_str();
	wc.hIconSm			= static_cast<HICON>(LoadImage(NULL, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED));

	//windowclassを登録できなかったらFalse
	if(0 == ::RegisterClassEx(&wc))
		return false;

	return true;
}
bool WindowBase::initWindowInstance(DWORD style, bool consider_frame)
{
	//ウィンドウモードだったらフレームの分を考慮する
	int width = m_width;
	int height = m_height;

	if(consider_frame)
	{
		width  +=::GetSystemMetrics(SM_CXDLGFRAME) * 2;
		height +=::GetSystemMetrics(SM_CYDLGFRAME) * 2 +::GetSystemMetrics(SM_CYCAPTION);
	}

	//ウィンドウを作成
	m_hWnd = CreateWindow(
				 m_name.c_str(),
				 m_title.c_str(),
				 style,				//ウィンドウの種類
				 CW_USEDEFAULT,		//Ｘ座標
				 CW_USEDEFAULT,		//Ｙ座標
				 m_width,			//幅
				 m_height,			//高さ
				 NULL,				//親ウィンドウのハンドル、親を作るときはNULL
				 NULL,				//メニューハンドル、クラスメニューを使うときはNULL
				 ::GetModuleHandle(0),	//インスタンスハンドル
				 NULL
			 );

	//ウィンドウハンドルを取得できなかったらfalse
	if(!m_hWnd)
		return false;

	return true;
}
void WindowBase::show(bool bShow)
{
	//ウィンドウ表示
	::UpdateWindow(m_hWnd);

	if(bShow)
		::ShowWindow(m_hWnd, SW_SHOW);
	else
		::ShowWindow(m_hWnd, SW_HIDE);
}
void WindowBase::destroy()
{
	::DestroyWindow(m_hWnd);
}
void WindowBase::setSize(int x, int y, int Width, int Height)
{
	m_width = Width;
	m_height = Height;

	::MoveWindow(m_hWnd, x, y, Width, Height, 1);
	this->show(true);
}
void WindowBase::setTitle(tstring title)
{
	m_title = title;
	SetWindowText(m_hWnd, title.c_str());
}
LRESULT CALLBACK WindowBase::dummyProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	//プロパティを参照し自身のポインタを取得
	WindowBase *pWindow = static_cast<WindowBase *>(::GetProp(hWnd, L"THIS_PTR"));

	//ウィンドウの破棄時
	if(msg == WM_DESTROY)
	{
		if(pWindow != 0)
			pWindow->windowProcedure(hWnd, msg, wp, lp);

		//関連付けを抹消する
		::RemoveProp(hWnd, L"THIS_PTR");
		::PostQuitMessage(0);
		return 0;
	}

	//取得できなかったらデフォルトのプロシージャ
	if(pWindow == 0)
		return ::DefWindowProc(hWnd, msg, wp, lp);

	//実際の動作をするプロシージャを呼び出し
	return pWindow->windowProcedure(hWnd, msg, wp, lp);
}
LRESULT WindowBase::windowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	//この関数をオーバーロードしプロシージャを変更可能
	return ::DefWindowProc(hWnd, msg, wp, lp);
}

int WindowBase::getWidth() const
{
	return m_width;
}

int WindowBase::getHeight() const
{
	return m_height;
}

HWND WindowBase::getHandle() const
{
	return m_hWnd;
}

bool WindowBase::isActive()
{
	return m_hWnd == ::GetActiveWindow();
}

void WindowBase::showCursor(bool show)
{
	::ShowCursor(show ? TRUE : FALSE);
}

//--------------------------------------------------------------------------------
//アプリケーションの基礎クラス
//--------------------------------------------------------------------------------
void IApplication::run()
{
	//アプリケーションの初期化
	initialize();

	//メインループ開始
	startMainLoop();
}
//--------------------------------------------------------------------------------
//通常ウィンドウズアプリインタフェース
//--------------------------------------------------------------------------------
WindowsAppBase::WindowsAppBase()
	: base(), m_Window(NULL)
{}

void WindowsAppBase::initialize()
{
	setWindow(WindowBase::create(_T("test")));
	getWindow()->show();
}

void WindowsAppBase::startMainLoop()
{
	assert(getWindow() != NULL);

	MSG msg;
	BOOL bRet;

	while((bRet = ::GetMessage(&msg, 0, 0, 0)) != 0)
	{
		if(bRet == -1)
		{
			::MessageBox(NULL, L"Error on GetMassage()", L"Error", MB_OK);
			break;
		}
		else
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}
}

void WindowsAppBase::setWindow(Window window)
{
	m_Window = window;
}

Window WindowsAppBase::getWindow()
{
	return m_Window;
}

const Window WindowsAppBase::getWindow() const
{
	return m_Window;
}

//--------------------------------------------------------------------------------
//通常ゲームアプリ
//--------------------------------------------------------------------------------
WindowsGameBase::WindowsGameBase()
	: base()
{}
void WindowsGameBase::initialize()
{
	setWindow(WindowBase::create(_T("test")));
	getWindow()->show();
}
void WindowsGameBase::startMainLoop()
{
	assert(getWindow() != NULL);

	//メッセージを待たないループ
	MSG msg;
	do
	{
		if(::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}

		mainProcess();
	}
	while(msg.message != WM_QUIT);
}
void WindowsGameBase::mainProcess()
{
	::Sleep(10);
}
}
}