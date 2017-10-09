#include "winbase.h"

static steady_clock::time_point nNow;

steady_clock::time_point GetNow()
{
	return nNow;
}

void FlushSteadyClock()
{
	nNow = steady_clock::now();	// 获取当前时间
}