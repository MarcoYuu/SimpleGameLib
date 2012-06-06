#pragma once

#include <list>

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

//-----------------------------------------------------------------------------------------------
//FPS計測クラス(移動平均)
//-----------------------------------------------------------------------------------------------
//Counter : 時間計測用クラス double getTimeInSeconds(void) が必要
template<typename Counter = ClockFreqCounter>
class FrameRateCounter
{
public:
	FrameRateCounter();
	~FrameRateCounter();

	double getFPS();
	void setAverageNum(size_t num);

private:
	Counter mCounter;

	std::list<double> mPastTimeList;
	double mTotalTime;
	double mPrevTime;
	double mFPS;

	size_t mNum;

	void update();
};

template<typename Counter>
FrameRateCounter<Counter>::FrameRateCounter()
	: mCounter()
	, mPastTimeList()
	, mTotalTime(0.0)
	, mPrevTime(mCounter.getTimeInSeconds())
	, mFPS(0.0)
	, mNum(10)
{
	setAverageNum(mNum);
}

template<typename Counter>
FrameRateCounter<Counter>::~FrameRateCounter()
{
}

template<typename Counter>
void FrameRateCounter<Counter>::update()
{
	double cur_time = mCounter.getTimeInSeconds();
	double diff = cur_time - mPrevTime;
	mPrevTime = cur_time;

	mPastTimeList.pop_front();
	mPastTimeList.push_back(diff);

	mFPS = 10.0 / (mTotalTime + diff);
	mTotalTime += diff - *mPastTimeList.begin();
}

template<typename Counter>
double FrameRateCounter<Counter>::getFPS()
{
	update();
	return mFPS;
}

template<typename Counter>
void FrameRateCounter<Counter>::setAverageNum(size_t num)
{
	mNum = num;
	mPastTimeList.resize(mNum, 0.0);
	update();
}


