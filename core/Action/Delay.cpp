#include "..\e2daction.h"

e2d::Delay::Delay(double duration)
	: _delta(0)
	, _delay(max(duration, 0))
{
}

e2d::Delay * e2d::Delay::clone() const
{
	return GC::create<Delay>(_delay);
}

e2d::Delay * e2d::Delay::reverse() const
{
	return GC::create<Delay>(_delay);
}

void e2d::Delay::reset()
{
	Action::reset();
	_delta = 0;
}

void e2d::Delay::_init()
{
	Action::_init();
}

void e2d::Delay::_update()
{
	Action::_update();

	_delta = Time::getTotalTime() - _last;

	if (_delta >= _delay)
	{
		this->stop();
	}
}

void e2d::Delay::_resetTime()
{
	Action::_resetTime();
	_last = Time::getTotalTime() - _delta;
}
