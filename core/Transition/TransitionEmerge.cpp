#include "..\e2dtransition.h"
#include "..\e2dnode.h"

e2d::TransitionEmerge::TransitionEmerge(double duration)
	: TransitionBase(duration)
{
}

void e2d::TransitionEmerge::_init(Scene * prev, Scene * next)
{
	TransitionBase::_init(prev, next);
	_sPrevLayerParam.opacity = 1;
	_sNextLayerParam.opacity = 0;
}

void e2d::TransitionEmerge::_updateCustom()
{
	_sPrevLayerParam.opacity = float(1 - _fRateOfProgress);
	_sNextLayerParam.opacity = float(_fRateOfProgress);

	if (_fRateOfProgress >= 1)
	{
		this->_stop();
	}
}

void e2d::TransitionEmerge::_reset()
{
}
