
#include <d3d9.h>
#include <d3dx9.h>

#include <graphic/texture.h>
#include <graphic/graphics_device.h>

// �����̃��C�u�����̖��O���
namespace yuu
{
// �O���t�B�b�N�`��֘A
namespace graphic
{

struct TextureManager::Param{
	boost::intrusive_ptr<IDirect3DTexture9> texture;
	Size size;
};

TextureManager::TextureManager(GraphicDevice device, const tstring &filename)
	:param(new Param())
{
	IDirect3DTexture9 *tex;
	D3DXCreateTextureFromFile(
		(IDirect3DDevice9*)device->getHandle(), filename.c_str(), &tex);
	param->texture = tex;

	D3DSURFACE_DESC desc;
	param->texture->GetLevelDesc(0, &desc);
	param->size.x  = (float)desc.Width;
	param->size.y  = (float)desc.Height;
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

}
}