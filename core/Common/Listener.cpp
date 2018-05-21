#include "..\e2dcommon.h"


e2d::Listener::Listener(const Function & func, const String & name, bool paused)
	: _name(name)
	, _callback(func)
	, _running(!paused)
	, _stopped(false)
{
}

void e2d::Listener::update()
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
