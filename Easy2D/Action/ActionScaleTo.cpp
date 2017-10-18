#include "..\eactions.h"

e2d::ActionScaleTo::ActionScaleTo(float duration, float scaleX, float scaleY) :
	ActionScaleBy(duration, 0, 0)
{
	m_nEndScaleX = scaleX;
	m_nEndScaleY = scaleY;
}

e2d::ActionScaleTo::~ActionScaleTo()
{
}

e2d::ActionScaleTo * e2d::ActionScaleTo::copy() const
{
	return new ActionScaleTo(m_nAnimationInterval / 1000.0f, m_nEndScaleX, m_nEndScaleY);
}

void e2d::ActionScaleTo::_init()
{
	ActionScaleBy::_init();
	m_nVariationX = m_nEndScaleX - m_nBeginScaleX;
	m_nVariationY = m_nEndScaleY - m_nBeginScaleY;
}

void e2d::ActionScaleTo::_reset()
{
	ActionScaleBy::_reset();
}
