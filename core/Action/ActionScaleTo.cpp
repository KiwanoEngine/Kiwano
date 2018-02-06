#include "..\eactions.h"

e2d::EActionScaleTo::EActionScaleTo(float duration, float scale)
	: EActionScaleBy(duration, 0, 0)
{
	m_nEndScaleX = scale;
	m_nEndScaleY = scale;
}

e2d::EActionScaleTo::EActionScaleTo(float duration, float scaleX, float scaleY)
	: EActionScaleBy(duration, 0, 0)
{
	m_nEndScaleX = scaleX;
	m_nEndScaleY = scaleY;
}

e2d::EActionScaleTo * e2d::EActionScaleTo::clone() const
{
	return new EActionScaleTo(m_fDuration, m_nEndScaleX, m_nEndScaleY);
}

void e2d::EActionScaleTo::_init()
{
	EActionScaleBy::_init();
	m_nVariationX = m_nEndScaleX - m_nBeginScaleX;
	m_nVariationY = m_nEndScaleY - m_nBeginScaleY;
}
