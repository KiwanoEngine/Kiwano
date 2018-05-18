#include "..\e2daction.h"


e2d::OpacityBy::OpacityBy(double duration, double opacity)
	: ActionGradual(duration)
{
	_deltaVal = opacity;
}

void e2d::OpacityBy::_init()
{
	ActionGradual::_init();

	if (_target)
	{
		_startVal = _target->getOpacity();
	}
}

void e2d::OpacityBy::_update()
{
	ActionGradual::_update();

	if (_target)
	{
		_target->setOpacity(_startVal + _deltaVal * _delta);
	}
}

e2d::OpacityBy * e2d::OpacityBy::clone() const
{
	return GC::create<OpacityBy>(_duration, _deltaVal);
}

e2d::OpacityBy * e2d::OpacityBy::reverse() const
{
	return GC::create<OpacityBy>(_duration, -_deltaVal);
}