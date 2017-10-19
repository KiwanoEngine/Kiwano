#include "..\eactions.h"
#include "..\Win\winbase.h"

e2d::EActionDelay::EActionDelay(float duration)
{
	setInterval(LONGLONG(duration * 1000));
}

e2d::EActionDelay * e2d::EActionDelay::clone() const
{
	return new EActionDelay(m_nAnimationInterval / 1000.0f);
}

void e2d::EActionDelay::_init()
{
	EAction::_init();
	// 记录当前时间
	m_tLast = GetNow();
}

void e2d::EActionDelay::_callOn()
{
	// 判断时间间隔是否足够
	if (GetInterval(m_tLast) > m_nAnimationInterval)
	{
		this->stop();
	}
}

void e2d::EActionDelay::_reset()
{
	EAction::_reset();
	// 记录当前时间
	m_tLast = GetNow();
}
