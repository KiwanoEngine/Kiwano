#include "..\eactions.h"
#include "..\Win\winbase.h"

e2d::ActionDelay::ActionDelay(float duration)
{
	setInterval(LONGLONG(duration * 1000));
}

e2d::ActionDelay::~ActionDelay()
{
}

e2d::ActionDelay * e2d::ActionDelay::copy() const
{
	return new ActionDelay(m_nAnimationInterval / 1000.0f);
}

void e2d::ActionDelay::_init()
{
	EAction::_init();
	// 记录当前时间
	m_nLast = GetNow();
}

void e2d::ActionDelay::_exec()
{
	// 判断时间间隔是否足够
	if (GetInterval(m_nLast) > m_nAnimationInterval)
	{
		this->stop();
	}
}

void e2d::ActionDelay::_reset()
{
	EAction::_reset();
	// 记录当前时间
	m_nLast = GetNow();
}
