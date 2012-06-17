// AudioCreator.h
// �I�[�f�B�I�N���X�w�b�_
// �ȈՐ���
// 			XAudio2��p���������Đ��N���X�̃w�b�_
//
// 0.3
#pragma once

#include <boost/intrusive_ptr.hpp>
#include <boost/utility.hpp>

#include <other/refference_count.h>
#include <other/utility.h>

// �����̃��C�u�����̖��O���
namespace yuu
{
// �I�[�f�B�I
namespace audio
{
//--------------------------------------------------------------------------------------
//
//
//�I�[�f�C�I�N���X
//
//
//--------------------------------------------------------------------------------------

class IAudioBase;
typedef boost::intrusive_ptr<IAudioBase> IAudio;

//--------------------------------------------------------------------------------------
//�I�[�f�B�I�֘A�֐��Q
//--------------------------------------------------------------------------------------
//
// �I�[�f�B�I�f�o�C�X�̏�����
// �������Ɏ��s�����ꍇ��O�𓊂��܂��B
// 
void InitAudio();

//
// �I�[�f�B�I�̃^�C�v
//
enum AudioType
{
	AT_NORMAL,		// �P��̉������ׂēǂݍ��ݍĐ����܂��B������Ԃ̓��[�v���܂���B value�͖�������܂��B
	AT_STREAMING,	// �P��̉����X�g���[�~���O���Đ����܂��B������Ԃ̓��[�v���܂��Bvalue�͖�������܂��B
	AT_SYNCMULTI,	// �P��̉��𕡐������̍Đ����܂��B���[�v�s�Bvalue�͓����Đ����ł��B
};

//
// �I�[�f�B�I�̍쐬
//
// �w�肵���t�@�C�����w�肵���`���ōĐ�����Đ��@���쐬���܂��B
// �Đ��������I�[�f�B�I�̌������쐬���܂��B
// �Đ��@���j�����ꂽ���_�Ŏg�p����Ă������\�[�X�͔j������܂��B
// �w��t�@�C����������Ȃ����A�Đ��C���^�t�F�[�X�̍쐬�Ɏ��s�����Ƃ�
// ���̊֐��͎��s���A���̏ꍇ��IAudio�Ƀk���|�C���^��Ԃ��܂��B
// 
IAudio CreateAudio(const tstring &filename, AudioType type, size_t value = 1);

//--------------------------------------------------------------------------------------
//�I�[�f�B�I�̃C���^�t�F�[�X
//--------------------------------------------------------------------------------------
class IAudioBase
	: public IRefferenceCount<IAudioBase>
	, boost::noncopyable
{
public:
	virtual ~IAudioBase() {}

	// �Đ��J�n
	// �Ȃ�
	virtual void start() = 0;
	// �ꎞ��~
	// �Ȃ�
	virtual void pause() = 0;
	// ��~
	// �Ȃ�
	virtual void stop() = 0;
	// �{�����[���ύX
	// �{�����[���̃Q�C��
	// �Ȃ�
	virtual void setVolume(float VolumeRate) = 0;
	// �s�b�`�ύX
	// �s�b�`
	// �Ȃ�
	virtual void setPitch(float PitchRate) = 0;
	// ���[�v�ݒ�
	// ���[�v��
	// �Ȃ�
	virtual void setLoop(bool loop) = 0;
};
}
}