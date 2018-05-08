#include "..\e2dtransition.h"
#include "..\e2dnode.h"

e2d::TransitionFade::TransitionFade(double duration)
	: TransitionBase(0)
	, _fFadeOutDuration(max(duration / 2, 0))
	, _fFadeInDuration(max(duration / 2, 0))
	, _bFadeOutTransioning(true)
{
}

e2d::TransitionFade::TransitionFade(double fadeOutDuration, double fadeInDuration)
	: TransitionBase(0)
	, _fFadeOutDuration(max(fadeOutDuration, 0))
	, _fFadeInDuration(max(fadeInDuration, 0))
	, _bFadeOutTransioning(true)
{
}

void e2d::TransitionFade::_init(Scene * prev, Scene * next)
{
	TransitionBase::_init(prev, next);
	if (_pPrevScene)
	{
		_bFadeOutTransioning = true;
		_fDuration = _fFadeOutDuration;
	}
	else
	{
		_bFadeOutTransioning = false;
		_fDuration = _fFadeInDuration;
	}
	_sPrevLayerParam.opacity = 1;
	_sNextLayerParam.opacity = 0;
}

void e2d::TransitionFade::_updateCustom()
{
	if (_bFadeOutTransioning)
	{
		_sPrevLayerParam.opacity = float(1 - _fRateOfProgress);
		if (_fRateOfProgress >= 1)
		{
			_bFadeOutTransioning = false;
			_fDuration = _fFadeInDuration;
			_fLast = Time::getTotalTime();
		}
	}
	else
	{
		_sNextLayerParam.opacity = float(_fRateOfProgress);
		if (_fRateOfProgress >= 1)
		{
			this->_stop();
		}
	}
}

void e2d::TransitionFade::_reset()
{
}
