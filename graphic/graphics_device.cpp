
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include <d3d9.h>

#include <cassert>

#include <graphic/graphics_device.h>
#include <graphic/buffer_object.h>
#include <graphic/texture.h>
#include <graphic/color.h>
#include <other/utility.h>

#include <other/com_release.h>

// 自分のライブラリの名前空間
namespace yuu
{
// グラフィック描画関連
namespace graphic
{

Rectangle::Rectangle() 
	: x1(0) 
	, y1(0) 
	, x2(0) 
	, y2(0)
{}

Rectangle::Rectangle( const Point &p1, const Point &p2 ) 
	: x1(p1.x)
	, y1(p2.y)
	, x2(p1.x)
	, y2(p2.y)
{}

Rectangle::Rectangle( long x1_, long y1_, long x2_, long y2_ ) 
	: x1(x1_)
	, y1(y1_)
	, x2(x2_)
	, y2(y2_)
{}

using app::Window;

Size GetSize( BackBufferSize size )
{
	switch(size){
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
struct GraphicDeviceManager::Param{

	boost::intrusive_ptr<IDirect3D9> direct3D9;
	boost::intrusive_ptr<IDirect3DDevice9> direct3D9Device;
	boost::intrusive_ptr<IDirect3DSurface9> backBuffer;

	Size size;
	bool initialized;

	Param()
		: direct3D9(0)
		, direct3D9Device(0)
		, backBuffer(0)
		, size()
		, initialized(false)
	{}
};

GraphicDeviceManager::GraphicDeviceManager()
	: param(new Param())
{}

GraphicDeviceManager::~GraphicDeviceManager()
{
	delete param;
}

GraphicDevice GraphicDeviceManager::create(Window window, BackBufferSize size, bool bWindowed, bool bVSync)
{
	GraphicDevice device(new GraphicDeviceManager());
	try
	{
		Size back_buf_size =GetSize(size);
		device->Init(window, (int)back_buf_size.x, (int)back_buf_size.y, bWindowed, bVSync);
	}
	catch(...)
	{
		return NULL;
	}
	return device;
}

void GraphicDeviceManager::Init(Window window, int WindowWidth, int WindowHeight, bool bWindowed, bool bVSync)
{
	if(param->initialized)
		return;

	param->size.set(WindowWidth, WindowHeight);

	IDirect3DDevice9 *device = NULL;
	try
	{
		//Direct3Dの初期化
		if(!(param->direct3D9 = Direct3DCreate9(D3D_SDK_VERSION)))
			throw std::runtime_error("Direct3D9の初期化に失敗");

		//ディスプレイモードの取得
		D3DDISPLAYMODE d3ddm;
		if(FAILED(param->direct3D9->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
			throw std::runtime_error("ディスプレイモードの取得に失敗");

		D3DPRESENT_PARAMETERS d3dpp =
		{
			static_cast<UINT>(WindowWidth),	// バックバッファ幅
			static_cast<UINT>(WindowHeight),// バックバッファ高さ
			d3ddm.Format,					// 画面フォーマット情報
			1,								// バックバッファ数
			D3DMULTISAMPLE_NONE,			// マルチサンプルをどうするか
			0,								// マルチサンプルの品質
			D3DSWAPEFFECT_DISCARD,			// スワップの際の処理
			NULL,							// 画面を描画するウィンドウハンドル
			bWindowed ? TRUE : FALSE,		// スクリーンモード	TRUE:ウィンドウ,FALSE:フルスクリーン
			TRUE,							// 深度･ステンシルバッファの作成
			D3DFMT_D24S8,					// ステンシルバッファフォーマット
			0,								// バッファ転送時のオプションフラグ
			D3DPRESENT_RATE_DEFAULT,		// フルスクリーンでのリフレッシュレート
			bVSync ?						// スワップタイミング
			D3DPRESENT_INTERVAL_DEFAULT :
			D3DPRESENT_INTERVAL_IMMEDIATE
		};

		//デバイスの作成
		if(FAILED(param->direct3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
											(HWND)window->getHandle(), D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &device)))
			if(FAILED(param->direct3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
												(HWND)window->getHandle(), D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &device)))
				if(FAILED(param->direct3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF,
													(HWND)window->getHandle(), D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &device)))
					if(FAILED(param->direct3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF,
														(HWND)window->getHandle(), D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &device)))
						throw std::runtime_error("Direct3D9の初期化に失敗");
	}
	catch(std::runtime_error &e)
	{
		SafeRelease(device);
		throw e;
	}
	param->direct3D9Device = device;

	// バックバッファへの参照の獲得
	IDirect3DSurface9* back =NULL;
	if(FAILED(param->direct3D9Device->GetRenderTarget(0, &back))){
		throw std::runtime_error("バックバッファへの参照の獲得に失敗");
	}
	param->backBuffer =back;

	// 初期化完了
	param->initialized =true;
}

const Size& GraphicDeviceManager::getBackBufferSize() const
{
	return param->size;
}

void GraphicDeviceManager::enableLighting( bool enable )
{
	param->direct3D9Device->SetRenderState(D3DRS_LIGHTING, 
		enable? TRUE : FALSE);
}

void GraphicDeviceManager::setCullingMode( CullingMode mode)
{
	param->direct3D9Device->SetRenderState(D3DRS_CULLMODE, 
		(mode==NONE)?			D3DCULL_NONE: 
		(mode==RIGHT_HANDED)?	RIGHT_HANDED:
		LEFT_HANDED);
}

void GraphicDeviceManager::enableTextureTransform( int stage, TextureTransformMode coord)
{
	int flag;
	switch(coord){
	case DISABLE:	flag =D3DTTFF_DISABLE;
	case COORD_1D:	flag =D3DTTFF_COUNT1;
	case COORD_2D:	flag =D3DTTFF_COUNT2;
	case COORD_3D:	flag =D3DTTFF_COUNT3;
	case COORD_4D:	flag =D3DTTFF_COUNT4;
	case PROJECTED:	flag =D3DTTFF_PROJECTED | D3DTTFF_COUNT3;
	default:		flag =D3DTTFF_DISABLE;
	}
	param->direct3D9Device->SetTextureStageState(stage, 
		D3DTSS_TEXTURETRANSFORMFLAGS, flag);
}

int getAddressingMode(TextureAddressingMode mode){
	switch(mode){
	case WRAP:			return D3DTADDRESS_WRAP;
	case MIRROR:		return D3DTADDRESS_MIRROR;
	case CLAMP:			return D3DTADDRESS_CLAMP;
	case BORDER:		return D3DTADDRESS_BORDER;
	case MIRROR_ONCE:	return D3DTADDRESS_MIRRORONCE;
	default:			return D3DTADDRESS_WRAP;
	}
}
void GraphicDeviceManager::setTextureAddressingMode( int stage, TextureAddressingMode u, TextureAddressingMode v )
{
	param->direct3D9Device->SetSamplerState(stage, D3DSAMP_ADDRESSU, getAddressingMode(u));
	param->direct3D9Device->SetSamplerState(stage, D3DSAMP_ADDRESSV, getAddressingMode(v));
}

void GraphicDeviceManager::setVertexBuffer( VertexBuffer buff, int stream, int offset, int stride )
{
	param->direct3D9Device->SetStreamSource(stream, (IDirect3DVertexBuffer9*)buff->getHandle(), offset, stride);
}

void GraphicDeviceManager::setIndexBuffer( IndexBuffer buff )
{
	param->direct3D9Device->SetIndices((IDirect3DIndexBuffer9*)buff->getHandle());
}

void GraphicDeviceManager::begin()
{
	param->direct3D9Device->BeginScene();
}

void GraphicDeviceManager::end()
{
	param->direct3D9Device->EndScene();
}

int GraphicDeviceManager::present()
{
	return param->direct3D9Device->Present(NULL, NULL, NULL, NULL);
}

void GraphicDeviceManager::clear()
{
	clear(Color::White);
}

void GraphicDeviceManager::clear( Color color )
{
	clear(ALL_TARGET, color);
}

void GraphicDeviceManager::clear( ClearTarget target, Color color )
{
	clear(target, color, NULL, 0);
}

void GraphicDeviceManager::clear( ClearTarget target, Color color, const Rectangle *area, int rect_num )
{
	clear(target, color, area, rect_num, 1.0f, 0);
}

void GraphicDeviceManager::clear( ClearTarget target, Color color, const Rectangle *area, int rect_num, float z, int stencil )
{
	DWORD flag =0;
	RENDERING_TARGET & target	? flag |=RENDERING_TARGET:0;
	STENCIL_BUFFER & target		? flag |=STENCIL_BUFFER:0;
	Z_BUFFER & target			? flag |=Z_BUFFER:0;
	param->direct3D9Device->Clear(rect_num, (D3DRECT*)area, flag, color, z, stencil);
}

void* GraphicDeviceManager::getHandle()
{
	return param->direct3D9Device.get();
}

const void* GraphicDeviceManager::getHandle() const
{
	return param->direct3D9Device.get();
}

void GraphicDeviceManager::setRenderTarget( RenderTerget target, int index )
{
	IDirect3DTexture9* tex =(IDirect3DTexture9*)target->getHandle();

	IDirect3DSurface9* render =NULL;
	tex->GetSurfaceLevel(0, &render);
	param->direct3D9Device->SetRenderTarget(index, render);
	SafeRelease(render);
}

void GraphicDeviceManager::resetRenderTargetToBackBuffer()
{
	param->direct3D9Device->SetRenderTarget(0, param->backBuffer.get());
}





}
}