

#include <cstring>
#include <fstream>

#include "audio_creator.h"
#include "pcm_reader.h"

// �����̃��C�u�����̖��O���
namespace yuu
{
	//--------------------------------------------------------------------------------------
	//Wave��̓C���^�t�F�[�X
	//--------------------------------------------------------------------------------------
	WaveFileReader::WaveFileReader()
		: base()
		, m_offset_to_data(0)
		, m_readed(0)
		, m_filename()
		, m_data_size(0)
	{
	}
	WaveFileReader::WaveFileReader(const std::string &filename)
		: base()
		, m_offset_to_data(0)
		, m_readed(0)
		, m_filename(filename)
		, m_data_size(0)
	{
		Analyze();
	}
	void WaveFileReader::open(const std::string &filename)
	{
		m_filename = filename;
		m_readed = 0;
		Analyze();
	}
	void WaveFileReader::Analyze()
	{
		//�ǂݍ��݃o�b�t�@
		char data[4] = {0};

		//�t�@�C�����J��
		std::ifstream ifs(m_filename.c_str(), std::ios::in | std::ios::binary);
		if(!ifs.is_open())
			throw std::runtime_error("�t�@�C�����J���̂Ɏ��s���܂����F" + m_filename);

		//RIFF�^�O�ǂݍ���
		const char RIFF[4] = {'R', 'I', 'F', 'F'};
		ifs.read(data, sizeof(data));
		if(memcmp(data, RIFF, sizeof(RIFF)) != 0)
			throw std::runtime_error("RIFF�t�@�C���ł͂���܂���F" + m_filename);
		//�t�@�C���T�C�Y�͎g��Ȃ��̂œǂݔ�΂�
		ifs.ignore(4);

		//WAVE�^�O�ǂݍ���
		const char WAVE[4] = {'W', 'A', 'V', 'E'};
		ifs.read(data, sizeof(data));
		if(memcmp(data, WAVE, sizeof(WAVE)) != 0)
			throw std::runtime_error("WAVE�t�@�C���ł͂���܂���F" + m_filename);

		//FMT_�^�O�ǂݍ���
		const char FMT_[4] = {'f', 'm', 't', ' '};
		ifs.read(data, sizeof(data));
		if(memcmp(data, FMT_, sizeof(FMT_)) != 0)
			throw std::runtime_error("�t�H�[�}�b�g���擾�ł��܂���F" + m_filename);
		//�t�H�[�}�b�g�T�C�Y�ǂݎ��
		size_t fmtsize = 0;
		ifs.read((char *)&fmtsize, sizeof(fmtsize));
		//�t�H�[�}�b�g�ǂݎ��
		ifs.read((char *)&m_format, 16);
		//�g�����ǂݔ�΂�
		ifs.ignore(fmtsize - 16);

		//���̃^�O�ǂݍ���
		const char FACT[4] = {'f', 'a', 'c', 't'};
		const char DATA[4] = {'d', 'a', 't', 'a'};
		ifs.read(data, sizeof(data));
		if(memcmp(data, FACT, sizeof(FACT)) == 0)
		{
			//fact������Ƃ��͓K���ɓǂݔ�΂�
			size_t factsize;
			ifs.read((char *)&factsize, sizeof(factsize));
			ifs.ignore(factsize);
		}
		else if(memcmp(data, DATA, sizeof(DATA)) != 0)
		{
			throw std::runtime_error("�f�[�^���擾�ł��܂���F" + m_filename);
		}

		//�g�`�f�[�^�̃T�C�Y�ǂݎ��
		ifs.read((char *)&m_data_size, sizeof(m_data_size));

		//�f�[�^�����܂ł̃I�t�Z�b�g�l�̎擾
		m_offset_to_data = (size_t)ifs.tellg();
	}
	size_t WaveFileReader::read(void *buff, size_t size)
	{
		std::ifstream ifs(m_filename.c_str(), std::ios::in | std::ios::binary);
		ifs.seekg(m_offset_to_data + m_readed);
		ifs.read((char *)buff, size);
		m_readed += (size_t)ifs.gcount();
		return m_readed;
	}
	void WaveFileReader::seekToTop()
	{
		m_readed = 0;
	}
	void WaveFileReader::seek(size_t size)
	{
		size_t tempsize = size;
		if(tempsize > m_data_size - m_readed)
			tempsize = m_data_size - m_readed;

		m_readed += tempsize;
	}
	bool WaveFileReader::isEOF() const
	{
		std::ifstream ifs(m_filename.c_str(), std::ios::in | std::ios::binary);
		ifs.seekg(m_offset_to_data + m_readed);
		return ifs.eof();
	}

	size_t WaveFileReader::getDataSize() const
	{
		return m_data_size;
	}

	void WaveFileReader::getWaveFormat( Format *format ) const
	{
		*format =m_format;
	}
}