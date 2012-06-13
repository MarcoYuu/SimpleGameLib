#pragma once

#include <d3d9.h>
#include <d3dx9.h>

#include <boost/intrusive_ptr.hpp>
#include <boost/utility.hpp>

#include "../other/refference_count.h"
#include "graphics_device.h"

// 自分のライブラリの名前空間
namespace yuu
{
// グラフィック描画関連
namespace graphic
{
	
class TextureManager;
class WritableTextureManager;
typedef boost::intrusive_ptr<TextureManager> Texture;
typedef boost::intrusive_ptr<WritableTextureManager> WritableTexture;

//--------------------------------------------------------------------------------------------------
// テクスチャ
//--------------------------------------------------------------------------------------------------

class TextureManager
	: public IRefferenceCount<TextureManager>
	, boost::noncopyable
{
public:
	static Texture create(GraphicDevice device, const tstring &filename);
	virtual ~TextureManager(){}

	Size getSize();
	IDirect3DTexture9 *get();

protected:
	TextureManager(){};
	TextureManager(GraphicDevice device, const tstring &filename);

	boost::intrusive_ptr<IDirect3DTexture9> texture;
	Size size;
};
}
}