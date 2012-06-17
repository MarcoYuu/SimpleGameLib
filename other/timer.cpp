

#pragma comment(lib, "winmm.lib")

#include <ctime>

#include <windows.h>
#include <mmsystem.h>

#include <other/timer.h>

namespace yuu{

//-----------------------------------------------------------------------------------------------
//clock()関数を用いた時間計測クラス
//-----------------------------------------------------------------------------------------------
double StdTimeCounter::getTimeInSeconds() const
{
	return (double)clock() / (double)CLOCKS_PER_SEC;
}

//-----------------------------------------------------------------------------------------------
//マルチメディタイマを用いた時間計測クラス
//-----------------------------------------------------------------------------------------------
MultiMediaCounter::MultiMediaCounter()
{
	timeBeginPeriod(1);
}
MultiMediaCounter::~MultiMediaCounter()
{
	timeEndPeriod(1);
}
double MultiMediaCounter::getTimeInSeconds() const
{
	return timeGetTime() / 1000.0;
}

//-----------------------------------------------------------------------------------------------
//clock()関数を用いた時間計測クラス
//-----------------------------------------------------------------------------------------------
ClockFreqCounter::ClockFreqCounter()
	: mCPUFreqency(0.0)
{
	LARGE_INTEGER count;
	LARGE_INTEGER freq;
	QueryPerformanceCounter(&count);
	QueryPerformanceFrequency(&freq);
	mCPUFreqency = (double)freq.QuadPart;
}

double ClockFreqCounter::getTimeInSeconds() const
{
	LARGE_INTEGER count;
	QueryPerformanceCounter(&count);
	return count.QuadPart / mCPUFreqency;
}

bool ClockFreqCounter::isAvailable()
{
	LARGE_INTEGER tmp;
	return ((QueryPerformanceCounter(&tmp) == 0) ? false : true);
}

}