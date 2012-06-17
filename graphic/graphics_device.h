#pragma once

#include <boost/utility.hpp>

#include <app/windows_window_base.h>
#include <other/refference_count.h>
#include <other/utility.h>
#include <graphic/forward_declaration.h>

// 自分のライブラリの名前空間
namespace yuu
{
// グラフィック描画関連
namespace graphic
{
//--------------------------------------------------------------------------------------------------
//DirectX9デバイス管理クラス
//--------------------------------------------------------------------------------------------------
struct Rectangle{
	long x1, y1;
	long x2, y2;

	Rectangle();
	Rectangle(const Point &p1, const Point &p2);
	Rectangle(long x1_, long y1_, long x2_, long y2_);

	long width() const{return x2-x1;}
	long height() const{return y2-y1;}
};

enum BackBufferSize
{
	WS_320x240,
	WS_640x480,
	WS_800x600,
	WS_1024x768,
	WS_1280x960,
	WS_1920x1080
};
Size GetSize(BackBufferSize);

enum CullingMode
{
	NONE, 
	RIGHT_HANDED, 
	LEFT_HANDED
};

enum TextureTransformMode{
	DISABLE,
	COORD_1D,
	COORD_2D,
	COORD_3D,
	COORD_4D,
	PROJECTED
};

enum TextureAddressingMode{
	WRAP,
	MIRROR,
	CLAMP,
	BORDER,
	MIRROR_ONCE
};

enum ClearTarget{
	RENDERING_TARGET =0x01,
	STENCIL_BUFFER   =0x02,
	Z_BUFFER         =0x04,

	RENDER_STENCIL   =RENDERING_TARGET|STENCIL_BUFFER,
	RENDER_Z         =RENDERING_TARGET|Z_BUFFER,
	Z_STENCIL        =Z_BUFFER|STENCIL_BUFFER,
	ALL_TARGET       =RENDERING_TARGET|Z_STENCIL
};

class GraphicDeviceManager
	: public RefferenceCount<GraphicDeviceManager>
	, boost::noncopyable
{
public:
	static GraphicDevice create(
		app::Window window, 
		BackBufferSize size = WS_800x600, 
		bool bWindowed      = true, 
		bool bVSync         = true
	);
	~GraphicDeviceManager();

	// バックバッファのサイズを取得
	const Size& getBackBufferSize() const;

	// ライティングを有効化
	void enableLighting(bool enable);
	// テクスチャ変換を有効化
	void enableTextureTransform(int stage, TextureTransformMode);
	
	// カリングの設定
	void setCullingMode(CullingMode mode);
	// テクスチャアドレッシングの設定
	void setTextureAddressingMode(int stage, TextureAddressingMode u, TextureAddressingMode v);
	// 頂点バッファの設定
	void setVertexBuffer(VertexBuffer buff, int stream, int offset, int stride);
	// 頂点インデックスを設定
	void setIndexBuffer(IndexBuffer buff);

	// 描画の開始
	void begin();
	// 描画の終了
	void end();
	// 画面のフリップ
	int present();

	// レンダーターゲットの変更
	void setRenderTarget(RenderTerget target, int index);
	void resetRenderTargetToBackBuffer();

	// バッファをクリア
	void clear();
	void clear(Color color);
	void clear(ClearTarget target, Color color);
	void clear(ClearTarget target, Color color, const Rectangle *area, int rect_num);
	void clear(ClearTarget target, Color color, const Rectangle *area, int rect_num, float z, int stencil);
	
	// 内部のハンドルを取得
	void* getHandle();
	const void* getHandle() const;

private:
	struct Param;
	Param *param;

	GraphicDeviceManager();
	void Init(app::Window window, 
		int WindowWidth, int WindowHeight, 
		bool bWindowed, bool bVSync);
};
}
}