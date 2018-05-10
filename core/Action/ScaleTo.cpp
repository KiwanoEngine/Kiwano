#include "..\e2daction.h"

e2d::ScaleTo::ScaleTo(double duration, double scale)
	: ScaleBy(duration, 0, 0)
{
	_endScaleX = scale;
	_endScaleY = scale;
}

e2d::ScaleTo::ScaleTo(double duration, double scaleX, double scaleY)
	: ScaleBy(duration, 0, 0)
{
	_endScaleX = scaleX;
	_endScaleY = scaleY;
}

e2d::ScaleTo * e2d::ScaleTo::clone() const
{
	return new (std::nothrow) ScaleTo(_duration, _endScaleX, _endScaleY);
}

void e2d::ScaleTo::_init()
{
	ScaleBy::_init();
	_deltaX = _endScaleX - _startScaleX;
	_deltaY = _endScaleY - _startScaleY;
}
