// BasewindowsApp.h
// アプリケーションクラスヘッダ

// 0.2

#pragma once

#if defined(_WIN32) | defined(_WIN64)
#include <windows.h>
#endif

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
	WT_DEFAULT,	// 通常
	WT_SIMPLE1,	// シンプル1
	WT_SIMPLE2	// シンプル2
};

// ウィンドウ基礎クラス
class WindowBase
	: public RefferenceCount<WindowBase>
	, boost::noncopyable
{
public:
	static Window create(
		tstring name, 
		int width           = 800, 
		int height          = 600,
		WindowType style    = WT_SIMPLE2, 
		bool consider_frame = true
	);
	virtual ~WindowBase();

	// ウィンドウの破棄
	void destroy();
	// ウィンドウの表示・非表示の切り替え
	void show(bool bShow = true);

	// 幅の取得
	int getWidth() const;
	// 高さの取得
	int getHeight() const;
	// ハンドルの取得
	void* getHandle() const;
	// ウィンドウ名の取得
	const tstring& getName() const;

	void setSize(int x, int y, int width, int height);
	void setName(tstring title);

	bool isActive();
	void showCursor(bool show);

protected:
#if defined(_WIN32) | defined(_WIN64)
	// プロシージャ(書き換え可能)
	virtual LRESULT callbackProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
	// オーバーライドする場合ウィンドウクラスの登録をしなければならない
	virtual bool initWindowClass();
	// 実際にウィンドウを作成し、ハンドルを登録しなければならない
	virtual bool initWindowInstance(WindowType style, bool consider_frame);
	// ハンドルの登録
	void setHandle(void* handle);
#endif

private:
	struct Param;
	Param *param;

	WindowBase(tstring name, int width, int height, WindowType style, bool consider_frame);

#if defined(_WIN32) | defined(_WIN64)
	// ウィンドウプロシージャ用のグローバル関数
	static LRESULT CALLBACK procedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
#endif	
};
}
}