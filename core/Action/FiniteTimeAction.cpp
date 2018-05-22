#include "..\e2daction.h"

e2d::FiniteTimeAction::FiniteTimeAction(double duration)
	: _delta(0)
	, _duration(max(duration, 0))
{
}

void e2d::FiniteTimeAction::reset()
{
	Action::reset();
	_delta = 0;
}

void e2d::FiniteTimeAction::_init()
{
	Action::_init();
}

void e2d::FiniteTimeAction::_update()
{
	Action::_update();

	if (_duration == 0)
	{
		_delta = 1;
		this->stop();
	}
	else
	{
		_delta = min((Time::getTotalTime() - _last) / _duration, 1);

		if (_delta >= 1)
		{
			this->stop();
		}
	}
}

void e2d::FiniteTimeAction::_resetTime()
{
	Action::_resetTime();
	_last = Time::getTotalTime() - _delta * _duration;
}
