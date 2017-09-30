#include "..\easy2d.h"

ActionDelay::ActionDelay(float duration)
{
	setInterval(UINT(duration * 1000));
}

ActionDelay::~ActionDelay()
{
}

ActionDelay * ActionDelay::copy() const
{
	return new ActionDelay(m_nMilliSeconds / 1000.0f);
}

void ActionDelay::_init()
{
	// 记录当前时间
	QueryPerformanceCounter(&m_nLast);
}

bool ActionDelay::_exec(LARGE_INTEGER nNow)
{
	if (m_bStop) return true;
	if (!m_bRunning) return false;

	// 判断时间间隔是否足够
	if (nNow.QuadPart - m_nLast.QuadPart > m_nAnimationInterval.QuadPart)
	{
		return true;
	}
	return false;
}

void ActionDelay::_reset()
{
	// 重新记录当前时间
	QueryPerformanceCounter(&m_nLast);
}
