
#pragma once

#include <boost/utility.hpp>

#include <other/refference_count.h>
#include <other/utility.h>
#include <graphic/forward_declaration.h>

// 自分のライブラリの名前空間
namespace yuu
{
// グラフィック描画関連
namespace graphic
{

enum TextureFormat{
	RGB_888, 
	ARGB_8888, 
	XRGB_8888
};

//--------------------------------------------------------------------------------------------------
//	通常テクスチャ
//--------------------------------------------------------------------------------------------------
class TextureManager
	: public RefferenceCount<TextureManager>
	, boost::noncopyable
{
public:
	static Texture create(GraphicDevice device, const tstring &filename);
	virtual ~TextureManager(){}

	Size getSize();

	void* getHandle();
	const void* getHandle() const;

protected:
	TextureManager();
	TextureManager(size_t width, size_t height);
	void setSize(size_t width, size_t height);
	void setHandle(const void *handle);

private:
	TextureManager(GraphicDevice device, const tstring &filename);

	struct Param;
	Param *param;
};

//--------------------------------------------------------------------------------------------------
// 書き込み可能テクスチャ
//--------------------------------------------------------------------------------------------------
class WritableTextureManager :public TextureManager{
public:
	static WritableTexture create(GraphicDevice device, size_t width, size_t height, TextureFormat);
	
	// 作成時のフォーマットにしたがいstride(1pixelのバイト数)を設定
	bool write(const void* data, size_t width, size_t height, size_t stride);

private:
	WritableTextureManager(GraphicDevice device, size_t width, size_t height, TextureFormat);
};

//--------------------------------------------------------------------------------------------------
// レンダーターゲットに指定できるテクスチャ
//--------------------------------------------------------------------------------------------------
class RenderTargetManager :public TextureManager{
public:
	static RenderTerget create(GraphicDevice device, size_t width, size_t height, TextureFormat);

private:
	RenderTargetManager(GraphicDevice device, size_t width, size_t height, TextureFormat);
};
}
}