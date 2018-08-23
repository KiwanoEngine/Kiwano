#include "..\e2daction.h"
#include "..\e2dmanager.h"

e2d::Action::Action() 
	: _running(false)
	, _done(false)
	, _initialized(false)
	, _target(nullptr)
{
	ActionManager::instance()->__add(this);
}

e2d::Action::~Action()
{
	ActionManager::instance()->__remove(this);
}

bool e2d::Action::isRunning()
{
	return _running;
}

void e2d::Action::resume()
{
	_running = true;
}

void e2d::Action::pause()
{
	_running = false;
}

void e2d::Action::stop()
{
	_done = true;
}

const e2d::String& e2d::Action::name() const
{
	return _name;
}

void e2d::Action::name(const String& name)
{
	_name = name;
}

e2d::Node * e2d::Action::target()
{
	return _target;
}

void e2d::Action::reset()
{
	_initialized = false;
	_done = false;
	_started = Time::now();
}

bool e2d::Action::_isDone()
{
	return _done;
}

void e2d::Action::_startWithTarget(Node* target)
{
	_target = target;
	_running = true;
	this->reset();
}

void e2d::Action::_init()
{
	_initialized = true;
	_started = Time::now();
}

void e2d::Action::_update()
{
	if (!_initialized)
	{
		_init();
	}
}

void e2d::Action::_resetTime()
{
}
