#include "..\e2daction.h"

e2d::Delay::Delay(double duration)
{
	_delay = max(duration, 0);
}

e2d::Delay * e2d::Delay::clone() const
{
	return new Delay(_delay);
}

void e2d::Delay::_init()
{
	Action::_init();
}

void e2d::Delay::_update()
{
	Action::_update();
	// 判断时间间隔是否足够
	if ((Time::getTotalTime() - _fLast) >= _delay)
	{
		this->stop();
	}
}
