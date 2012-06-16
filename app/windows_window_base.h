// BasewindowsApp.h
// アプリケーションクラスヘッダ

// 0.2

#pragma once

#include <windows.h>

#include <boost/intrusive_ptr.hpp>
#include <boost/utility.hpp>

#include <other/refference_count.h>
#include <other/utility.h>

// 自分のライブラリの名前空間
namespace yuu
{
// アプリケーション
namespace app
{

class WindowBase;
typedef boost::intrusive_ptr<WindowBase> Window;

//--------------------------------------------------------------------------------
//ウィンドウの基底クラス
//--------------------------------------------------------------------------------
// ウィンドウタイプ
enum WindowType
{
	DEFAULT = WS_OVERLAPPEDWINDOW,	// <通常
	SIMPLE1 = WS_POPUP | WS_DLGFRAME | WS_SYSMENU | WS_CAPTION,	// <シンプル1
	SIMPLE2 = WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX	// <シンプル2
};

// ウィンドウ基礎クラス
// 継承してプロシージャを変更できる\n
class WindowBase
	: public IRefferenceCount<WindowBase>
	, boost::noncopyable
{
public:
	static Window create(
		tstring name, 
		int width = 800, 
		int height = 600,
		WindowType style = SIMPLE2, 
		bool consider_frame = true);
	virtual ~WindowBase() {}

	void destroy();
	void show(bool bShow = true);

	int getWidth() const;
	int getHeight() const;
	HWND getHandle() const;

	bool isActive();
	void setSize(int x, int y, int width, int height);
	void setTitle(tstring title);

	void showCursor(bool show);

protected:
	virtual LRESULT windowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
	virtual bool initWindowClass();
	virtual bool initWindowInstance(WindowType style, bool consider_frame);

private:
	HWND m_hWnd;

	tstring m_name;
	tstring m_title;
	int m_width;
	int m_height;

	static LRESULT CALLBACK dummyProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
	WindowBase(tstring name, int width, int height, WindowType style, bool consider_frame);
};
}
}