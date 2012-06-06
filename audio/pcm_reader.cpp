#include <cstring>
#include <fstream>
#include "pcm_reader.h"

// 自分のライブラリの名前空間
namespace yuu
{
//--------------------------------------------------------------------------------------
//解析機
//--------------------------------------------------------------------------------------
IPCMReader::IPCMReader()
	: m_data_size(0)
{}

unsigned int IPCMReader::getDataSize() const
{
	return m_data_size;
}

const WAVEFORMATEX *IPCMReader::getWaveFormat(WAVEFORMATEX *wfx) const
{
	if(wfx != NULL)
		*wfx = m_wfx;

	return &m_wfx;
}
//--------------------------------------------------------------------------------------
//Wave解析インタフェース
//--------------------------------------------------------------------------------------
WaveFileReader::WaveFileReader()
	: base()
	, m_offset_to_data(0)
	, m_readed(0)
	, m_filename()
{
}
WaveFileReader::WaveFileReader(const std::string &filename)
	: base()
	, m_offset_to_data(0)
	, m_readed(0)
	, m_filename(filename)
{
	Analyze();
}
WaveFileReader::~WaveFileReader()
{
}
void WaveFileReader::open(const std::string &filename)
{
	m_filename = filename;
	m_readed = 0;
	Analyze();
}
void WaveFileReader::Analyze()
{
	//読み込みバッファ
	char data[4] = {0};

	//ファイルを開く
	std::ifstream ifs(m_filename.c_str(), std::ios::in | std::ios::binary);
	if(!ifs.is_open())
		throw std::runtime_error("ファイルを開くのに失敗しました：" + m_filename);

	//RIFFタグ読み込み
	const char RIFF[4] = {'R', 'I', 'F', 'F'};
	ifs.read(data, sizeof(data));
	if(memcmp(data, RIFF, sizeof(RIFF)) != 0)
		throw std::runtime_error("RIFFファイルではありません：" + m_filename);
	//ファイルサイズは使わないので読み飛ばし
	ifs.ignore(4);

	//WAVEタグ読み込み
	const char WAVE[4] = {'W', 'A', 'V', 'E'};
	ifs.read(data, sizeof(data));
	if(memcmp(data, WAVE, sizeof(WAVE)) != 0)
		throw std::runtime_error("WAVEファイルではありません：" + m_filename);

	//FMT_タグ読み込み
	const char FMT_[4] = {'f', 'm', 't', ' '};
	ifs.read(data, sizeof(data));
	if(memcmp(data, FMT_, sizeof(FMT_)) != 0)
		throw std::runtime_error("フォーマットが取得できません：" + m_filename);
	//フォーマットサイズ読み取り
	std::size_t fmtsize = 0;
	ifs.read((char *)&fmtsize, sizeof(fmtsize));
	//フォーマット読み取り
	ifs.read((char *)&m_wfx, 16);
	//拡張情報読み飛ばし
	ifs.ignore(fmtsize - 16);

	//次のタグ読み込み
	const char FACT[4] = {'f', 'a', 'c', 't'};
	const char DATA[4] = {'d', 'a', 't', 'a'};
	ifs.read(data, sizeof(data));
	if(memcmp(data, FACT, sizeof(FACT)) == 0)
	{
		//factがあるときは適当に読み飛ばす
		std::size_t factsize;
		ifs.read((char *)&factsize, sizeof(factsize));
		ifs.ignore(factsize);
	}
	else if(memcmp(data, DATA, sizeof(DATA)) != 0)
	{
		throw std::runtime_error("データが取得できません：" + m_filename);
	}

	//波形データのサイズ読み取り
	ifs.read((char *)&m_data_size, sizeof(m_data_size));

	//データ部分までのオフセット値の取得
	m_offset_to_data = (std::size_t)ifs.tellg();
}
void WaveFileReader::read(void *buff, std::size_t size, std::size_t *readed)
{
	std::ifstream ifs(m_filename.c_str(), std::ios::in | std::ios::binary);
	ifs.seekg(m_offset_to_data + m_readed);
	ifs.read((char *)buff, size);
	*readed = (size_t)ifs.gcount();
	m_readed += *readed;
}
void WaveFileReader::seekToTop()
{
	m_readed = 0;
}
void WaveFileReader::seek(std::size_t size)
{
	std::size_t tempsize = size;
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
}