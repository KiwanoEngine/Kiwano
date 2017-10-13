#include "..\easy2d.h"

ActionOpacityTo::ActionOpacityTo(float duration, float opacity) :
	ActionOpacityBy(duration, 0)
{
	m_nEndVal = opacity;
}

ActionOpacityTo::~ActionOpacityTo()
{
}

ActionOpacityTo * ActionOpacityTo::copy() const
{
	return new ActionOpacityTo(m_nAnimationInterval / 1000.0f, m_nEndVal);
}

void ActionOpacityTo::_init()
{
	ActionOpacityBy::_init();
	m_nVariation = m_nEndVal - m_nBeginVal;
}

void ActionOpacityTo::_reset()
{
	ActionOpacityBy::_reset();
}
