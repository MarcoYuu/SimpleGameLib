// BasewindowsApp.cpp
// アプリケーションクラス実装
//
// 0.2

#include <cassert>
#include <stdexcept>

#include <app/windows_window_base.h>
#include <other/utility.h>

// 自分のライブラリの名前空間
namespace yuu
{
// アプリケーション
namespace app
{

static inline DWORD ConvertWindowStyle(WindowType style){
	switch(style){
	case WT_DEFAULT:	return WS_OVERLAPPEDWINDOW;
	case WT_SIMPLE1:	return WS_POPUP | WS_DLGFRAME | WS_SYSMENU | WS_CAPTION;
	case WT_SIMPLE2:	return WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX;
	default:			return WS_OVERLAPPEDWINDOW;
	}
}

//--------------------------------------------------------------------------------
//ウィンドウの基底クラス
//--------------------------------------------------------------------------------
struct WindowBase::Param{
	HWND hWnd;
	tstring name;
	tstring title;
	int width;
	int height;

	Param(const tstring& Name, int Width, int Height)
		: hWnd(NULL)
		, name(Name)
		, title(Name)
		, width(Width)
		, height(Height)
	{}
};
WindowBase::WindowBase(tstring Name, int Width, int Height, WindowType style, bool consider_frame)
	: param(new Param(Name, Width, Height))
{
	if(!initWindowClass())
		throw std::runtime_error("ウィンドウの登録に失敗しました");
	if(!initWindowInstance(style, consider_frame))
		throw std::runtime_error("ウィンドウ作成に失敗しました");

	//自身のハンドルをプロパティに追加
	//::SetProp(m_hWnd, L"THIS_PTR", static_cast<HANDLE>(this));
	::SetWindowLongPtr(param->hWnd, GWLP_USERDATA, (LONG_PTR)this);
}

WindowBase::~WindowBase()
{
	delete param;
}

Window WindowBase::create(tstring Name, int Width, int Height, WindowType style, bool consider_frame)
{
	return Window(new WindowBase(Name, Width, Height, style, consider_frame));
}

bool WindowBase::initWindowClass()
{
	WNDCLASSEX wc;

	//ウィンドウクラスのパラメータ
	wc.cbSize			= sizeof(WNDCLASSEX);
	wc.style			= CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc		= static_cast<WNDPROC>(procedure);	//ダミープロシージャ名
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= ::GetModuleHandle(0);
	wc.hIcon			= static_cast<HICON>(LoadImage(NULL, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED));
	wc.hCursor			= static_cast<HCURSOR>(LoadImage(NULL, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED));
	wc.hbrBackground	= static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= param->name.c_str();
	wc.hIconSm			= static_cast<HICON>(LoadImage(NULL, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED));

	//windowclassを登録できなかったらFalse
	if(0 == ::RegisterClassEx(&wc))
		return false;

	return true;
}

bool WindowBase::initWindowInstance(WindowType style, bool consider_frame)
{
	//ウィンドウモードだったらフレームの分を考慮する
	int width  = param->width;
	int height = param->height;

	if(consider_frame)
	{
		width  +=::GetSystemMetrics(SM_CXDLGFRAME) * 2;
		height +=::GetSystemMetrics(SM_CYDLGFRAME) * 2 +::GetSystemMetrics(SM_CYCAPTION);
	}

	DWORD win_style =ConvertWindowStyle(style);

	//ウィンドウを作成
	param->hWnd = CreateWindow(
		param->name.c_str(),
		param->title.c_str(),
		win_style,				//ウィンドウの種類
		CW_USEDEFAULT,			//Ｘ座標
		CW_USEDEFAULT,			//Ｙ座標
		param->width,			//幅
		param->height,			//高さ
		NULL,					//親ウィンドウのハンドル、親を作るときはNULL
		NULL,					//メニューハンドル、クラスメニューを使うときはNULL
		::GetModuleHandle(0),	//インスタンスハンドル
		NULL
	);

	//ウィンドウハンドルを取得できなかったらfalse
	if(!param->hWnd)
		return false;

	return true;
}
void WindowBase::destroy()
{
	::DestroyWindow(param->hWnd);
}
void WindowBase::show(bool bShow)
{
	//ウィンドウ表示
	::UpdateWindow(param->hWnd);

	if(bShow)
		::ShowWindow(param->hWnd, SW_SHOW);
	else
		::ShowWindow(param->hWnd, SW_HIDE);
}
int WindowBase::getWidth() const
{
	return param->width;
}

int WindowBase::getHeight() const
{
	return param->height;
}

void* WindowBase::getHandle() const
{
	return param->hWnd;
}

const tstring& WindowBase::getName() const
{
	return param->name;
}

void WindowBase::setSize(int x, int y, int Width, int Height)
{
	param->width = Width;
	param->height = Height;

	::MoveWindow(param->hWnd, x, y, Width, Height, 1);
	this->show(true);
}
void WindowBase::setName(tstring title)
{
	param->title = title;
	SetWindowText(param->hWnd, title.c_str());
}
bool WindowBase::isActive()
{
	return param->hWnd == ::GetActiveWindow();
}

void WindowBase::showCursor(bool show)
{
	::ShowCursor(show ? TRUE : FALSE);
}

void WindowBase::setHandle( void* handle )
{
	param->hWnd =(HWND)handle;
}

LRESULT WindowBase::callbackProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	//この関数をオーバーロードしプロシージャを変更可能
	return ::DefWindowProc(hWnd, msg, wp, lp);
}

LRESULT CALLBACK WindowBase::procedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	//プロパティを参照し自身のポインタを取得
	//WindowBase *pWindow = static_cast<WindowBase *>(::GetProp(hWnd, L"THIS_PTR"));
	WindowBase *pWindow = (WindowBase*)::GetWindowLongPtr(hWnd, GWLP_USERDATA);

	//ウィンドウの破棄時
	if(msg == WM_DESTROY)
	{
		if(pWindow != 0)
			pWindow->callbackProcedure(hWnd, msg, wp, lp);

		//関連付けを抹消する
		//::RemoveProp(hWnd, L"THIS_PTR");
		::PostQuitMessage(0);
		return 0;
	}

	//取得できなかったらデフォルトのプロシージャ
	if(pWindow == 0)
		return ::DefWindowProc(hWnd, msg, wp, lp);

	//実際の動作をするプロシージャを呼び出し
	return pWindow->callbackProcedure(hWnd, msg, wp, lp);
}
}
}