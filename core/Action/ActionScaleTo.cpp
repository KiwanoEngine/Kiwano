#include "..\e2daction.h"

e2d::ActionScaleTo::ActionScaleTo(double duration, double scale)
	: ActionScaleBy(duration, 0, 0)
{
	_nEndScaleX = scale;
	_nEndScaleY = scale;
}

e2d::ActionScaleTo::ActionScaleTo(double duration, double scaleX, double scaleY)
	: ActionScaleBy(duration, 0, 0)
{
	_nEndScaleX = scaleX;
	_nEndScaleY = scaleY;
}

e2d::ActionScaleTo * e2d::ActionScaleTo::clone() const
{
	return new ActionScaleTo(_fDuration, _nEndScaleX, _nEndScaleY);
}

void e2d::ActionScaleTo::_init()
{
	ActionScaleBy::_init();
	_nVariationX = _nEndScaleX - _nBeginScaleX;
	_nVariationY = _nEndScaleY - _nBeginScaleY;
}
