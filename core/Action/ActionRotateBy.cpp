#include "..\eactions.h"


e2d::EActionRotateBy::EActionRotateBy(float duration, float rotation) :
	EActionGradual(duration)
{
	m_nVariation = rotation;
}

void e2d::EActionRotateBy::_init()
{
	EActionGradual::_init();
	if (m_pTarget)
	{
		m_nBeginVal = m_pTarget->getRotation();
	}
}

void e2d::EActionRotateBy::_update()
{
	EActionGradual::_update();

	if (m_pTarget == nullptr)
	{
		this->stop();
		return;
	}

	// Ðý×ª½Úµã
	m_pTarget->setRotation(m_nBeginVal + m_nVariation * m_fRateOfProgress);
}

e2d::EActionRotateBy * e2d::EActionRotateBy::clone() const
{
	return new EActionRotateBy(m_fDuration, m_nVariation);
}

e2d::EActionRotateBy * e2d::EActionRotateBy::reverse() const
{
	return new EActionRotateBy(m_fDuration, -m_nVariation);
}