#include "..\etransitions.h"
#include "..\Win\winbase.h"

e2d::ETransition::ETransition(float duration)
	: m_bEnd(false)
	, m_fTotalDuration(duration * 1000)
	, m_fDuration(0)
	, m_fRateOfProgress(0)
	, m_pPrevScene(nullptr)
	, m_pNextScene(nullptr)
{
}

bool e2d::ETransition::isEnding()
{
	return m_bEnd;
}

bool e2d::ETransition::_isDelayEnough()
{
	// 判断时间间隔是否足够
	if (m_fTotalDuration == 0)
	{
		m_fRateOfProgress = 1;
		return true;
	}

	if (IsIntervalFull(m_tLast, m_nAnimationInterval))
	{
		// 重新记录时间
		m_tLast.QuadPart += m_nAnimationInterval.QuadPart;
		m_fDuration += static_cast<float>(ToMilliseconds(m_nAnimationInterval.QuadPart));
		// 计算动画进度
		m_fRateOfProgress = m_fDuration / m_fTotalDuration;
		return true;
	}
	return false;
}

void e2d::ETransition::_stop()
{
	m_bEnd = true;
	_reset();
}

void e2d::ETransition::_setTarget(EScene * prev, EScene * next)
{
	m_tLast = GetNow();
	m_pPrevScene = prev;
	m_pNextScene = next;
	_init();
}
