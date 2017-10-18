#include "..\eactions.h"

e2d::ActionMoveTo::ActionMoveTo(float duration, EPoint pos) :
	ActionMoveBy(duration, EVec())
{
	m_EndPos = pos;
}

e2d::ActionMoveTo::~ActionMoveTo()
{
}

e2d::ActionMoveTo * e2d::ActionMoveTo::copy() const
{
	return new ActionMoveTo(m_nAnimationInterval / 1000.0f, m_EndPos);
}

void e2d::ActionMoveTo::_init()
{
	ActionMoveBy::_init();
	m_MoveVector = m_EndPos - m_BeginPos;
}

void e2d::ActionMoveTo::_reset()
{
	ActionMoveBy::_reset();
}
