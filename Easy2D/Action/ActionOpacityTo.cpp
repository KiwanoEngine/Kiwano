#include "..\easy2d.h"

ActionOpacityTo::ActionOpacityTo(float duration, float opacity) :
	ActionOpacityBy(duration, 0)
{
	m_nEndVal = opacity;
}

ActionOpacityTo::~ActionOpacityTo()
{
}

ActionOpacityTo * ActionOpacityTo::copy()
{
	auto a = new ActionOpacityTo(*this);
	a->_reset();
	return a;
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
