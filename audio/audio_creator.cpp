// AudioCreator.h
// オーディオクラス実装
// 簡易説明
// 			XAudio2を用いた音声再生クラスの実装
//
// 0.3
#pragma warning(disable:4290)

#include <ctime>
#include <cstring>
#include <cassert>

#include <fstream>
#include <vector>
#include <memory>
#include <stdexcept>

#include <process.h>
#include <xaudio2.h>

#include "../audio/audio_creator.h"
#include "../audio/pcm_reader.h"
#include "../audio/mta_coinitiarizer.h"

// 自分のライブラリの名前空間
namespace yuu
{
// オーディオ
namespace audio
{
//--------------------------------------------------------------------------------------
//XAudio2ルートクラス
//--------------------------------------------------------------------------------------
// XAudio2ルートクラス
//
// XAudio2のエンジンとマスターボイスを管理
//
class XAudioEngine
{
	IXAudio2 *m_pXAudio2;	// <XAudio2のエンジン
	IXAudio2MasteringVoice *m_pMasteringVoice;	// <XAudio2のマスターボイス

	bool initialized;

	//生成、代入とコピー禁止
	XAudioEngine()
		: m_pXAudio2(NULL)
		, m_pMasteringVoice(NULL)
		, initialized(false)
	{
		//COMの初期化
		yuu::MTA_CoInitiarizer::Init();
	}
	XAudioEngine(const XAudioEngine &rhs);
	XAudioEngine &operator=(const XAudioEngine &rhs);

public:
	~XAudioEngine()
	{
		//マスターボイスの削除
		if(m_pMasteringVoice)
		{
			m_pMasteringVoice->DestroyVoice();
			m_pMasteringVoice = NULL;
		}
		//エンジンの削除  **** 必ずすべてのボイス削除後に消すこと ****
		if(m_pXAudio2)
		{
			m_pXAudio2->Release();
			m_pXAudio2 = NULL;
		}
	}

	// 初期化
	// なし
	void init()
	{
		if(initialized)
			return;

		//デバッグモードならデバッグエンジンを使う
		UINT32 flags = 0;

#ifdef _DEBUG
		flags |= XAUDIO2_DEBUG_ENGINE;
#endif

		//エンジンの作成
		if(FAILED(XAudio2Create(&m_pXAudio2, flags)))
			throw std::runtime_error("XAudio2の初期化に失敗しました");

		//マスターボイスの作成
		if(FAILED(m_pXAudio2->CreateMasteringVoice(&m_pMasteringVoice)))
			throw std::runtime_error("XAudio2のマスターボイス作成にに失敗しました");

		initialized = true;
	}

	// インスタンスの取得
	// このクラスのインスタンスへの参照
	static XAudioEngine &getInstance()
	{
		static XAudioEngine instance;  // 唯一のインスタンス
		return instance;
	}
	// デバイスの取得
	// XAudio2のエンジンへのポインタ
	IXAudio2 *getDevice()
	{
		return m_pXAudio2;
	}
	// 初期化されたかどうか
	bool isInitialized()
	{
		return initialized;
	}
};

//--------------------------------------------------------------------------------------
//オーディオのベース
//--------------------------------------------------------------------------------------
class XAudio2Base : public IAudio
{
	typedef std::vector<IXAudio2SourceVoice *> SourseVoiseList;
protected:
	WaveFileReader m_pcm;	// <Wave解析
	SourseVoiseList m_src_voice_list;	// <ソースボイス

	XAUDIO2_BUFFER m_xaudio_buff;// <バッファ

protected:
	// 生成、代入とコピー禁止
	// コンストラクタ
	// ソースボイスの個数
	XAudio2Base(size_t voice_num = 1)
		: m_pcm()
		, m_src_voice_list(voice_num, 0)
	{}

public:
	virtual ~XAudio2Base() {}
	virtual void stop() {};
};

//--------------------------------------------------------------------------------------
//通常のオーディオ：ソースボイス1個、ループあり、全読み込み
//--------------------------------------------------------------------------------------
// 通常のオーディオプレーヤー
//
// すべて読み込み再生する。
//
class NormalAudio : public XAudio2Base
{
	typedef XAudio2Base base;

	// 生成関数にはアクセス許可
	friend Audio CreateAudio(const tstring &, AudioType, size_t);

private:
	std::vector<char> m_buff;	// <バッファ

private:
	// コンストラクタ
	// 再生ファイル名
	// ループ可否
	// std::runtime_error("失敗理由")
	NormalAudio(const tstring &filename);

public:
	~NormalAudio();

	void start();
	void pause();
	void stop();
	void setVolume(float VolumeRate);
	void setPitch(float PitchRate);
	void setLoop(bool loop);
};
//--------------------------------------------------------------------------------------
//複数再生のオーディオ：ソースボイスn個、ループなし、全読み込み
//--------------------------------------------------------------------------------------
// 複数再生する効果音向けオーディオプレーヤー
//
// すべて読み込み再生する。指定個数だけ同時再生できる。
//
class SyncAudio : public XAudio2Base
{
	typedef XAudio2Base Super;

	//生成関数にはアクセス許可
	friend Audio CreateAudio(const tstring &, AudioType, size_t);

private:
	//バッファ
	std::vector<char> m_buff;

	int m_CurNum;
	const int m_SyncNum;

private:
	//生成、代入とコピー禁止
	// コンストラクタ
	// 再生ファイル名
	// 同時再生数
	// std::runtime_error("失敗理由")
	SyncAudio(const tstring &filename, int syncnum);

public:
	~SyncAudio();

	void start();
	void pause();
	void stop();
	void setVolume(float VolumeRate);
	void setPitch(float PitchRate);
	void setLoop(bool loop);
};
//--------------------------------------------------------------------------------------
//ストリーミングオーディオ：ソースボイス1個、ループあり、部分読み込み
//--------------------------------------------------------------------------------------
// ストリーミングオーディオプレーヤー
//
// ストリーミング再生する。
//
class StreamingAudio : public XAudio2Base
{
	typedef XAudio2Base Super;

	//生成関数にはアクセス許可
	friend Audio CreateAudio(const tstring &, AudioType, size_t);

private:
	//ストリーミングスレッド関数とハンドル
	static unsigned int __stdcall StreamingThread(void *thisptr);
	HANDLE m_hStreamingThread;
	//スレッド状態
	enum PLAYSTATE {START, STOP, END} m_state;

private:
	//非同期読み込みイベントハンドル
	class VoiceCallback;
	std::shared_ptr<VoiceCallback> m_pCallBack;
	HANDLE m_hBufferEndEvent;

private:
	//バッファ
	int m_UseBufNum;
	static const size_t BUFFER_NUM = 4;
	std::vector<char> m_buff[BUFFER_NUM];

	//ループフラグ
	bool m_loop;

private:
	// コンストラクタ
	// 再生ファイル名
	// ループ可否
	// std::runtime_error("失敗理由")
	StreamingAudio(const tstring &filename);

public:
	~StreamingAudio();

	void start();
	void pause();
	void stop();
	void setVolume(float VolumeRate);
	void setPitch(float PitchRate);
	void setLoop(bool loop);

	bool GetLoop()
	{
		return m_loop;
	}
};
//--------------------------------------------------------------------------------------
//フェード補助クラス
//--------------------------------------------------------------------------------------
// フェード補助クラス
//
// オーディオをフェードイン・アウトするのを補助。
// 音量の増減はごく単純な線形補完。
//
class FadeHelper
	: public RefferenceCount<FadeHelper>, boost::noncopyable
{
private:
	Audio m_audio;

	float m_volume;
	float m_const;
	float m_time;
	float m_pasttime;
	float m_fadetime;

	bool m_end;

	HANDLE m_hFadeThread;
	HANDLE m_hFadeEvent;

	static unsigned int __stdcall FadeThread(void *thisptr);

public:
	// コンストラクタ
	// フェードさせたいオーディオ
	// std::runtime_error("失敗理由")
	FadeHelper(Audio audio);
	// デストラクタ
	~FadeHelper();

	// 管理しているオーディオを取得
	// なし
	Audio GetManagedAudio();

	// フェードイン開始
	// フェードイン完了までの秒数。ただし厳密にこの時間では終了しない。
	// なし
	void FadeIn(float second);
	// フェードアウト開始
	// フェードアウト完了までの秒数。ただし厳密にこの時間では終了しない。
	// なし
	void FadeOut(float second);
};

//--------------------------------------------------------------------------------------
//通常のオーディオ：ソースボイス1個、ループありかなしか
//--------------------------------------------------------------------------------------
NormalAudio::NormalAudio(const tstring &filename)
	: base()
{
#ifdef _UNICODE
	//wevaを開く
	m_pcm.open(WStringToString(filename));
#elif
	m_pcm.open(filename);
#endif

	IPCMReader::Format format;
	m_pcm.getWaveFormat(&format);

	//ソースボイスの作成
	if(FAILED(XAudioEngine::getInstance().getDevice()->CreateSourceVoice(
				  &m_src_voice_list[0], (WAVEFORMATEX*)&format)))
		throw std::runtime_error("ソースボイスの作成に失敗しました");

	//データの読み取り
	
	m_buff.resize(m_pcm.getDataSize());
	size_t read =m_pcm.read(&m_buff[0], m_pcm.getDataSize());

	//XAudio用のバッファ
	std::memset(&m_xaudio_buff, 0, sizeof(m_xaudio_buff));

	m_xaudio_buff.AudioBytes = (UINT)read;
	m_xaudio_buff.pAudioData = pointer_cast<BYTE>(&m_buff[0]);
	m_xaudio_buff.Flags		 = XAUDIO2_END_OF_STREAM;
	m_xaudio_buff.LoopCount	 = 0;

	m_src_voice_list[0]->SubmitSourceBuffer(&m_xaudio_buff);
}
NormalAudio::~NormalAudio()
{
	//終了
	m_src_voice_list[0]->Stop();
	m_src_voice_list[0]->DestroyVoice();
}
void NormalAudio::start()
{
	XAUDIO2_VOICE_STATE VoiceState = {};
	m_src_voice_list[0]->GetState(&VoiceState);

	if(VoiceState.BuffersQueued <= 0)
		m_src_voice_list[0]->SubmitSourceBuffer(&m_xaudio_buff);

	m_src_voice_list[0]->Start();
}
void NormalAudio::pause()
{
	m_src_voice_list[0]->Stop();
}
void NormalAudio::stop()
{
	m_src_voice_list[0]->Stop();
	m_src_voice_list[0]->FlushSourceBuffers();
}
void NormalAudio::setVolume(float VolumeRate)
{
	m_src_voice_list[0]->SetVolume(VolumeRate);
}
void NormalAudio::setPitch(float PitchRate)
{
	m_src_voice_list[0]->SetFrequencyRatio(PitchRate);
}
void NormalAudio::setLoop(bool loop)
{
	if(!loop)
		m_src_voice_list[0]->ExitLoop();
	m_xaudio_buff.LoopCount = loop ? XAUDIO2_LOOP_INFINITE : 0;
}
//--------------------------------------------------------------------------------------
//複数再生のオーディオ：ソースボイスn個、ループなし、全読み込み
//--------------------------------------------------------------------------------------
SyncAudio::SyncAudio(const tstring &filename, int syncnum)
	: Super(syncnum),
	  m_SyncNum(syncnum),
	  m_CurNum(0)
{
#ifdef _UNICODE
	//wevaを開く
	m_pcm.open(WStringToString(filename));
#elif
	m_pcm.open(filename);
#endif

	//ソースボイスの作成
	int i = 0;
	try
	{
		IPCMReader::Format format;
		m_pcm.getWaveFormat(&format);
		for(i = 0; i < syncnum; ++i)
		{
			if(FAILED(XAudioEngine::getInstance().getDevice()->CreateSourceVoice(
						  &m_src_voice_list[i], (WAVEFORMATEX*)&format)))
				throw std::runtime_error("ソースボイスの作成に失敗しました");
		}
	}
	catch(std::exception &e)
	{
		for(int j = 0; j < i; ++j)
			m_src_voice_list[j]->DestroyVoice();

		throw e;
	}

	//データの読み取り
	m_buff.resize(m_pcm.getDataSize());
	size_t read =m_pcm.read(&m_buff[0], m_pcm.getDataSize());

	//XAudio用のバッファ
	std::memset(&m_xaudio_buff, 0, sizeof(m_xaudio_buff));

	m_xaudio_buff.AudioBytes = (UINT)read;
	m_xaudio_buff.pAudioData = pointer_cast<BYTE>(&m_buff[0]);
	m_xaudio_buff.Flags = XAUDIO2_END_OF_STREAM;

	m_src_voice_list[0]->SubmitSourceBuffer(&m_xaudio_buff);
	++m_CurNum;
}
SyncAudio::~SyncAudio()
{
	//終了
	for(int i = 0; i < m_SyncNum; ++i)
	{
		m_src_voice_list[i]->Stop();
		m_src_voice_list[i]->DestroyVoice();
	}
}
void SyncAudio::start()
{
	XAUDIO2_VOICE_STATE VoiceState = {};
	m_src_voice_list[m_CurNum % m_SyncNum]->GetState(&VoiceState);

	if(VoiceState.BuffersQueued <= 0)
		m_src_voice_list[m_CurNum % m_SyncNum]->SubmitSourceBuffer(&m_xaudio_buff);

	m_src_voice_list[m_CurNum % m_SyncNum]->Start();
	++m_CurNum;
}
void SyncAudio::pause()
{
	for(int i = 0; i < m_SyncNum; ++i)
		m_src_voice_list[i]->Stop();
}
void SyncAudio::stop()
{
	for(int i = 0; i < m_SyncNum; ++i)
		m_src_voice_list[i]->Stop();
	for(int i = 0; i < m_SyncNum; ++i)
		m_src_voice_list[i]->FlushSourceBuffers();
}
void SyncAudio::setVolume(float VolumeRate)
{
	for(int i = 0; i < m_SyncNum; ++i)
		m_src_voice_list[i]->SetVolume(VolumeRate);
}
void SyncAudio::setPitch(float PitchRate)
{
	for(int i = 0; i < m_SyncNum; ++i)
		m_src_voice_list[i]->SetFrequencyRatio(PitchRate);
}
void SyncAudio::setLoop(bool loop)
{
	UNREFERENCED_PARAMETER(loop);
}
//--------------------------------------------------------------------------------------
//ストリーミングオーディオ：ソースボイス1個、ループあり、部分読み込み
//--------------------------------------------------------------------------------------
class StreamingAudio::VoiceCallback : public IXAudio2VoiceCallback
{
	friend class StreamingAudio;

private:
	StreamingAudio &m_stream;

	//コンストラクタ
	VoiceCallback(StreamingAudio &stream)
		: m_stream(stream)
	{}

	//使用するメソッド
	void __stdcall OnBufferEnd(void *pBufferContext)
	{
		SetEvent(m_stream.m_hBufferEndEvent);
	}

	void __stdcall OnStreamEnd()
	{
#ifdef _DEBUG
		OutputDebugString(L"ストリーミングファイル末尾になりました\n");
#endif
		if(!m_stream.GetLoop())
		{
			m_stream.pause();
		}
	}

	//使用しないメソッド
	void __stdcall OnVoiceProcessingPassEnd() {}
	void __stdcall OnVoiceProcessingPassStart(UINT32 SamplesRequired) {}
	void __stdcall OnBufferStart(void *pBufferContext) {}
	void __stdcall OnLoopEnd(void *pBufferContext) {}
	void __stdcall OnVoiceError(void *pBufferContext, HRESULT Error) {}
};
StreamingAudio::StreamingAudio(const tstring &filename)
	: Super(),
	  m_hStreamingThread(NULL),
	  m_hBufferEndEvent(CreateEvent(NULL, FALSE, FALSE, NULL)),
	  m_pCallBack(NULL),
	  m_UseBufNum(0),
	  m_loop(true),
	  m_state(START)
{
	try
	{
		if(m_hBufferEndEvent == 0)
			throw std::runtime_error("ストリーミングイベントの作成に失敗しました");

		//コールバックに設定
		m_pCallBack.reset(new VoiceCallback(*this));

#ifdef _UNICODE
		//wevaを開く
		m_pcm.open(WStringToString(filename));
#elif
		m_pcm.open(filename);
#endif

		//ソースボイスの作成
		IPCMReader::Format format;
		m_pcm.getWaveFormat(&format);
		if(FAILED(XAudioEngine::getInstance().getDevice()->CreateSourceVoice(
					  &m_src_voice_list[0], (WAVEFORMATEX*)&format, 0, 8.0f, m_pCallBack.get())))
			throw std::runtime_error("ソースボイスの作成に失敗しました");

		//データ情報の読み取り
		size_t bufsize =
			format.nChannels *
			format.nSamplesPerSec *
			format.wBitsPerSample /
			8;
		//一秒間のバッファサイズ*4
		for(int i = 0; i < BUFFER_NUM; ++i)
			m_buff[i].resize(bufsize);
		std::memset(&m_xaudio_buff, 0, sizeof(m_xaudio_buff));

		//とりあえず一つ入れておく
		size_t read =m_pcm.read(&m_buff[m_UseBufNum].at(0), m_buff[m_UseBufNum].size());
		//XAudio用のバッファ
		m_xaudio_buff.AudioBytes = (UINT)read;
		m_xaudio_buff.pAudioData = pointer_cast<BYTE>(&m_buff[m_UseBufNum].at(0));
		m_xaudio_buff.Flags =
			(m_buff[m_UseBufNum].size() > read) ? XAUDIO2_END_OF_STREAM : 0;
		//キューに追加
		m_src_voice_list[0]->SubmitSourceBuffer(&m_xaudio_buff);
		++m_UseBufNum;

		//ストリーミングスレッド開始
		m_hStreamingThread = reinterpret_cast<HANDLE>(_beginthreadex(
								 NULL, 0, StreamingAudio::StreamingThread, this, 0, NULL));

		if(m_hStreamingThread == 0)
			throw std::runtime_error("ストリーミングスレッドの作成に失敗しました");
	}
	catch(std::runtime_error &e)
	{
		throw e;
	}
}
StreamingAudio::~StreamingAudio()
{
	DWORD Code;
	GetExitCodeThread(m_hStreamingThread, &Code);
	if(Code == STILL_ACTIVE)
	{
		//スレッド起動中なら終了状態にしてスレッド終了を待つ
		m_state = END;
		SetEvent(m_hBufferEndEvent);
		WaitForSingleObject(m_hStreamingThread, INFINITE);
	}

	//終了
	CloseHandle(m_hBufferEndEvent);
	CloseHandle(m_hStreamingThread);
	m_src_voice_list[0]->Stop();
	m_src_voice_list[0]->DestroyVoice();
}
unsigned int __stdcall StreamingAudio::StreamingThread(void *thisptr)
{
	StreamingAudio *_this = (StreamingAudio *)thisptr;

	//終了状態になるまでスレッドを動かす
	while(true)
	{
		//バッファ終了まで待機
		WaitForSingleObject(_this->m_hBufferEndEvent, INFINITE);

		//終了状態ならスレッドを終える
		if(_this->m_state == END)
			break;
		//停止状態なら何もしない
		else if(_this->m_state == STOP)
			continue;

		//ボイスの状態を取得してキューが満杯なら何もしない
		XAUDIO2_VOICE_STATE VoiceState = {};
		_this->m_src_voice_list[0]->GetState(&VoiceState);
		if(VoiceState.BuffersQueued >= BUFFER_NUM)
			continue;

		//バッファに残りがある限り読み取り
		while(_this->m_state != END)
		{
			//ボイスの状態を取得してキューが満杯なら脱出
			_this->m_src_voice_list[0]->GetState(&VoiceState);
			if(VoiceState.BuffersQueued >= BUFFER_NUM)
				break;

			//データの読み取り
			size_t read=_this->m_pcm.read(
				_this->m_buff[_this->m_UseBufNum].data(),
				_this->m_buff[_this->m_UseBufNum].size());

			//読み取りを最後まで行ったときは頭出し
			if(_this->m_buff[_this->m_UseBufNum].size() > read
					|| _this->m_pcm.isEOF())
			{
				//頭出し
				_this->m_pcm.seekToTop();
			}

			//読み出しが0でなかったらキューに追加
			if(read != 0)
			{
				_this->m_xaudio_buff.AudioBytes = (UINT)read;
				_this->m_xaudio_buff.pAudioData = pointer_cast<BYTE>(&_this->m_buff[_this->m_UseBufNum].at(0));
				_this->m_xaudio_buff.Flags =
					(_this->m_buff[_this->m_UseBufNum].size() > read) ? XAUDIO2_END_OF_STREAM : 0;
				_this->m_src_voice_list[0]->SubmitSourceBuffer(&_this->m_xaudio_buff);

				//バッファ切り替え
				++_this->m_UseBufNum;
				if(_this->m_UseBufNum >= BUFFER_NUM)
					_this->m_UseBufNum = 0;
			}
		}
	}
#ifdef _DEBUG
	OutputDebugString(L"**************** ストリーミングスレッドが終了しました **************** \n");
#endif
	return 0;
}
void StreamingAudio::start()
{
	//ボイスの状態を取得してキューの数を確認
	XAUDIO2_VOICE_STATE VoiceState;
	m_src_voice_list[0]->GetState(&VoiceState);

	if(VoiceState.BuffersQueued <= 0)
		SetEvent(m_hBufferEndEvent);

	m_state = START;

	m_src_voice_list[0]->Start();
}
void StreamingAudio::pause()
{
	m_src_voice_list[0]->Stop();
}
void StreamingAudio::stop()
{
	m_src_voice_list[0]->Stop();
	m_src_voice_list[0]->FlushSourceBuffers();
	m_pcm.seekToTop();
}
void StreamingAudio::setVolume(float VolumeRate)
{
	m_src_voice_list[0]->SetVolume(VolumeRate);
}
void StreamingAudio::setPitch(float PitchRate)
{
	m_src_voice_list[0]->SetFrequencyRatio(PitchRate);
}
void StreamingAudio::setLoop(bool loop)
{
	m_loop = loop;
}

//--------------------------------------------------------------------------------------
//フェードイン・フェードアウト補助クラス
//--------------------------------------------------------------------------------------
unsigned int __stdcall FadeHelper::FadeThread(void *thisptr)
{
	FadeHelper &_this = *pointer_cast<FadeHelper>(thisptr);

	while(true)
	{
		WaitForSingleObject(_this.m_hFadeEvent, INFINITE);

		if(_this.m_end)
			break;

		Sleep(15);

		_this.m_pasttime	= static_cast<float>(clock() / CLOCKS_PER_SEC) - _this.m_time ;
		_this.m_time		= static_cast<float>(clock() / CLOCKS_PER_SEC);

		_this.m_volume += _this.m_const * _this.m_pasttime;

		if(_this.m_volume > 1.0f)
		{
			_this.m_volume = 1.0f;
			ResetEvent(_this.m_hFadeEvent);

#ifdef _DEBUG
			OutputDebugString(L"**************** FadeIn Over. ***************\n");
#endif
		}
		else if(_this.m_volume < 0.0f)
		{
			_this.m_volume = 0.0f;
			_this.m_audio->pause();
			ResetEvent(_this.m_hFadeEvent);

#ifdef _DEBUG
			OutputDebugString(L"**************** FadeOut  Over. ***************\n");
#endif
		}

		_this.m_audio->setVolume(_this.m_volume);
	}

#ifdef _DEBUG
	OutputDebugString(L"**************** Fade Thread is Successfull Over. ***************\n");
#endif
	return 0;
}

FadeHelper::FadeHelper(Audio audio)
	: m_audio(audio),
	  m_volume(0),
	  m_const(0),
	  m_time(0),
	  m_pasttime(0),
	  m_fadetime(0),
	  m_end(false),
	  m_hFadeThread(NULL),
	  m_hFadeEvent(NULL)
{
	if(NULL == (m_hFadeEvent  = CreateEvent(NULL, TRUE, FALSE, NULL)))
		throw std::runtime_error("フェードイベントの作成に失敗しました");
	if(NULL == (m_hFadeThread = reinterpret_cast<HANDLE>(_beginthreadex(
									NULL, 0, FadeHelper::FadeThread, this, 0, NULL))))
		throw std::runtime_error("フェードスレッドの作成に失敗しました");
}
FadeHelper::~FadeHelper()
{
	m_end = true;
	SetEvent(m_hFadeEvent);
	WaitForSingleObject(m_hFadeThread, INFINITE);

	CloseHandle(m_hFadeEvent);
	CloseHandle(m_hFadeThread);
}

Audio FadeHelper::GetManagedAudio()
{
	return m_audio;
}
void FadeHelper::FadeIn(float second)
{
	m_volume	= 0;
	m_const		= 1.0f / second;
	m_pasttime	= 0;
	m_fadetime	= second;
	m_time		= static_cast<float>(clock() / CLOCKS_PER_SEC);

	m_audio->setVolume(m_volume);
	m_audio->start();
	SetEvent(m_hFadeEvent);
}
void FadeHelper::FadeOut(float second)
{
	m_const		= -1.0f / second;
	m_pasttime	= 0;
	m_fadetime	= second;
	m_time		= static_cast<float>(clock() / CLOCKS_PER_SEC);

	SetEvent(m_hFadeEvent);
}

//--------------------------------------------------------------------------------------
//オーディオ関連関数群
//--------------------------------------------------------------------------------------
void InitAudio()
{
	XAudioEngine::getInstance().init();
}

Audio CreateAudio(const tstring &filename, AudioType type, size_t value)
{
	//初期化がすでに行われている
	assert(XAudioEngine::getInstance().isInitialized());

	try
	{
		switch(type)
		{
		case AT_NORMAL:
			return Audio(new NormalAudio(filename));
		case AT_STREAMING:
			return Audio(new StreamingAudio(filename));
		case AT_SYNCMULTI:
			return Audio(new SyncAudio(filename, (int)value));
		default:
			return Audio(NULL);
		}
	}
	catch(...)
	{
		return Audio(NULL);
	}
}
}
}