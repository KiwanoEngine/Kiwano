#include "..\eactions.h"

e2d::ActionMoveTo::ActionMoveTo(float duration, Point pos) :
	ActionMoveBy(duration, Vector())
{
	m_EndPos = pos;
}

e2d::ActionMoveTo * e2d::ActionMoveTo::clone() const
{
	return new ActionMoveTo(m_fDuration, m_EndPos);
}

void e2d::ActionMoveTo::_init()
{
	ActionMoveBy::_init();
	m_MoveVec = m_EndPos - m_BeginPos;
}
