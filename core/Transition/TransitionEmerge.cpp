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
	_sPrevLayerParam.opacity = float(1 - _delta);
	_sNextLayerParam.opacity = float(_delta);

	if (_delta >= 1)
	{
		this->_stop();
	}
}

void e2d::TransitionEmerge::_reset()
{
}
