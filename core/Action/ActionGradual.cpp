#include "..\eactions.h"

e2d::ActionGradual::ActionGradual(float duration)
	: m_fRateOfProgress(0)
{
	m_fDuration = max(duration, 0);
}

void e2d::ActionGradual::_init()
{
	Action::_init();
}

void e2d::ActionGradual::_update()
{
	Action::_update();
	// 判断时间间隔是否足够
	if (m_fDuration == 0)
	{
		m_fRateOfProgress = 1;
		this->stop();
		return;
	}
	// 计算动画进度
	m_fRateOfProgress = min((Time::getTotalTime() - m_fLast) / m_fDuration, 1);
	// 判断动作是否结束
	if (m_fRateOfProgress >= 1)
	{
		this->stop();
	}
}
