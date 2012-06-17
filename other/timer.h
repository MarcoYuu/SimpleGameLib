#pragma once

namespace yuu{

//-----------------------------------------------------------------------------------------------
//���Ԍv���p�̃C���^�t�F�[�X��
//-----------------------------------------------------------------------------------------------
//�p�����Ȃ��Ă�FPSCounter��getTimeInSeconds������΂悢
class ICountTime
{
public:
	virtual ~ICountTime() {}
	virtual double getTimeInSeconds() const = 0;
};

//-----------------------------------------------------------------------------------------------
//clock()�֐���p�������Ԍv���N���X
//-----------------------------------------------------------------------------------------------
class StdTimeCounter : public ICountTime
{
public:
	double getTimeInSeconds() const;
};

//-----------------------------------------------------------------------------------------------
//�}���`���f�B�^�C�}��p�������Ԍv���N���X
//-----------------------------------------------------------------------------------------------
class MultiMediaCounter : public ICountTime
{
public:
	MultiMediaCounter();
	~MultiMediaCounter();

	double getTimeInSeconds() const;
};

//-----------------------------------------------------------------------------------------------
//clock()�֐���p�������Ԍv���N���X
//-----------------------------------------------------------------------------------------------
class ClockFreqCounter : public ICountTime
{
private:
	double mCPUFreqency;

public:
	ClockFreqCounter();

	double getTimeInSeconds() const;
	static bool isAvailable();
};

}