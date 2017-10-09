#include "..\easy2d.h"
#include "..\Win\winbase.h"

Animation::Animation(float duration)
{
	m_nDuration = 0;
	m_nTotalDuration = UINT(duration * 1000);
}

Animation::~Animation()
{
}

bool Animation::_isEnd() const
{
	return m_nDuration >= m_nTotalDuration;
}

void Animation::_init()
{
	Action::_init();
	// 记录当前时间
	m_nLast = steady_clock::now();
}

bool Animation::_isDelayEnough(steady_clock::time_point nNow)
{
	// 判断时间间隔是否足够
	if (duration_cast<milliseconds>(nNow - m_nLast).count() > m_nAnimationInterval)
	{
		// 重新记录时间
		m_nLast += milliseconds(m_nAnimationInterval);
		m_nDuration += m_nAnimationInterval;
		return true;
	}
	return false;
}

void Animation::_reset()
{
	Action::_reset();
	m_nDuration = 0;
	// 记录当前时间
	m_nLast = steady_clock::now();
}
