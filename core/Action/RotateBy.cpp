#include "..\e2daction.h"
#include "..\e2dnode.h"


e2d::RotateBy::RotateBy(double duration, double rotation)
	: FiniteTimeAction(duration)
{
	_deltaVal = rotation;
}

void e2d::RotateBy::_init()
{
	FiniteTimeAction::_init();

	if (_target)
	{
		_startVal = _target->getRotation();
	}
}

void e2d::RotateBy::_update()
{
	FiniteTimeAction::_update();

	if (_target)
	{
		_target->setRotation(_startVal + _deltaVal * _delta);
	}
}

e2d::RotateBy * e2d::RotateBy::clone() const
{
	return Create<RotateBy>(_duration, _deltaVal);
}

e2d::RotateBy * e2d::RotateBy::reverse() const
{
	return Create<RotateBy>(_duration, -_deltaVal);
}