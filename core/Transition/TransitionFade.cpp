#include "..\e2dtransition.h"
#include "..\e2dnode.h"

e2d::TransitionFade::TransitionFade(double duration)
	: Transition(0)
	, _fadeOutDuration(max(duration / 2, 0))
	, _fadeInDuration(max(duration / 2, 0))
	, _fadeOutTransioning(true)
{
}

e2d::TransitionFade::TransitionFade(double fadeOutDuration, double fadeInDuration)
	: Transition(0)
	, _fadeOutDuration(max(fadeOutDuration, 0))
	, _fadeInDuration(max(fadeInDuration, 0))
	, _fadeOutTransioning(true)
{
}

e2d::TransitionFade * e2d::TransitionFade::create(double duration)
{
	return Create<TransitionFade>(duration);
}

e2d::TransitionFade * e2d::TransitionFade::create(double fadeOutDuration, double fadeInDuration)
{
	return Create<TransitionFade>(fadeOutDuration, fadeInDuration);
}

void e2d::TransitionFade::_init(Scene * prev, Scene * next)
{
	Transition::_init(prev, next);
	if (_outScene)
	{
		_fadeOutTransioning = true;
		_duration = _fadeOutDuration;
	}
	else
	{
		_fadeOutTransioning = false;
		_duration = _fadeInDuration;
	}
	_outLayerParam.opacity = 1;
	_inLayerParam.opacity = 0;
}

void e2d::TransitionFade::_updateCustom()
{
	if (_fadeOutTransioning)
	{
		_outLayerParam.opacity = float(1 - _delta);
		if (_delta >= 1)
		{
			_fadeOutTransioning = false;
			_duration = _fadeInDuration;
			_last = Time::getTotalTime();
		}
	}
	else
	{
		_inLayerParam.opacity = float(_delta);
		if (_delta >= 1)
		{
			this->_stop();
		}
	}
}

void e2d::TransitionFade::_reset()
{
}
