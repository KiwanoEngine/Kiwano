#include "..\e2daction.h"


e2d::RotateBy::RotateBy(double duration, double rotation)
	: ActionGradual(duration)
{
	_deltaVal = rotation;
}

e2d::RotateBy * e2d::RotateBy::create(double duration, double rotation)
{
	return GC::create<RotateBy>(duration, rotation);
}

void e2d::RotateBy::_init()
{
	ActionGradual::_init();

	if (_target)
	{
		_startVal = _target->getRotation();
	}
}

void e2d::RotateBy::_update()
{
	ActionGradual::_update();

	if (_target)
	{
		_target->setRotation(_startVal + _deltaVal * _delta);
	}
}

e2d::RotateBy * e2d::RotateBy::clone() const
{
	return GC::create<RotateBy>(_duration, _deltaVal);
}

e2d::RotateBy * e2d::RotateBy::reverse() const
{
	return GC::create<RotateBy>(_duration, -_deltaVal);
}