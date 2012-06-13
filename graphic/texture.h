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
	
class TextureManager;
class WritableTextureManager;
typedef boost::intrusive_ptr<TextureManager> Texture;
typedef boost::intrusive_ptr<WritableTextureManager> WritableTexture;

//--------------------------------------------------------------------------------------------------
// �e�N�X�`��
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