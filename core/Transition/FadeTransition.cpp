#include "..\e2dtransition.h"
#include "..\e2dnode.h"

e2d::FadeTransition::FadeTransition(float duration)
	: Transition(duration)
{
}

bool e2d::FadeTransition::init(Scene * prev, Scene * next)
{
	if (Transition::init(prev, next))
	{
		_outLayerParam.opacity = 1;
		_inLayerParam.opacity = 0;
		return true;
	}
	return false;
}

void e2d::FadeTransition::update()
{
	Transition::update();

	if (_delta < 0.5)
	{
		_outLayerParam.opacity = 1 - _delta * 2;
		_inLayerParam.opacity = 0;
	}
	else
	{
		_outLayerParam.opacity = 0;
		_inLayerParam.opacity = (_delta - 0.5f) * 2;
		if (_delta >= 1)
		{
			this->stop();
		}
	}
}

void e2d::FadeTransition::reset()
{
}
