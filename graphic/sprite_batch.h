
#pragma once

#include <boost/utility.hpp>

#include <other/refference_count.h>
#include <graphic/forward_declaration.h>

// �����̃��C�u�����̖��O���
namespace yuu
{
// �O���t�B�b�N�`��֘A
namespace graphic
{
//--------------------------------------------------------------------------------------------------
// �X�v���C�g�o�b�`
//--------------------------------------------------------------------------------------------------
class SpriteBatchSystem
	: public IRefferenceCount<SpriteBatchSystem>
	, boost::noncopyable
{
public:
	static SpriteBatch create(GraphicDevice device, size_t size);
	~SpriteBatchSystem() {}

	void begin();
	void end();
	void setTexture(Texture texture);

	void draw(const Point2f &position);
	void draw(const Point2f &position, const Color &color);
	void draw(const Point2f &position, const Color &color, float scale, float rotation);
	void draw(const Point2f &position, const Color &color, float scale, float rotation, float depth);

	enum ColorBlendMode
	{
		BLEND_NOUSE_ALPHA,	// �A���t�@�u�����h��؂�܂�
		BLEND_DEFAULT,		// �ʏ피�����������܂�
		BLEND_MODURATE,		// ��Z�������܂�
		BLEND_INVERSE,		// ���F�̃e�N�X�`�����ォ��d�˂邱�ƂŐF�𔽓]���܂�
		BLEND_ADDITION1,	// ���Z�������܂�
		BLEND_ADDITION2,	// ���Z�������܂� @attention �񐄏�
		BLEND_SUBTRACT1,	// ���Z���܂� @attention �񐄏�
		BLEND_SUBTRACT2		// ���Z���܂� @attention �񐄏�
	};
	void blendMode(ColorBlendMode mode);

private:
	SpriteBatchSystem(GraphicDevice device, size_t max_sprite);
	void setIndices();
	void createEffect();

private:
	struct Param;
	Param *param;
};
}
}