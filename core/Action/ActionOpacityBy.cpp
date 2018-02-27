#include "..\eactions.h"


e2d::ActionOpacityBy::ActionOpacityBy(double duration, double opacity) :
	ActionGradual(duration)
{
	m_nVariation = opacity;
}

void e2d::ActionOpacityBy::_init()
{
	ActionGradual::_init();
	if (m_pTarget)
	{
		m_nBeginVal = m_pTarget->getOpacity();
	}
}

void e2d::ActionOpacityBy::_update()
{
	ActionGradual::_update();

	if (m_pTarget == nullptr)
	{
		this->stop();
		return;
	}
	// 设置节点透明度
	m_pTarget->setOpacity(m_nBeginVal + m_nVariation * m_fRateOfProgress);
}

e2d::ActionOpacityBy * e2d::ActionOpacityBy::clone() const
{
	return new ActionOpacityBy(m_fDuration, m_nVariation);
}

e2d::ActionOpacityBy * e2d::ActionOpacityBy::reverse() const
{
	return new ActionOpacityBy(m_fDuration, -m_nVariation);
}