#include "..\e2dcommon.h"
#include "..\e2dtool.h"


e2d::Listener::Listener()
	: _name()
	, _callback()
	, _running(true)
	, _stopped(false)
{
}

e2d::Listener::Listener(const Function & func, const String & name, bool paused)
	: _name(name)
	, _callback(func)
	, _running(!paused)
	, _stopped(false)
{
}

void e2d::Listener::_update()
{
	if (_callback)
	{
		_callback();
	}
}

bool e2d::Listener::isRunning() const
{
	return _running;
}

e2d::String e2d::Listener::getName() const
{
	return _name;
}

void e2d::Listener::setName(const String & name)
{
	_name = name;
}

void e2d::Listener::setFunc(const Function & func)
{
	_callback = func;
}

void e2d::Listener::start()
{
	_running = true;
}

void e2d::Listener::stop()
{
	_running = false;
}
