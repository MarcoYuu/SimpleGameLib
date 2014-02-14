// AudioCreator.h
// �I�[�f�B�I�N���X����
// �ȈՐ���
// 			XAudio2��p���������Đ��N���X�̎���
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

// �����̃��C�u�����̖��O���
namespace yuu
{
// �I�[�f�B�I
namespace audio
{
//--------------------------------------------------------------------------------------
//XAudio2���[�g�N���X
//--------------------------------------------------------------------------------------
// XAudio2���[�g�N���X
//
// XAudio2�̃G���W���ƃ}�X�^�[�{�C�X���Ǘ�
//
class XAudioEngine
{
	IXAudio2 *m_pXAudio2;	// <XAudio2�̃G���W��
	IXAudio2MasteringVoice *m_pMasteringVoice;	// <XAudio2�̃}�X�^�[�{�C�X

	bool initialized;

	//�����A����ƃR�s�[�֎~
	XAudioEngine()
		: m_pXAudio2(NULL)
		, m_pMasteringVoice(NULL)
		, initialized(false)
	{
		//COM�̏�����
		yuu::MTA_CoInitiarizer::Init();
	}
	XAudioEngine(const XAudioEngine &rhs);
	XAudioEngine &operator=(const XAudioEngine &rhs);

public:
	~XAudioEngine()
	{
		//�}�X�^�[�{�C�X�̍폜
		if(m_pMasteringVoice)
		{
			m_pMasteringVoice->DestroyVoice();
			m_pMasteringVoice = NULL;
		}
		//�G���W���̍폜  **** �K�����ׂẴ{�C�X�폜��ɏ������� ****
		if(m_pXAudio2)
		{
			m_pXAudio2->Release();
			m_pXAudio2 = NULL;
		}
	}

	// ������
	// �Ȃ�
	void init()
	{
		if(initialized)
			return;

		//�f�o�b�O���[�h�Ȃ�f�o�b�O�G���W�����g��
		UINT32 flags = 0;

#ifdef _DEBUG
		flags |= XAUDIO2_DEBUG_ENGINE;
#endif

		//�G���W���̍쐬
		if(FAILED(XAudio2Create(&m_pXAudio2, flags)))
			throw std::runtime_error("XAudio2�̏������Ɏ��s���܂���");

		//�}�X�^�[�{�C�X�̍쐬
		if(FAILED(m_pXAudio2->CreateMasteringVoice(&m_pMasteringVoice)))
			throw std::runtime_error("XAudio2�̃}�X�^�[�{�C�X�쐬�ɂɎ��s���܂���");

		initialized = true;
	}

	// �C���X�^���X�̎擾
	// ���̃N���X�̃C���X�^���X�ւ̎Q��
	static XAudioEngine &getInstance()
	{
		static XAudioEngine instance;  // �B��̃C���X�^���X
		return instance;
	}
	// �f�o�C�X�̎擾
	// XAudio2�̃G���W���ւ̃|�C���^
	IXAudio2 *getDevice()
	{
		return m_pXAudio2;
	}
	// ���������ꂽ���ǂ���
	bool isInitialized()
	{
		return initialized;
	}
};

//--------------------------------------------------------------------------------------
//�I�[�f�B�I�̃x�[�X
//--------------------------------------------------------------------------------------
class XAudio2Base : public IAudio
{
	typedef std::vector<IXAudio2SourceVoice *> SourseVoiseList;
protected:
	WaveFileReader m_pcm;	// <Wave���
	SourseVoiseList m_src_voice_list;	// <�\�[�X�{�C�X

	XAUDIO2_BUFFER m_xaudio_buff;// <�o�b�t�@

protected:
	// �����A����ƃR�s�[�֎~
	// �R���X�g���N�^
	// �\�[�X�{�C�X�̌�
	XAudio2Base(size_t voice_num = 1)
		: m_pcm()
		, m_src_voice_list(voice_num, 0)
	{}

public:
	virtual ~XAudio2Base() {}
	virtual void stop() {};
};

//--------------------------------------------------------------------------------------
//�ʏ�̃I�[�f�B�I�F�\�[�X�{�C�X1�A���[�v����A�S�ǂݍ���
//--------------------------------------------------------------------------------------
// �ʏ�̃I�[�f�B�I�v���[���[
//
// ���ׂēǂݍ��ݍĐ�����B
//
class NormalAudio : public XAudio2Base
{
	typedef XAudio2Base base;

	// �����֐��ɂ̓A�N�Z�X����
	friend Audio CreateAudio(const tstring &, AudioType, size_t);

private:
	std::vector<char> m_buff;	// <�o�b�t�@

private:
	// �R���X�g���N�^
	// �Đ��t�@�C����
	// ���[�v��
	// std::runtime_error("���s���R")
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
//�����Đ��̃I�[�f�B�I�F�\�[�X�{�C�Xn�A���[�v�Ȃ��A�S�ǂݍ���
//--------------------------------------------------------------------------------------
// �����Đ�������ʉ������I�[�f�B�I�v���[���[
//
// ���ׂēǂݍ��ݍĐ�����B�w������������Đ��ł���B
//
class SyncAudio : public XAudio2Base
{
	typedef XAudio2Base Super;

	//�����֐��ɂ̓A�N�Z�X����
	friend Audio CreateAudio(const tstring &, AudioType, size_t);

private:
	//�o�b�t�@
	std::vector<char> m_buff;

	int m_CurNum;
	const int m_SyncNum;

private:
	//�����A����ƃR�s�[�֎~
	// �R���X�g���N�^
	// �Đ��t�@�C����
	// �����Đ���
	// std::runtime_error("���s���R")
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
//�X�g���[�~���O�I�[�f�B�I�F�\�[�X�{�C�X1�A���[�v����A�����ǂݍ���
//--------------------------------------------------------------------------------------
// �X�g���[�~���O�I�[�f�B�I�v���[���[
//
// �X�g���[�~���O�Đ�����B
//
class StreamingAudio : public XAudio2Base
{
	typedef XAudio2Base Super;

	//�����֐��ɂ̓A�N�Z�X����
	friend Audio CreateAudio(const tstring &, AudioType, size_t);

private:
	//�X�g���[�~���O�X���b�h�֐��ƃn���h��
	static unsigned int __stdcall StreamingThread(void *thisptr);
	HANDLE m_hStreamingThread;
	//�X���b�h���
	enum PLAYSTATE {START, STOP, END} m_state;

private:
	//�񓯊��ǂݍ��݃C�x���g�n���h��
	class VoiceCallback;
	std::shared_ptr<VoiceCallback> m_pCallBack;
	HANDLE m_hBufferEndEvent;

private:
	//�o�b�t�@
	int m_UseBufNum;
	static const size_t BUFFER_NUM = 4;
	std::vector<char> m_buff[BUFFER_NUM];

	//���[�v�t���O
	bool m_loop;

private:
	// �R���X�g���N�^
	// �Đ��t�@�C����
	// ���[�v��
	// std::runtime_error("���s���R")
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
//�t�F�[�h�⏕�N���X
//--------------------------------------------------------------------------------------
// �t�F�[�h�⏕�N���X
//
// �I�[�f�B�I���t�F�[�h�C���E�A�E�g����̂�⏕�B
// ���ʂ̑����͂����P���Ȑ��`�⊮�B
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
	// �R���X�g���N�^
	// �t�F�[�h���������I�[�f�B�I
	// std::runtime_error("���s���R")
	FadeHelper(Audio audio);
	// �f�X�g���N�^
	~FadeHelper();

	// �Ǘ����Ă���I�[�f�B�I���擾
	// �Ȃ�
	Audio GetManagedAudio();

	// �t�F�[�h�C���J�n
	// �t�F�[�h�C�������܂ł̕b���B�����������ɂ��̎��Ԃł͏I�����Ȃ��B
	// �Ȃ�
	void FadeIn(float second);
	// �t�F�[�h�A�E�g�J�n
	// �t�F�[�h�A�E�g�����܂ł̕b���B�����������ɂ��̎��Ԃł͏I�����Ȃ��B
	// �Ȃ�
	void FadeOut(float second);
};

//--------------------------------------------------------------------------------------
//�ʏ�̃I�[�f�B�I�F�\�[�X�{�C�X1�A���[�v���肩�Ȃ���
//--------------------------------------------------------------------------------------
NormalAudio::NormalAudio(const tstring &filename)
	: base()
{
#ifdef _UNICODE
	//weva���J��
	m_pcm.open(WStringToString(filename));
#elif
	m_pcm.open(filename);
#endif

	IPCMReader::Format format;
	m_pcm.getWaveFormat(&format);

	//�\�[�X�{�C�X�̍쐬
	if(FAILED(XAudioEngine::getInstance().getDevice()->CreateSourceVoice(
				  &m_src_voice_list[0], (WAVEFORMATEX*)&format)))
		throw std::runtime_error("�\�[�X�{�C�X�̍쐬�Ɏ��s���܂���");

	//�f�[�^�̓ǂݎ��
	
	m_buff.resize(m_pcm.getDataSize());
	size_t read =m_pcm.read(&m_buff[0], m_pcm.getDataSize());

	//XAudio�p�̃o�b�t�@
	std::memset(&m_xaudio_buff, 0, sizeof(m_xaudio_buff));

	m_xaudio_buff.AudioBytes = (UINT)read;
	m_xaudio_buff.pAudioData = pointer_cast<BYTE>(&m_buff[0]);
	m_xaudio_buff.Flags		 = XAUDIO2_END_OF_STREAM;
	m_xaudio_buff.LoopCount	 = 0;

	m_src_voice_list[0]->SubmitSourceBuffer(&m_xaudio_buff);
}
NormalAudio::~NormalAudio()
{
	//�I��
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
//�����Đ��̃I�[�f�B�I�F�\�[�X�{�C�Xn�A���[�v�Ȃ��A�S�ǂݍ���
//--------------------------------------------------------------------------------------
SyncAudio::SyncAudio(const tstring &filename, int syncnum)
	: Super(syncnum),
	  m_SyncNum(syncnum),
	  m_CurNum(0)
{
#ifdef _UNICODE
	//weva���J��
	m_pcm.open(WStringToString(filename));
#elif
	m_pcm.open(filename);
#endif

	//�\�[�X�{�C�X�̍쐬
	int i = 0;
	try
	{
		IPCMReader::Format format;
		m_pcm.getWaveFormat(&format);
		for(i = 0; i < syncnum; ++i)
		{
			if(FAILED(XAudioEngine::getInstance().getDevice()->CreateSourceVoice(
						  &m_src_voice_list[i], (WAVEFORMATEX*)&format)))
				throw std::runtime_error("�\�[�X�{�C�X�̍쐬�Ɏ��s���܂���");
		}
	}
	catch(std::exception &e)
	{
		for(int j = 0; j < i; ++j)
			m_src_voice_list[j]->DestroyVoice();

		throw e;
	}

	//�f�[�^�̓ǂݎ��
	m_buff.resize(m_pcm.getDataSize());
	size_t read =m_pcm.read(&m_buff[0], m_pcm.getDataSize());

	//XAudio�p�̃o�b�t�@
	std::memset(&m_xaudio_buff, 0, sizeof(m_xaudio_buff));

	m_xaudio_buff.AudioBytes = (UINT)read;
	m_xaudio_buff.pAudioData = pointer_cast<BYTE>(&m_buff[0]);
	m_xaudio_buff.Flags = XAUDIO2_END_OF_STREAM;

	m_src_voice_list[0]->SubmitSourceBuffer(&m_xaudio_buff);
	++m_CurNum;
}
SyncAudio::~SyncAudio()
{
	//�I��
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
//�X�g���[�~���O�I�[�f�B�I�F�\�[�X�{�C�X1�A���[�v����A�����ǂݍ���
//--------------------------------------------------------------------------------------
class StreamingAudio::VoiceCallback : public IXAudio2VoiceCallback
{
	friend class StreamingAudio;

private:
	StreamingAudio &m_stream;

	//�R���X�g���N�^
	VoiceCallback(StreamingAudio &stream)
		: m_stream(stream)
	{}

	//�g�p���郁�\�b�h
	void __stdcall OnBufferEnd(void *pBufferContext)
	{
		SetEvent(m_stream.m_hBufferEndEvent);
	}

	void __stdcall OnStreamEnd()
	{
#ifdef _DEBUG
		OutputDebugString(L"�X�g���[�~���O�t�@�C�������ɂȂ�܂���\n");
#endif
		if(!m_stream.GetLoop())
		{
			m_stream.pause();
		}
	}

	//�g�p���Ȃ����\�b�h
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
			throw std::runtime_error("�X�g���[�~���O�C�x���g�̍쐬�Ɏ��s���܂���");

		//�R�[���o�b�N�ɐݒ�
		m_pCallBack.reset(new VoiceCallback(*this));

#ifdef _UNICODE
		//weva���J��
		m_pcm.open(WStringToString(filename));
#elif
		m_pcm.open(filename);
#endif

		//�\�[�X�{�C�X�̍쐬
		IPCMReader::Format format;
		m_pcm.getWaveFormat(&format);
		if(FAILED(XAudioEngine::getInstance().getDevice()->CreateSourceVoice(
					  &m_src_voice_list[0], (WAVEFORMATEX*)&format, 0, 8.0f, m_pCallBack.get())))
			throw std::runtime_error("�\�[�X�{�C�X�̍쐬�Ɏ��s���܂���");

		//�f�[�^���̓ǂݎ��
		size_t bufsize =
			format.nChannels *
			format.nSamplesPerSec *
			format.wBitsPerSample /
			8;
		//��b�Ԃ̃o�b�t�@�T�C�Y*4
		for(int i = 0; i < BUFFER_NUM; ++i)
			m_buff[i].resize(bufsize);
		std::memset(&m_xaudio_buff, 0, sizeof(m_xaudio_buff));

		//�Ƃ肠���������Ă���
		size_t read =m_pcm.read(&m_buff[m_UseBufNum].at(0), m_buff[m_UseBufNum].size());
		//XAudio�p�̃o�b�t�@
		m_xaudio_buff.AudioBytes = (UINT)read;
		m_xaudio_buff.pAudioData = pointer_cast<BYTE>(&m_buff[m_UseBufNum].at(0));
		m_xaudio_buff.Flags =
			(m_buff[m_UseBufNum].size() > read) ? XAUDIO2_END_OF_STREAM : 0;
		//�L���[�ɒǉ�
		m_src_voice_list[0]->SubmitSourceBuffer(&m_xaudio_buff);
		++m_UseBufNum;

		//�X�g���[�~���O�X���b�h�J�n
		m_hStreamingThread = reinterpret_cast<HANDLE>(_beginthreadex(
								 NULL, 0, StreamingAudio::StreamingThread, this, 0, NULL));

		if(m_hStreamingThread == 0)
			throw std::runtime_error("�X�g���[�~���O�X���b�h�̍쐬�Ɏ��s���܂���");
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
		//�X���b�h�N�����Ȃ�I����Ԃɂ��ăX���b�h�I����҂�
		m_state = END;
		SetEvent(m_hBufferEndEvent);
		WaitForSingleObject(m_hStreamingThread, INFINITE);
	}

	//�I��
	CloseHandle(m_hBufferEndEvent);
	CloseHandle(m_hStreamingThread);
	m_src_voice_list[0]->Stop();
	m_src_voice_list[0]->DestroyVoice();
}
unsigned int __stdcall StreamingAudio::StreamingThread(void *thisptr)
{
	StreamingAudio *_this = (StreamingAudio *)thisptr;

	//�I����ԂɂȂ�܂ŃX���b�h�𓮂���
	while(true)
	{
		//�o�b�t�@�I���܂őҋ@
		WaitForSingleObject(_this->m_hBufferEndEvent, INFINITE);

		//�I����ԂȂ�X���b�h���I����
		if(_this->m_state == END)
			break;
		//��~��ԂȂ牽�����Ȃ�
		else if(_this->m_state == STOP)
			continue;

		//�{�C�X�̏�Ԃ��擾���ăL���[�����t�Ȃ牽�����Ȃ�
		XAUDIO2_VOICE_STATE VoiceState = {};
		_this->m_src_voice_list[0]->GetState(&VoiceState);
		if(VoiceState.BuffersQueued >= BUFFER_NUM)
			continue;

		//�o�b�t�@�Ɏc�肪�������ǂݎ��
		while(_this->m_state != END)
		{
			//�{�C�X�̏�Ԃ��擾���ăL���[�����t�Ȃ�E�o
			_this->m_src_voice_list[0]->GetState(&VoiceState);
			if(VoiceState.BuffersQueued >= BUFFER_NUM)
				break;

			//�f�[�^�̓ǂݎ��
			size_t read=_this->m_pcm.read(
				_this->m_buff[_this->m_UseBufNum].data(),
				_this->m_buff[_this->m_UseBufNum].size());

			//�ǂݎ����Ō�܂ōs�����Ƃ��͓��o��
			if(_this->m_buff[_this->m_UseBufNum].size() > read
					|| _this->m_pcm.isEOF())
			{
				//���o��
				_this->m_pcm.seekToTop();
			}

			//�ǂݏo����0�łȂ�������L���[�ɒǉ�
			if(read != 0)
			{
				_this->m_xaudio_buff.AudioBytes = (UINT)read;
				_this->m_xaudio_buff.pAudioData = pointer_cast<BYTE>(&_this->m_buff[_this->m_UseBufNum].at(0));
				_this->m_xaudio_buff.Flags =
					(_this->m_buff[_this->m_UseBufNum].size() > read) ? XAUDIO2_END_OF_STREAM : 0;
				_this->m_src_voice_list[0]->SubmitSourceBuffer(&_this->m_xaudio_buff);

				//�o�b�t�@�؂�ւ�
				++_this->m_UseBufNum;
				if(_this->m_UseBufNum >= BUFFER_NUM)
					_this->m_UseBufNum = 0;
			}
		}
	}
#ifdef _DEBUG
	OutputDebugString(L"**************** �X�g���[�~���O�X���b�h���I�����܂��� **************** \n");
#endif
	return 0;
}
void StreamingAudio::start()
{
	//�{�C�X�̏�Ԃ��擾���ăL���[�̐����m�F
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
//�t�F�[�h�C���E�t�F�[�h�A�E�g�⏕�N���X
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
		throw std::runtime_error("�t�F�[�h�C�x���g�̍쐬�Ɏ��s���܂���");
	if(NULL == (m_hFadeThread = reinterpret_cast<HANDLE>(_beginthreadex(
									NULL, 0, FadeHelper::FadeThread, this, 0, NULL))))
		throw std::runtime_error("�t�F�[�h�X���b�h�̍쐬�Ɏ��s���܂���");
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
//�I�[�f�B�I�֘A�֐��Q
//--------------------------------------------------------------------------------------
void InitAudio()
{
	XAudioEngine::getInstance().init();
}

Audio CreateAudio(const tstring &filename, AudioType type, size_t value)
{
	//�����������łɍs���Ă���
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