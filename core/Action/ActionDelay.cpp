#include "..\eactions.h"

e2d::ActionDelay::ActionDelay(double duration)
{
	m_fDelayTime = max(duration, 0);
}

e2d::ActionDelay * e2d::ActionDelay::clone() const
{
	return new ActionDelay(m_fDelayTime);
}

void e2d::ActionDelay::_init()
{
	Action::_init();
}

void e2d::ActionDelay::_update()
{
	Action::_update();
	// 判断时间间隔是否足够
	if ((Time::getTotalTime() - m_fLast) >= m_fDelayTime)
	{
		this->stop();
	}
}
