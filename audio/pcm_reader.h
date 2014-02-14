// PCM.h
// PCM解析クラスヘッダ
// 簡易説明
// 			PCM音源を解析するクラス。とりあえず今はWAVEのみ対応。
//
// 0.3
#pragma once

#include <string>

// 自分のライブラリの名前空間
namespace yuu
{
	//--------------------------------------------------------------------------------------
	//PCM解析機インタフェース
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
		// デストラクタ
		virtual ~IPCMReader() {}

		// WaveFileオープン
		// ファイル名
		// なし
		virtual void open(const std::string &filename) = 0;
		// データ取得
		// 読み取りバッファ
		// 読み取り量
		// 実際に読み取った量
		virtual size_t read(void *buff, size_t size) = 0;
		// データ部分の初めに移動
		// なし
		virtual void seekToTop() = 0;
		// 指定分だけ進めるシーク
		// シークサイズ
		// なし
		virtual void seek(size_t size) = 0;
		// EOF検出
		// なし
		virtual bool isEOF() const = 0;
		// データサイズ取得
		// なし
		virtual size_t getDataSize() const =0;
		// フォーマット取得
		// WAVEFORMATEXへのポインタ
		// なし
		virtual void getWaveFormat(Format *format) const=0;
	};

	//--------------------------------------------------------------------------------------
	//Wave解析インタフェース
	//--------------------------------------------------------------------------------------
	// Wave解析インタフェース
	//
	// Wave音源を解析するクラス
	//
	class WaveFileReader: public IPCMReader
	{
	private:
		typedef IPCMReader base;

		size_t m_offset_to_data;// データ部分までのオフセット値
		size_t m_readed;		// 読みだしたデータサイズ
		std::string m_filename;	// 開いているファイル名

		Format m_format;		// WAVEフォーマット
		size_t m_data_size;		// データサイズ

		//Wave解析
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