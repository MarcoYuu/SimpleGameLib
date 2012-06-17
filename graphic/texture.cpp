
#include <d3d9.h>
#include <d3dx9.h>

#include <graphic/texture.h>
#include <graphic/graphics_device.h>

#include <other/com_release.h>

// 自分のライブラリの名前空間
namespace yuu
{
// グラフィック描画関連
namespace graphic
{

//--------------------------------------------------------------------------------------------------
// 通常テクスチャ
//--------------------------------------------------------------------------------------------------
struct TextureManager::Param{
	boost::intrusive_ptr<IDirect3DTexture9> texture;
	Size size;

	Param() 
		: texture()
		, size() 
	{}
	Param(IDirect3DTexture9 *p, size_t w, size_t h)
		: texture(p)
		, size(w, h)
	{}
};

TextureManager::TextureManager()
	:param(new Param())
{

}

TextureManager::TextureManager( size_t width, size_t height )
	:param(new Param(NULL, width, height))
{

}

TextureManager::TextureManager(GraphicDevice device, const tstring &filename)
	:param(new Param())
{
	IDirect3DTexture9 *tex;
	D3DXCreateTextureFromFile(
		(IDirect3DDevice9*)device->getHandle(), filename.c_str(), &tex);
	param->texture = tex;

	D3DSURFACE_DESC desc;
	param->texture->GetLevelDesc(0, &desc);
	param->size.x  = desc.Width;
	param->size.y  = desc.Height;
}

Texture TextureManager::create(GraphicDevice device, const tstring &filename)
{
	return Texture(new TextureManager(device, filename));
}

Size TextureManager::getSize()
{
	return param->size;
}

void* TextureManager::getHandle()
{
	return param->texture.get();
}

const void* TextureManager::getHandle() const
{
	return param->texture.get();
}

void TextureManager::setSize( size_t width, size_t height )
{
	param->size.set(width, height);
}

void TextureManager::setHandle( const void *handle )
{
	param->texture.reset((IDirect3DTexture9*)handle);
}

//--------------------------------------------------------------------------------------------------
// 書き込み可能テクスチャ
//--------------------------------------------------------------------------------------------------
static D3DFORMAT ConvertFormatToDirectX(TextureFormat format){
	switch(format){
	case RGB_888:	return D3DFMT_R8G8B8;
	case ARGB_8888: return D3DFMT_A8R8G8B8;
	case XRGB_8888: return D3DFMT_X8R8G8B8;
	default:		return D3DFMT_A8R8G8B8;
	}
}
WritableTextureManager::WritableTextureManager( GraphicDevice device, size_t width, size_t height, TextureFormat f )
	: TextureManager(width, height)
{
	IDirect3DDevice9 *dev =(IDirect3DDevice9*)device->getHandle();
	IDirect3DTexture9 *tex;

	// 動的テクスチャ作成、デバイスロストで消失する
	dev->CreateTexture((UINT)width, (UINT)height, 1, 
		D3DUSAGE_DYNAMIC, ConvertFormatToDirectX(f), D3DPOOL_DEFAULT, &tex, NULL);

	setHandle(tex);
}

WritableTexture WritableTextureManager::create( GraphicDevice device, size_t width, size_t height, TextureFormat f )
{
	return WritableTexture(new WritableTextureManager(device, width, height, f));
}

bool WritableTextureManager::write( const void* data, size_t width, size_t height, size_t stride )
{
	IDirect3DTexture9 *tex =(IDirect3DTexture9*)getHandle();
	D3DLOCKED_RECT locked_rect;

	// 書き込み領域のロック
	if(FAILED(tex->LockRect(0, &locked_rect, NULL, D3DLOCK_DISCARD))){
		if(FAILED(tex->LockRect(0, &locked_rect, NULL, 0))){
			return false;
		}
	}
	memcpy(locked_rect.pBits, data, width*height*stride);
	tex->UnlockRect(0);
	return true;
}

//--------------------------------------------------------------------------------------------------
// レンダーターゲットに指定できるテクスチャ
//--------------------------------------------------------------------------------------------------
RenderTargetManager::RenderTargetManager( 
	GraphicDevice device, size_t width, size_t height, TextureFormat f )
	:TextureManager(width, height)
{
	IDirect3DDevice9 *dev =(IDirect3DDevice9*)device->getHandle();
	IDirect3DTexture9 *tex;

	// 動的テクスチャ作成、デバイスロストで消失する
	dev->CreateTexture((UINT)width, (UINT)height, 1, 
		D3DUSAGE_RENDERTARGET, ConvertFormatToDirectX(f), D3DPOOL_DEFAULT, &tex, NULL);

	setHandle(tex);
}

RenderTerget RenderTargetManager::create( 
	GraphicDevice device, size_t width, size_t height, TextureFormat f )
{
	return RenderTerget(new RenderTargetManager(device, width, height, f));
}

}
}