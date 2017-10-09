#include "..\easy2d.h"
#include "..\Win\winbase.h"

ActionDelay::ActionDelay(float duration)
{
	setInterval(LONGLONG(duration * 1000));
}

ActionDelay::~ActionDelay()
{
}

ActionDelay * ActionDelay::copy() const
{
	return new ActionDelay(m_nAnimationInterval / 1000.0f);
}

void ActionDelay::_init()
{
	Action::_init();
	// 记录当前时间
	m_nLast = steady_clock::now();
}

void ActionDelay::_exec(steady_clock::time_point nNow)
{
	// 判断时间间隔是否足够
	if (duration_cast<milliseconds>(nNow - m_nLast).count() > m_nAnimationInterval)
	{
		this->stop();
	}
}

void ActionDelay::_reset()
{
	Action::_reset();
	// 记录当前时间
	m_nLast = steady_clock::now();
}
