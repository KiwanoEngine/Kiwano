#include "..\e2daction.h"


e2d::ActionScaleBy::ActionScaleBy(double duration, double scale)
	: ActionGradual(duration)
{
	_nVariationX = scale;
	_nVariationY = scale;
}

e2d::ActionScaleBy::ActionScaleBy(double duration, double scaleX, double scaleY)
	: ActionGradual(duration)
{
	_nVariationX = scaleX;
	_nVariationY = scaleY;
}

void e2d::ActionScaleBy::_init()
{
	ActionGradual::_init();
	if (_pTarget)
	{
		_nBeginScaleX = _pTarget->getScaleX();
		_nBeginScaleY = _pTarget->getScaleY();
	}
}

void e2d::ActionScaleBy::_update()
{
	ActionGradual::_update();

	if (_pTarget == nullptr)
	{
		this->stop();
		return;
	}

	// Ëõ·Å½Úµã
	_pTarget->setScale(
		_nBeginScaleX + _nVariationX * _fRateOfProgress, 
		_nBeginScaleY + _nVariationY * _fRateOfProgress);
}

e2d::ActionScaleBy * e2d::ActionScaleBy::clone() const
{
	return new ActionScaleBy(_fDuration, _nVariationX, _nVariationY);
}

e2d::ActionScaleBy * e2d::ActionScaleBy::reverse() const
{
	return new ActionScaleBy(_fDuration, -_nVariationX, -_nVariationY);
}