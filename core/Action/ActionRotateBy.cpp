#include "..\eactions.h"


e2d::ActionRotateBy::ActionRotateBy(float duration, float rotation) :
	ActionGradual(duration)
{
	m_nVariation = rotation;
}

void e2d::ActionRotateBy::_init()
{
	ActionGradual::_init();
	if (m_pTarget)
	{
		m_nBeginVal = m_pTarget->getRotation();
	}
}

void e2d::ActionRotateBy::_update()
{
	ActionGradual::_update();

	if (m_pTarget == nullptr)
	{
		this->stop();
		return;
	}

	// Ðý×ª½Úµã
	m_pTarget->setRotation(m_nBeginVal + m_nVariation * m_fRateOfProgress);
}

e2d::ActionRotateBy * e2d::ActionRotateBy::clone() const
{
	return new ActionRotateBy(m_fDuration, m_nVariation);
}

e2d::ActionRotateBy * e2d::ActionRotateBy::reverse() const
{
	return new ActionRotateBy(m_fDuration, -m_nVariation);
}