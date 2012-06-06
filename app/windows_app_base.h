// BasewindowsApp.h
// アプリケーションクラスヘッダ

// 0.2

#pragma once

#pragma warning(disable:4290)

#include <windows.h>

#include <boost/intrusive_ptr.hpp>
#include <boost/utility.hpp>

#include "../other/refference_count.h"
#include "../other/utility.h"

// 自分のライブラリの名前空間
namespace yuu
{
// アプリケーション
namespace app
{
class WindowBase;
class IApplication;
class WindowsAppBase;
class WindowsGameBase;

// ウィンドウクラスへのポインタ
//
// このクラスへの参照が0になると自動的にメモリ上から削除される。
// 逆に言えば、インスタンスへの参照がある限りメモリ上に存在する。
typedef boost::intrusive_ptr<WindowBase> Window;

//--------------------------------------------------------------------------------
//ウィンドウの基底クラス
//--------------------------------------------------------------------------------
// ウィンドウタイプ
enum WINDOW_TYPE
{
	DEFAULT = WS_OVERLAPPEDWINDOW,	// <通常
	SIMPLE1 = WS_POPUP | WS_DLGFRAME | WS_SYSMENU | WS_CAPTION,	// <シンプル1
	SIMPLE2 = WS_OVERLAPPEDWINDOW &~WS_THICKFRAME &~WS_MAXIMIZEBOX	// <シンプル2
};

// ウィンドウ基礎クラス
// 継承してプロシージャを変更できる\n
class WindowBase
	: public IRefferenceCount<WindowBase>, boost::noncopyable
{
public:
	static Window create(
		tstring name, int width = 800, int height = 600,
		DWORD style = SIMPLE2, bool consider_frame = true);
	virtual ~WindowBase() {}

	void destroy();
	void show(bool bShow = true);

	int getWidth() const;
	int getHeight() const;
	HWND getHandle() const;

	void setSize(int x, int y, int width, int height);
	void setTitle(tstring title);

	void showCursor(bool show);

protected:
	virtual LRESULT windowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);

private:
	HWND m_hWnd;
	std::wstring m_name;
	std::wstring m_title;
	int m_width;
	int m_height;

	virtual bool initWindowClass();
	virtual bool initWindowInstance(DWORD style, bool consider_frame);

	static LRESULT CALLBACK dummyProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);

	WindowBase(
		tstring name, int width = 800, int height = 600,
		DWORD style = SIMPLE2, bool consider_frame = true);
};
//--------------------------------------------------------------------------------
//アプリケーションループの基礎クラス
//--------------------------------------------------------------------------------
// アプリケーションインタフェース
class IApplication :boost::noncopyable
{
public:
	virtual ~IApplication() {}
	void run();

protected:
	virtual void initialize() = 0;
	virtual void startMainLoop() = 0;
};

//--------------------------------------------------------------------------------
//通常ウィンドウズアプリインタフェース
//--------------------------------------------------------------------------------
// ウィンドウズアプリインタフェース
class WindowsAppBase : public IApplication
{
	typedef IApplication base;
public:
	WindowsAppBase();
	virtual ~WindowsAppBase() {}

protected:
	virtual void initialize();
	virtual void startMainLoop();

	void setWindow(Window window);
	Window getWindow();
	const Window getWindow() const;

private:
	Window m_Window;
};
//--------------------------------------------------------------------------------
//通常ゲームアプリインタフェース
//--------------------------------------------------------------------------------
// ウィンドウズゲームアプリインタフェース
class WindowsGameBase : public WindowsAppBase
{
	typedef WindowsAppBase base;
public:
	WindowsGameBase();
	virtual ~WindowsGameBase() {}

protected:
	void startMainLoop();
	virtual void initialize();
	virtual void mainProcess();
};
}
}