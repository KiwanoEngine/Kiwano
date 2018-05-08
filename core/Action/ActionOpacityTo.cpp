#include "..\e2daction.h"


e2d::ActionOpacityTo::ActionOpacityTo(double duration, double opacity) :
	ActionOpacityBy(duration, 0)
{
	_nEndVal = opacity;
}

e2d::ActionOpacityTo * e2d::ActionOpacityTo::clone() const
{
	return new ActionOpacityTo(_fDuration, _nEndVal);
}

void e2d::ActionOpacityTo::_init()
{
	ActionOpacityBy::_init();
	_nVariation = _nEndVal - _nBeginVal;
}
