#include "..\eactions.h"


e2d::EActionRotateTo::EActionRotateTo(float duration, float opacity) :
	EActionRotateBy(duration, 0)
{
	m_nEndVal = opacity;
}

e2d::EActionRotateTo * e2d::EActionRotateTo::clone() const
{
	return new EActionRotateTo(m_nAnimationInterval / 1000.0f, m_nEndVal);
}

void e2d::EActionRotateTo::_init()
{
	EActionRotateBy::_init();
	m_nVariation = m_nEndVal - m_nBeginVal;
}

void e2d::EActionRotateTo::_reset()
{
	EActionRotateBy::_reset();
}
