#include <cassert>
#include "graphics_device.h"
#include "../other/utility.h"

// 自分のライブラリの名前空間
namespace yuu
{
// グラフィック描画関連
namespace graphic
{
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
	int width, height;

	switch(size)
	{
	case WS_640x480:
		width = 640;
		height = 480;
		break;
	case WS_800x600:
		width = 800;
		height = 600;
		break;
	case WS_1024x768:
		width = 1024;
		height = 768;
		break;
	case WS_1280x960:
		width = 1280;
		height = 960;
		break;
	case WS_1920x1080:
		width = 1920;
		height = 1080;
		break;
	default:
		width = 640;
		height = 480;
		break;
	}

	GraphicDevice device = new GraphicDeviceManager();
	try
	{
		device->Init(hWnd, width, height, bWindowed, bVSync);
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