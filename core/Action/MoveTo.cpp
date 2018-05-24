#include "..\e2daction.h"
#include "..\e2dnode.h"

e2d::MoveTo::MoveTo(double duration, Point pos)
	: MoveBy(duration, Vector())
{
	_endPos = pos;
}

e2d::MoveTo * e2d::MoveTo::clone() const
{
	return Create<MoveTo>(_duration, _endPos);
}

void e2d::MoveTo::_init()
{
	MoveBy::_init();
	_deltaPos = _endPos - _startPos;
}
