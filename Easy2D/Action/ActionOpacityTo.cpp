#include "..\eactions.h"


e2d::ActionOpacityTo::ActionOpacityTo(float duration, float opacity) :
	ActionOpacityBy(duration, 0)
{
	m_nEndVal = opacity;
}

e2d::ActionOpacityTo::~ActionOpacityTo()
{
}

e2d::ActionOpacityTo * e2d::ActionOpacityTo::copy() const
{
	return new ActionOpacityTo(m_nAnimationInterval / 1000.0f, m_nEndVal);
}

void e2d::ActionOpacityTo::_init()
{
	ActionOpacityBy::_init();
	m_nVariation = m_nEndVal - m_nBeginVal;
}

void e2d::ActionOpacityTo::_reset()
{
	ActionOpacityBy::_reset();
}
