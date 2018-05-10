#include "..\e2daction.h"

e2d::ActionGradual::ActionGradual(double duration)
	: _delta(0)
{
	_duration = max(duration, 0);
}

void e2d::ActionGradual::_init()
{
	Action::_init();
}

void e2d::ActionGradual::_update()
{
	Action::_update();

	if (_duration == 0)
	{
		_delta = 1;
		this->stop();
		return;
	}

	_delta = min((Time::getTotalTime() - _last) / _duration, 1);

	if (_delta >= 1)
	{
		this->stop();
	}
}
