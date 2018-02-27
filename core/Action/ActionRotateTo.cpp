#include "..\eactions.h"


e2d::ActionRotateTo::ActionRotateTo(double duration, double rotation) :
	ActionRotateBy(duration, 0)
{
	m_nEndVal = rotation;
}

e2d::ActionRotateTo * e2d::ActionRotateTo::clone() const
{
	return new ActionRotateTo(m_fDuration, m_nEndVal);
}

void e2d::ActionRotateTo::_init()
{
	ActionRotateBy::_init();
	m_nVariation = m_nEndVal - m_nBeginVal;
}
