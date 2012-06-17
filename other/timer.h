#pragma once

namespace yuu{

//-----------------------------------------------------------------------------------------------
//時間計測用のインタフェース提供
//-----------------------------------------------------------------------------------------------
//継承しなくてもFPSCounterはgetTimeInSecondsがあればよい
class ICountTime
{
public:
	virtual ~ICountTime() {}
	virtual double getTimeInSeconds() const = 0;
};

//-----------------------------------------------------------------------------------------------
//clock()関数を用いた時間計測クラス
//-----------------------------------------------------------------------------------------------
class StdTimeCounter : public ICountTime
{
public:
	double getTimeInSeconds() const;
};

//-----------------------------------------------------------------------------------------------
//マルチメディタイマを用いた時間計測クラス
//-----------------------------------------------------------------------------------------------
class MultiMediaCounter : public ICountTime
{
public:
	MultiMediaCounter();
	~MultiMediaCounter();

	double getTimeInSeconds() const;
};

//-----------------------------------------------------------------------------------------------
//clock()関数を用いた時間計測クラス
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