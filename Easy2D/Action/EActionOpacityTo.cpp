#include "..\eactions.h"


e2d::EActionOpacityTo::EActionOpacityTo(float duration, float opacity) :
	EActionOpacityBy(duration, 0)
{
	m_nEndVal = opacity;
}

e2d::EActionOpacityTo * e2d::EActionOpacityTo::clone() const
{
	return new EActionOpacityTo(m_fTotalDuration / 1000, m_nEndVal);
}

void e2d::EActionOpacityTo::_init()
{
	EActionOpacityBy::_init();
	m_nVariation = m_nEndVal - m_nBeginVal;
}

void e2d::EActionOpacityTo::_reset()
{
	EActionOpacityBy::_reset();
}
