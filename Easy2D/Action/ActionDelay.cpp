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

void ActionDelay::_exec(LARGE_INTEGER nNow)
{
	// 判断时间间隔是否足够
	if (nNow.QuadPart - m_nLast.QuadPart > m_nAnimationInterval.QuadPart)
	{
		this->stop();
	}
}

void ActionDelay::_reset()
{
	Action::_reset();
	// 重新记录当前时间
	QueryPerformanceCounter(&m_nLast);
}
