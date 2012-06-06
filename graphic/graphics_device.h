#pragma once

#pragma warning(disable:4290)

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include <d3d9.h>

#include <boost/intrusive_ptr.hpp>
#include <boost/utility.hpp>

#include "../other/refference_count.h"
#include "../other/utility.h"
#include "../other/geometry.h"

// 自分のライブラリの名前空間
namespace yuu
{
// グラフィック描画関連
namespace graphic
{
//--------------------------------------------------------------------------------------------------
// 形状
//--------------------------------------------------------------------------------------------------
struct Rectangle
{
	Point2f point[2];
};

struct Tetragon
{
	Point2f point[4];
};

typedef Vector2f Size;

//--------------------------------------------------------------------------------------------------
//DirectX9デバイス管理クラス
//--------------------------------------------------------------------------------------------------
enum WINDOW_SIZE
{
	WS_640x480,
	WS_800x600,
	WS_1024x768,
	WS_1280x960,
	WS_1920x1080
};
Size GetSize(WINDOW_SIZE);

class GraphicDeviceManager;
typedef boost::intrusive_ptr<GraphicDeviceManager> GraphicDevice;

class GraphicDeviceManager
	: public IRefferenceCount<GraphicDeviceManager>
	, boost::noncopyable
{
public:
	static GraphicDevice create(HWND hWnd, WINDOW_SIZE size = WS_800x600, bool bWindowed = true, bool bVSync = true);
	~GraphicDeviceManager();

	IDirect3DDevice9 *device()
	{
		return m_Direct3D9Device.get();
	}
	Size getBackBufferSize() const
	{
		return size;
	}

private:
	boost::intrusive_ptr<IDirect3D9> m_Direct3D9;
	boost::intrusive_ptr<IDirect3DDevice9> m_Direct3D9Device;

	Size size;

	GraphicDeviceManager();
	void Init(HWND hWnd, int WindowWidth, int WindowHeight, bool bWindowed, bool bVSync);
};
}
}