#include "..\e2daction.h"


e2d::OpacityTo::OpacityTo(double duration, double opacity) :
	OpacityBy(duration, 0)
{
	_endVal = opacity;
}

e2d::OpacityTo * e2d::OpacityTo::clone() const
{
	return new OpacityTo(_duration, _endVal);
}

void e2d::OpacityTo::_init()
{
	OpacityBy::_init();
	_deltaVal = _endVal - _startVal;
}
