#include "..\e2daction.h"
#include "..\e2dnode.h"

e2d::ScaleTo::ScaleTo(float duration, float scale)
	: ScaleBy(duration, 0, 0)
{
	_endScaleX = scale;
	_endScaleY = scale;
}

e2d::ScaleTo::ScaleTo(float duration, float scaleX, float scaleY)
	: ScaleBy(duration, 0, 0)
{
	_endScaleX = scaleX;
	_endScaleY = scaleY;
}

e2d::ScaleTo * e2d::ScaleTo::clone() const
{
	return new (e2d::autorelease) ScaleTo(_duration, _endScaleX, _endScaleY);
}

void e2d::ScaleTo::_init()
{
	ScaleBy::_init();
	_deltaX = _endScaleX - _startScaleX;
	_deltaY = _endScaleY - _startScaleY;
}
