#include "..\e2dtransition.h"
#include "..\e2dnode.h"

e2d::TransitionFade::TransitionFade(double duration)
	: Transition(duration)
{
}

void e2d::TransitionFade::_init(Scene * prev, Scene * next)
{
	Transition::_init(prev, next);
	_outLayerParam.opacity = 1;
	_inLayerParam.opacity = 0;
}

void e2d::TransitionFade::_updateCustom()
{
	if (_delta < 0.5)
	{
		_outLayerParam.opacity = 1 - float(_delta) * 2;
		_inLayerParam.opacity = 0;
	}
	else
	{
		_outLayerParam.opacity = 0;
		_inLayerParam.opacity = float(_delta - 0.5) * 2;
		if (_delta >= 1)
		{
			this->_stop();
		}
	}
}

void e2d::TransitionFade::_reset()
{
}
