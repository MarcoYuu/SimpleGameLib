#pragma once

#include <list>

namespace yuu{


//-----------------------------------------------------------------------------------------------
//FPS計測クラス(移動平均)
//-----------------------------------------------------------------------------------------------
//Counter : 時間計測用クラス double getTimeInSeconds(void) が必要
template<typename Counter>
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

	double update();
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
double FrameRateCounter<Counter>::update()
{
	double cur_time = mCounter.getTimeInSeconds();
	double diff = cur_time - mPrevTime;
	mPrevTime = cur_time;

	mPastTimeList.pop_front();
	mPastTimeList.push_back(diff);

	mFPS = mNum / (mTotalTime + diff);
	mTotalTime += diff - mPastTimeList.front();

	return diff;
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


}