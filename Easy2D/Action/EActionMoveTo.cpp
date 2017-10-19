#include "..\eactions.h"

e2d::EActionMoveTo::EActionMoveTo(float duration, EPoint pos) :
	EActionMoveBy(duration, EVec())
{
	m_EndPos = pos;
}

e2d::EActionMoveTo * e2d::EActionMoveTo::clone() const
{
	return new EActionMoveTo(m_nAnimationInterval / 1000.0f, m_EndPos);
}

void e2d::EActionMoveTo::_init()
{
	EActionMoveBy::_init();
	m_MoveVector = m_EndPos - m_BeginPos;
}

void e2d::EActionMoveTo::_reset()
{
	EActionMoveBy::_reset();
}
