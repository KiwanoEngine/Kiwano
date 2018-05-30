#include "..\e2dbase.h"
#include <thread>
#include <chrono>
using namespace std::chrono;


// 游戏开始时间
static steady_clock::time_point s_tStart;
// 当前时间
static steady_clock::time_point s_tNow;
// 上一帧刷新时间
static steady_clock::time_point s_tLastUpdate;
// 每一帧间隔
static milliseconds s_tExceptedInvertal;

// 游戏开始时长
static unsigned int s_nTotalTime = 0;


double e2d::Time::getTotalTime()
{
	return s_nTotalTime / 1000.0;
}

unsigned int e2d::Time::getTotalTimeMilliseconds()
{
	return s_nTotalTime;
}

double e2d::Time::getDeltaTime()
{
	return s_tExceptedInvertal.count() / 1000.0;
}

unsigned int e2d::Time::getDeltaTimeMilliseconds()
{
	return static_cast<unsigned int>(s_tExceptedInvertal.count());
}

bool e2d::Time::__init()
{
	s_tStart = s_tLastUpdate = s_tNow = steady_clock::now();
	s_tExceptedInvertal = milliseconds(15);
	return true;
}

bool e2d::Time::__isReady()
{
	return s_tExceptedInvertal < duration_cast<milliseconds>(s_tNow - s_tLastUpdate);
}

void e2d::Time::__updateNow()
{
	// 刷新时间
	s_tNow = steady_clock::now();
}

void e2d::Time::__updateLast()
{
	s_tLastUpdate += s_tExceptedInvertal;

	s_tNow = steady_clock::now();
	s_nTotalTime = static_cast<unsigned int>(duration_cast<milliseconds>(s_tNow - s_tStart).count());
}

void e2d::Time::__reset()
{
	s_tLastUpdate = s_tNow = steady_clock::now();
	s_nTotalTime = static_cast<unsigned int>(duration_cast<milliseconds>(s_tNow - s_tStart).count());
}

void e2d::Time::__sleep()
{
	// 计算挂起时长
	int nWaitMS = 16 - static_cast<int>(duration_cast<milliseconds>(s_tNow - s_tLastUpdate).count());
	
	if (nWaitMS > 1)
	{
		// 挂起线程，释放 CPU 占用
		std::this_thread::sleep_for(milliseconds(nWaitMS));
	}
}
