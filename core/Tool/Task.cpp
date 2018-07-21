#include "..\e2dtool.h"


e2d::Task::Task(const Function & func, const String & name)
	: _running(true)
	, _stopped(false)
	, _runTimes(0)
	, _totalTimes(-1)
	, _delay(0.0)
	, _lastTime(0.0)
	, _callback(func)
	, _name(name)
{
}

e2d::Task::Task(const Function & func, double delay, int times, const String & name)
	: _running(true)
	, _stopped(false)
	, _runTimes(0)
	, _totalTimes(times)
	, _delay(std::max(delay, 0.0))
	, _lastTime(0.0)
	, _callback(func)
	, _name(name)
{
}

void e2d::Task::pause()
{
	_running = false;
}

void e2d::Task::resume()
{
	_running = true;
	updateTime();
}

void e2d::Task::update()
{
	if (_callback)
	{
		_callback();
	}

	++_runTimes;
	_lastTime += _delay;

	if (_runTimes == _totalTimes)
	{
		_stopped = true;
	}
}

bool e2d::Task::isReady() const
{
	if (_running)
	{
		if (_delay == 0)
		{
			return true;
		}
		if ((Game::getInstance()->getTotalTime() - _lastTime) >= _delay)
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

void e2d::Task::updateTime()
{
	_lastTime = Game::getInstance()->getTotalTime();
}