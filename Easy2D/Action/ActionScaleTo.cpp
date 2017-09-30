#include "..\easy2d.h"

ActionScaleTo::ActionScaleTo(float duration, float scaleX, float scaleY) :
	ActionScaleBy(duration, 0, 0)
{
	m_nEndScaleX = scaleX;
	m_nEndScaleY = scaleY;
}

ActionScaleTo::~ActionScaleTo()
{
}

ActionScaleTo * ActionScaleTo::copy() const
{
	return new ActionScaleTo(m_nMilliSeconds / 1000.0f, m_nEndScaleX, m_nEndScaleY);
}

void ActionScaleTo::_init()
{
	ActionScaleBy::_init();
	m_nVariationX = m_nEndScaleX - m_nBeginScaleX;
	m_nVariationY = m_nEndScaleY - m_nBeginScaleY;
}

void ActionScaleTo::_reset()
{
	ActionScaleBy::_reset();
}
