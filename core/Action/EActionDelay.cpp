#include "..\eactions.h"
#include "..\Win\winbase.h"

e2d::EActionDelay::EActionDelay(float duration)
{
	setInterval(static_cast<LONGLONG>(duration * 1000));
}

e2d::EActionDelay * e2d::EActionDelay::clone() const
{
	return new EActionDelay(static_cast<float>(ToMilliseconds(m_nAnimationInterval.QuadPart)) / 1000.0f);
}

void e2d::EActionDelay::_init()
{
	EAction::_init();
}

void e2d::EActionDelay::_update()
{
	EAction::_update();
	// 判断时间间隔是否足够
	if (IsIntervalFull(m_tLast, m_nAnimationInterval))
	{
		this->stop();
	}
}

void e2d::EActionDelay::_reset()
{
	EAction::_reset();
}
