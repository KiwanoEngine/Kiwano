#include "..\e2daction.h"
#include "..\e2dmanager.h"

e2d::Loop::Loop(Action * action, int times /* = -1 */)
	: _action(action)
	, _times(0)
	, _totalTimes(times)
{
	ASSERT(_action, "Loop NULL pointer exception!");
	_action->retain();
}

e2d::Loop::~Loop()
{
}

e2d::Loop * e2d::Loop::clone() const
{
	return new Loop(_action->clone());
}

void e2d::Loop::_init()
{
	Action::_init();
	_action->_target = _target;
	_action->_init();
}

void e2d::Loop::_update()
{
	Action::_update();

	if (_times == _totalTimes)
	{
		this->stop();
		return;
	}

	_action->_update();

	if (_action->_isDone())
	{
		_times++;
		
		Action::reset();
		_action->reset();
	}
}

void e2d::Loop::reset()
{
	Action::reset();

	_action->reset();
	_times = 0;
}

void e2d::Loop::onDestroy()
{
	Action::onDestroy();
	SafeRelease(&_action);
}

void e2d::Loop::_resetTime()
{
	_action->_resetTime();
}
