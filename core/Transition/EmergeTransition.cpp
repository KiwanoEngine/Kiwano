#include "..\e2dtransition.h"
#include "..\e2dnode.h"

e2d::EmergeTransition::EmergeTransition(double duration)
	: Transition(duration)
{
}

void e2d::EmergeTransition::_init(Scene * prev, Scene * next)
{
	Transition::_init(prev, next);
	_outLayerParam.opacity = 1;
	_inLayerParam.opacity = 0;
}

void e2d::EmergeTransition::_updateCustom()
{
	_outLayerParam.opacity = float(1 - _delta);
	_inLayerParam.opacity = float(_delta);

	if (_delta >= 1)
	{
		this->_stop();
	}
}

void e2d::EmergeTransition::_reset()
{
}
