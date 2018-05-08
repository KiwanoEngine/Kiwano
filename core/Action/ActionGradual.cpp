#include "..\e2daction.h"

e2d::ActionGradual::ActionGradual(double duration)
	: _fRateOfProgress(0)
{
	_fDuration = max(duration, 0);
}

void e2d::ActionGradual::_init()
{
	ActionBase::_init();
}

void e2d::ActionGradual::_update()
{
	ActionBase::_update();
	// 判断时间间隔是否足够
	if (_fDuration == 0)
	{
		_fRateOfProgress = 1;
		this->stop();
		return;
	}
	// 计算动画进度
	_fRateOfProgress = min((Time::getTotalTime() - _fLast) / _fDuration, 1);
	// 判断动作是否结束
	if (_fRateOfProgress >= 1)
	{
		this->stop();
	}
}
