
#pragma once

#include <boost/utility.hpp>

#include <other/refference_count.h>
#include <other/utility.h>
#include <graphic/forward_declaration.h>

// �����̃��C�u�����̖��O���
namespace yuu
{
// �O���t�B�b�N�`��֘A
namespace graphic
{

enum TextureFormat{
	RGB_888, 
	ARGB_8888, 
	XRGB_8888
};

//--------------------------------------------------------------------------------------------------
//	�ʏ�e�N�X�`��
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
// �������݉\�e�N�X�`��
//--------------------------------------------------------------------------------------------------
class WritableTextureManager :public TextureManager{
public:
	static WritableTexture create(GraphicDevice device, size_t width, size_t height, TextureFormat);
	
	// �쐬���̃t�H�[�}�b�g�ɂ�������stride(1pixel�̃o�C�g��)��ݒ�
	bool write(const void* data, size_t width, size_t height, size_t stride);

private:
	WritableTextureManager(GraphicDevice device, size_t width, size_t height, TextureFormat);
};

//--------------------------------------------------------------------------------------------------
// �����_�[�^�[�Q�b�g�Ɏw��ł���e�N�X�`��
//--------------------------------------------------------------------------------------------------
class RenderTargetManager :public TextureManager{
public:
	static RenderTerget create(GraphicDevice device, size_t width, size_t height, TextureFormat);

private:
	RenderTargetManager(GraphicDevice device, size_t width, size_t height, TextureFormat);
};
}
}