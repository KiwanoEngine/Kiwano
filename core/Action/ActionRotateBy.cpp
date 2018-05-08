#include "..\e2daction.h"


e2d::ActionRotateBy::ActionRotateBy(double duration, double rotation) :
	ActionGradual(duration)
{
	_nVariation = rotation;
}

void e2d::ActionRotateBy::_init()
{
	ActionGradual::_init();
	if (_pTarget)
	{
		_nBeginVal = _pTarget->getRotation();
	}
}

void e2d::ActionRotateBy::_update()
{
	ActionGradual::_update();

	if (_pTarget == nullptr)
	{
		this->stop();
		return;
	}

	// Ðý×ª½Úµã
	_pTarget->setRotation(_nBeginVal + _nVariation * _fRateOfProgress);
}

e2d::ActionRotateBy * e2d::ActionRotateBy::clone() const
{
	return new ActionRotateBy(_fDuration, _nVariation);
}

e2d::ActionRotateBy * e2d::ActionRotateBy::reverse() const
{
	return new ActionRotateBy(_fDuration, -_nVariation);
}