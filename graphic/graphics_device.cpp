#include <cassert>
#include "graphics_device.h"
#include "../other/utility.h"

// 自分のライブラリの名前空間
namespace yuu
{
// グラフィック描画関連
namespace graphic
{

Size GetSize( WINDOW_SIZE size )
{
	switch(size)
	{
	case WS_320x240:	return Size(320, 240);
	case WS_640x480:	return Size(640, 480);
	case WS_800x600:	return Size(800, 600);
	case WS_1024x768:	return Size(1024, 768);
	case WS_1280x960:	return Size(1280, 960);
	case WS_1920x1080:	return Size(1920, 1080);
	default:			return Size(640, 480);
	}
}

//--------------------------------------------------------------------------------------------------
//DirectX9デバイス管理クラス
//--------------------------------------------------------------------------------------------------
GraphicDeviceManager::GraphicDeviceManager()
	: m_Direct3D9(0),
	  m_Direct3D9Device(0)
{}

GraphicDeviceManager::~GraphicDeviceManager()
{}

GraphicDevice GraphicDeviceManager::create(HWND hWnd, WINDOW_SIZE size, bool bWindowed, bool bVSync)
{
	Size back_buf_size =GetSize(size);

	GraphicDevice device = new GraphicDeviceManager();
	try
	{
		device->Init(hWnd, (int)back_buf_size.x, (int)back_buf_size.y, bWindowed, bVSync);
	}
	catch(...)
	{
		return NULL;
	}
	return device;
}

void GraphicDeviceManager::Init(HWND hWnd, int WindowWidth, int WindowHeight, bool bWindowed, bool bVSync)
{
	static bool initialized = false;

	if(initialized)
		return;

	size.set((float)WindowWidth, (float)WindowHeight);

	IDirect3DDevice9 *device = 0;
	try
	{
		//Direct3Dの初期化
		if(!(m_Direct3D9 = Direct3DCreate9(D3D_SDK_VERSION)))
			throw std::runtime_error("Direct3D9の初期化に失敗");

		//ディスプレイモードの取得
		D3DDISPLAYMODE d3ddm;
		if(FAILED(m_Direct3D9->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
			throw std::runtime_error("ディスプレイモードの取得に失敗");

		D3DPRESENT_PARAMETERS d3dpp =
		{
			static_cast<UINT>(WindowWidth),	//バックバッファ幅
			static_cast<UINT>(WindowHeight),//バックバッファ高さ
			d3ddm.Format,					//画面フォーマット情報
			1,								//バックバッファ数
			D3DMULTISAMPLE_NONE,			//マルチサンプルをどうするか
			0,								//マルチサンプルの品質
			D3DSWAPEFFECT_DISCARD,			//スワップの際の処理
			NULL,							//画面を描画するウィンドウハンドル
			bWindowed ? TRUE : FALSE,			//スクリーンモード	TRUE:ウィンドウ,FALSE:フルスクリーン
			TRUE,							//深度･ステンシルバッファの作成
			D3DFMT_D24S8,					//ステンシルバッファフォーマット
			0,								//バッファ転送時のオプションフラグ
			D3DPRESENT_RATE_DEFAULT,		//フルスクリーンでのリフレッシュレート
			bVSync ?							//スワップタイミング
			D3DPRESENT_INTERVAL_DEFAULT :
			D3DPRESENT_INTERVAL_IMMEDIATE
		};

		//デバイスの作成
		if(FAILED(m_Direct3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
											hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &device)))
			if(FAILED(m_Direct3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
												hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &device)))
				if(FAILED(m_Direct3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF,
													hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &device)))
					if(FAILED(m_Direct3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF,
														hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &device)))
						throw std::runtime_error("Direct3D9の初期化に失敗");
	}
	catch(std::runtime_error &e)
	{
		SafeRelease(device);
		throw e;
	}
	m_Direct3D9Device = device;
}

}
}