#include "..\e2daction.h"

e2d::ActionMoveTo::ActionMoveTo(double duration, Point pos) :
	ActionMoveBy(duration, Vector())
{
	_EndPos = pos;
}

e2d::ActionMoveTo * e2d::ActionMoveTo::clone() const
{
	return new ActionMoveTo(_fDuration, _EndPos);
}

void e2d::ActionMoveTo::_init()
{
	ActionMoveBy::_init();
	_MoveVec = _EndPos - _BeginPos;
}
