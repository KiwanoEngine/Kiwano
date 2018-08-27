#include "..\e2dtransition.h"
#include "..\e2dnode.h"

e2d::BoxTransition::BoxTransition(Scene* scene, float duration)
	: Transition(scene, duration)
{
}

bool e2d::BoxTransition::_init(Game * game, Scene * prev)
{
	if (Transition::_init(game, prev))
	{
		_inLayerParam.opacity = 0;
		return true;
	}
	return false;
}

void e2d::BoxTransition::_update()
{
	Transition::_update();

	auto size = Window::getInstance()->getSize();
	if (_delta <= 0.5)
	{
		_outLayerParam.contentBounds = D2D1::RectF(
			size.width * _delta,
			size.height * _delta,
			size.width * (1 - _delta),
			size.height * (1 - _delta)
		);
	}
	else
	{
		_outLayerParam.opacity = 0;
		_inLayerParam.opacity = 1;
		_inLayerParam.contentBounds = D2D1::RectF(
			size.width * (1 - _delta),
			size.height * (1 - _delta),
			size.width * _delta,
			size.height * _delta
		);
		if (_delta >= 1)
		{
			this->_stop();
		}
	}
}
