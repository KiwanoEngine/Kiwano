#include "..\e2dtransition.h"
#include "..\e2dnode.h"

e2d::BoxTransition::BoxTransition(float duration)
	: Transition(duration)
{
}

bool e2d::BoxTransition::init(Scene * prev, Scene * next)
{
	if (Transition::init(prev, next))
	{
		_inLayerParam.opacity = 0;
		return true;
	}
	return false;
}

void e2d::BoxTransition::update()
{
	Transition::update();

	if (_delta <= 0.5)
	{
		_outLayerParam.contentBounds = D2D1::RectF(
			_windowSize.width * _delta,
			_windowSize.height * _delta,
			_windowSize.width * (1 - _delta),
			_windowSize.height * (1 - _delta)
		);
	}
	else
	{
		_outLayerParam.opacity = 0;
		_inLayerParam.opacity = 1;
		_inLayerParam.contentBounds = D2D1::RectF(
			_windowSize.width * (1 - _delta),
			_windowSize.height * (1 - _delta),
			_windowSize.width * _delta,
			_windowSize.height * _delta
		);
		if (_delta >= 1)
		{
			this->stop();
		}
	}
}

void e2d::BoxTransition::reset()
{
}
