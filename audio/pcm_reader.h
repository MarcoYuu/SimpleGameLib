// PCM.h
// PCM��̓N���X�w�b�_
// �ȈՐ���
// 			PCM��������͂���N���X�B�Ƃ肠��������WAVE�̂ݑΉ��B
//
// 0.3
#pragma once

#include <string>

// �����̃��C�u�����̖��O���
namespace yuu
{
	//--------------------------------------------------------------------------------------
	//PCM��͋@�C���^�t�F�[�X
	//--------------------------------------------------------------------------------------
	class IPCMReader
	{
	public:
		struct Format{
			unsigned short	wFormatTag;
			unsigned int	nChannels;
			unsigned int	nSamplesPerSec;
			unsigned short	nAvgBytesPerSec;
			unsigned int	nBlockAlign;
			unsigned int	wBitsPerSample;
			unsigned int	cbSize;
		};

	public:
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
		virtual size_t read(void *buff, size_t size) = 0;
		// �f�[�^�����̏��߂Ɉړ�
		// �Ȃ�
		virtual void seekToTop() = 0;
		// �w�蕪�����i�߂�V�[�N
		// �V�[�N�T�C�Y
		// �Ȃ�
		virtual void seek(size_t size) = 0;
		// EOF���o
		// �Ȃ�
		virtual bool isEOF() const = 0;
		// �f�[�^�T�C�Y�擾
		// �Ȃ�
		virtual size_t getDataSize() const =0;
		// �t�H�[�}�b�g�擾
		// WAVEFORMATEX�ւ̃|�C���^
		// �Ȃ�
		virtual void getWaveFormat(Format *format) const=0;
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

		size_t m_offset_to_data;// �f�[�^�����܂ł̃I�t�Z�b�g�l
		size_t m_readed;		// �ǂ݂������f�[�^�T�C�Y
		std::string m_filename;	// �J���Ă���t�@�C����

		Format m_format;		// WAVE�t�H�[�}�b�g
		size_t m_data_size;		// �f�[�^�T�C�Y

		//Wave���
		void Analyze();

	public:
		WaveFileReader();
		WaveFileReader(const std::string &filename);

		void open(const std::string &filename);
		size_t read(void *buff, size_t size);
		void seekToTop();
		void seek(size_t size);
		bool isEOF() const;
		size_t getDataSize() const;
		void getWaveFormat(Format *format) const;
	};
}