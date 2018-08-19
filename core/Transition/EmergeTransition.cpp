#include "..\e2dtransition.h"
#include "..\e2dnode.h"

e2d::EmergeTransition::EmergeTransition(Scene* scene, float duration)
	: Transition(scene, duration)
{
}

bool e2d::EmergeTransition::_init(Game * game, Scene * prev)
{
	if (Transition::_init(game, prev))
	{
		_outLayerParam.opacity = 1;
		_inLayerParam.opacity = 0;
		return true;
	}
	return false;
}

void e2d::EmergeTransition::_update()
{
	Transition::_update();

	_outLayerParam.opacity = 1 - _delta;
	_inLayerParam.opacity = _delta;

	if (_delta >= 1)
	{
		this->_stop();
	}
}
