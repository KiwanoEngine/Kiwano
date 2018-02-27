#include "..\eactions.h"


e2d::ActionScaleBy::ActionScaleBy(double duration, double scale)
	: ActionGradual(duration)
{
	m_nVariationX = scale;
	m_nVariationY = scale;
}

e2d::ActionScaleBy::ActionScaleBy(double duration, double scaleX, double scaleY)
	: ActionGradual(duration)
{
	m_nVariationX = scaleX;
	m_nVariationY = scaleY;
}

void e2d::ActionScaleBy::_init()
{
	ActionGradual::_init();
	if (m_pTarget)
	{
		m_nBeginScaleX = m_pTarget->getScaleX();
		m_nBeginScaleY = m_pTarget->getScaleY();
	}
}

void e2d::ActionScaleBy::_update()
{
	ActionGradual::_update();

	if (m_pTarget == nullptr)
	{
		this->stop();
		return;
	}

	// Ëõ·Å½Úµã
	m_pTarget->setScale(
		m_nBeginScaleX + m_nVariationX * m_fRateOfProgress, 
		m_nBeginScaleY + m_nVariationY * m_fRateOfProgress);
}

e2d::ActionScaleBy * e2d::ActionScaleBy::clone() const
{
	return new ActionScaleBy(m_fDuration, m_nVariationX, m_nVariationY);
}

e2d::ActionScaleBy * e2d::ActionScaleBy::reverse() const
{
	return new ActionScaleBy(m_fDuration, -m_nVariationX, -m_nVariationY);
}