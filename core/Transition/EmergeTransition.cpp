#include "..\e2dtransition.h"
#include "..\e2dnode.h"

e2d::EmergeTransition::EmergeTransition(float duration)
	: Transition(duration)
{
}

bool e2d::EmergeTransition::init(Scene * prev, Scene * next)
{
	if (Transition::init(prev, next))
	{
		_outLayerParam.opacity = 1;
		_inLayerParam.opacity = 0;
		return true;
	}
	return false;
}

void e2d::EmergeTransition::update()
{
	Transition::update();

	_outLayerParam.opacity = 1 - _delta;
	_inLayerParam.opacity = _delta;

	if (_delta >= 1)
	{
		this->stop();
	}
}

void e2d::EmergeTransition::reset()
{
}
