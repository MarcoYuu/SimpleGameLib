#include "texture.h"

// �����̃��C�u�����̖��O���
namespace yuu
{
// �O���t�B�b�N�`��֘A
namespace graphic
{
TextureManager::TextureManager(GraphicDevice device, const tstring &filename)
{
	IDirect3DTexture9 *tex;
	D3DXCreateTextureFromFile(device->device(), filename.c_str(), &tex);
	texture = tex;

	D3DSURFACE_DESC desc;
	texture->GetLevelDesc(0, &desc);
	size.x  = (float)desc.Width;
	size.y  = (float)desc.Height;
}

Texture TextureManager::create(GraphicDevice device, const tstring &filename)
{
	return Texture(new TextureManager(device, filename));
}

Size TextureManager::getSize()
{
	return size;
}

IDirect3DTexture9 *TextureManager::get()
{
	return texture.get();
}
}
}