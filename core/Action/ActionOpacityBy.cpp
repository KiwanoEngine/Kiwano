#include "..\e2daction.h"


e2d::ActionOpacityBy::ActionOpacityBy(double duration, double opacity) :
	ActionGradual(duration)
{
	_nVariation = opacity;
}

void e2d::ActionOpacityBy::_init()
{
	ActionGradual::_init();
	if (_pTarget)
	{
		_nBeginVal = _pTarget->getOpacity();
	}
}

void e2d::ActionOpacityBy::_update()
{
	ActionGradual::_update();

	if (_pTarget == nullptr)
	{
		this->stop();
		return;
	}
	// 设置节点透明度
	_pTarget->setOpacity(_nBeginVal + _nVariation * _fRateOfProgress);
}

e2d::ActionOpacityBy * e2d::ActionOpacityBy::clone() const
{
	return new ActionOpacityBy(_fDuration, _nVariation);
}

e2d::ActionOpacityBy * e2d::ActionOpacityBy::reverse() const
{
	return new ActionOpacityBy(_fDuration, -_nVariation);
}