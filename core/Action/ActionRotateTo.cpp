#include "..\e2daction.h"


e2d::ActionRotateTo::ActionRotateTo(double duration, double rotation) :
	ActionRotateBy(duration, 0)
{
	_nEndVal = rotation;
}

e2d::ActionRotateTo * e2d::ActionRotateTo::clone() const
{
	return new ActionRotateTo(_fDuration, _nEndVal);
}

void e2d::ActionRotateTo::_init()
{
	ActionRotateBy::_init();
	_nVariation = _nEndVal - _nBeginVal;
}
