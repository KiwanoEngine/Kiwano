#include "..\e2dbase.h"
#include <thread>
#include <chrono>
using namespace std::chrono;


// 游戏开始时间
static steady_clock::time_point s_tStart;
// 当前时间
static steady_clock::time_point s_tNow;
// 上一帧刷新时间
static steady_clock::time_point s_tLast;
// 固定的刷新时间
static steady_clock::time_point s_tFixed;
// 每一帧间隔
static milliseconds s_tExceptedInvertal;


double e2d::Time::getTotalTime()
{
	return duration_cast<microseconds>(s_tNow - s_tStart).count() / 1000.0 / 1000.0;
}

unsigned int e2d::Time::getTotalTimeMilliseconds()
{
	return static_cast<unsigned int>(duration_cast<milliseconds>(s_tNow - s_tStart).count());
}

double e2d::Time::getDeltaTime()
{
	return duration_cast<microseconds>(s_tNow - s_tLast).count() / 1000.0 / 1000.0;
}

unsigned int e2d::Time::getDeltaTimeMilliseconds()
{
	return static_cast<unsigned int>(duration_cast<milliseconds>(s_tNow - s_tLast).count());
}

bool e2d::Time::__init()
{
	s_tStart = s_tFixed = s_tLast = s_tNow = steady_clock::now();
	s_tExceptedInvertal = milliseconds(15);
	return true;
}

bool e2d::Time::__isReady()
{
	return s_tExceptedInvertal < duration_cast<milliseconds>(s_tNow - s_tFixed);
}

void e2d::Time::__updateNow()
{
	// 刷新时间
	s_tNow = steady_clock::now();
}

void e2d::Time::__updateLast()
{
	s_tFixed += s_tExceptedInvertal;

	s_tLast = s_tNow;
	s_tNow = steady_clock::now();
}

void e2d::Time::__reset()
{
	s_tLast = s_tFixed = s_tNow = steady_clock::now();
}

void e2d::Time::__sleep()
{
	// 计算挂起时长
	int nWaitMS = 16 - static_cast<int>(duration_cast<milliseconds>(s_tNow - s_tFixed).count());
	
	if (nWaitMS > 1)
	{
		// 挂起线程，释放 CPU 占用
		std::this_thread::sleep_for(milliseconds(nWaitMS));
	}
}
