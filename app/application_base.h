#pragma once

#include <boost/intrusive_ptr.hpp>
#include <boost/utility.hpp>

#include <app/windows_window_base.h>
#include <other/refference_count.h>
#include <other/utility.h>

// 自分のライブラリの名前空間
namespace yuu
{
// アプリケーション
namespace app
{

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