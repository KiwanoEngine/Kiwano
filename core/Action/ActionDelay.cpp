#include "..\e2daction.h"

e2d::ActionDelay::ActionDelay(double duration)
{
	_fDelayTime = max(duration, 0);
}

e2d::ActionDelay * e2d::ActionDelay::clone() const
{
	return new ActionDelay(_fDelayTime);
}

void e2d::ActionDelay::_init()
{
	ActionBase::_init();
}

void e2d::ActionDelay::_update()
{
	ActionBase::_update();
	// 判断时间间隔是否足够
	if ((Time::getTotalTime() - _fLast) >= _fDelayTime)
	{
		this->stop();
	}
}
