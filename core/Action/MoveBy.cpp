#include "..\e2daction.h"


e2d::MoveBy::MoveBy(double duration, Vector vector)
	: FiniteTimeAction(duration)
{
	_deltaPos = vector;
}

void e2d::MoveBy::_init()
{
	FiniteTimeAction::_init();

	if (_target)
	{
		_startPos = _target->getPos();
	}
}

void e2d::MoveBy::_update()
{
	FiniteTimeAction::_update();

	if (_target)
	{
		_target->setPos(_startPos + _deltaPos * _delta);
	}
}

e2d::MoveBy * e2d::MoveBy::clone() const
{
	return GC::create<MoveBy>(_duration, _deltaPos);
}

e2d::MoveBy * e2d::MoveBy::reverse() const
{
	return GC::create<MoveBy>(_duration, -_deltaPos);
}