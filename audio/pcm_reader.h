// PCM.h
// PCM解析クラスヘッダ
// 簡易説明
// 			PCM音源を解析するクラス。とりあえず今はWAVEのみ対応。
//
// 0.3
#pragma once
#pragma warning(disable:4290)

#include <xaudio2.h>
#include <string>
#include <stdexcept>
#include "../other/refference_count.h"

// 自分のライブラリの名前空間
namespace yuu
{
//--------------------------------------------------------------------------------------
//PCM解析機インタフェース
//--------------------------------------------------------------------------------------
// PCM解析インタフェース
//
// PCM音源を解析するインタフェース
//
class IPCMReader
{
protected:
	WAVEFORMATEX m_wfx;			// <WAVEフォーマット
	std::size_t m_data_size;		// <データサイズ

public:
	// コンストラクタ
	IPCMReader();
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
	// なし
	virtual void read(void *buff, std::size_t size, std::size_t *readed) = 0;
	// データ部分の初めに移動
	// なし
	virtual void seekToTop() = 0;
	// 指定分だけ進めるシーク
	// シークサイズ
	// なし
	virtual void seek(std::size_t size) = 0;
	// EOF検出
	// なし
	virtual bool isEOF() const = 0;
	// データサイズ取得
	// なし
	std::size_t getDataSize() const;
	// フォーマット取得
	// WAVEFORMATEXへのポインタ
	// なし
	const WAVEFORMATEX *getWaveFormat(WAVEFORMATEX *wfx = NULL) const;
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

	std::size_t m_offset_to_data;	// <データ部分までのオフセット値
	std::size_t m_readed;		// <読みだしたデータサイズ
	std::string m_filename;		// <開いているファイル名

	//Wave解析
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