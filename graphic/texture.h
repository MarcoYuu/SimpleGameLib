
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

	void* getHandle();
	const void* getHandle() const;

protected:
	TextureManager(){};
	TextureManager(GraphicDevice device, const tstring &filename);

	struct Param;
	Param *param;
};
}
}