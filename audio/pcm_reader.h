// PCM.h
// PCM��̓N���X�w�b�_
// �ȈՐ���
// 			PCM��������͂���N���X�B�Ƃ肠��������WAVE�̂ݑΉ��B
//
// 0.3
#pragma once
#pragma warning(disable:4290)

#include <xaudio2.h>
#include <string>
#include <stdexcept>
#include "../other/refference_count.h"

// �����̃��C�u�����̖��O���
namespace yuu
{
//--------------------------------------------------------------------------------------
//PCM��͋@�C���^�t�F�[�X
//--------------------------------------------------------------------------------------
// PCM��̓C���^�t�F�[�X
//
// PCM��������͂���C���^�t�F�[�X
//
class IPCMReader
{
protected:
	WAVEFORMATEX m_wfx;			// <WAVE�t�H�[�}�b�g
	std::size_t m_data_size;		// <�f�[�^�T�C�Y

public:
	// �R���X�g���N�^
	IPCMReader();
	// �f�X�g���N�^
	virtual ~IPCMReader() {}

	// WaveFile�I�[�v��
	// �t�@�C����
	// �Ȃ�
	virtual void open(const std::string &filename) = 0;
	// �f�[�^�擾
	// �ǂݎ��o�b�t�@
	// �ǂݎ���
	// ���ۂɓǂݎ������
	// �Ȃ�
	virtual void read(void *buff, std::size_t size, std::size_t *readed) = 0;
	// �f�[�^�����̏��߂Ɉړ�
	// �Ȃ�
	virtual void seekToTop() = 0;
	// �w�蕪�����i�߂�V�[�N
	// �V�[�N�T�C�Y
	// �Ȃ�
	virtual void seek(std::size_t size) = 0;
	// EOF���o
	// �Ȃ�
	virtual bool isEOF() const = 0;
	// �f�[�^�T�C�Y�擾
	// �Ȃ�
	std::size_t getDataSize() const;
	// �t�H�[�}�b�g�擾
	// WAVEFORMATEX�ւ̃|�C���^
	// �Ȃ�
	const WAVEFORMATEX *getWaveFormat(WAVEFORMATEX *wfx = NULL) const;
};

//--------------------------------------------------------------------------------------
//Wave��̓C���^�t�F�[�X
//--------------------------------------------------------------------------------------
// Wave��̓C���^�t�F�[�X
//
// Wave��������͂���N���X
//
class WaveFileReader: public IPCMReader
{
private:
	typedef IPCMReader base;

	std::size_t m_offset_to_data;	// <�f�[�^�����܂ł̃I�t�Z�b�g�l
	std::size_t m_readed;		// <�ǂ݂������f�[�^�T�C�Y
	std::string m_filename;		// <�J���Ă���t�@�C����

	//Wave���
	void Analyze();

public:
	WaveFileReader();
	WaveFileReader(const std::string &filename);
	~WaveFileReader();

	void open(const std::string &filename);
	void read(void *buff, std::size_t size, std::size_t *readed);
	void seekToTop();
	void seek(std::size_t size);
	bool isEOF() const;
};
}