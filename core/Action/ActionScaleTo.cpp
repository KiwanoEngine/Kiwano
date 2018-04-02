#include "..\eaction.h"

e2d::ActionScaleTo::ActionScaleTo(double duration, double scale)
	: ActionScaleBy(duration, 0, 0)
{
	m_nEndScaleX = scale;
	m_nEndScaleY = scale;
}

e2d::ActionScaleTo::ActionScaleTo(double duration, double scaleX, double scaleY)
	: ActionScaleBy(duration, 0, 0)
{
	m_nEndScaleX = scaleX;
	m_nEndScaleY = scaleY;
}

e2d::ActionScaleTo * e2d::ActionScaleTo::clone() const
{
	return new ActionScaleTo(m_fDuration, m_nEndScaleX, m_nEndScaleY);
}

void e2d::ActionScaleTo::_init()
{
	ActionScaleBy::_init();
	m_nVariationX = m_nEndScaleX - m_nBeginScaleX;
	m_nVariationY = m_nEndScaleY - m_nBeginScaleY;
}
