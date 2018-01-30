#include "..\ebase.h"
#include "..\etransitions.h"

e2d::ETransition::ETransition(float duration)
	: m_bEnd(false)
	, m_fLast(0)
	, m_fRateOfProgress(0)
	, m_pPrevScene(nullptr)
	, m_pNextScene(nullptr)
{
	m_fDuration = max(duration, 0);
}

bool e2d::ETransition::isEnding()
{
	return m_bEnd;
}

void e2d::ETransition::_calcRateOfProgress()
{
	// 判断时间间隔是否足够
	if (m_fDuration == 0)
	{
		m_fRateOfProgress = 1;
		return;
	}

	// 计算动画进度
	m_fRateOfProgress = min((ETime::getTotalTime() - m_fLast) / m_fDuration, 1);
}

void e2d::ETransition::_stop()
{
	m_bEnd = true;
	_reset();
}

void e2d::ETransition::_setTarget(EScene * prev, EScene * next)
{
	m_fLast = ETime::getTotalTime();
	m_pPrevScene = prev;
	m_pNextScene = next;
	_init();
}
