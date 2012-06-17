// AudioCreator.h
// オーディオクラスヘッダ
// 簡易説明
// 			XAudio2を用いた音声再生クラスのヘッダ
//
// 0.3
#pragma once

#include <boost/intrusive_ptr.hpp>
#include <boost/utility.hpp>

#include <other/refference_count.h>
#include <other/utility.h>

// 自分のライブラリの名前空間
namespace yuu
{
// オーディオ
namespace audio
{
//--------------------------------------------------------------------------------------
//
//
//オーデイオクラス
//
//
//--------------------------------------------------------------------------------------

class IAudioBase;
typedef boost::intrusive_ptr<IAudioBase> IAudio;

//--------------------------------------------------------------------------------------
//オーディオ関連関数群
//--------------------------------------------------------------------------------------
//
// オーディオデバイスの初期化
// 初期化に失敗した場合例外を投げます。
// 
void InitAudio();

//
// オーディオのタイプ
//
enum AudioType
{
	AT_NORMAL,		// 単一の音をすべて読み込み再生します。初期状態はループしません。 valueは無視されます。
	AT_STREAMING,	// 単一の音をストリーミングしつつ再生します。初期状態はループします。valueは無視されます。
	AT_SYNCMULTI,	// 単一の音を複数同時の再生します。ループ不可。valueは同時再生数です。
};

//
// オーディオの作成
//
// 指定したファイルを指定した形式で再生する再生機を作成します。
// 再生したいオーディオの個数だけ作成します。
// 再生機が破棄された時点で使用されていたリソースは破棄されます。
// 指定ファイルが見つからないか、再生インタフェースの作成に失敗したとき
// この関数は失敗し、その場合はIAudioにヌルポインタを返します。
// 
IAudio CreateAudio(const tstring &filename, AudioType type, size_t value = 1);

//--------------------------------------------------------------------------------------
//オーディオのインタフェース
//--------------------------------------------------------------------------------------
class IAudioBase
	: public IRefferenceCount<IAudioBase>
	, boost::noncopyable
{
public:
	virtual ~IAudioBase() {}

	// 再生開始
	// なし
	virtual void start() = 0;
	// 一時停止
	// なし
	virtual void pause() = 0;
	// 停止
	// なし
	virtual void stop() = 0;
	// ボリューム変更
	// ボリュームのゲイン
	// なし
	virtual void setVolume(float VolumeRate) = 0;
	// ピッチ変更
	// ピッチ
	// なし
	virtual void setPitch(float PitchRate) = 0;
	// ループ設定
	// ループ可否
	// なし
	virtual void setLoop(bool loop) = 0;
};
}
}