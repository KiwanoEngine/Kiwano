#include "..\ebase.h"

// 上一帧与当前帧的时间间隔
static int s_nInterval = 0;
// 游戏开始时长
static float s_fTotalTime = 0;


float e2d::ETime::getTotalTime()
{
	return s_fTotalTime;
}

int e2d::ETime::getDeltaTime()
{
	return s_nInterval;
}



#if _MSC_VER > 1600

#include <thread>
#include <chrono>
using namespace std::chrono;


// 游戏开始时间
static steady_clock::time_point s_tStart;
// 当前时间
static steady_clock::time_point s_tNow;
// 上一帧刷新时间
static steady_clock::time_point s_tLast;


bool e2d::ETime::__init()
{
	s_tStart = s_tLast = s_tNow = steady_clock::now();
	return true;
}

void e2d::ETime::__uninit()
{
}

void e2d::ETime::__updateNow()
{
	s_tNow = steady_clock::now();
	s_fTotalTime = static_cast<float>(duration_cast<milliseconds>(s_tNow - s_tStart).count()) / 1000.0f;
	s_nInterval = static_cast<int>(duration_cast<milliseconds>(s_tNow - s_tLast).count());
}

void e2d::ETime::__updateLast()
{
	s_tLast = s_tNow;
}

void e2d::ETime::__sleep()
{
	// 计算挂起时长
	int nWaitMS = 16 - s_nInterval;
	// 挂起线程，释放 CPU 占用
	if (nWaitMS > 1)
	{
		std::this_thread::sleep_for(milliseconds(nWaitMS));
	}
}


#else


#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

// 时钟频率
static LARGE_INTEGER s_tFreq;
// 当前时间
static LARGE_INTEGER s_tNow;
// 游戏开始时间
static LARGE_INTEGER s_tStart;
// 上一帧画面绘制时间
static LARGE_INTEGER s_tLast;



bool e2d::ETime::__init()
{
	bool bRet = false;
	if (::timeBeginPeriod(1))
	{
		// 修改时间精度
		if (::QueryPerformanceFrequency(&s_tFreq))	// 获取时钟频率
		{

			if (::QueryPerformanceCounter(&s_tNow))		// 刷新当前时间
			{
				s_tStart = s_tLast = s_tNow;
				bRet = true;
			}
		}
	}
	return bRet;
}

void e2d::ETime::__uninit()
{
	::timeEndPeriod(1);	// 重置时间精度
}

void e2d::ETime::__updateNow()
{
	::QueryPerformanceCounter(&s_tNow);
	s_fTotalTime = static_cast<float>(s_tNow.QuadPart - s_tStart.QuadPart) / s_tFreq.QuadPart;
	s_nInterval = static_cast<int>((s_tNow.QuadPart - s_tLast.QuadPart) * 1000LL / s_tFreq.QuadPart);
}

void e2d::ETime::__updateLast()
{
	s_tLast = s_tNow;
}

void e2d::ETime::__sleep()
{
	// 计算挂起时长
	int nWaitMS = 16 - s_nInterval;
	// 挂起线程，释放 CPU 占用
	if (nWaitMS > 1)
	{
		::Sleep(nWaitMS);
	}
}


#endif // _MSC_VER > 1600
