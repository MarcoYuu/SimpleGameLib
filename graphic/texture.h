#pragma once

#include <d3d9.h>
#include <d3dx9.h>

#include <boost/intrusive_ptr.hpp>
#include <boost/utility.hpp>

#include "../other/refference_count.h"
#include "graphics_device.h"

// �����̃��C�u�����̖��O���
namespace yuu
{
// �O���t�B�b�N�`��֘A
namespace graphic
{
//--------------------------------------------------------------------------------------------------
// �e�N�X�`��
//--------------------------------------------------------------------------------------------------
class TextureManager;
typedef boost::intrusive_ptr<TextureManager> Texture;

class TextureManager
	: public IRefferenceCount<TextureManager>
	, boost::noncopyable
{
public:
	static Texture create(GraphicDevice device, const tstring &filename);
	~TextureManager()
	{}

	Size getSize();
	IDirect3DTexture9 *get();

private:
	boost::intrusive_ptr<IDirect3DTexture9> texture;
	Size size;

	TextureManager(GraphicDevice device, const tstring &filename);
};
}
}