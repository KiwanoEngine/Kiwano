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
	// 判断时间间隔是否足够
	if (_duration == 0)
	{
		_delta = 1;
		this->stop();
		return;
	}
	// 计算动画进度
	_delta = min((Time::getTotalTime() - _last) / _duration, 1);
	// 判断动作是否结束
	if (_delta >= 1)
	{
		this->stop();
	}
}
