#include "..\eactions.h"

e2d::EActionScaleTo::EActionScaleTo(float duration, float scaleX, float scaleY) :
	EActionScaleBy(duration, 0, 0)
{
	m_nEndScaleX = scaleX;
	m_nEndScaleY = scaleY;
}

e2d::EActionScaleTo::~EActionScaleTo()
{
}

e2d::EActionScaleTo * e2d::EActionScaleTo::copy() const
{
	return new EActionScaleTo(m_nAnimationInterval / 1000.0f, m_nEndScaleX, m_nEndScaleY);
}

void e2d::EActionScaleTo::_init()
{
	EActionScaleBy::_init();
	m_nVariationX = m_nEndScaleX - m_nBeginScaleX;
	m_nVariationY = m_nEndScaleY - m_nBeginScaleY;
}

void e2d::EActionScaleTo::_reset()
{
	EActionScaleBy::_reset();
}
