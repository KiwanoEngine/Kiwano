#include "..\e2daction.h"


e2d::ScaleBy::ScaleBy(double duration, double scale)
	: ActionGradual(duration)
{
	_deltaX = scale;
	_deltaY = scale;
}

e2d::ScaleBy::ScaleBy(double duration, double scaleX, double scaleY)
	: ActionGradual(duration)
{
	_deltaX = scaleX;
	_deltaY = scaleY;
}

e2d::ScaleBy * e2d::ScaleBy::create(double duration, double scale)
{
	return GC::create<ScaleBy>(duration, scale);
}

e2d::ScaleBy * e2d::ScaleBy::create(double duration, double scaleX, double scaleY)
{
	return GC::create<ScaleBy>(duration, scaleX, scaleY);
}

void e2d::ScaleBy::_init()
{
	ActionGradual::_init();

	if (_target)
	{
		_startScaleX = _target->getScaleX();
		_startScaleY = _target->getScaleY();
	}
}

void e2d::ScaleBy::_update()
{
	ActionGradual::_update();

	if (_target)
	{
		_target->setScale(_startScaleX + _deltaX * _delta, _startScaleY + _deltaY * _delta);
	}
}

e2d::ScaleBy * e2d::ScaleBy::clone() const
{
	return GC::create<ScaleBy>(_duration, _deltaX, _deltaY);
}

e2d::ScaleBy * e2d::ScaleBy::reverse() const
{
	return GC::create<ScaleBy>(_duration, -_deltaX, -_deltaY);
}