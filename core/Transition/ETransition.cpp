#include "..\ebase.h"
#include "..\etransitions.h"

e2d::Transition::Transition(double duration)
	: m_bEnd(false)
	, m_fLast(0)
	, m_fRateOfProgress(0)
	, m_pPrevScene(nullptr)
	, m_pNextScene(nullptr)
{
	m_fDuration = max(duration, 0);
}

bool e2d::Transition::isEnding()
{
	return m_bEnd;
}

void e2d::Transition::_calcRateOfProgress()
{
	// 判断时间间隔是否足够
	if (m_fDuration == 0)
	{
		m_fRateOfProgress = 1;
		return;
	}

	// 计算动画进度
	m_fRateOfProgress = min((Time::getTotalTime() - m_fLast) / m_fDuration, 1);
}

void e2d::Transition::_stop()
{
	m_bEnd = true;
	_reset();
}

void e2d::Transition::_setTarget(Scene * prev, Scene * next)
{
	m_fLast = Time::getTotalTime();
	m_pPrevScene = prev;
	m_pNextScene = next;
	_init();
}
