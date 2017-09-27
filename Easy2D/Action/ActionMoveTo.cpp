#include "..\easy2d.h"

ActionMoveTo::ActionMoveTo(float duration, CPoint pos) :
	ActionMoveBy(duration, CVector())
{
	m_EndPos = pos;
}

ActionMoveTo::~ActionMoveTo()
{
}

ActionMoveTo * ActionMoveTo::copy()
{
	auto a = new ActionMoveTo(*this);
	a->_reset();
	return a;
}

void ActionMoveTo::_init()
{
	ActionMoveBy::_init();
	m_MoveVector = m_EndPos - m_BeginPos;
}

void ActionMoveTo::_reset()
{
	ActionMoveBy::_reset();
}
