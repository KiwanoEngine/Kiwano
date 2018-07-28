#include "..\e2daction.h"

e2d::FiniteTimeAction::FiniteTimeAction(float duration)
	: _delta(0)
	, _duration(std::max(duration, 0.f))
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
		_delta = std::min((Game::getInstance()->getTotalDuration().seconds() - _last) / _duration, 1.f);

		if (_delta >= 1)
		{
			this->stop();
		}
	}
}

void e2d::FiniteTimeAction::_resetTime()
{
	Action::_resetTime();
	_last = Game::getInstance()->getTotalDuration().seconds() - _delta * _duration;
}
