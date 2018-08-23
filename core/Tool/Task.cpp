#include "..\e2dtool.h"


e2d::Task::Task(const Function & func, const String & name)
	: _running(true)
	, _stopped(false)
	, _runTimes(0)
	, _totalTimes(-1)
	, _delay()
	, _callback(func)
	, _name(name)
{
}

e2d::Task::Task(const Function & func, float delay, int times, const String & name)
	: _running(true)
	, _stopped(false)
	, _runTimes(0)
	, _delay(std::max(delay, 0.f))
	, _totalTimes(times)
	, _callback(func)
	, _name(name)
{
}

void e2d::Task::start()
{
	_running = true;
	_lastTime = Time::now();
}

void e2d::Task::stop()
{
	_running = false;
}

void e2d::Task::_update()
{
	if (_totalTimes == 0)
	{
		_stopped = true;
		return;
	}

	++_runTimes;
	_lastTime += _delay;

	if (_callback)
	{
		_callback();
	}

	if (_runTimes == _totalTimes)
	{
		_stopped = true;
		return;
	}
}

bool e2d::Task::_isReady() const
{
	if (_running)
	{
		if (_delay.milliseconds() == 0)
		{
			return true;
		}
		if (Time::now() - _lastTime >= _delay)
		{
			return true;
		}
	}
	return false;
}

bool e2d::Task::isRunning() const
{
	return _running;
}

e2d::String e2d::Task::getName() const
{
	return _name;
}
